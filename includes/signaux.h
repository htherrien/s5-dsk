/*******************************************************************************
 * Projet S5
 * @file    signaux.h
 * @author  Hugo Therrien
 * @date    8 novembre 2017
 * @version 0.1
 *
 * Définit des structures de données utiles pour les signaux à 3 axes
 *
 *
 ******************************************************************************/

#ifndef SIGNAUX_H
#define SIGNAUX_H

#include <cassert.h>

#define TAILLE_CORR 64

#define TAILLE_FFT 256

#define TAILLE_MOYENNE_MOBILE 15

#define FE 400 /* Fréquence d'échantillonage */

#define FSE 40 /* Fréquence de sous-échantillonage */

#define FACTEUR_L (FE/FSE)

/* Vérifie que la MOYENNE a un taille plus petite que la correlation */
CASSERT(TAILLE_MOYENNE_MOBILE <= TAILLE_CORR, signaux)

typedef struct
{
    int x[TAILLE_CORR];
    int y[TAILLE_CORR];
    int z[TAILLE_CORR];
}Signal3Axes;
#pragma STRUCT_ALIGN(Signal3Axes, TAILLE_CORR*4);

typedef struct
{
    int x[TAILLE_CORR];
    int y[TAILLE_CORR];
    int z[TAILLE_CORR];
    /* Valeur de son autocorrelation */
    long long autocorrelX;
    long long autocorrelY;
    long long autocorrelZ;
    /* Moyenne du signal */
    int moyenneX;
    int moyenneY;
    int moyenneZ;
}Signal3AxesReference;

typedef struct
{
    int* x;
    int* y;
    int* z;
}Signal3AxesPtr;

#endif /* SIGNAUX_H */
