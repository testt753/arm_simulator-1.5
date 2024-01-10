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
#include "arm_branch_other.h"
#include "arm_constants.h"
#include "util.h"
#include <debug.h>
#include <stdlib.h>


int arm_branch(arm_core p, uint32_t ins) {
    if(GET_COND(ins) == 0xF){
        return UNDEFINED_INSTRUCTION;
    }else{
        uint32_t offset = ins & 0x00FFFFFF;
        int32_t signed_offset = (int32_t)(offset << 8) >> 6;
        uint32_t pc = arm_read_register(p, 15);
        if(GET_LINK(ins)){
            arm_write_register(p, 14, pc - 4);
        }
        arm_write_register(p, 15, pc + signed_offset);
        return 0;
    }
    return UNDEFINED_INSTRUCTION;
}

int arm_coprocessor_others_swi(arm_core p, uint32_t ins) {
    if (get_bit(ins, 24)) {
        return SOFTWARE_INTERRUPT;
    }
    return UNDEFINED_INSTRUCTION;
}

int arm_miscellaneous(arm_core p, uint32_t ins) {
    switch(GET_MISC(ins)){
        case 0b000:
            if(get_bit(ins, 21)){
                uint32_t operand = arm_read_register(p, GET_RM(ins));
                if(operand & UnallocMask){
                    fprintf(stderr, "UNPREDICTABLE");
                }
                uint32_t byte_mask =   (get_bit(GET_FIELD_MASK(ins), 0) ? 0x000000FF : 0x00000000)
                                    | (get_bit(GET_FIELD_MASK(ins), 1) ? 0x0000FF00 : 0x00000000)
                                    | (get_bit(GET_FIELD_MASK(ins), 2) ? 0x00FF0000 : 0x00000000)
                                    | (get_bit(GET_FIELD_MASK(ins), 3) ? 0xFF000000 : 0x00000000);
                uint32_t mask;
                if(!GET_R(ins)){
                    if(arm_in_a_privileged_mode(p)){
                        if(operand & StateMask)
                            fprintf(stderr, "UNPREDICTABLE");
                        else{
                            mask = byte_mask & (UserMask | PrivMask);
                        }
                    }else{
                        mask = byte_mask & UserMask;
                    }
                    arm_write_cpsr(p, (arm_read_cpsr(p) & ~mask) | (operand & mask));
                }
            }else{
                if(GET_RD(ins) == 15)
                    fprintf(stderr, "UNPREDICTABLE");
                if(GET_R(ins)){
                    arm_write_register(p, GET_RD(ins), arm_read_spsr(p));
                }else{
                    arm_write_register(p, GET_RD(ins), arm_read_cpsr(p));
                }
            }
            return 0;
        case 0b0001:
            if(get_bit(ins, 22)){
                if(GET_RD(ins) == 15 || GET_RM(ins) == 15)
                    fprintf(stderr, "UNPREDICTABLE");
                if(!arm_read_register(p, GET_RM(ins))){
                    arm_write_register(p, GET_RD(ins), 32);
                }else{
                    int i = 32;
                    while(!get_bit(ins, --i));
                    arm_write_register(p, GET_RD(ins), 31 - i);
                }
            }else{
                uint32_t v_rm = arm_read_register(p, GET_RM(ins));
                if(get_bits(v_rm, 1, 0) == 0b10)
                    fprintf(stderr, "UNPREDICTABLE");
                if(get_bit(v_rm, 0))
                    arm_write_cpsr(p, set_bit(arm_read_cpsr(p), 5));
                else
                    arm_write_cpsr(p, clr_bit(arm_read_cpsr(p), 5));
                
                arm_write_register(p, 15, v_rm & 0xFFFFFFFE);
            }
            return 0;
        case 0b0011:
            uint32_t target = arm_read_register(p, GET_RM(ins));
            if(get_bits(target, 1, 0) == 0b10)
                fprintf(stderr, "UNPREDICTABLE");
            
            arm_write_register(p, 14, arm_read_register(p, 15) - 4);
            if(get_bit(target, 0))
                arm_write_cpsr(p, set_bit(arm_read_cpsr(p), 5));
            else
                arm_write_cpsr(p, clr_bit(arm_read_cpsr(p), 5));
                
            arm_write_register(p, 15, target & 0xFFFFFFFE);
            return 0;
        case 0b0101:
            int32_t v_rd;
            if(GET_RD(ins) == 15 || GET_RM(ins) == 15 || GET_RN(ins) == 15)
                fprintf(stderr, "UNPREDICTABLE");
            switch(GET_Q_OP(ins)){
                case 0b00:
                    v_rd = SignedSat(arm_read_register(p, GET_RM(ins)) + arm_read_register(p, GET_RN(ins)), 32);
                    arm_write_register(p, GET_RD(ins), (uint32_t) v_rd);
                    if(SignedDoesSat(v_rd, 32)){
                        arm_write_cpsr(p, set_bit(arm_read_cpsr(p), 27));
                    }
                    return 0;
                case 0b01:
                    v_rd = SignedSat(arm_read_register(p, GET_RM(ins)) - arm_read_register(p, GET_RN(ins)), 32);
                    arm_write_register(p, GET_RD(ins), (uint32_t) v_rd);
                    if(SignedDoesSat(v_rd, 32)){
                        arm_write_cpsr(p, set_bit(arm_read_cpsr(p), 27));
                    }
                    return 0;
                case 0b10:
                    int32_t tmp = SignedSat(arm_read_register(p, GET_RN(ins)) * 2, 32);
                    v_rd = SignedSat(arm_read_register(p, GET_RM(ins)) + tmp, 32);
                    arm_write_register(p, GET_RD(ins), (uint32_t) v_rd);
                    if(SignedDoesSat(v_rd, 32) || SignedDoesSat(tmp, 32)){
                        arm_write_cpsr(p, set_bit(arm_read_cpsr(p), 27));
                    }
                    return 0;
                case 0b11:
                    tmp = SignedSat(arm_read_register(p, GET_RN(ins)) * 2, 32);
                    v_rd = SignedSat(arm_read_register(p, GET_RM(ins)) - tmp, 32);
                    arm_write_register(p, GET_RD(ins), (uint32_t) v_rd);
                    if(SignedDoesSat(v_rd, 32) || SignedDoesSat(tmp, 32)){
                        arm_write_cpsr(p, set_bit(arm_read_cpsr(p), 27));
                    }
                    return 0;
                default:
                    return UNDEFINED_INSTRUCTION;
            }
        case 0b0111:
            if(GET_COND(ins) != 0b1110)
                return PREFETCH_ABORT;
            break;
        case 0b1000:
        case 0b1010:
        case 0b1100:
        case 0b1110:
            if(GET_RD_MULT(ins) == 15 || GET_RM(ins) == 15 || GET_RN_MULT(ins) == 15)
                fprintf(stderr, "UNPREDICTABLE");
            int32_t operand1, operand2;
            switch(GET_MISC_MUL_OP(ins)){
                case 0b00:            
                    // Signe-étend selon les valeurs de x et y
                    operand1 = GET_X(ins) ? (int32_t)((int16_t)(arm_read_register(p, GET_RM(ins)) >> 16)) : (int32_t)((int16_t)(arm_read_register(p, GET_RM(ins))));
                    operand2 = GET_Y(ins) ? (int32_t)((int16_t)(arm_read_register(p, GET_RS(ins)) >> 16)) : (int32_t)((int16_t)(arm_read_register(p, GET_RS(ins))));
                    
                    v_rd = operand1 * operand2 + (int32_t) arm_read_register(p, GET_RN_MULT(ins));

                    // Vérification du débordement
                    if (OverflowFrom(operand1 * operand2, (int32_t) arm_read_register(p, GET_RN_MULT(ins)), v_rd, 0)) {
                        arm_write_cpsr(p, set_bit(arm_read_cpsr(p), 27));
                    } else {
                        arm_write_cpsr(p, clr_bit(arm_read_cpsr(p), 27));
                    }
                    return 0;
                case 0b01:
                    operand2 = GET_Y(ins) ? (int32_t)((int16_t)(arm_read_register(p, GET_RS(ins)) >> 16)) : (int32_t)((int16_t)(arm_read_register(p, GET_RS(ins))));
                    if(GET_X(ins)){
                        int64_t tmp1 = operand2 * arm_read_register(p, GET_RM(ins));
                        arm_write_register(p, GET_RD_MULT(ins), (uint32_t)(tmp1 >> 16));
                    }else{
                        int64_t tmp1 = operand2 * arm_read_register(p, GET_RM(ins));
                        v_rd = (int32_t)(tmp1 >> 16) + arm_read_register(p, GET_RN_MULT(ins));
                        arm_write_register(p, GET_RD_MULT(ins), v_rd);
                        if (OverflowFrom(tmp1, arm_read_register(p, GET_RN_MULT(ins)), v_rd, 0)) {
                            arm_write_cpsr(p, set_bit(arm_read_cpsr(p), 27));
                        } else {
                            arm_write_cpsr(p, clr_bit(arm_read_cpsr(p), 27));
                        }
                    }
                    return 0;
                case 0b10:
                    operand1 = GET_X(ins) ? (int32_t)((int16_t)(arm_read_register(p, GET_RM(ins)) >> 16)) : (int32_t)((int16_t)(arm_read_register(p, GET_RM(ins))));
                    operand2 = GET_Y(ins) ? (int32_t)((int16_t)(arm_read_register(p, GET_RS(ins)) >> 16)) : (int32_t)((int16_t)(arm_read_register(p, GET_RS(ins))));
                    uint32_t v_rhi = arm_read_register(p, GET_HI(ins));
                    uint32_t v_rlo = arm_read_register(p, GET_LO(ins));
                    uint64_t tmp = (operand1 * operand2) + v_rlo;
                    arm_write_register(p, GET_LO(ins), tmp);
                    v_rhi = v_rhi + (operand1 * operand2 > 0 ? 0xFFFFFFFF : 0) + (tmp > 0xFFFFFFFF);
                    arm_write_register(p, GET_HI(ins), v_rhi);
                    return 0;
                case 0b11:
                    operand1 = GET_X(ins) ? (int32_t)((int16_t)(arm_read_register(p, GET_RM(ins)) >> 16)) : (int32_t)((int16_t)(arm_read_register(p, GET_RM(ins))));
                    operand2 = GET_Y(ins) ? (int32_t)((int16_t)(arm_read_register(p, GET_RS(ins)) >> 16)) : (int32_t)((int16_t)(arm_read_register(p, GET_RS(ins))));
                    arm_write_register(p, GET_RD_MULT(ins), operand1 * operand2);
                    return 0;
                default:
                    return UNDEFINED_INSTRUCTION;
            }
        default:
           return UNDEFINED_INSTRUCTION; 
    }
    return UNDEFINED_INSTRUCTION;
}
