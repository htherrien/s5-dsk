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

#include "signaux.h"

void correler3AxesDemo(Signal3AxesPtr* p_signalACorrelerPtr, Signal3AxesReference* p_signal3AxesReference);

/*
 * Effectue la correlation sur les 3 axes et retourne 1 si le signal � correler est suffisament semblable
 * au signal de r�f�rence
 */
int correler3Axes(Signal3AxesPtr* p_signalACorrelerPtr, Signal3AxesReference* p_signal3AxesReference);

/*
 * Remplit la structure Signal3AxesReference � partir des donn�es comprises dans ses champs x,y et z
 */
void autoCorreler3Axes(Signal3AxesReference* p_signal3AxesReference);

#endif /* CORRELATION_3_AXES_H */
