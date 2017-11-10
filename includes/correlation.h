/*******************************************************************************
 * Projet S5
 * @file    correlation.h
 * @author  Hugo Therrien
 * @date    8 novembre 2017
 * @version 0.1
 *
 * D�finit la signature des fonctions de corr�lation en C
 *
 ******************************************************************************/
 
#ifndef CORRELATION_H
#define CORRELATION_H

/*
 * Effectue la corr�lation sur un tampon circulaire de 64 ints de long
 */
int* correlation64(int p_signal1[],
                   int p_signal_ref[],
                   long long* p_sortie,
                   int* p_moyenne);

#endif /* CORRELATION_H */
