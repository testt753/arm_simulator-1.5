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
#include "arm_instruction.h"
#include "arm_exception.h"
#include "arm_data_processing.h"
#include "arm_load_store.h"
#include "arm_branch_other.h"
#include "arm_constants.h"
#include "util.h"

// Dans arm_instruction.c


#define COND_EQ = 0x0
#define COND_NE = 0x1
#define COND_CS = 0x2  
#define COND_CC = 0x3
#define COND_MI = 0x4
#define COND_PL = 0x5
#define COND_VS = 0x6
#define COND_VC = 0x7
#define COND_HI = 0x8
#define COND_LS = 0x9 
#define COND_GE = 0xA
#define COND_LT = 0xB  
#define COND_GT = 0xC 
#define COND_LE = 0xD 
#define COND_AL = 0xE

int check_condition(arm_core r, uint8_t cond) {
    uint32_t cpsr  = arm_read_cpsr(r);

    switch(cond) {

        case 0x0:
            return (cpsr & Z) != 0;

        case 0x1:
            return (cpsr & Z) == 0;

        case 0x2:
            return (cpsr & C) != 0;

        case 0x3:
            return (cpsr & C) == 0;

        case 0x4:
            return (cpsr & N) != 0;

        case 0x5:
            return (cpsr & N) == 0;

        case 0x6:
            return (cpsr & V) != 0; 

        case 0x7:
            return (cpsr & V) == 0;

        case 0x8:
            return (cpsr & C) != 0 && (cpsr & Z) == 0;
        
        case 0x9:
            return (cpsr & C) == 0 && (cpsr & Z) != 0;
        
        case 0xA:
            return ((cpsr & N) == (cpsr & V));

        case 0xB:
            return ((cpsr & N) != (cpsr & V));

        case 0xC:
            return ((cpsr & Z) == 0 && (cpsr & N) == (cpsr & V));

        case 0xD:
            return ((cpsr & Z) != 0 && (cpsr & N) != (cpsr & V));

        case 0xE:
            return 1; 

        default:
            return 0;
    }  
}

int execute_instruction(arm_core p, uint32_t x) {
    switch (GET_GROUP(x)) {
        case 0b000:
            if (IS_MULT(x))  
                return arm_data_processing_shift(p, x);
            else if (IS_LS(x))
                return arm_load_store(p, x);
            else if (IS_MISC(x)) 
                return arm_miscellaneous(p, x);
            else {
                return arm_data_processing_shift(p, x);
            }

        case 0b001:
            return arm_data_processing_immediate_msr(p, x);
            
        case 0b010:
            return arm_load_store_multiple(p, x);
        
        case 0b011:
            return arm_load_store(p, x);

        case 0b100:
            return arm_load_store_multiple(p, x);

        case 0b101:
            return arm_branch(p, x);

        case 0b110:
            return arm_coprocessor_load_store(p, x);

        case 0b111:
            return rm_coprocessor_others_swi(p, x);

        default:
            return -1;
    }
}

int arm_execute_instruction(arm_core p) {
    uint32_t instruction = 0;
    if (!p)  
        return 0;
    int error = arm_fetch(p, &instruction);
    if (error == -1) 
        return 0;

    if(check_condition(p, GET_COND(instruction))){
        return execute_instruction(p, instruction);
    }
    return 0;
}

int arm_step(arm_core p) {
    int result;

    result = arm_execute_instruction(p);
    if (result) {
        return arm_exception(p, result);
    }
    return result;
}
