/*******************************************************************************
 * Projet S5
 * @file    dsk.h
 * @author  Hugo Therrien
 * @date    7 novembre 2017
 * @version 0.1
 *
 * Contient des définitions d'adresses spécifiques au DSK.
 *
 ******************************************************************************/

#ifndef DSK_H
#define DSK_H

#define DSK6713_FREQ    225000000

/* Interrupts */
#define GPINT4      0x4
#define EMURTDXRX   0xA
#define TINT0       0x1
#define TINT1       0x2


/* Registers */
#define CTL0 *(unsigned int*)           0x01940000
#define PRD0 *(unsigned int*)           0x01940004
#define CTL1 *(unsigned int*)           0x01980000
#define PRD1 *(unsigned int*)           0x01980004
#define MUXH *(unsigned int*)           0x019C0000
#define MUXL *(unsigned int*)           0x019C0004
#define CECTL3 *(unsigned int*)         0x01800014
#define GPEN *(unsigned int*)           0x01B00000
#define GPDIR *(unsigned int*)          0x01B00004
#define GPPOL *(unsigned int*)          0x01B00024

#define TLC1550_ADDR *(unsigned int*)   0xB0380000

#define SDRAM                           0x80000000
#define CPLD_USER_REG *(unsigned int*)  0x90080000

/* AIC23 addresses */
#define DSK6713_AIC23_INPUT_MIC         0x0015
#define DSK6713_AIC23_INPUT_LINE        0x0011

#endif /* DSK_H */
