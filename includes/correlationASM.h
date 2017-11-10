/*******************************************************************************
 * Projet S5
 * @file    correlationASM.h
 * @author  Hugo Therrien
 * @date    8 novembre 2017
 * @version 0.2
 *
 * Définit la signature des fonctions de corrélation en assembleur
 *
 ******************************************************************************/
 
#ifndef CORRELATION_ASM_H
#define CORRELATION_ASM_H

/*
 * Effectue la corrélation sur un tampon circulaire de 64 ints de long
 */
int* correlationASM64(int p_signal1[],
                      int p_signal_ref[],
                      long long* p_sortie,
                      int* p_moyenne);

#endif /* CORRELATION_ASM_H */
