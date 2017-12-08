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
#include "curseur.h"

/* Tampon d'acquisition des valeurs brutes */
static Signal3Axes tamponAcqUnFltr; // Déja aligné
/* Tampon d'acquisition filtre passe-bas */
static Signal3Axes tamponAcqFltr;  // Déja aligné
static int indexTamponAcqFltr = 0;
/* Tampon d'acquisition filtre passe-bas et sous-echantilloné */
static Signal3Axes tamponAcqDwnsmp; // Déja aligné

static Signal3AxesReference mouvementsEnregistres[NB_MOUVEMENTS];     // Déja aligné

static float signalAFFT[TAILLE_FFT*2];
#pragma DATA_ALIGN(signalAFFT, 8);

Mouvements flagEnregistrement = AUCUN_MOUVEMENT;

void sauvegarderAcc(DonneeAccel* echantillonAcc)
{
    static int callibreX = 0;
    static int callibreY = 0;
    static int callibreZ = 0;
    static int nbEchCallib = 0;
    const int NB_CALLIB = 256;

    static Signal3AxesPtr tamponAcqPtr = {tamponAcqUnFltr.x, tamponAcqUnFltr.y, tamponAcqUnFltr.z};
    static int signalAFFTIndex = 0;
    static int compteurDownsample = 0;
    static int indexTamponAcqDwnsmp = 0;
    static int ancienMode = MODE_CORRELATION;

    const char* mouvementsCommandesUART[] =
    {
        "max",  // MOUVEMENT_MAXIMISER
        "min",  // MOUVEMENT_MINIMISER
        "ferm", // MOUVEMENT_FERMER
        "",     // NB_MOUVEMENTS
        ""      // AUCUN_MOUVEMENT
    };

    if(nbEchCallib < NB_CALLIB)
    {
        callibreX += echantillonAcc->x;
        callibreY += echantillonAcc->y;
        callibreZ += echantillonAcc->z;
        nbEchCallib++;
        if(nbEchCallib == NB_CALLIB)
        {
            callibreX = callibreX >> 8;
            callibreY = callibreY >> 8;
            callibreZ = callibreZ >> 8;
        }
        return;
    }

    /* Acquisiton d'un nouvel echantillon */
    *tamponAcqPtr.x = echantillonAcc->x - callibreX;
    *tamponAcqPtr.y = echantillonAcc->y - callibreY;
    *tamponAcqPtr.z = echantillonAcc->z - callibreZ;

    /* FILTRAGE */
    tamponAcqPtr.x = moyenneMobile64(tamponAcqPtr.x, &tamponAcqFltr.x[indexTamponAcqFltr], TAILLE_MOYENNE_MOBILE);
    tamponAcqPtr.y = moyenneMobile64(tamponAcqPtr.y, &tamponAcqFltr.y[indexTamponAcqFltr], TAILLE_MOYENNE_MOBILE);
    tamponAcqPtr.z = moyenneMobile64(tamponAcqPtr.z, &tamponAcqFltr.z[indexTamponAcqFltr], TAILLE_MOYENNE_MOBILE);

    if(MODE_CORRELATION == echantillonAcc->mode)
    {
        if(flagEnregistrement != AUCUN_MOUVEMENT)
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
                Mouvements i, indexMax = AUCUN_MOUVEMENT;
                uint8_t maximumCorr = 0;

                tamponAcqDwnsmp.x[indexTamponAcqDwnsmp] = tamponAcqFltr.x[indexTamponAcqFltr];
                tamponAcqDwnsmp.y[indexTamponAcqDwnsmp] = tamponAcqFltr.y[indexTamponAcqFltr];
                tamponAcqDwnsmp.z[indexTamponAcqDwnsmp] = tamponAcqFltr.z[indexTamponAcqFltr];

                Signal3AxesPtr signalACorreler;
                signalACorreler.x = &tamponAcqDwnsmp.x[indexTamponAcqDwnsmp];
                signalACorreler.y = &tamponAcqDwnsmp.y[indexTamponAcqDwnsmp];
                signalACorreler.z = &tamponAcqDwnsmp.z[indexTamponAcqDwnsmp];

                for(i = MOUVEMENT_MAXIMISER; i < NB_MOUVEMENTS; i++)
                {
                    uint8_t resCorr;
                    resCorr = correler3Axes(&signalACorreler, &mouvementsEnregistres[i]);
                    if(resCorr > maximumCorr)
                    {
                        maximumCorr = resCorr;
                        indexMax = i;
                    }
                }

                /* Envoyer le résultat maximal de la correlation au PIC en tout temps */
                sendUART((const char*) &maximumCorr, MCBSP_DEV0);

                /* Envoyer le résultat maximal de la correlation au PIC si dépasse le seuil */
                if(maximumCorr > TRESHOLD_CORREL)
                {
                    sendUART(mouvementsCommandesUART[indexMax], MCBSP_DEV1);
                }
                compteurDownsample = 0;
            }

            /* Tampon de sous-echantillonage plein */
            indexTamponAcqDwnsmp++;
            if(TAILLE_CORR == indexTamponAcqDwnsmp)
            {
                indexTamponAcqDwnsmp = 0;
            }

            /* FFT */
            signalAFFT[signalAFFTIndex*2] = echantillonAcc->y;
            signalAFFT[signalAFFTIndex*2 + 1] = 0;
            signalAFFTIndex++;

            /* Si le tampon est rempli */
            if(TAILLE_FFT == signalAFFTIndex)
            {
                /* Envoyer un clic gauche si mouvement reussi */
                if(faireFFT(signalAFFT))
                {
                    sendUART("cg", MCBSP_DEV1);
                }
                signalAFFTIndex = 0;
            }

        }
    }

    else if(MODE_SOURIS == echantillonAcc->mode)
    {
        /* Vider le tampon si l'on passe de mode correlation a mode souris */
        if(MODE_CORRELATION == ancienMode)
        {
            int i;
            for(i = 0; i < TAILLE_CORR; i++)
            {
                tamponAcqDwnsmp.x[i] = 0;
                tamponAcqDwnsmp.y[i] = 0;
                tamponAcqDwnsmp.z[i] = 0;
            }
        }


        /* Curseur */
        curseur(tamponAcqFltr.y[indexTamponAcqFltr], tamponAcqFltr.x[indexTamponAcqFltr]);
    }

    /* Tampon de filtrage plein */
    indexTamponAcqFltr++;
    if(TAILLE_CORR == indexTamponAcqFltr)
    {
        indexTamponAcqFltr = 0;
    }

    ancienMode = echantillonAcc->mode;
}

