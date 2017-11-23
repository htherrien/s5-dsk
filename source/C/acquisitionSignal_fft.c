/*******************************************************************************
 * Projet S5
 * @file    acquisitionSignal_fft.c
 * @author  Louis D'Amour
 * @date    22 novembre 2017
 * @version 0.1
 *
 * Fait l'acquisition d'un nouvel echantillon pour l'algorithme de FFT
 *
 ******************************************************************************/

#include <dsk6713_led.h>

#include "acquisitionSignal_fft.h"
#include "capteurs440.dat" // Signaux enregistres

extern int dipStatus;

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
