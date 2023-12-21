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

int arm_load_store(arm_core p, uint32_t ins) {
    uint8_t rd = GET_RD(ins);
    uint8_t rn = GET_RN(ins);
    uint32_t addr;
    if(GET_GROUP(ins) == 0b010){
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
            uint8_t rm = GET_RM(ins);
            uint32_t index;
            if(!GET_SHIFT_IMM(ins) && !GET_SHIFT(ins)){
                index = arm_read_register(p, rm);
            }else{
                index = get_shift(p, ins, 0, 0);
            }
            if(GET_U(ins))
                addr = arm_read_register(p, rn) + index;
            else
                addr = arm_read_register(p, rn) - index;
            if(GET_W(ins)){
                arm_write_register(p, rn, addr);
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
            arm_write_register(p, 15, data&= 0xFFFFFFFE);
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
            arm_write_word(p, addr, arm_read_register(p, rd));
        }
        return 0;
    }
    return UNDEFINED_INSTRUCTION;
}

int arm_load_store_multiple(arm_core p, uint32_t ins) {
    return UNDEFINED_INSTRUCTION;
}

int arm_coprocessor_load_store(arm_core p, uint32_t ins) {
    /* Not implemented */
    return UNDEFINED_INSTRUCTION;
}