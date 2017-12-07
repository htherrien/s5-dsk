/*******************************************************************************
 * Projet S5
 * @file    correlations3Axes.c
 * @author  Hugo Therrien
 * @date    8 novembre 2017
 * @version 0.1
 *
 * Fonctions de correlation sur les 3 axes.
 *
 ******************************************************************************/

#include <dsk6713_led.h>
#include <math.h>

#include "correlationASM.h"
#include "correlations3Axes.h"

void correler3AxesDemo(Signal3AxesPtr* p_signalACorrelerPtr, Signal3AxesReference* p_signal3AxesReference)
{
    /* Effectuer la correlation */
    long long resultatCorrX, resultatCorrY, resultatCorrZ;
    int moyenneX, moyenneY, moyenneZ;
    p_signalACorrelerPtr->x = correlationASM64(p_signalACorrelerPtr->x,
                                               p_signal3AxesReference->x,
                                               &resultatCorrX,
                                               &moyenneX);
    p_signalACorrelerPtr->y = correlationASM64(p_signalACorrelerPtr->y,
                                               p_signal3AxesReference->y,
                                               &resultatCorrY,
                                               &moyenneY);
    p_signalACorrelerPtr->z = correlationASM64(p_signalACorrelerPtr->z,
                                               p_signal3AxesReference->z,
                                               &resultatCorrZ,
                                               &moyenneZ);

    #define TRESHOLD_X    40367877LL
    #define TRESHOLD_Y   810989952LL
    #define TRESHOLD_Z 15577154910LL

    #define DEL_SIGNAL_PRESENT 0
    #define DEL_CORRELATION_REUSSIE 1
    #define DEL_CORRELATION_ECHOUEE 2

    /* Allumer la DEL de correlation reussie si correlation reussie */
    if(resultatCorrX > TRESHOLD_X && resultatCorrY > TRESHOLD_Y && resultatCorrZ > TRESHOLD_Z)
    {
        DSK6713_LED_on(DEL_CORRELATION_REUSSIE);
        DSK6713_LED_off(DEL_CORRELATION_ECHOUEE);
    }
    else
    {
        DSK6713_LED_on(DEL_CORRELATION_ECHOUEE);
        DSK6713_LED_off(DEL_CORRELATION_REUSSIE);
    }

}

uint8_t correler3Axes(Signal3AxesPtr* p_signalACorrelerPtr, Signal3AxesReference* p_signal3AxesReference)
{
    /* Effectuer la correlation */
    long long resultatCorrX, resultatCorrY, resultatCorrZ;
    float ratioX, ratioY, ratioZ;// Ratio des moyennes
    float min;
    int moyenneX, moyenneY, moyenneZ;


    p_signalACorrelerPtr->x = correlationASM64(p_signalACorrelerPtr->x,
                                               p_signal3AxesReference->x,
                                               &resultatCorrX,
                                               &moyenneX);

    p_signalACorrelerPtr->y = correlationASM64(p_signalACorrelerPtr->y,
                                               p_signal3AxesReference->y,
                                               &resultatCorrY,
                                               &moyenneY);
    p_signalACorrelerPtr->z = correlationASM64(p_signalACorrelerPtr->z,
                                               p_signal3AxesReference->z,
                                               &resultatCorrZ,
                                               &moyenneZ);
    ratioX = ((float)moyenneX/(float)p_signal3AxesReference->moyenneX)*
             ((float)resultatCorrX/(float)p_signal3AxesReference->autocorrelX);
    ratioY = ((float)moyenneY/(float)p_signal3AxesReference->moyenneY)*
            ((float)resultatCorrY/(float)p_signal3AxesReference->autocorrelY);
    ratioZ = ((float)moyenneZ/(float)p_signal3AxesReference->moyenneZ)*
            ((float)resultatCorrZ/(float)p_signal3AxesReference->autocorrelZ);

    min = ratioX;
    if(ratioY < min)
    {
        min = ratioY;
    }
    if(ratioZ < min)
    {
        min = ratioZ;
    }

    return (uint8_t)(min * 100);
}


void autoCorreler3Axes(Signal3AxesReference* p_signal3AxesReference)
{
    correlationASM64(p_signal3AxesReference->x,
                     p_signal3AxesReference->x,
                     &p_signal3AxesReference->autocorrelX,
                     &p_signal3AxesReference->moyenneX);

    correlationASM64(p_signal3AxesReference->y,
                     p_signal3AxesReference->y,
                     &p_signal3AxesReference->autocorrelY,
                     &p_signal3AxesReference->moyenneY);

    correlationASM64(p_signal3AxesReference->z,
                     p_signal3AxesReference->z,
                     &p_signal3AxesReference->autocorrelZ,
                     &p_signal3AxesReference->moyenneZ);

}
