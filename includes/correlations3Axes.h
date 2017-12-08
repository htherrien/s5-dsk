/*******************************************************************************
 * Projet S5
 * @file    correlations3Axes.h
 * @author  Hugo Therrien
 * @date    8 novembre 2017
 * @version 0.2
 *
 * D�finit les fonctions de corr�lation sur les 3 axes.
 *
 ******************************************************************************/

#ifndef CORRELATION_3_AXES_H
#define CORRELATION_3_AXES_H

#include <stdint.h>
#include "signaux.h"

/*
 * Effectue la correlation sur les 3 axes et retourne la plus faible valeur de corr�lation en pourcentage
 */
uint8_t correler3Axes(Signal3AxesPtr* p_signalACorrelerPtr, Signal3AxesReference* p_signal3AxesReference);

/*
 * Remplit la structure Signal3AxesReference � partir des donn�es comprises dans ses champs x,y et z
 */
void autoCorreler3Axes(Signal3AxesReference* p_signal3AxesReference);

#endif /* CORRELATION_3_AXES_H */
