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
#include "arm_data_processing.h"
#include "arm_exception.h"
#include "arm_constants.h"
#include "arm_branch_other.h"
#include "util.h"
#include "debug.h"


void update_cpsr(arm_core p, int bZ, int bN, int bC, int bV) {
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


uint32_t get_shift(arm_core p, uint32_t ins, int reg) {

	uint8_t rm = GET_RM(ins);
	uint8_t amount;
	uint8_t rs;
	uint32_t v_rs;
	if(!reg)
		amount = GET_SHIFT_IMM(ins);
	else{
		rs = GET_RS(ins);
		v_rs = arm_read_register(p, rs);
		amount = GET_BITS(v_rs, 7, 0);
	}
	uint32_t result;
	uint32_t shifter_carry_out = -1;
	uint32_t value = arm_read_register(p, rm);

	switch(GEt_SHIFT(ins)) {

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
	update_cpsr(p, -1, -1, shifter_carry_out, -1);
	return result;
}

void logic_s(arm_core p, uint8_t s, uint8_t rd, uint32_t result){
	if(s && rd == 15){
		if(arm_current_mode_has_spsr(p)){
			arm_write_cpsr(p, arm_read_spsr(p));
		}
	} else if(s){
		update_cpsr(p, !result, get_bit(result, 31), -1, -1);
	}
}

void sb_s(arm_core p, uint8_t s, uint8_t rd, uint32_t result, uint32_t op1, uint32_t op2){
	if(s && rd == 15){
		if(arm_current_mode_has_spsr(p)){
			arm_write_cpsr(p, arm_read_spsr(p));
		}
	} else if(s){
		update_cpsr(p, !result, get_bit(result, 31), !BorrowFrom(op1, op2), OverflowFrom(op1, op2, result, 1));
	}
}
void ad_s(arm_core p, uint8_t s, uint8_t rd, uint64_t result, uint32_t op1, uint32_t op2){
	if(s && rd == 15){
		if(arm_current_mode_has_spsr(p)){
			arm_write_cpsr(p, arm_read_spsr(p));
		}
	} else if(s){
		update_cpsr(p, !result, get_bit(result, 31), result > 0xFFFFFFFF, OverflowFrom(op1, op2, (uint32_t)result, 0));
	}
}
int dp(arm_core p, uint8_t rn, uint8_t rd, uint32_t ops, uint8_t opcd, uint8_t s){
	uint64_t tmp;
	uint32_t result;
	uint32_t v_rn = arm_read_register(p, rn);
	switch (opcd)
	{
		case AND:
			result = v_rn & ops;
			logic_s(p, s, rd, result);
			break;
		case EOR:
			result = v_rn ^ ops;
			logic_s(p, s, rd, result);
			break;
		case SUB:
			result = v_rn - ops;
			sb_s(p, s, rd, result, v_rn, ops);
			break;
		case RSB:
			result = ops - v_rn;
			sb_s(p, s, rd, result, ops, v_rn);
			break;
		case ADD:
			tmp = v_rn + ops;
			result = tmp;
			ad_s(p, s, rd, tmp, v_rn, ops);
			break;
		case ADC:
			uint8_t Cflag = arm_read_cpsr(p) >> C & 0x1;
			tmp = v_rn + ops + Cflag;
			result = tmp;
			ad_s(p, s, rd, tmp, v_rn, ops + Cflag);
			break;
		case SBC:
			uint8_t NCflag = ~(arm_read_cpsr(p) >> C & 0x1);
			result = v_rn - ops - NCflag;
			sb_s(p, s, rd, result, v_rn, ops - NCflag);
			break;
		case RSC:
			uint8_t NCflag = (arm_read_cpsr(p) >> C & 0x1 == 0);
			result = ops - v_rn - NCflag;
			sb_s(p, s, rd, result, ops, v_rn - NCflag);
			break;
		case TST:
			result = v_rn & ops;
			logic_s(p, s, rd, result);
			return 1;
		case TEQ:
			result = v_rn ^ ops;
			logic_s(p, s, rd, result);
			return 1;
		case CMP:
			result = v_rn - ops;
			sb_s(p, s, rd, result, v_rn, ops);
			return 1;
		case CMN:
			tmp = v_rn + ops;
			ad_s(p, s, rd, tmp, v_rn, ops);
			return 1;
		case ORR:
			result = v_rn | ops;
			logic_s(p, s, rd, result);
			break;
		case MOV:
			result = ops;
			logic_s(p, s, rd, result);
			break;
		case BIC:
			result = v_rn & ~ ops;
			logic_s(p, s, rd, result);
			break;
		case MVN:
			result = ~ops;
			logic_s(p, s, rd, result);
			break;
		default:
			return UNDEFINED_INSTRUCTION;
		
		arm_write_register(p, rd, result);
	}
}
/* Decoding functions for different classes of instructions */
int arm_data_processing_shift(arm_core p, uint32_t ins) {
	if(IS_MULT(ins)){
		uint8_t b23 = get_bit(ins, 23), b22 = get_bit(ins, 22), b21 = get_bit(ins, 21);
		uint8_t rs = GET_RS(ins);
		uint8_t rm = GET_RM(ins);
		uint32_t v_rs = arm_read_register(p, rs);
		uint32_t v_rm = arm_read_register(p, rm);
		// TERMINAISON ANTICIPE
		uint64_t result = v_rm * v_rs;
		if(!b23 && !b22){
			uint8_t rd = GET_RD(ins);
			uint32_t v_rd;
			v_rd = result & 0xFFFFFFFF;
			if(b21){
				uint8_t rn = GET_RN(ins);
				uint32_t v_rn = arm_read_register(p, rn);
				v_rd = v_rd + v_rn;
			}
			arm_write_register(p, rd, v_rd);
			if(GET_S(ins)){
				int bN = (v_rd >> N) & 1;
				int bZ = (v_rd == 0);
				update_cpsr(p, bZ, bN, -1, -1);
			}
		}else{
			if(b23){
				uint8_t rhi = GET_HI(ins);
				uint8_t rlo = GET_LO(ins);
				uint32_t v_rhi;
				uint32_t v_rlo;
				if(!b22){
					uint32_t v_rhi = (result >> 32) & 0xFFFFFFFF;
					uint32_t v_rlo = result & 0xFFFFFFFF;
					if(b21){
						uint32_t tv_rhi = arm_read_register(p, rhi);
						uint32_t tv_rlo = arm_read_register(p, rlo);
						uint64_t stemp = (uint64_t)v_rlo + (uint64_t)tv_rlo;
						v_rlo = (uint32_t)stemp;
						v_rhi = v_rhi + tv_rhi;
						if (stemp > 0xFFFFFFFF) {
							v_rhi += 1; 
						}
					}
				}else{
					int64_t product = (int64_t)((int32_t)v_rm) * ((int32_t)v_rs);
					uint32_t v_rhi = (uint32_t)(product >> 32) & 0xFFFFFFFF;
					uint32_t v_rlo = (uint32_t) product & 0xFFFFFFFF;
					if(b21){
						uint32_t tv_rhi = arm_read_register(p, rhi);
						uint32_t tv_rlo = arm_read_register(p, rlo);
						uint64_t stemp = (uint64_t)v_rlo + (uint64_t)tv_rlo;
						v_rlo = (uint32_t)stemp;
						v_rhi = v_rhi + tv_rhi;
						if (stemp > 0xFFFFFFFF) {
							v_rhi += 1; 
						}
					}
				}
				arm_write_register(p, rhi, v_rhi);
				arm_write_register(p, rlo, v_rlo);
				if(GET_S(ins)){
					int bN = (v_rhi >> N) & 1;
					int bZ = (v_rhi == 0) && (v_rlo == 0);
					update_cpsr(p, bZ, bN, -1, -1);
				}
			}
		}
		return 1;
	}else if (GET_GROUP(ins) == 0b000){
		uint8_t rn = GET_RN(ins);
		uint8_t rd = GET_RD(ins);
		uint32_t ops;
		uint8_t opcd = GET_OPCD(ins);
		uint8_t s = GET_S(ins);
		
		int pc = 0;
		if(!get_bit(ins, 4)){
			if(rn != 15)
				ops = get_shift(p, ins, 0);
			else{
				uint64_t tmp;
				ops = arm_read_register(p,GET_RM(ins));
				tmp = ops + 8;
				ops = tmp;
				update_cpsr(p, -1, -1, tmp > 0xFFFFFFFF, -1); 
			}
		}else{
			if(rn != 15 && GET_RS(ins) != 15)
				ops = get_shift(p, ins, 1);
			else{
				uint64_t tmp;
				ops = arm_read_register(p,GET_RM(ins));
				tmp = ops + 8;
				ops = tmp;
				update_cpsr(p, -1, -1, tmp > 0xFFFFFFFF, -1); 
			}
		}
		dp(p, rn, rd, ops, opcd, s);
	}
    return UNDEFINED_INSTRUCTION;
}

int arm_data_processing_immediate_msr(arm_core p, uint32_t ins) {
    return UNDEFINED_INSTRUCTION;
}

