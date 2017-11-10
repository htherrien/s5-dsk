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

#include <acquisition_signal.h>
#include <dsk6713_dip.h>
#include <dsk6713_led.h>

#include "setup.h"
#include "enable_interrupts.h"
#include "signaux3axes.h"
#include "acquisition_signal.h"
#include "correlations3axes.h"

int dip_status = 0;
extern int signal1_x[], signal1_y[], signal1_z[];

static Signal3AxesCorr signalACorreler; // Déja aligné.

static Signal3AxesReference signalReference;

void init_signal_reference(void)
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
    acquistion_correlation_demo(&signalACorrelerPtr);
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
  init_signal_reference();
  enable_interrupts();

  for(;;)
  {
      dip_status = !DSK6713_DIP_get(0) + (!DSK6713_DIP_get(1)<<1) + (!DSK6713_DIP_get(2)<<2) + (!DSK6713_DIP_get(3)<<3);
  }
}
