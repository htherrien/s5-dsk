/*******************************************************************************
 * Projet S5
 * @file    main.c
 * @author  Hugo Therrien, Hugo Daniel, Louis D'Amour, Marc-Andr� Roireau
 * @date    23 novembre 2017
 * @version 0.5
 *
 * Fichier de d�mo de la correlation.
 *
 ******************************************************************************/

#include <csl_gpio.h>
#include <csl_mcbsp.h>
#include <dsk6713_dip.h>
#include <dsk6713.h>
#include <stdio.h>
#include <stdlib.h>

#include "setup.h"
#include "enableInterrupts.h"
#include "acquisitionSignal.h"
#include "SPI_driver.h"
#include "interruptions.h"
#include "../../messagesUART/messagesUART.h"

extern void vectors();

int dipStatus = 0;

MCBSP_Handle MCBSP0Handle;
MCBSP_Handle MCBSP1Handle;

void main(void)
{
  GPIO_Handle gp0Handle =  GPIO_open(GPIO_DEV0, GPIO_OPEN_RESET);
  setup();
  SPI_init(&MCBSP0Handle, MCBSP_DEV0);
  SPI_init(&MCBSP1Handle, MCBSP_DEV1);
  pinConfig(&gp0Handle, vectors);
  resetSignauxReference();
  enableInterrupts();

  MCBSP_write(MCBSP0Handle, SPI_READ_DATA);
  DSK6713_waitusec(10);
  MCBSP_read(MCBSP0Handle);

  MCBSP_write(MCBSP1Handle, SPI_READ_DATA);
  DSK6713_waitusec(10);
  MCBSP_read(MCBSP1Handle);


  for(;;)
  {
      dipStatus = !DSK6713_DIP_get(0) + (!DSK6713_DIP_get(1)<<1) + (!DSK6713_DIP_get(2)<<2) + (!DSK6713_DIP_get(3)<<3);
      DSK6713_waitusec(1000*100);
  }
}
