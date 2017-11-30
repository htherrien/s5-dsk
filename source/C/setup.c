/*******************************************************************************
 * Projet S5
 * @file    setup.c
 * @author  Hugo Therrien, Marc-André Roireau
 * @date    29 novembre 2017
 * @version 0.2
 *
 * Contient toutes les fonctions d'initilisation du DSP
 *
 ******************************************************************************/

#include "setup.h"

#include <dsk6713_led.h>

#include "acquisitionSignal.h"
#include "dsk.h"
#include "moyenneMobile.h"
#include "correlations3Axes.h"

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

/*!! Lors de l'acquisition, il va falloi prévoir un tampon de taille TAILLE_CORR + TAILLE_MOYENNE_MOBILE */
void initSignalReference(Signal3AxesReference* signalReference)
{
    int i;
    Signal3AxesReference tampon_acquisition;
    extern int signal1_x[], signal1_y[], signal1_z[];

    for(i = 0; i < TAILLE_CORR; i++)
    {
        tampon_acquisition.x[i] = signal1_x[i];
        tampon_acquisition.y[i] = signal1_y[i];
        tampon_acquisition.z[i] = signal1_z[i];
    }
    /* Filtrage du signal */
    for(i = 0; i < TAILLE_CORR; i++)
    {
        moyenneMobile64(&tampon_acquisition.x[i], &signalReference->x[i], TAILLE_MOYENNE_MOBILE);
        moyenneMobile64(&tampon_acquisition.y[i], &signalReference->y[i], TAILLE_MOYENNE_MOBILE);
        moyenneMobile64(&tampon_acquisition.z[i], &signalReference->z[i], TAILLE_MOYENNE_MOBILE);
    }

    autoCorreler3Axes(signalReference);
}


void pinConfig(GPIO_Handle* gpHandle, void* vectors)
{
    // Set pin4 as interrupt on falling edge
    GPIO_pinEnable(*gpHandle, GPIO_PIN4);
    GPIO_pinDirection(*gpHandle, GPIO_PIN4, GPIO_INPUT);
    GPIO_intPolarity(*gpHandle, GPIO_GPINT4, GPIO_FALLING);

    // Set pin0 as low to enable Port1 on U2 on the DSK MotherBoard
    GPIO_pinEnable(*gpHandle, GPIO_PIN0);
    GPIO_pinDirection(*gpHandle, GPIO_PIN0, GPIO_OUTPUT);
    GPIO_pinWrite(*gpHandle, GPIO_PIN0, 0);

    // Enable PIN4 interrupt
    IRQ_setVecs(vectors);
    IRQ_map(IRQ_EVT_GPINT4, 4);
    IRQ_reset(IRQ_EVT_GPINT4);
    IRQ_enable(IRQ_EVT_GPINT4);
}
