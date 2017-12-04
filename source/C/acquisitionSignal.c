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


extern int dipStatus;

static Signal3Axes tamponAcquisitionFiltre; // Déja aligné
