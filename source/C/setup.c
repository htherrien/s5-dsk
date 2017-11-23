/*******************************************************************************
 * Projet S5
 * @file    setup.c
 * @author  Hugo Therrien
 * @date    7 novembre 2017
 * @version 0.1
 *
 * Contient toutes les fonctions d'initilisation du DSP
 *
 ******************************************************************************/

#include <dsk6713_led.h>

#include "acquisitionSignal.h"
#include "setup.h"
#include "dsk.h"

void setup(void)
{
    DSK6713_LED_init();
    configAndStartTimer0AsClock300Hz();
}

void configAndStartTimer0AsClock10Hz(void)
{
    CTL0 &= ~0x00000080; /* Put clock in hold */
    PRD0  = 2812500;     /* Set clock frequency to 10 Hz */
    CTL0 |= 0x00000200; /* Set internal clock source */
    CTL0 |= 0x00000100; /* Set as clock mode */
    CTL0 |= 0x00000004; /* TSTAT is driven on TOUT */
    CTL0 |= 0x00000001; /* TOUT is a timer output pin */
    CTL0 |= 0x000000C0; /* Start timer */
}

void configAndStartTimer0AsClock30Hz(void)
{
    CTL0 &= ~0x00000080; /* Put clock in hold */
    PRD0  = 937500;     /* Set clock frequency to 30 Hz */
    CTL0 |= 0x00000200; /* Set internal clock source */
    CTL0 |= 0x00000100; /* Set as clock mode */
    CTL0 |= 0x00000004; /* TSTAT is driven on TOUT */
    CTL0 |= 0x00000001; /* TOUT is a timer output pin */
    CTL0 |= 0x000000C0; /* Start timer */
}

void configAndStartTimer0AsClock300Hz(void)
{
    CTL0 &= ~0x00000080; /* Put clock in hold */
    PRD0  = 93750;     /* Set clock frequency to 300 Hz */
    CTL0 |= 0x00000200; /* Set internal clock source */
    CTL0 |= 0x00000100; /* Set as clock mode */
    CTL0 |= 0x00000004; /* TSTAT is driven on TOUT */
    CTL0 |= 0x00000001; /* TOUT is a timer output pin */
    CTL0 |= 0x000000C0; /* Start timer */
}
