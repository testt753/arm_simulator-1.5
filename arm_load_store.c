/*
Armator - simulateur de jeu d'instruction ARMv5T � but p�dagogique
Copyright (C) 2011 Guillaume Huard
Ce programme est libre, vous pouvez le redistribuer et/ou le modifier selon les
termes de la Licence Publique G�n�rale GNU publi�e par la Free Software
Foundation (version 2 ou bien toute autre version ult�rieure choisie par vous).

Ce programme est distribu� car potentiellement utile, mais SANS AUCUNE
GARANTIE, ni explicite ni implicite, y compris les garanties de
commercialisation ou d'adaptation dans un but sp�cifique. Reportez-vous � la
Licence Publique G�n�rale GNU pour plus de d�tails.

Vous devez avoir re�u une copie de la Licence Publique G�n�rale GNU en m�me
temps que ce programme ; si ce n'est pas le cas, �crivez � la Free Software
Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307,
�tats-Unis.

Contact: Guillaume.Huard@imag.fr
	 B�timent IMAG
	 700 avenue centrale, domaine universitaire
	 38401 Saint Martin d'H�res
*/
#include "arm_load_store.h"
#include "arm_exception.h"
#include "arm_constants.h"
#include "util.h"
#include "debug.h"

void update_cpsr_ls(arm_core p, int bZ, int bN, int bC, int bV) {
    // Masque pour ne modifier que les bits ZNCV
    uint32_t mask = (1u << Z) | (1u << N) | (1u << C) | (1u << V);

    // Valeurs actuelles des bits ZNCV
	uint32_t cpsr = arm_read_cpsr(p);
    uint32_t current_flags = cpsr & mask;

    // Met à jour les bits ZNCV seulement si la valeur est 0 ou 1
    if (bZ == 0 || bZ == 1) {
        current_flags &= ~(1u << Z);
        current_flags |= (bZ != 0) ? (1u << Z) : 0;
    }

    if (bN == 0 || bN == 1) {
        current_flags &= ~(1u << N);
        current_flags |= (bN != 0) ? (1u << N) : 0;
    }

    if (bC == 0 || bC == 1) {
        current_flags &= ~(1u << C);
        current_flags |= (bC != 0) ? (1u << C) : 0;
    }

    if (bV == 0 || bV == 1) {
        current_flags &= ~(1u << V);
        current_flags |= (bV != 0) ? (1u << V) : 0;
    }

    // Met à jour les bits ZNCV dans CPSR
    cpsr &= ~mask;
    cpsr |= current_flags;

	arm_write_cpsr(p, cpsr);
}

