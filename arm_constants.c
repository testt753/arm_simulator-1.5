/*
Armator - simulateur de jeu d'instruction ARMv5T � but p�dagogique
Copyright (C) 2011 Guillaume Huard
Ce programme est libre, vous pouvez le redistribuer et/ou le modifier selon les
termes de la Licence Publique G�n�rale GNU publi�e par la Free Software
Foundation (verion 2 ou bien toute autre verion ult�rieure choisie par vous).

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
	 700 avenue centrale, domaine univeritaire
	 38401 Saint Martin d'H�res
*/
#include <stdlib.h>
#include <string.h>
#include "arm_constants.h"
#include "util.h"

static char *arm_mode_names[] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, "USR", "FIQ", "IRQ", "SVC", 0, 0, 0,
    "ABT", 0, 0, 0, "UND", 0, 0, 0, "SYS"
};

static char *arm_register_names[] = { "R00", "R01", "R02", "R03", "R04", "R05",
    "R06", "R07", "R08", "R09", "R10", "R11",
    "R12", "SP", "LR", "PC", "CPSR", "SPSR"
};

static char *arm_exception_names[] = { NULL,
    "reset",
    "data abort",
    "fast interrupt",
    "interrupt",
    "imprecise abort",
    "prefetch abort",
    "undefined instruction",
    "software interrupt",
};

char *arm_get_exception_name(unsigned char exception) {
    if (exception < 8)
        return arm_exception_names[exception];
    else
        return NULL;
}

char *arm_get_mode_name(uint8_t mode) {
    return arm_mode_names[mode];
}

int8_t arm_get_mode_number(char *name) {
    for (int i = 0; i < 32; i++)
        if (arm_mode_names[i] && strcmp(name, arm_mode_names[i]) == 0)
            return i;
    return -1;
}

char *arm_get_register_name(uint8_t reg) {
    return arm_register_names[reg];
}

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

uint32_t get_shift(arm_core p, uint32_t ins, int S, int reg) {

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
	    update_cpsr(p, -1, -1, shifter_carry_out, -1);
	return result;
}