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




uint32_t get_shift_imm(arm_core p, uint8_t imm, uint8_t rotate_amt) {

	uint32_t shifter_op;
	uint32_t shifter_carry_out;

	if(rotate_amt == 0) {
		shifter_op = imm;
		shifter_carry_out = get_bit(arm_read_cpsr(p), C);
	}
	else {
		shifter_op = (imm >> rotate_amt*2) | (imm << (32 - rotate_amt*2)); 
		shifter_carry_out = get_bit(shifter_op, 31); 
	}
	update_cpsr(p, -1, -1, shifter_carry_out, -1);	
	return shifter_op; 
}



void logic_s(arm_core p, uint8_t s, uint8_t rd, uint32_t result){
	if(s && rd == 15){
		if(arm_current_mode_has_spsr(p)){
			arm_write_cpsr(p, arm_read_spsr(p));
		}
	} else if(s){
		update_cpsr(p, result == 0, get_bit(result, 31), -1, -1);
	}
}

void sb_s(arm_core p, uint8_t s, uint8_t rd, uint32_t result, uint32_t op1, uint32_t op2){
	if(s && rd == 15){
		if(arm_current_mode_has_spsr(p)){
			arm_write_cpsr(p, arm_read_spsr(p));
		}
	} else if(s){
		update_cpsr(p, result == 0, get_bit(result, 31), !BorrowFrom(op1, op2), OverflowFrom(op1, op2, result, 1));
	}
}
void ad_s(arm_core p, uint8_t s, uint8_t rd, uint64_t result, uint32_t op1, uint32_t op2){
	if(s && rd == 15){
		if(arm_current_mode_has_spsr(p)){
			arm_write_cpsr(p, arm_read_spsr(p));
		}
	} else if(s){
		update_cpsr(p, result==0, get_bit(result, 31), result > 0xFFFFFFFF, OverflowFrom(op1, op2, (uint32_t)result, 0));
	}
}
int dp(arm_core p, uint8_t rn, uint8_t rd, uint32_t ops, uint8_t opcd, uint8_t s){
	uint64_t tmp;
	uint32_t result = 0;
	uint32_t v_rn = arm_read_register(p, rn);
	uint8_t NCflag, Cflag;
	switch (opcd)
	{
		case AND:
			result = v_rn & ops;
			arm_write_register(p, rd, result);
			logic_s(p, s, rd, result);
			return 0;
		case EOR:
			result = v_rn ^ ops;
			arm_write_register(p, rd, result);
			logic_s(p, s, rd, result);
			return 0;
		case SUB:
			result = v_rn - ops;
			arm_write_register(p, rd, result);
			sb_s(p, s, rd, result, v_rn, ops);
			return 0;
		case RSB:
			result = ops - v_rn;
			arm_write_register(p, rd, result);
			sb_s(p, s, rd, result, ops, v_rn);
			return 0;
		case ADD:
			tmp = v_rn + ops;
			result = tmp;
			arm_write_register(p, rd, result);
			ad_s(p, s, rd, tmp, v_rn, ops);
			return 0;
		case ADC:
			Cflag = get_bit(arm_read_cpsr(p), C);
			tmp = v_rn + ops + Cflag;
			result = tmp;
			arm_write_register(p, rd, result);
			ad_s(p, s, rd, tmp, v_rn, ops + Cflag);
			return 0;
		case SBC:
			NCflag = ~get_bit(arm_read_cpsr(p), C);
			result = v_rn - ops - NCflag;
			arm_write_register(p, rd, result);
			sb_s(p, s, rd, result, v_rn, ops - NCflag);
			return 0;
		case RSC:
			NCflag = ~get_bit(arm_read_cpsr(p), C);
			result = ops - v_rn - NCflag;
			arm_write_register(p, rd, result);
			sb_s(p, s, rd, result, ops, v_rn - NCflag);
			return 0;
		case TST:
			result = v_rn & ops;
			arm_write_register(p, rd, result);
			logic_s(p, s, rd, result);
			return 0;
		case TEQ:
			result = v_rn ^ ops;
			arm_write_register(p, rd, result);
			logic_s(p, s, rd, result);
			return 0;
		case CMP:
			result = v_rn - ops;
			arm_write_register(p, rd, result);
			sb_s(p, s, rd, result, v_rn, ops);
			return 0;
		case CMN:
			tmp = v_rn + ops;
			arm_write_register(p, rd, result);
			ad_s(p, s, rd, tmp, v_rn, ops);
			return 0;
		case ORR:
			result = v_rn | ops;
			arm_write_register(p, rd, result);
			logic_s(p, s, rd, result);
			return 0;
		case MOV:
			result = ops;
			arm_write_register(p, rd, result);
			logic_s(p, s, rd, result);
			return 0;
		case BIC:
			result = v_rn & ~ ops;
			arm_write_register(p, rd, result);
			logic_s(p, s, rd, result);
			return 0;
		case MVN:
			result = ~ops;
			arm_write_register(p, rd, result);
			logic_s(p, s, rd, result);
			return 0;
		default:
			return UNDEFINED_INSTRUCTION;
	}
	return UNDEFINED_INSTRUCTION;
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
					v_rhi = (result >> 32) & 0xFFFFFFFF;
					v_rlo = result & 0xFFFFFFFF;
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
					v_rhi = (uint32_t)(product >> 32) & 0xFFFFFFFF;
					v_rlo = (uint32_t) product & 0xFFFFFFFF;
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
		return 0;
	}else if (GET_GROUP(ins) == 0b000){
		uint8_t rn = GET_RN(ins);
		uint8_t rd = GET_RD(ins);
		uint32_t ops;
		uint8_t opcd = GET_OPCD(ins);
		uint8_t s = GET_S(ins);

		if(!get_bit(ins, 4)){
			if(rn != 15)
				ops = get_shift(p, ins, 1, 0);
			else{
				//uint64_t tmp;
				ops = arm_read_register(p,GET_RM(ins));
				//tmp = ops + 8;
				//ops = tmp;
				//update_cpsr(p, -1, -1, ops > 0xFFFFFFFF, -1); 
			}
		}else{
			if(rn != 15 && GET_RS(ins) != 15)
				ops = get_shift(p, ins, 1, 1);
			else{
				//uint64_t tmp;
				ops = arm_read_register(p,GET_RM(ins));
				//tmp = ops + 8;
				//ops = tmp;
				//update_cpsr(p, -1, -1, tmp > 0xFFFFFFFF, -1); 
			}
		}
		return dp(p, rn, rd, ops, opcd, s);
	}
    return UNDEFINED_INSTRUCTION;
}

int arm_data_processing_immediate_msr(arm_core p, uint32_t ins) {
	if(IS_MISC(ins)) {
	}else {
		uint8_t rn = GET_RN(ins);
		uint8_t rd = GET_RD(ins);
		uint32_t ops = get_shift_imm(p, GET_IMM8(ins), GET_ROT_IMM(ins));
		uint8_t opcd = GET_OPCD(ins);
		uint8_t s = GET_S(ins);
		return dp(p, rn, rd, ops, opcd, s);
	}
    return UNDEFINED_INSTRUCTION;
}

