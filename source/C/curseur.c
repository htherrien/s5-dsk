#include <stdio.h>
#include <stdlib.h>
#include <csl_mcbsp.h>
#include "curseur.h"
#include "interruptions.h"

void curseur(int acc_x , int acc_y)
 {
    static int compteur_4 = 0;
    static int sum_x = 0;
    static int sum_y = 0;
    static char buffer[64];
    const int MAX_X = 10000;
    const int MAX_Y = 10000;

    compteur_4++;
    sum_x += acc_x;
    sum_y += acc_y;
    if(compteur_4 == 4)
    {
        int fin_x;
        int fin_y;
         fin_x = (sum_x * 10) / MAX_X;
         fin_y = (sum_y * 10) / MAX_Y;

         snprintf(buffer,sizeof(buffer), "x:%d y:%d", fin_x, fin_y);
         sendUART(buffer, MCBSP_DEV1);

         sum_x = 0;
         sum_y = 0;
         compteur_4 = 0;
    }
 }
