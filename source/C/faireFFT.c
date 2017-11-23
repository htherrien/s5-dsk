/*******************************************************************************
 * Projet S5
 * @file    faireFFT.c
 * @author  Louis D'Amour
 * @date    22 novembre 2017
 * @version 0.1
 *
 ******************************************************************************/
#include <DSPF_sp_cfftr2_dit.h>
#include "twiddles_512.h"
#include "signaux.h"
#include "faireFFT.h"
#include "hamming.h"

#define F_DEPART 155.0
#define I_DEPART (F_DEPART/(440.0/2.0)*(TAILLE_FFT))
#define SEUIL_FFT 500000000

int faireFFT(float signalAFFT[])
{
    int i;
    /* Fenetrage */
    for(i = 0; i < TAILLE_FFT; i++)
    {
        signalAFFT[2*i] = (float)signalAFFT[2*i] * hamming[i];
    }

    /* FFT */
    DSPF_sp_cfftr2_dit(signalAFFT, w, TAILLE_FFT);
    /* Remise dans l'ordre normal */
    bit_rev(signalAFFT, TAILLE_FFT);

    /* Détection de seuil */
    for(i = I_DEPART; i <= TAILLE_FFT; i += 2)
    {
        if (signalAFFT[i]*signalAFFT[i] + signalAFFT[i+1]*signalAFFT[i+1] > SEUIL_FFT)
        {
            return 1;
        }
    }
return 0;
}
