/*******************************************************************************
 * Projet S5
 * @file    acquisitionSignal.c
 * @author  Hugo Therrien, Louis D'Amour, Hugo Daniel
 * @date    22 novembre 2017
 * @version 0.3
 *
 * Fait l'acquisition d'un nouvel echantillon pour la FFT ou la corrélaiton
 *
 ******************************************************************************/

#include <dsk6713_led.h>

#include "acquisitionSignal.h"
#include "moyenneMobile.h"

static Signal3Axes tamponAcquisitionFiltre; // Déja aligné

void sauvegarderAcc(DonneeAccel* echantillonAcc)
{
    #define BUFFER_SZ 512
    static DonneeAccel buffer[BUFFER_SZ];
    static int bufferIndex = 0;
    buffer[bufferIndex] = *echantillonAcc;
    bufferIndex++;
    if(BUFFER_SZ == bufferIndex)
    {
        bufferIndex = 0;
    }
}
