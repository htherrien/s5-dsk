/*******************************************************************************
 * Projet S5
 * @file    acquisitionSignal.h
 * @author  Hugo Therrien, Louis D'Amour, Hugo Daniel
 * @date    23 novembre 2017
 * @version 0.3
 *
 * Fonctions manipulation les données dans les tampons d'acquisition
 *
 ******************************************************************************/

#ifndef ACQUISITION_SIGNAL_H
#define ACQUISITION_SIGNAL_H

#include "signaux.h"
#include "../messagesUART/messagesUART.h"

#define TRESHOLD_X    40367877LL
#define TRESHOLD_Y   810989952LL
#define TRESHOLD_Z 15577154910LL

#define DEL_SIGNAL_PRESENT 0
#define DEL_CORRELATION_REUSSIE 1
#define DEL_CORRELATION_ECHOUEE 2

void sauvegarderAcc(DonneeAccel* echantillonAcc);

#endif /* ACQUISITION_SIGNAL_H */
