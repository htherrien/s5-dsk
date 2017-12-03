/*******************************************************************************
 * Projet S5
 * @file    interruptions.c
 * @author  Hugo Therrien, Hugo Daniel, Louis D'Amour, Marc-André Roireau
 * @date    29 novembre 2017
 * @version 0.1
 *
 * Contient les interruptions
 *
 ******************************************************************************/

#include <DSK6713_led.h>
#include <csl_mcbsp.h>
#include <DSK6713.h>

#include "signaux.h"
#include "acquisitionSignal.h"
#include "correlations3Axes.h"
#include "faireFFT.h"
#include "SPI_driver.h"

// FFT (daml2601)
static float signalAFFT[2*TAILLE_FFT];
#pragma DATA_ALIGN(signalAFFT, 8)

static Signal3Axes signalACorreler; // Déja aligné.
Signal3AxesReference signalReference;

unsigned char MCBSP0SendBuffer[32];
unsigned char MCBSP1SendBuffer[32];

interrupt void intTimer0(void)
{
    static Signal3AxesPtr signalACorrelerPtr = {signalACorreler.x, signalACorreler.y, signalACorreler.z};
    int resultat;
    acquistionCorrelationDemo(&signalACorrelerPtr);
    resultat = correler3Axes(&signalACorrelerPtr, &signalReference);

    // FFT (daml2601)
    int resultat_fft = 0;
    static int i = 0;
    acquistionCorrelationDemoFFT(&signalAFFT[i]);
    i = i + 2;
    if (i == 2*TAILLE_FFT)
    {
        resultat_fft = faireFFT(signalAFFT);
        i = 0;
    }

    if(resultat)
    {
        DSK6713_LED_on(0);
    }
    else
    {
        DSK6713_LED_off(0);
    }
    if(resultat_fft)
    {
        DSK6713_LED_on(3);
    }
    else
    {
        DSK6713_LED_off(3);
    }
}

/*
 * Interruption d'envoi de données UART
 */
interrupt void intTimer1(void)
{
    ;
}


/*
 * Interruption de réception de données du PIC.
 */
interrupt void c_int04(void)
{
    static char UARTData;
    extern MCBSP_Handle MCBSP0Handle;

    MCBSP_write(MCBSP0Handle, SPI_READ_DATA);
    DSK6713_waitusec(10);
    UARTData = (MCBSP_read(MCBSP0Handle) & 0xFF);
}

/*
 * Interruption de réception de données de l'ordinateur.
 */
interrupt void c_int05(void)
{
    static char UARTData;
    extern MCBSP_Handle MCBSP1Handle;

    MCBSP_write(MCBSP1Handle, SPI_READ_DATA);
    DSK6713_waitusec(10);
    UARTData = (MCBSP_read(MCBSP1Handle) & 0xFF);
}
