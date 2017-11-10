/*******************************************************************************
 * Projet S5
 * @file    acquisitionSignal.h
 * @author  Hugo Therrien
 * @date    8 novembre 2017
 * @version 0.1
 *
 * Fonctions manipulation les données dans les tampons d'acquisition
 *
 ******************************************************************************/

#ifndef ACQUISITION_SIGNAL_H
#define ACQUISITION_SIGNAL_H

#include "signaux3Axes.h"

#define TRESHOLD_X    40367877LL
#define TRESHOLD_Y   810989952LL
#define TRESHOLD_Z 15577154910LL

#define DEL_SIGNAL_PRESENT 0
#define DEL_CORRELATION_REUSSIE 1
#define DEL_CORRELATION_ECHOUEE 2


/*
 * Partie acquisiton:
 *   Faire l'acquisition d'un nouvel echantillon en fonction des DIPs en binaire.
 *   0    : signal de référence (signal_1) correlé avec un signal nul
 *   1    : signal de référence (signal_1) correlé avec signal_1 (autocorrelation)
 *   2    : signal de référence (signal_1) correlé avec signal_2 (signal du même mouvement)
 *   3    : signal de référence (signal_1) correlé avec signal_3 (signal de mouvement différent)
 *   4    : signal de référence (signal_1) correlé avec signal_4 (signal de mouvement différent)
 *   5    : signal de référence (signal_1) correlé avec signal_5 (signal de mouvement différent)
 *   6    : signal de référence (signal_1) correlé avec signal_6 (signal de mouvement différent)
 *   7    : signal de référence (signal_1) correlé avec signal_7 (signal de mouvement différent)
 *   8-15 : signal de référence (signal_1) correlé avec un signal nul
 *
 * Partie correlation:
 *   Allumer/éteindre la DEL0 pour indiquer la présence/absence d'un signal.
 *   Si aucun signal, éteindre toutes les DELs.
 *   Si un signal est présent, effectuer la corrélation.
 *     Si la corrélation est réussie, allumer la DEL1 et éteindre la DEL2
 *     Si la correlation échoue, allumer DEL2 et eintendre la DEL1
 */
void acquistionCorrelationDemo(Signal3AxesPtr *p_signalACorrelerPtr);

/*
 * Vide les tampons du signal à correler (initialise tous les éléments à 0)
 */
void viderTamponAcquisitionCorrel(Signal3AxesPtr* p_tamponAcquisitionPtr);


#endif /* ACQUISITION_SIGNAL_H */
