/*******************************************************************************
 * Projet S5
 * @file    signaux3Axes.h
 * @author  Hugo Therrien
 * @date    8 novembre 2017
 * @version 0.1
 *
 * Définit des structures de données utiles pour les signaux à 3 axes
 *
 * TODO
 * + Ajouter les définitions pour la FFT
 * + Ajouter les définitons pour le filtrage
 *
 ******************************************************************************/

#ifndef SIGNAUX_3_AXES_H
#define SIGNAUX_3_AXES_H

#define TAILLE_CORR 64


typedef struct
{
    int x[TAILLE_CORR];
    int y[TAILLE_CORR];
    int z[TAILLE_CORR];
}Signal3AxesCorr;
#pragma STRUCT_ALIGN(Signal3AxesCorr, TAILLE_CORR*4);

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

// FFT (daml2601)
#define TAILLE_FFT 256

#endif /* SIGNAUX_3_AXES_H */
