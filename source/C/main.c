/*******************************************************************************
 * Projet S5
 * @file    main.c
 * @author  Hugo Therrien, Hugo Daniel, Louis D'Amour
 * @date    23 novembre 2017
 * @version 0.5
 *
 * Fichier de démo de la correlation.
 *
 ******************************************************************************/

#include <dsk6713_dip.h>
#include <dsk6713_led.h>

#include "acquisitionSignal.h"
#include "setup.h"
#include "enableInterrupts.h"
#include "signaux.h"
#include "acquisitionSignal.h"
#include "correlations3Axes.h"
#include "FaireFFT.h"
#include "moyenneMobile.h"

int dipStatus = 0;
extern int signal1_x[], signal1_y[], signal1_z[];
//extern int position;

static Signal3Axes signalACorreler; // Déja aligné.

// FFT (daml2601)
static float signalAFFT[2*TAILLE_FFT];
#pragma DATA_ALIGN(signalAFFT, 8)

static Signal3AxesReference signalReference;

/*!! Lors de l'acquisition, il va falloi prévoir un tampon de taille TAILLE_CORR + TAILLE_MOYENNE_MOBILE */
void initSignalReference(void)
{
    int i;
    Signal3AxesReference tampon_acquisition;
    for(i = 0; i < TAILLE_CORR; i++)
    {
        tampon_acquisition.x[i] = signal1_x[i];
        tampon_acquisition.y[i] = signal1_y[i];
        tampon_acquisition.z[i] = signal1_z[i];
    }
    /* Filtrage du signal */
    for(i = 0; i < TAILLE_CORR; i++)
    {
        moyenneMobile64(&tampon_acquisition.x[i], &signalReference.x[i], TAILLE_MOYENNE_MOBILE);
        moyenneMobile64(&tampon_acquisition.y[i], &signalReference.y[i], TAILLE_MOYENNE_MOBILE);
        moyenneMobile64(&tampon_acquisition.z[i], &signalReference.z[i], TAILLE_MOYENNE_MOBILE);
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

int SIGNAL_A_FILTRER[TAILLE_CORR];
#pragma DATA_ALIGN(SIGNAL_A_FILTRER, TAILLE_CORR*sizeof(int));

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