uint32_t get_shift_ls(arm_core p, uint32_t ins, int S, int reg) {

	uint8_t rm = GET_RM(ins);
	uint8_t amount;
	uint8_t rs;
	uint32_t v_rs;
	if(!reg)
		amount = GET_SHIFT_IMM(ins);
	else{
		rs = GET_RS(ins);
		v_rs = arm_read_register(p, rs);
		amount = get_bits(v_rs, 7, 0);
	}
	uint32_t result;
	uint32_t shifter_carry_out = -1;
	uint32_t value = arm_read_register(p, rm);

	switch(GET_SHIFT(ins)) {

		case LSL:     
			if(amount == 0) { 
				result = value;  
			} else {
				if(!reg || amount < 32) {
					result = value << amount;
					shifter_carry_out = get_bit(value, (32 - amount));
				}else if(reg) {
					result = 0;
					if(amount == 32){
						shifter_carry_out = get_bit(value, 0);
					}else{
						shifter_carry_out = 0;
					}
				}
			}
		break;

		case LSR:
			if(!reg){
				if(amount == 0) {
					result = 0;  
				} else { 
					result = value >> amount;
					shifter_carry_out = get_bit(value, (amount - 1));
				}
			}else{
				if(amount == 0) {
					result = value;
				}else if(amount < 32) {
					result = value >> amount;
					shifter_carry_out = get_bit(value, (amount - 1));
				}else {
					result = 0;
					if(amount == 32){
						shifter_carry_out = get_bit(value, 31);
					}else{
						shifter_carry_out = 0;
					}
				}
			}
		break;

		case ASR:
			if(!reg){
				if(amount == 0) {
					uint8_t tmp = get_bit(value, (amount - 1));
					if(!tmp){
						result = 0;
					}else{
						result = 0xFFFFFFFF;
					}
					shifter_carry_out = tmp;
				} else {
					result = (int32_t)value >> amount;
					shifter_carry_out = get_bit(value, (amount - 1));
				}
			}else {
				if(amount == 0) {
					result = value;
				}else if(amount < 32) {
					uint8_t tmp = get_bit(value, (amount - 1));
					if(!tmp){
						result = 0;
					}else{
						result = 0xFFFFFFFF;
					}
					shifter_carry_out = tmp;
				}else {
					if(get_bit(value, 31) == 0){
						result = 0;
					}else{
						result = 0xFFFFFFFF;
					}
					shifter_carry_out = get_bit(value, 31);
				}
			}
		break;

		case ROR:
			if(!reg){
				if(amount == 0) { 
					if(arm_read_cpsr(p) & C) {
						result = (value >> 1) | (1 << 31);  
					} else {
						result = value >> 1;
					}
				} else {
					result = (value >> amount) | 
							(value << (32 - amount)); 
					shifter_carry_out = get_bit(value, (amount - 1));
				}
			}else {
				if(amount == 0){
					result = value;
				}else{
					amount = get_bits(v_rs, 4, 0);
					if(amount){
						result = value;
						shifter_carry_out = get_bit(value, 31);
					}else{
						result = (value >> amount) | 
							(value << (32 - amount)); 
						shifter_carry_out = get_bit(value, (amount - 1));
					}
				}
			}
		break;
	}
    if(S)
	    update_cpsr_ls(p, -1, -1, shifter_carry_out, -1);
	return result;
}

