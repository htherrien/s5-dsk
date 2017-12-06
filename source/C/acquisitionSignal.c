/*******************************************************************************
 * Projet S5
 * @file    acquisitionSignal.c
 * @author  Hugo Therrien, Louis D'Amour, Hugo Daniel
 * @date    22 novembre 2017
 * @version 0.3
 *
 * Fait l'acquisition d'un nouvel echantillon pour la FFT ou la corrélaiton
 *
 ******************************************************************************/

#include <csl_mcbsp.h>

#include "acquisitionSignal.h"
#include "moyenneMobile.h"
#include "correlations3Axes.h"
#include "faireFFT.h"
#include "interruptions.h"

/* Tampon d'acquisition des valeurs brutes */
static Signal3Axes tamponAcqUnFltr; // Déja aligné
/* Tampon d'acquisition filtre passe-bas */
static Signal3Axes tamponAcqFltr;  // Déja aligné
static int indexTamponAcqFltr = 0;
/* Tampon d'acquisition filtre passe-bas et sous-echantilloné */
static Signal3Axes tamponAcqDwnsmp; // Déja aligné


static Signal3AxesReference mouvement1;     // Déja aligné
static Signal3AxesReference mouvement2;     // Déja aligné

static float signalAFFT[TAILLE_FFT*2];
#pragma DATA_ALIGN(signalAFFT, 8);

int flagEnregistrement = 0;

void sauvegarderAcc(DonneeAccel* echantillonAcc)
{
    static Signal3AxesPtr tamponAcqPtr = {tamponAcqUnFltr.x, tamponAcqUnFltr.y, tamponAcqUnFltr.z};
    static int signalAFFTIndex = 0;
    static int compteurDownsample = 0;
    static int indexTamponAcqDwnsmp = 0;

    /* FILTRAGE */
    tamponAcqPtr.x = moyenneMobile64(tamponAcqPtr.x, &tamponAcqFltr.x[indexTamponAcqFltr], TAILLE_MOYENNE_MOBILE);
    tamponAcqPtr.y = moyenneMobile64(tamponAcqPtr.y, &tamponAcqFltr.y[indexTamponAcqFltr], TAILLE_MOYENNE_MOBILE);
    tamponAcqPtr.z = moyenneMobile64(tamponAcqPtr.z, &tamponAcqFltr.z[indexTamponAcqFltr], TAILLE_MOYENNE_MOBILE);

    if(flagEnregistrement)
    {
        enregistrerMouvement(echantillonAcc);
    }
    else
    {
        /* SOUS-ECHANTILLONAGE */
        compteurDownsample++;

        /* Reception d'un echantillon, faire la correlation */
        if(FACTEUR_L == compteurDownsample)
        {
            tamponAcqDwnsmp.x[indexTamponAcqDwnsmp] = tamponAcqFltr.x[indexTamponAcqFltr];
            tamponAcqDwnsmp.y[indexTamponAcqDwnsmp] = tamponAcqFltr.y[indexTamponAcqFltr];
            tamponAcqDwnsmp.z[indexTamponAcqDwnsmp] = tamponAcqFltr.z[indexTamponAcqFltr];

            Signal3AxesPtr signalACorreler;
            signalACorreler.x = &tamponAcqDwnsmp.x[indexTamponAcqDwnsmp];
            signalACorreler.y = &tamponAcqDwnsmp.y[indexTamponAcqDwnsmp];
            signalACorreler.z = &tamponAcqDwnsmp.z[indexTamponAcqDwnsmp];

            /* Si la correlation avec mouvement 1 a reussi */
            if(correler3Axes(&signalACorreler, &mouvement1))
            {
                /* Minimiser la fenetre */
                sendUART("min\n", MCBSP_DEV1);
                sendUART("i", MCBSP_DEV0); // i = 0x69
            }

            /* Si la correlation avec mouvement 2 a reussi */
            else if(correler3Axes(&signalACorreler, &mouvement2))
            {
                /* Minimiser la fenetre */
                sendUART("max\n", MCBSP_DEV1);
                sendUART("j", MCBSP_DEV0); // j = 0x6A
            }
            compteurDownsample = 0;
        }

        /* Tampon de sous-echantillonage plein */
        indexTamponAcqDwnsmp++;
        if(TAILLE_CORR == indexTamponAcqDwnsmp)
        {
            indexTamponAcqDwnsmp = 0;
        }
    }


    /* Tampon de filtrage plein */
    indexTamponAcqFltr++;
    if(TAILLE_CORR == indexTamponAcqFltr)
    {
        indexTamponAcqFltr = 0;
    }

    if(MODE_SOURIS == echantillonAcc->mode)
    {
        /* FFT */
        signalAFFT[signalAFFTIndex++] = echantillonAcc->y;

        /* Si le tampon est rempli */
        if(TAILLE_FFT == signalAFFTIndex)
        {
            faireFFT(signalAFFT);
            signalAFFTIndex = 0;
        }
    }
}

void resetSignauxReference(void)
{
    int i;
    for(i = 0; i < TAILLE_CORR; i++)
    {
        mouvement1.x[i] = 0;
        mouvement1.y[i] = 0;
        mouvement1.z[i] = 0;
        mouvement2.x[i] = 0;
        mouvement2.y[i] = 0;
        mouvement2.z[i] = 0;
    }
    mouvement1.autocorrelX = 0;
    mouvement1.autocorrelY = 0;
    mouvement1.autocorrelZ = 0;
    mouvement1.moyenneX = 0;
    mouvement1.moyenneY = 0;
    mouvement1.moyenneZ = 0;

    mouvement2.autocorrelX = 0;
    mouvement2.autocorrelY = 0;
    mouvement2.autocorrelZ = 0;
    mouvement2.moyenneX = 0;
    mouvement2.moyenneY = 0;
    mouvement2.moyenneZ = 0;
}

void enregistrerMouvement(DonneeAccel* echantillonAcc)
{
    static int compteurDownsample = 0;
    static int indexTamponAcqDwnsmp = 0;

    Signal3AxesReference* mouvementSelectione = &mouvement1;

    if(1 == flagEnregistrement)
    {
        mouvementSelectione = &mouvement1;
    }
    else if(2 == flagEnregistrement)
    {
        mouvementSelectione = &mouvement2;
    }

    /* SOUS-ECHANTILLONAGE */
    compteurDownsample++;

    if(FACTEUR_L == compteurDownsample)
    {
        mouvementSelectione->x[indexTamponAcqDwnsmp] = tamponAcqFltr.x[indexTamponAcqFltr];
        mouvementSelectione->y[indexTamponAcqDwnsmp] = tamponAcqFltr.y[indexTamponAcqFltr];
        mouvementSelectione->z[indexTamponAcqDwnsmp] = tamponAcqFltr.z[indexTamponAcqFltr];
        compteurDownsample = 0;
    }

    indexTamponAcqDwnsmp++;
    if(TAILLE_CORR == indexTamponAcqDwnsmp)
    {
        autoCorreler3Axes(mouvementSelectione);
        indexTamponAcqDwnsmp = 0;
        flagEnregistrement = 0;
    }
}
