/******************************************************************
Version 1.0

Auteur : Anthony Forin Hugo Daniel

Date de crÃ©ation : 2017/11/21

Liste de modification:
1. ModifÃ© par: NAME LASTNAME  Date: JJ/MM/AAAA

Description : Moyenne mobile, Necessite

PropriÃ©tÃ© : Les S de Sherbrooke
*****************************************************************/


#include "FIR1.h"


/* Filtre FIR moyenne mobile pour entrée

  Createur:    Anthony Fortin, Hugo Daniel
  Date:        2017/11/21
  Revisions:

  Description:
    Filtre FIR moyenne mobile pour entré
  Entrée :
    ENTREE1: Signal d'entrée brute, pour avoir un signal avec moins de bruit
  Retour :
    DESCRIPTION : Moyenne de l'échantillon courant et des 4 derniers
  Note :
*/

int FIR1(int tableauInput[])
{
    int OUT = 0;
    int temp = 0;
    static int position = 0;

    //int coeffFIR1 = 1/(Ordre+1);
// Boucle qui inclu les échantillon passé en fonction de l'Ordre du filtre
    int i = 0;
    for (i=0+position; i<Ordre+position+1 ; i++)
    {
        OUT = tableauInput[i] + OUT;
        temp = tableauInput[i];
    }
    position = position+1;
    OUT = OUT*coeffFIR1;   // Applique le coefficient 1/(ORDRE+1)

   return OUT;
}