int arm_load_store(arm_core p, uint32_t ins) {
    uint8_t rd = GET_RD(ins);
    uint8_t rn = GET_RN(ins);
    uint32_t addr;
	
    if(GET_GROUP(ins) == 0b010){
		if(rn == 15){
			return 1; //TODO
		}
        if(GET_P(ins)){
            if(GET_U(ins))
                addr = GET_IMM12(ins) + arm_read_register(p, rn);
            else
                addr = GET_IMM12(ins) - arm_read_register(p, rn);
            if(GET_W(ins)){
                arm_write_register(p, rn, addr);
            }
        }else{
            uint32_t v_rn = arm_read_register(p, rn);
            addr = v_rn;
            if(GET_U(ins)){
                arm_write_register(p, rn, v_rn + GET_IMM12(ins));
            }else{
                arm_write_register(p, rn, v_rn + GET_IMM12(ins));
            }
        }
    }else{
        if(GET_GROUP(ins) == 0b011){
            uint8_t rm = GET_RM(ins);\
			if(rm == 15){
				return 1; //TODO
			}
            uint32_t index;
			if(GET_P(ins)){
				if(get_bit(ins, 4))
					return UNDEFINED_INSTRUCTION;
				if(!GET_SHIFT_IMM(ins) && !GET_SHIFT(ins)){
					index = arm_read_register(p, rm);
				}else{
					index = get_shift_ls(p, ins, 0, 0);
				}
				if(GET_U(ins))
					addr = arm_read_register(p, rn) + index;
				else
					addr = arm_read_register(p, rn) - index;
				if(GET_W(ins)){
					if(rn == 15){
						return 1; //TODO
					}
					arm_write_register(p, rn, addr);
				}
			}else{
				if(GET_W(ins) || rn == 15)
					return 1; //TODO
				if(get_bit(ins, 4))
					return UNDEFINED_INSTRUCTION;
				if(!GET_SHIFT_IMM(ins) && !GET_SHIFT(ins)){
					index = arm_read_register(p, rm);
				}else{
					index = get_shift_ls(p, ins, 0, 0);
				}
				if(GET_U(ins))
					addr = arm_read_register(p, rn) + index;
				else
					addr = arm_read_register(p, rn) - index;

				arm_write_register(p, rn, addr);
			}
        }else{
			if(GET_GROUP(ins) == 0b000){
				if(GET_P(ins)){
					if(get_bit(ins, 22)){
						uint8_t offset_8 = (GET_IMMH(ins) << 4) | GET_IMML(ins);
						if(GET_U(ins))
							addr = arm_read_register(p, rn) + offset_8;
						else
							addr = arm_read_register(p, rn) - offset_8;

						if(GET_W(ins)){
							if(GET_RN(ins) == 15)
								return 1; //TODO
							arm_write_register(p, rn, addr);
						}
					}else{
						if(GET_RM(ins) == 15){
							return 1; //TODO
						}
						if(GET_U(ins))
							addr = arm_read_register(p, rn) + arm_read_register(p, GET_RM(ins));
						else
							addr = arm_read_register(p, rn) - arm_read_register(p, GET_RM(ins));
						if(GET_W(ins)){
							if(GET_RN(ins) == 15 || GET_RN(ins) == GET_RM(ins))
								return 1; //TODO
							arm_write_register(p, rn, addr);
						}
					}

				}else{
					if(GET_W(ins))
						return 1; //TODO
					if(get_bit(ins, 22)){
						if(GET_RN(ins) == 15){
							return 1; //TODO
						}
						uint8_t offset_8 = (GET_IMMH(ins) << 4) | GET_IMML(ins);
						if(GET_U(ins))
							addr = arm_read_register(p, rn) + offset_8;
						else
							addr = arm_read_register(p, rn) - offset_8;
					}else{
						if(GET_RN(ins) == 15 || GET_RM(ins) == 15 || GET_RM(ins) == GET_RM(ins)){
							return 1; //TODO
						}
						if(GET_U(ins))
							addr = arm_read_register(p, rn) + arm_read_register(p, GET_RM(ins));
						else
							addr = arm_read_register(p, rn) - arm_read_register(p, GET_RM(ins));
					}
				}

				switch(GET_MISC(ins)){
					case 0b1001:
						if(GET_RM(ins) == 15 || GET_RN(ins) == 15 || GET_RD(ins) == 15 || GET_RM(ins) == GET_RN(ins) || GET_RN(ins) == GET_RD(ins))
							return 1; //TODO
						
						if(!GET_B(ins)){
							uint32_t temp;
							arm_read_word(p, addr, &temp);
							if (addr % 4 != 0) {
								/*
								// Adresse non alignée
								int rotate_amount = 8 * (addr % 4);
								temp = (temp >> rotate_amount) | (temp << (32 - rotate_amount));
								*/
								return DATA_ABORT;
							}
							arm_write_word(p, addr, arm_read_register(p, GET_RM(ins)));
							arm_write_register(p, GET_RD(ins), temp);
						}else{
							uint8_t temp;
							arm_read_byte(p ,addr, &temp);
							arm_write_byte(p, addr, arm_read_register(p, GET_RM(ins)));
							arm_write_register(p, rd, (uint8_t)temp);
						}
						return 0;
					case 0b1011:
						uint16_t temp;
						if(!GET_L(ins)){
							if(!get_bit(addr, 0)){
								arm_write_half(p, addr, arm_read_register(p, GET_RD(ins)));
							}else
								return DATA_ABORT; //TODO
						}else{
							if(!get_bit(addr, 0)){
								arm_read_half(p, addr, &temp);
							}else
								return DATA_ABORT; //TODO
							arm_write_register(p, GET_RD(ins), temp);
						}
					case 0b1101:
					case 0b1110:
					case 0b1111:
						if(GET_L(ins)){
							if(!get_bit(ins, 5)){
								uint8_t data;
								arm_read_byte(p, addr, &data);
								arm_write_register(p, GET_RD(ins), (int32_t)data);
							}else{
								uint16_t data;
								
								if(!get_bit(addr, 0)){
									arm_read_half(p, addr, &data);
								}else
									return 1; //TODO
								
								arm_write_register(p, GET_RD(ins), (int32_t)data);
							}
						}else{
							if(!get_bit(ins, 5)){
								if(GET_RD(ins) != 14 && !get_bit(arm_read_register(p, GET_RD(ins)),0) && (!(addr % 4) || !get_bit(addr, 2))){
									uint32_t tmp;
									arm_read_word(p, addr, &tmp);
									arm_write_register(p, GET_RD(ins), tmp);
									arm_read_word(p, addr + 4, &tmp);
									arm_write_register(p, GET_RD(ins) + 1, tmp);
								}else{
									if(!(addr % 4)){
										return DATA_ABORT;
									}else{
										return 1; //TODO
									}
								}
							}else{
								if(GET_RD(ins) != 14 && !get_bit(arm_read_register(p, GET_RD(ins)),0) && (!(addr % 4) || !get_bit(addr, 2))){
									arm_write_word(p, addr, arm_read_register(p, GET_RD(ins)));
									arm_write_word(p, addr + 4, arm_read_register(p, GET_RD(ins) + 1));
								}else{
									if(!(addr % 4)){
										return DATA_ABORT;
									}else{
										return 1; //TODO
									}
								}
							}
						}
					default:
						return UNDEFINED_INSTRUCTION;
				}
				return 0;
			}
		}
    }
    if(GET_L(ins)){
        uint32_t data; 
        if(!GET_B(ins)){
            arm_read_word(p, addr, &data);

			// Vérifier l'alignement de l'adresse
			if (addr % 4 != 0) {
				// Adresse non alignée
				int rotate_amount = 8 * (addr % 4);
				data = (data >> rotate_amount) | (data << (32 - rotate_amount));
			}

			if (rd == 15) {
				arm_write_register(p, 15, data &= 0xFFFFFFFE);
				if(data & 0x1)
					arm_write_cpsr(p, set_bit(arm_read_cpsr(p), 5));
				else
					arm_write_cpsr(p, clr_bit(arm_read_cpsr(p), 5));
			} else{
				arm_write_register(p, rd, data);
			}
        }else{
            uint8_t tmp = 0;
            arm_read_byte(p ,addr, &tmp);
            data = tmp;
            arm_write_register(p, rd, data);
        }
        return 0;
    }else{
        if(GET_B(ins)){
            arm_write_byte(p, addr, get_bits(arm_read_register(p, rd), 7, 0));
        }else{
			if (addr % 4 != 0) {
				return DATA_ABORT;
			}
            arm_write_word(p, addr, arm_read_register(p, rd));
        }
        return 0;
    }
    return UNDEFINED_INSTRUCTION;
}

