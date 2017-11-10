/*******************************************************************************
 * Projet S5
 * @file    acquisitionSignal.h
 * @author  Hugo Therrien
 * @date    8 novembre 2017
 * @version 0.1
 *
 * Fonctions manipulation les donn�es dans les tampons d'acquisition
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
 *   0    : signal de r�f�rence (signal_1) correl� avec un signal nul
 *   1    : signal de r�f�rence (signal_1) correl� avec signal_1 (autocorrelation)
 *   2    : signal de r�f�rence (signal_1) correl� avec signal_2 (signal du m�me mouvement)
 *   3    : signal de r�f�rence (signal_1) correl� avec signal_3 (signal de mouvement diff�rent)
 *   4    : signal de r�f�rence (signal_1) correl� avec signal_4 (signal de mouvement diff�rent)
 *   5    : signal de r�f�rence (signal_1) correl� avec signal_5 (signal de mouvement diff�rent)
 *   6    : signal de r�f�rence (signal_1) correl� avec signal_6 (signal de mouvement diff�rent)
 *   7    : signal de r�f�rence (signal_1) correl� avec signal_7 (signal de mouvement diff�rent)
 *   8-15 : signal de r�f�rence (signal_1) correl� avec un signal nul
 *
 * Partie correlation:
 *   Allumer/�teindre la DEL0 pour indiquer la pr�sence/absence d'un signal.
 *   Si aucun signal, �teindre toutes les DELs.
 *   Si un signal est pr�sent, effectuer la corr�lation.
 *     Si la corr�lation est r�ussie, allumer la DEL1 et �teindre la DEL2
 *     Si la correlation �choue, allumer DEL2 et eintendre la DEL1
 */
void acquistionCorrelationDemo(Signal3AxesPtr *p_signalACorrelerPtr);

/*
 * Vide les tampons du signal � correler (initialise tous les �l�ments � 0)
 */
void viderTamponAcquisitionCorrel(Signal3AxesPtr* p_tamponAcquisitionPtr);


#endif /* ACQUISITION_SIGNAL_H */
