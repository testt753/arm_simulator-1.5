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
#include "arm_exception.h"
#include "arm_constants.h"
#include "arm_core.h"
#include "util.h"

// Not supported below ARMv6, should read as 0
#define CP15_reg1_EEbit 0
#define Exception_bit_9 (CP15_reg1_EEbit << 9)

int arm_exception(arm_core p, uint8_t exception) {
    uint32_t cpsr = 0x1d3 | Exception_bit_9;
    uint32_t old_cpsr = arm_read_cpsr(p);
    uint32_t old_pc = arm_read_register(p, 15);
    /* As there is no operating system in our simulator, we handle
     * software interrupts here :
     * - 0x123456 is the end of the simulation
     * - other opcodes can be used for any custom behavior,
     *   such as my_putchar given as an example
     */
    if (exception == SOFTWARE_INTERRUPT) {
        cpsr = arm_read_cpsr(p);
        cpsr = set_bits(cpsr, 4, 0, 0b10011);
        cpsr = clr_bit(cpsr, 5);
        cpsr =set_bit(cpsr, 7);
        cpsr = clr_bit(cpsr, 9);
        uint32_t value;
        uint32_t address = arm_read_register(p, 15);
        address -= 8;
        uint32_t instruction;
        arm_read_word(p, address, &instruction);
        instruction &= 0xFFFFFF;
        switch (instruction) {
        case 0x123456:
            return END_SIMULATION;
        case 0x000001:
            value = arm_read_register(p, 0);
            putchar(value);
            return 0;
        }
    }
    /* Aside from SWI, we only support RESET initially */
    /* Semantics of reset interrupt (ARM manual A2-18) */
    if (exception == RESET) {
        arm_write_cpsr(p, cpsr);
        arm_write_spsr(p, old_cpsr);
        arm_write_register(p, 14, old_pc);
        arm_write_register(p, 15, 0);
    }else if(exception == UNDEFINED_INSTRUCTION){
        cpsr = arm_read_cpsr(p);
        cpsr = set_bits(cpsr, 4, 0, 0b11011);
        cpsr = clr_bit(cpsr, 5);
        cpsr =set_bit(cpsr, 7);
        cpsr = clr_bit(cpsr, 9);
        arm_write_cpsr(p, cpsr);
        arm_write_spsr(p, old_cpsr);
        arm_write_register(p, 14, old_pc - 4);
        /*traitement PC*/
        uint32_t instruction;
        arm_read_word(p, old_pc-8, &instruction);
        printf("Undefined instruction 0x%08x at 0x%08x\n", instruction, old_pc-8); 
        /**/
        arm_write_register(p, 15, arm_read_register(p, 14));
        arm_write_cpsr(p, arm_read_spsr(p));

    } else if(exception == PREFETCH_ABORT){
        cpsr = arm_read_cpsr(p);
        cpsr = set_bits(cpsr, 4, 0, 0b10111);
        cpsr = clr_bit(cpsr, 5);
        cpsr =set_bit(cpsr, 7);
        cpsr = clr_bit(cpsr, 9);
        arm_write_cpsr(p, cpsr);
        arm_write_spsr(p, old_cpsr);
        arm_write_register(p, 14, old_pc - 4);
        /*traitement PC*/
        printf("PREFETCH ABORT\n"); 
        return END_SIMULATION;
        /**/
        arm_write_register(p, 15, arm_read_register(p, 14) - 4);
        arm_write_cpsr(p, arm_read_spsr(p));
    } else if(exception == DATA_ABORT){
        cpsr = arm_read_cpsr(p);
        cpsr = set_bits(cpsr, 4, 0, 0b10111);
        cpsr = clr_bit(cpsr, 5);
        cpsr =set_bit(cpsr, 7);
        cpsr = clr_bit(cpsr, 9);
        arm_write_cpsr(p, cpsr);
        arm_write_spsr(p, old_cpsr);
        arm_write_register(p, 14, old_pc);
        /*traitement PC*/
        uint32_t instruction;
        arm_read_word(p, old_pc-8, &instruction);
        printf("DATA ABORT 0x%08x at 0x%08x\n", instruction, old_pc-8); 
        /**/
        arm_write_register(p, 15, arm_read_register(p, 14) - 4);
        arm_write_cpsr(p, arm_read_spsr(p));
    } else if(exception == INTERRUPT){
        cpsr = arm_read_cpsr(p);
        cpsr = set_bits(cpsr, 4, 0, 0b10010);
        cpsr = clr_bit(cpsr, 5);
        cpsr =set_bit(cpsr, 7);
        cpsr = clr_bit(cpsr, 9);
        arm_write_cpsr(p, cpsr);
        arm_write_spsr(p, old_cpsr);
        arm_write_register(p, 14, old_pc);
        /*traitement PC*/
        uint32_t instruction;
        arm_read_word(p, old_pc-8, &instruction);
        printf("IRQ\n"); 
        /**/
        arm_write_register(p, 15, arm_read_register(p, 14) - 4);
        arm_write_cpsr(p, arm_read_spsr(p));
    }else if(exception == FAST_INTERRUPT){
        cpsr = arm_read_cpsr(p);
        cpsr = set_bits(cpsr, 4, 0, 0b10010);
        cpsr = clr_bit(cpsr, 5);
        cpsr =set_bit(cpsr, 6);
        cpsr =set_bit(cpsr, 7);
        cpsr = clr_bit(cpsr, 9);
        arm_write_cpsr(p, cpsr);
        arm_write_spsr(p, old_cpsr);
        arm_write_register(p, 14, old_pc);
        /*traitement PC*/
        uint32_t instruction;
        arm_read_word(p, old_pc-8, &instruction);
        printf("FIQ\n"); 
        /**/
        arm_write_register(p, 15, arm_read_register(p, 14) - 4);
        arm_write_cpsr(p, arm_read_spsr(p));
    }
    return exception;
}
