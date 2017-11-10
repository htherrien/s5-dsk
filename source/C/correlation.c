/*******************************************************************************
 * Projet S5
 * @file    correlation.h
 * @author  Hugo Therrien
 * @date    8 novembre 2017
 * @version 0.1
 *
 * Implémente les fonctions de corrélation en C
 *
 ******************************************************************************/

#include "correlation.h"

 int* correlation64(int* p_signal1,
                    int* p_signalRef,
                    long long* p_sortie,
                    int* p_moyenne)
{
  int i = 64;
  long long accumulateur = 0, somme = 0;
  p_signal1++;
  while(i--)
  {
    accumulateur += p_signal1[i]*p_signalRef[i];
    somme += abs(p_signal1[i]);
  }
  *p_sortie = accumulateur;
  *p_moyenne = (int)somme >> 6; // Diviser par 64
  return p_signal1;
}
