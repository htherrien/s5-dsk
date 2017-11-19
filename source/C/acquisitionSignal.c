/*******************************************************************************
 * Projet S5
 * @file    acquisitionSignal.c
 * @author  Hugo Therrien
 * @date    8 novembre 2017
 * @version 0.1
 *
 * Fait l'acquisition d'un nouvel echantillon.
 *
 *
 ******************************************************************************/

#include <dsk6713_led.h>

#include "acquisitionSignal.h"
#include "capteurs.dat" // Signaux enregistres

extern int dipStatus;

void acquistionCorrelationDemo(Signal3AxesPtr *p_signalACorrelerPtr)
{
    static int i = 0;

    /* Acquisition en fonction des DIPs */
    switch(dipStatus)
    {
    case 0:
        *p_signalACorrelerPtr->x = 0;
        *p_signalACorrelerPtr->y = 0;
        *p_signalACorrelerPtr->z = 0;
        i++;
        break;
    case 1:
        *p_signalACorrelerPtr->x = signal1_x[i];
        *p_signalACorrelerPtr->y = signal1_y[i];
        *p_signalACorrelerPtr->z = signal1_z[i];
        i++;
        break;
    case 2:
        *p_signalACorrelerPtr->x = signal2_x[i];
        *p_signalACorrelerPtr->y = signal2_y[i];
        *p_signalACorrelerPtr->z = signal2_z[i];
        i++;
        break;
    case 3:
        *p_signalACorrelerPtr->x = signal3_x[i];
        *p_signalACorrelerPtr->y = signal3_y[i];
        *p_signalACorrelerPtr->z = signal3_z[i];
        i++;
        break;
    case 4:
        *p_signalACorrelerPtr->x = signal4_x[i];
        *p_signalACorrelerPtr->y = signal4_y[i];
        *p_signalACorrelerPtr->z = signal4_z[i];
        i++;
        break;
    case 5:
        *p_signalACorrelerPtr->x = signal5_x[i];
        *p_signalACorrelerPtr->y = signal5_y[i];
        *p_signalACorrelerPtr->z = signal5_z[i];
        i++;
        break;
    case 6:
        *p_signalACorrelerPtr->x = signal6_x[i];
        *p_signalACorrelerPtr->y = signal6_y[i];
        *p_signalACorrelerPtr->z = signal6_z[i];
        i++;
        break;
    case 7:
        *p_signalACorrelerPtr->x = signal7_x[i];
        *p_signalACorrelerPtr->y = signal7_y[i];
        *p_signalACorrelerPtr->z = signal7_z[i];
        i++;
        break;
    default:
        *p_signalACorrelerPtr->x = 0;
        *p_signalACorrelerPtr->y = 0;
        *p_signalACorrelerPtr->z = 0;
        i++;
        break;
    }
    if(i == 64)
    {
        i = 0;
    }
}

void viderTamponAcquisitionCorrel(Signal3AxesPtr* p_tamponAcquisitionPtr)
{
    int i;
    for(i = 0; i < TAILLE_CORR; i++)
    {
        *p_tamponAcquisitionPtr->x = 0;
        *p_tamponAcquisitionPtr->y = 0;
        *p_tamponAcquisitionPtr->z = 0;
    }
}