void resetSignauxReference(void)
{
    int i;
    for(i = 0; i < TAILLE_CORR; i++)
    {
        tamponAcqUnFltr.x[i] = 0;
        tamponAcqUnFltr.y[i] = 0;
        tamponAcqUnFltr.z[i] = 0;
        tamponAcqFltr.x[i] = 0;
        tamponAcqFltr.y[i] = 0;
        tamponAcqFltr.z[i] = 0;
        tamponAcqDwnsmp.x[i] = 0;
        tamponAcqDwnsmp.y[i] = 0;
        tamponAcqDwnsmp.z[i] = 0;
    }

    Mouvements j;
    for(j = MOUVEMENT_MAXIMISER; j < NB_MOUVEMENTS; j++)
    {
        for(i = 0; i < TAILLE_CORR; i++)
        {
            mouvementsEnregistres[j].x[i] = 0;
            mouvementsEnregistres[j].y[i] = 0;
            mouvementsEnregistres[j].z[i] = 0;
        }
        // Pour éviter la division par 0
        mouvementsEnregistres[j].autocorrelX = 1;
        mouvementsEnregistres[j].autocorrelY = 1;
        mouvementsEnregistres[j].autocorrelZ = 1;
        mouvementsEnregistres[j].moyenneX = 1;
        mouvementsEnregistres[j].moyenneY = 1;
        mouvementsEnregistres[j].moyenneZ = 1;
    }
}

void enregistrerMouvement(DonneeAccel* echantillonAcc)
{
    static int compteurDownsample = 0;
    static int indexTamponAcqDwnsmp = 0;

    Signal3AxesReference* mouvementSelectione;

    mouvementSelectione = &mouvementsEnregistres[flagEnregistrement];

    /* SOUS-ECHANTILLONAGE */
    compteurDownsample++;

    if(FACTEUR_L == compteurDownsample)
    {
        mouvementSelectione->x[indexTamponAcqDwnsmp] = tamponAcqFltr.x[indexTamponAcqFltr];
        mouvementSelectione->y[indexTamponAcqDwnsmp] = tamponAcqFltr.y[indexTamponAcqFltr];
        mouvementSelectione->z[indexTamponAcqDwnsmp] = tamponAcqFltr.z[indexTamponAcqFltr];
        indexTamponAcqDwnsmp++;
        compteurDownsample = 0;
    }


    if(TAILLE_CORR == indexTamponAcqDwnsmp)
    {
        autoCorreler3Axes(mouvementSelectione);
        indexTamponAcqDwnsmp = 0;
        sendUART("et", MCBSP_DEV1);
        flagEnregistrement = AUCUN_MOUVEMENT;
    }
}
