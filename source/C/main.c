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

int dipStatus = 0;
extern int signal1_x[], signal1_y[], signal1_z[];

static Signal3AxesCorr signalACorreler; // Déja aligné.

static Signal3AxesReference signalReference;

void initSignalReference(void)
{
    int i, signalfiltre[64];
    for(i = 0; i < 64; i++)
    {
        signalfiltre[i] = 0;
    }
    //Filtrage des donnees
    for (i = 5;i<64;i++)
    {
        signalfiltre[i] = FIR1(signal1_x);
    }


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
    if(resultat)
    {
        DSK6713_LED_on(0);
    }
    else
    {
        DSK6713_LED_off(0);
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
