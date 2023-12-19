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
	}
    return UNDEFINED_INSTRUCTION;
}

int arm_data_processing_immediate_msr(arm_core p, uint32_t ins) {
    return UNDEFINED_INSTRUCTION;
}

