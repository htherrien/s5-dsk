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

#include <dsk6713_led.h>

#include "acquisitionSignal.h"
#include "moyenneMobile.h"
#include "capteurs.dat" // Signaux enregistres
#include "capteurs440.dat"

extern int dipStatus;

static Signal3Axes tamponAcquisitionFiltre; // Déja aligné

void acquistionCorrelationDemo(Signal3AxesPtr *p_signalACorrelerPtr)
{
    static int i = 0;

    /* Acquisition en fonction des DIPs */
    switch(dipStatus)
    {
    case 0:
        tamponAcquisitionFiltre.x[i] = 0;
        tamponAcquisitionFiltre.y[i] = 0;
        tamponAcquisitionFiltre.z[i] = 0;
        break;
    case 1:
        tamponAcquisitionFiltre.x[i] = signal1_x[i];
        tamponAcquisitionFiltre.y[i] = signal1_y[i];
        tamponAcquisitionFiltre.z[i] = signal1_z[i];
        break;
    case 2:
        tamponAcquisitionFiltre.x[i] = signal2_x[i];
        tamponAcquisitionFiltre.y[i] = signal2_y[i];
        tamponAcquisitionFiltre.z[i] = signal2_z[i];
        break;
    case 3:
        tamponAcquisitionFiltre.x[i] = signal3_x[i];
        tamponAcquisitionFiltre.y[i] = signal3_y[i];
        tamponAcquisitionFiltre.z[i] = signal3_z[i];
        break;
    case 4:
        tamponAcquisitionFiltre.x[i] = signal4_x[i];
        tamponAcquisitionFiltre.y[i] = signal4_y[i];
        tamponAcquisitionFiltre.z[i] = signal4_z[i];
        break;
    case 5:
        tamponAcquisitionFiltre.x[i] = signal5_x[i];
        tamponAcquisitionFiltre.y[i] = signal5_y[i];
        tamponAcquisitionFiltre.z[i] = signal5_z[i];
        break;
    case 6:
        tamponAcquisitionFiltre.x[i] = signal6_x[i];
        tamponAcquisitionFiltre.y[i] = signal6_y[i];
        tamponAcquisitionFiltre.z[i] = signal6_z[i];
        break;
    case 7:
        tamponAcquisitionFiltre.x[i] = signal7_x[i];
        tamponAcquisitionFiltre.y[i] = signal7_y[i];
        tamponAcquisitionFiltre.z[i] = signal7_z[i];
        break;
    default:
        tamponAcquisitionFiltre.x[i] = 0;
        tamponAcquisitionFiltre.y[i] = 0;
        tamponAcquisitionFiltre.z[i] = 0;
        break;
    }

    /* Filtrer le signal */
    moyenneMobile64(&tamponAcquisitionFiltre.x[i], p_signalACorrelerPtr->x, TAILLE_MOYENNE_MOBILE);
    moyenneMobile64(&tamponAcquisitionFiltre.y[i], p_signalACorrelerPtr->y, TAILLE_MOYENNE_MOBILE);
    moyenneMobile64(&tamponAcquisitionFiltre.z[i], p_signalACorrelerPtr->z, TAILLE_MOYENNE_MOBILE);

    i++;

    if(i == 64)
    {
        i = 0;
    }

}

void acquistionCorrelationDemoFFT(float *p_signalAFFTPtr)
{
    static int i = 0;

    /* Acquisition en fonction des DIPs */
    switch(dipStatus)
    {
    case 0:
        *p_signalAFFTPtr = 0;
        *(p_signalAFFTPtr+1) = 0;
        i++;
        if(i == 64)
        {
            i = 0;
        }
        break;
    case 1:
        *p_signalAFFTPtr = signal1_y440[i];
        *(p_signalAFFTPtr+1) = 0;
        i++;
        if(i == tailleSignal1_x440)
        {
            i = 0;
        }
        break;
    case 2:
        *p_signalAFFTPtr = signal2_y440[i];
        *(p_signalAFFTPtr+1) = 0;
        i++;
        if(i == tailleSignal2_x440)
        {
            i = 0;
        }
        break;
    case 3:
        *p_signalAFFTPtr = signal3_y440[i];
        *(p_signalAFFTPtr+1) = 0;
        i++;
        if(i == tailleSignal3_x440)
        {
            i = 0;
        }
        break;
    case 4:
        *p_signalAFFTPtr = signal4_y440[i];
        *(p_signalAFFTPtr+1) = 0;
        i++;
        if(i == tailleSignal4_x440)
        {
            i = 0;
        }
        break;
    case 5:
        *p_signalAFFTPtr = signal5_y440[i];
        *(p_signalAFFTPtr+1) = 0;
        i++;
        if(i == tailleSignal5_x440)
        {
            i = 0;
        }
        break;
    default:
        *p_signalAFFTPtr = 0;
        *(p_signalAFFTPtr+1) = 0;
        i++;
        if(i == 64)
        {
            i = 0;
        }
        break;
    }
}
