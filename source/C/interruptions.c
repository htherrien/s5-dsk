/*******************************************************************************
 * Projet S5
 * @file    interruptions.c
 * @author  Hugo Therrien, Hugo Daniel, Louis D'Amour, Marc-André Roireau
 * @date    29 novembre 2017
 * @version 0.1
 *
 * Contient les interruptions
 *
 ******************************************************************************/

#include <DSK6713_led.h>
#include <csl_mcbsp.h>
#include <DSK6713.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>

#include "signaux.h"
#include "acquisitionSignal.h"
#include "correlations3Axes.h"
#include "faireFFT.h"
#include "SPI_driver.h"

// FFT (daml2601)
static float signalAFFT[2*TAILLE_FFT];
#pragma DATA_ALIGN(signalAFFT, 8)

static Signal3Axes signalACorreler; // Déja aligné.
Signal3AxesReference signalReference;

#define SEND_BUFFER_SIZE 32
static unsigned char MCBSP0SendBuffer[SEND_BUFFER_SIZE];
static int MCBSP0SendBufferBusy = 0;
static unsigned char MCBSP1SendBuffer[SEND_BUFFER_SIZE];
static int MCBSP1SendBufferBusy = 0;

interrupt void intTimer0(void)
{
    ;
}

/*
 * Interruption d'envoi de données UART
 */
interrupt void intTimer1(void)
{
    extern MCBSP_Handle MCBSP0Handle;
    extern MCBSP_Handle MCBSP1Handle;
    static int indexMCBSP0 = 0;
    static int indexMCBSP1 = 0;

    if(1 == MCBSP0SendBufferBusy)
    {
        if(!MCBSP0SendBuffer[indexMCBSP0])
        {
            MCBSP_write(MCBSP0Handle, SPI_WRITE_DATA('\n'));
            DSK6713_waitusec(10);
            MCBSP_read(MCBSP0Handle);
            indexMCBSP0 = 0;
            MCBSP0SendBufferBusy = 0;

        }
        else
        {
            MCBSP_write(MCBSP0Handle, SPI_WRITE_DATA(MCBSP0SendBuffer[indexMCBSP0]));
            DSK6713_waitusec(10);
            MCBSP_read(MCBSP0Handle);
            indexMCBSP0++;
        }
    }

    if(1 == MCBSP1SendBufferBusy)
    {
        if(!MCBSP1SendBuffer[indexMCBSP1])
        {
            MCBSP_write(MCBSP1Handle, SPI_WRITE_DATA('\n'));
            DSK6713_waitusec(10);
            MCBSP_read(MCBSP1Handle);
            indexMCBSP1 = 0;
            MCBSP1SendBufferBusy = 0;

        }
        else
        {
            MCBSP_write(MCBSP1Handle, SPI_WRITE_DATA(MCBSP1SendBuffer[indexMCBSP1]));
            DSK6713_waitusec(10);
            MCBSP_read(MCBSP1Handle);
            indexMCBSP1++;
        }
    }

}

void sendUART(const unsigned char* message, const int MCBSP_NO)
{
    assert(strlen(message) < SEND_BUFFER_SIZE);
    if(MCBSP_DEV0 == MCBSP_NO)
    {
        while(MCBSP0SendBufferBusy); // Blocks the DSP is more than 1 command is received at the same time
        strcpy(MCBSP0SendBuffer, message);
        MCBSP0SendBufferBusy = 1;
    }
    else if(MCBSP_DEV1 == MCBSP_NO)
    {
        while(MCBSP1SendBufferBusy); // Blocks the DSP is more than 1 command is received at the same time
        strcpy(MCBSP1SendBuffer, message);
        MCBSP1SendBufferBusy = 1;
    }
    else
    {
        abort();
    }
}


/*
 * Interruption de réception de données du PIC.
 */
interrupt void c_int04(void)
{
    static char UARTData;
    extern MCBSP_Handle MCBSP0Handle;

    MCBSP_write(MCBSP0Handle, SPI_READ_DATA);
    DSK6713_waitusec(10);
    UARTData = (MCBSP_read(MCBSP0Handle) & 0xFF);
}

/*
 * Interruption de réception de données de l'ordinateur.
 */
interrupt void c_int05(void)
{
    static char UARTData;
    extern MCBSP_Handle MCBSP1Handle;

    MCBSP_write(MCBSP1Handle, SPI_READ_DATA);
    DSK6713_waitusec(10);
    UARTData = (MCBSP_read(MCBSP1Handle) & 0xFF);
}
