/*******************************************************************************
 * Projet S5
 * @file    main.c
 * @author  Hugo Therrien, Hugo Daniel, Louis D'Amour, Marc-André Roireau
 * @date    23 novembre 2017
 * @version 0.5
 *
 * Fichier de démo de la correlation.
 *
 ******************************************************************************/

#include <csl_gpio.h>
#include <csl_mcbsp.h>
#include <dsk6713_dip.h>
#include <dsk6713.h>

#include "setup.h"
#include "enableInterrupts.h"
#include "SPI_driver.h"

extern void vectors();

int dipStatus = 0;

MCBSP_Handle MCBSP0Handle;
MCBSP_Handle MCBSP1Handle;

void main(void)
{
  GPIO_Handle gp0Handle;
  setup();
  SPI_init(&MCBSP0Handle);
  int4_init(&gp0Handle, vectors);
  enableInterrupts();

  for(;;)
  {
      dipStatus = !DSK6713_DIP_get(0) + (!DSK6713_DIP_get(1)<<1) + (!DSK6713_DIP_get(2)<<2) + (!DSK6713_DIP_get(3)<<3);

      MCBSP_write(MCBSP0Handle, SPI_WRITE_DATA(0X34));
      DSK6713_waitusec(10);
      MCBSP_read(MCBSP0Handle);
      DSK6713_waitusec(1000*100);
  }
}
