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
#ifndef __ARM_CONSTANTS_H__
#define __ARM_CONSTANTS_H__
#include <stdint.h>
#include "arm_core.h"

/* ARM Modes */
#define USR 0x10
#define FIQ 0x11
#define IRQ 0x12
#define SVC 0x13
#define ABT 0x17
#define UND 0x1b
#define SYS 0x1f

/* ARM Exceptions (by priority) */
#define RESET                   1
#define DATA_ABORT              2
#define FAST_INTERRUPT          3
#define INTERRUPT               4
#define IMPRECISE_ABORT         5	// Unsupported, ARMV6
#define PREFETCH_ABORT          6
#define UNDEFINED_INSTRUCTION   7
#define SOFTWARE_INTERRUPT      8
/* The last one is not realy an exception, but, as we handle software interrupts
 * within the simulator and we decide there to end the simulation, this is a way
 * to tell it to the outside world
 */
#define END_SIMULATION          9

/* Some CPSR bits */
#define N 31
#define Z 30
#define C 29
#define V 28

/* shift operations */
#define LSL 0
#define LSR 1
#define ASR 2
#define ROR 3

/* Bit mask constants for msr */
/* We simulate architecture v5T */
#define UnallocMask 0x0FFFFF00
#define UserMask    0xF0000000
#define PrivMask    0x0000000F
#define StateMask   0x00000020

/* *** */
#define GET_GROUP(x) get_bits(x, 27, 25)
#define GET_COND(x) get_bits(x, 31, 28)
#define GET_OPCD(x) get_bits(x, 24, 21)   
#define GET_SAT_ADDSUB_OP(x) get_bits(x, 22, 21)  
#define GET_SIGNED_MUL_OP(x) get_bits(x, 22, 21)   
#define GET_RN(x) get_bits(x, 19, 16)
#define GET_RD(x) get_bits(x, 15, 12)
#define GET_RS(x) get_bits(x, 11, 8)
#define GET_RM(x) get_bits(x, 3, 0)
#define GET_S(x) get_bit(x, 20)
#define GET_HI(x) GET_RN(x)
#define GET_LO(x) GET_RD(x)
#define GET_ROT_IMM(x)  get_bits(x, 11, 8)
#define GET_SHIFT_IMM(x) get_bits(x, 11, 7)
#define GET_SHIFT(x) get_bits(x, 6, 5)
#define GET_IMM8(x) get_bits(x, 7, 0)
#define GET_IMMH(x) get_bits(x, 11, 8)
#define GET_IMML(x) get_bits(x, 3, 0)
#define GET_IMM12(x) get_bits(x, 11, 0)
#define GET_LINK(x) get_bit(x, 24)
#define GET_L(x) get_bit(x, 20)
#define GET_W(x) get_bit(x, 21)
#define GET_B(x) get_bit(x, 22)
#define GET_U(x) get_bit(x, 23)
#define GET_P(x) get_bit(x, 24)
#define GET_R(x) get_bit(x, 22)
#define GET_X(x) get_bit(x, 5)
#define GET_Y(x) get_bit(x, 6)
#define GET_FIELD_MASK(x) get_bits(x, 19, 16)
#define GET_MISC(x) get_bits(x, 7, 4)
#define GET_Q_OP(x) get_bits(x, 22, 21)
#define GET_MISC_MUL_OP(x) get_bits(x, 22, 21)
#define IS_MULT(x) (GET_COND(x) !=  0b1111 && get_bits(x, 27, 24) == 0b0000 && get_bits(x, 7, 4) == 0b1001)  
#define IS_LS(x) (GET_COND(x) !=  0b1111 && GET_GROUP(x) == 0b000 && get_bits(x, 7, 4) == 0b1001 && !( get_bit(x ,24) && get_bits(x, 6, 5) == 0b00))    
#define IS_MISC(x) ( GET_COND(x) !=  0b1111 && !( !get_bit(x, 25) &&  get_bit(x, 7) && get_bit(x, 4)) && get_bits(x, 27, 26) == 0b00 && get_bits(x, 24, 23) == 0b10 && get_bit(x, 20) == 0b0 )

char *arm_get_exception_name(unsigned char exception);
char *arm_get_mode_name(uint8_t mode);
int8_t arm_get_mode_number(char *name);
char *arm_get_register_name(uint8_t reg);
#endif
