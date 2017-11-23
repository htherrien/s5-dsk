/*******************************************************************************
 * Projet S5
 * @file    main.c
 * @author  Hugo Therrien
 * @date    7 novembre 2017
 * @version 0.4
 *
 * Fichier de démo de la correlation.
 *
 ******************************************************************************/

#include <dsk6713_dip.h>
#include <dsk6713_led.h>

#include "acquisitionSignal.h"
#include "setup.h"
#include "enableInterrupts.h"
#include "signaux3Axes.h"
#include "acquisitionSignal.h"
#include "correlations3Axes.h"
#include "FaireFFT.h"

int dipStatus = 0;
extern int signal1_x[], signal1_y[], signal1_z[];

static Signal3AxesCorr signalACorreler; // Déja aligné.

// FFT (daml2601)
static float signalAFFT[2*TAILLE_FFT];
#pragma DATA_ALIGN(signalAFFT, 8)



static Signal3AxesReference signalReference;

void initSignalReference(void)
{
    int i;
    for(i = 0; i < TAILLE_CORR; i++)
    {
        signalReference.x[i] = signal1_x[i];
        signalReference.y[i] = signal1_y[i];
        signalReference.z[i] = signal1_z[i];
    }
    autoCorreler3Axes(&signalReference);
}


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


void main(void)
{
  setup();
  initSignalReference();
  enableInterrupts();

  for(;;)
  {
      dipStatus = !DSK6713_DIP_get(0) + (!DSK6713_DIP_get(1)<<1) + (!DSK6713_DIP_get(2)<<2) + (!DSK6713_DIP_get(3)<<3);
  }
}
