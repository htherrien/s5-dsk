/*******************************************************************************
 * Projet S5
 * @file    moyenneMobile.h
 * @author  Hugo Therrien
 * @date    23 novembre 2017
 * @version 0.1
 *
 * Définit la signature des fonctions de moyenne mobile
 *
 ******************************************************************************/
 
#ifndef MOYENNE_MOBILE_H
#define MOYENNE_MOBILE_H

/*
 * Effectue la corrélation sur un tampon circulaire de 64 ints de long
 */
int* moyenneMobile64(int* x, int* y, int taille);

#endif /* MOYENNE_MOBILE_H */
