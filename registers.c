/*
Armator - simulateur de jeu d'instruction ARMv5T � but p�dagogique
Copyright (C) 2011 Guillaume Huard
Ce programme est libre, vous pouvez le redistribuer et/ou le modifier selon les
termes de la Licence Publique G�n�rale GNU publi�e par la Free Software
Foundation (version 2 ou bien toute autre version ult�rieure choisie par vous).

Ce programme est distribu� car potentiellement utile, mais SANS AUCUNE
GARANTIE, ni explicite ni implicite, y compris les garanties de
commercialisation ou d'adaptation dans un but sp�ciFIQue. Reportez-vous � la
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
#include "registers.h"
#include "arm_constants.h"
#include <stdlib.h>

#define NB_MODES 7
#define NB_REGS 16

static int8_t mode_index(uint8_t mode) {
    if (mode == SYS) {  // SYS
        return 6;
    } else if (mode == UND) {  // UND
        return 5;
    } else if (mode == ABT) {  // ABT
        return 4;
    } else if (mode == SVC) {  // SVC
        return 3;
    } else if (mode == IRQ) {  // IRQ
        return 2;
    } else if (mode == FIQ) {  // FIQ
        return 1;
    } else if (mode == USR){  // USR
        return 0;
    } else {
        return -1;
    }
}

static int valid_mode(uint8_t mode) {
    return mode == USR || mode == FIQ || mode == IRQ || mode == SVC || mode == ABT || mode == UND || mode == SYS; 
}
struct registers_data {
    uint32_t * r[31];
    uint32_t *registers[NB_MODES][NB_REGS]; 
    uint8_t current_mode;
    uint32_t cpsr;
    uint32_t * spsrs[NB_MODES]; 
};

static uint8_t privileged_modes[NB_MODES] = {0, 1, 1, 1, 1, 1, 1};

registers registers_create() {
    registers r = malloc(sizeof(struct registers_data));
    int l =0;
    if (r) {
        for(int m = 0; m < NB_MODES; m++){
            for(int j = 0; j < NB_REGS; j++){
                r->registers[m][j] = NULL;
                if(l < 31){
                    r->r[l] = NULL;
                    l++;
                }
            }
        }
        r->cpsr = 0;
        l = 0;
        for(int m = 0; m < NB_MODES; m++)
            r->spsrs[m] = NULL;

        for (int i = 0; i < NB_REGS; i++) {
            r->registers[0][i] = malloc(sizeof(uint32_t));
            r->r[l] = r->registers[0][i];
            l++;
            if (!r->registers[0][i]) {
                registers_destroy(r);
                return NULL;
            }
        }

        // la même adresse mémoire pour R0-R7 dans tous les modes
        for (int m = 1; m <= 6; m++) {
            for (int i = 0; i <= 7; i++) {
                r->registers[m][i] = r->registers[0][i];
            }
        }

        // FIQ a son propre R8-R12
        for (int i = 8; i <= 12; i++) {
            r->registers[1][i] = malloc(sizeof(uint32_t));
            r->r[l] = r->registers[1][i];
            l++;
            if (!r->registers[1][i]) {
                registers_destroy(r);
                return NULL;
            }
        }

        // la même adresse mémoire pour R8-R12 dans tous les modes apart FIQ
        for (int m = 2; m <= 6; m++) {
            for (int i = 8; i <= 12; i++) {
                r->registers[m][i] = r->registers[0][i];
            }
        }

        // La même adresse mémoire pour R13 et R14 entre USR et SYS
        r->registers[6][13] = r->registers[0][13];
        r->registers[6][14] = r->registers[0][14];

        // Chaque mode  a son propre R13 et R14
        for (int m = 1; m < 6; m++) {
            for (int i = 13; i <= 14; i++) {
                r->registers[m][i] = malloc(sizeof(uint32_t));
                r->r[l] = r->registers[m][i];
                l++;
                if (!r->registers[m][i]) {
                    registers_destroy(r);
                    return NULL;
                }
            }
        }
        

        // adresse mémoire pour R15 dans tous les modes
        for (int m = 1; m <= 6; m++) {
            r->registers[m][15] = r->registers[0][15];
        }

        // scpsr
        for(int m = 1; m < 6; m++){
            r->spsrs[m] = malloc(sizeof(uint32_t));
            if (!r->spsrs[m]) {
                registers_destroy(r);
                return NULL;
            }
        }

        r->cpsr = 0x00000010 | USR; // User mode, ARM state
        r->current_mode = USR;
    }
    return r;
}

void registers_destroy(registers r) {
    if (r) {
        for(int l = 0; l  <31 ; l++){
            free(r->r[l]);
        }
        for (int m = 1; m <= 6; m++) {
            if(r->spsrs[m]){
                free(r->spsrs[m]);
                r->spsrs[m] = NULL;
            }
        }
        free(r);
    }
}

 
uint8_t registers_get_mode(registers r) {
    if(!r)
        return 0;
    return r->current_mode;  
}

static int registers_mode_has_spsr(uint8_t mode) {
    return (mode != USR) && (mode != SYS);
}

int registers_current_mode_has_spsr(registers r) {
    if(r)
        return registers_mode_has_spsr(r->current_mode);
    else
        return 0;
}

int registers_in_a_privileged_mode(registers r) {
    if(r)
        return privileged_modes[mode_index(r->current_mode)];
    else
        return 0;
}

uint32_t registers_read(registers r, uint8_t reg, uint8_t mode) {
    if(r && reg < NB_REGS && reg >= 0 && valid_mode(mode)){
        return *r->registers[mode_index(mode)][reg];
    }
    else{
        return -1;
    }
}

uint32_t registers_read_cpsr(registers r) {
    if(r)
        return r->cpsr;
    else
        return 0;
}

uint32_t registers_read_spsr(registers r, uint8_t mode) {
    if ( !valid_mode(mode) || !registers_mode_has_spsr(mode)) {
        return 0;
    }
    return *r->spsrs[mode_index(mode)];
}

void registers_write(registers r, uint8_t reg, uint8_t mode, uint32_t value) {
    if(r && reg < NB_REGS && reg >= 0 && valid_mode(mode)){  
        *r->registers[mode_index(mode)][reg] = value;
    }
}

void registers_write_cpsr(registers r, uint32_t value) {
    if(r){
        if(valid_mode(0x1f&value)){
            r->cpsr = value;
            r->current_mode = 0x1f&value;
        }
    }
}

void registers_write_spsr(registers r, uint8_t mode, uint32_t value) {
    if ( valid_mode(mode) && registers_mode_has_spsr(mode)) {
        *r->spsrs[mode_index(mode)] = value;
    }
}