// fonction qui execute les affectations depuis les registres 
// dans LDM1,LDM3 et STM1  
int manipule_regs(arm_core p, uint32_t ins, uint32_t addr, int cas){ 
	uint32_t data_ri;
	uint8_t rn = GET_RN(ins);
	if(cas==1){					//load case
		for(int i=0;i<15;i++){
			if(rn == i && GET_W(ins))
				return 1; //TODO
			if (get_bit(ins,i)){
				arm_read_word(p,addr,&data_ri);
				arm_write_register(p, i, data_ri);
				addr=addr+4;
			}
		}
		return 0;
	}
	else{						//store case
		int b = 0;
		for(int i=0;i<15;i++){
			if (get_bit(ins,i)){
				if(b && i == rn)
					return 1; //TODO
				b = 1;
				data_ri=arm_read_register(p,i);
				arm_write_word(p,addr,data_ri);
				addr=addr+4;
			}
		}
		return 0;
	}	
}


int arm_load_store_multiple(arm_core p, uint32_t ins) {
    uint8_t rn = GET_RN(ins);
	uint32_t v_rn = arm_read_register(p,rn);
	uint8_t reg_select=0;
	uint32_t data_ri;
	uint32_t addr;
    uint32_t addr_start,addr_end;
	//calcul du nombre de registre à 1 dans l'instruction
	for(int i=0;i<16;i++){
		if (get_bit(ins,i)){
			reg_select++;
		}
	}

	if(!reg_select)
		return 1; //TODO
//definition des adresses start et end 

	//si on incremente apres p=0 & u=1
	if (!GET_P(ins) && GET_U(ins)){
		addr_start=v_rn;
		addr_end=v_rn+((reg_select*4)-4);
		if (GET_W(ins)){
			arm_write_register(p, rn, v_rn+(reg_select*4));
		}
	}

	//si on incremente avant p=1 & u=1
	else if (GET_P(ins) && GET_U(ins)){
		addr_start=v_rn+4;
		addr_end=v_rn+(reg_select*4);
		if (GET_W(ins)){
			arm_write_register(p, rn, v_rn+(reg_select*4));
		}
	}

	//si on decremente apres p=0 & u=0
	else if (!GET_P(ins) && !GET_U(ins)){
		addr_start=v_rn-((reg_select*4)+4);
		addr_end=v_rn;
		if (GET_W(ins)){
			arm_write_register(p, rn, v_rn-(reg_select*4));
		}
	}

	//si on decremente avant p=1 & u=0
	else if (GET_P(ins) && !GET_U(ins)){
		addr_start=v_rn-(reg_select*4);
		addr_end=v_rn-4;
		if (GET_W(ins)){
			arm_write_register(p, rn, v_rn-(reg_select*4));
		}
	}
	addr= addr_start;

	if(!(addr % 4))
		return DATA_ABORT;
	if((GET_B(ins) && !arm_current_mode_has_spsr(p)) || GET_RN(ins) == 15)
		return 1; //TODO
    if(GET_L(ins)){					//load case
		if(GET_B(ins)){						// si bit B=1
			//LDM(2)
			if(!get_bit(ins,15)){			// si pc=0
				for(int i=0;i<15;i++){
					if (get_bit(ins,i)){
						arm_read_word(p,addr,&data_ri);
						arm_write_usr_register(p,i,data_ri);
						addr=addr+4;
					}
				}
				//assert end_address == address - 4
				if(verify_address(addr,addr_end))
					return 1; //TODO
			}
			//LDM(3)
			else{
				if(manipule_regs( p, ins, addr, 1))
					return 1; //TODO
				// if current mode has spsr
				if (arm_current_mode_has_spsr(p)){
					arm_write_cpsr(p,arm_read_spsr(p));
				}else{
					return 1; //TODO
				}

				uint32_t value;
				arm_read_word(p,addr,&value);
				arm_write_register(p,15,value);
				addr=addr+4;
				if(verify_address(addr,addr_end))
					return 1; //TODO
			}
		}
		//LDM(1)
		else{
			if(manipule_regs( p, ins, addr, 1))
				return 1; //TODO
			if (get_bit(ins,15)){
				uint32_t value;
				arm_read_word(p,addr,&value);
				arm_write_register(p,15,value&=0xFFFFFFFE);
				get_bit(value, 0) ? arm_write_cpsr(p, set_bit(5, arm_read_cpsr(p))) : arm_write_cpsr(p, clr_bit(5, arm_read_cpsr(p)));
				addr=addr+4;
			}
			if(verify_address(addr,addr_end))
				return 1; //TODO
		}
		return 0;
	}
	else{								//store case
		if(!GET_B(ins)){				//STM1
			if(manipule_regs( p, ins, addr, 0))
				return 1; //TODO
			if(verify_address(addr,addr_end))
				return 1; //TODO
		}
		else{						//STM2
			if(GET_W(ins))
				return 1; //TODO
			int b = 0;
			for(int i=0;i<16;i++){
				if (get_bit(ins,i)){
					if(b && i == rn)
						return 1; //TODO
					b = 1;
					data_ri=arm_read_usr_register(p,i);
					arm_write_word(p,addr,data_ri);
					addr=addr+4;
				}
			}
		}
		return 0;
	}
    return UNDEFINED_INSTRUCTION;
}

int arm_coprocessor_load_store(arm_core p, uint32_t ins) {
    /* Not implemented */
    return UNDEFINED_INSTRUCTION;
}