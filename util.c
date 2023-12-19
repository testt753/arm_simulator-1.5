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
#include "util.h"

/* We implement asr because shifting a signed is non portable in ANSI C */
uint32_t asr(uint32_t value, uint8_t shift) {
    return (value >> shift) | (get_bit(value, 31) ? ~0 << (32 - shift) : 0);
}

uint32_t ror(uint32_t value, uint8_t rotation) {
    return (value >> rotation) | (value << (32 - rotation));
}

int is_big_endian() {
    static uint32_t one = 1;
    return ((*(uint8_t *) & one) == 0);
}

int OverflowFrom(int32_t op1, int32_t op2, int32_t result, int isSub) {

    int op1Sign = (op1 >> 31) & 0x1;
    int op2Sign = (op2 >> 31) & 0x1;
    int resSign = (result >> 31) & 0x1;

    if(isSub) {
        // Soustraction
        if(op1Sign != op2Sign && op1Sign != resSign) {
            return 1; 
        }
    }
    else {
        // Addition  
        if(op1Sign == op2Sign && op1Sign != resSign) {
            return 1;
        }
    }

    return 0; 
}

int BorrowFrom(uint32_t op1, uint32_t op2) {

  if(op1 < op2) {
    // op2 ne peut pas être soustrait à op1 
    // sans emprunter (retenue)
    return 1; 
  } else {
    return 0;
  }
  
}