/*******************************************************************************
 * Projet S5
 * @file    faireFFT.c
 * @author  Hugo Therrien, Hugo Daniel
 * @date    23 novembre 2017
 * @version 0.1
 *
 ******************************************************************************/
#include "moyenneMobile.h"

/* Fonction privée */
int* moyenneMobileASM64(int* x, long* y, int taille);

int* moyenneMobile64(int* x, int* y, int taille)
{
    long y_a_diviser;
    x = moyenneMobileASM64(x, &y_a_diviser, taille);
    *y = y_a_diviser / taille;
    return x;
}
