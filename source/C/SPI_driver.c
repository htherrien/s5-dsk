/********************************************************
**  Session 5 - APP6 - Téléphonie par DSP
**  Fichier SPI_driver.c
**  Auteurs :
**            Marc-André Roireau - roim1901
**            Hugo Therrien      - theh2203
**  Date : 13 novembre 2017
********************************************************/

#include <csl_mcbsp.h>
#include <dsk6713.h>

#include "SPI_driver.h"

static MCBSP_Config MCBSP_SPI_CFG = {

        MCBSP_FMKS(SPCR, FREE, NO)              |
        MCBSP_FMKS(SPCR, SOFT, NO)              |
        MCBSP_FMKS(SPCR, FRST, YES)             |
        MCBSP_FMKS(SPCR, GRST, YES)             |
        MCBSP_FMKS(SPCR, XINTM, XRDY)           |
        MCBSP_FMKS(SPCR, XSYNCERR, NO)          |
        MCBSP_FMKS(SPCR, XRST, YES)             |
        MCBSP_FMKS(SPCR, DLB, OFF)              |
        MCBSP_FMKS(SPCR, RJUST, RZF)            |
        MCBSP_FMKS(SPCR, CLKSTP, DELAY)         |
        MCBSP_FMKS(SPCR, DXENA, OFF)            |
        MCBSP_FMKS(SPCR, RINTM, RRDY)           |
        MCBSP_FMKS(SPCR, RSYNCERR, NO)          |
        MCBSP_FMKS(SPCR, RRST, YES),

        MCBSP_FMKS(RCR, RPHASE, SINGLE)         |
        MCBSP_FMKS(RCR, RFRLEN2, DEFAULT)       |
        MCBSP_FMKS(RCR, RWDLEN2, DEFAULT)       |
        MCBSP_FMKS(RCR, RCOMPAND, MSB)          |
        MCBSP_FMKS(RCR, RFIG, NO)               |
        MCBSP_FMKS(RCR, RDATDLY, 1BIT)          |
        MCBSP_FMKS(RCR, RFRLEN1, OF(0))         | // This changes to 1 FRAME
        MCBSP_FMKS(RCR, RWDLEN1, 16BIT)         | // This changes to 16 bits per frame
        MCBSP_FMKS(RCR, RWDREVRS, DISABLE),

        MCBSP_FMKS(XCR, XPHASE, SINGLE)         |
        MCBSP_FMKS(XCR, XFRLEN2, DEFAULT)       |
        MCBSP_FMKS(XCR, XWDLEN2, DEFAULT)       |
        MCBSP_FMKS(XCR, XCOMPAND, MSB)          |
        MCBSP_FMKS(XCR, XFIG, NO)               |
        MCBSP_FMKS(XCR, XDATDLY, 1BIT)          |
        MCBSP_FMKS(XCR, XFRLEN1, OF(0))         | // This changes to 1 FRAME
        MCBSP_FMKS(XCR, XWDLEN1, 16BIT)         | // This changes to 16 bits per frame
        MCBSP_FMKS(XCR, XWDREVRS, DISABLE),

        MCBSP_FMKS(SRGR, GSYNC, DEFAULT)        |
        MCBSP_FMKS(SRGR, CLKSP, DEFAULT)        |
        MCBSP_FMKS(SRGR, CLKSM, DEFAULT)        |
        MCBSP_FMKS(SRGR, FSGM, DEFAULT)         |
        MCBSP_FMKS(SRGR, FPER, DEFAULT)         |
        MCBSP_FMKS(SRGR, FWID, DEFAULT)         |
        MCBSP_FMKS(SRGR, CLKGDV, OF(25)),

        MCBSP_MCR_DEFAULT,
        MCBSP_RCER_DEFAULT,
        MCBSP_XCER_DEFAULT,

        MCBSP_FMKS(PCR, XIOEN, SP)              |
        MCBSP_FMKS(PCR, RIOEN, SP)              |
        MCBSP_FMKS(PCR, FSXM, INTERNAL)         | // INTERNAL
        MCBSP_FMKS(PCR, FSRM, EXTERNAL)         |
        MCBSP_FMKS(PCR, CLKXM, OUTPUT)          | // OUTPUT : MCBSP is MASTER
        MCBSP_FMKS(PCR, CLKRM, INPUT)           |
        MCBSP_FMKS(PCR, CLKSSTAT, DEFAULT)      |
        MCBSP_FMKS(PCR, DXSTAT, DEFAULT)        |
        MCBSP_FMKS(PCR, FSXP, ACTIVELOW)        |
        MCBSP_FMKS(PCR, FSRP, ACTIVELOW)        |
        MCBSP_FMKS(PCR, CLKXP, RISING)          |
        MCBSP_FMKS(PCR, CLKRP, RISING)
};

/*
 * Sets McBSP0 as external and configures it in SPI mode for the MAX-3111.
 * Configures the MAX-3111.
 */
void SPI_init(MCBSP_Handle* MCBSPHandle, const int DEV_NO)
{
    if(DEV_NO == MCBSP_DEV0)
    {
        DSK6713_rset(DSK6713_MISC, DSK6713_rget(DSK6713_MISC) | 0x1); // Set McBSP0 as external
    }
    else if(DEV_NO == MCBSP_DEV1)
    {
        DSK6713_rset(DSK6713_MISC, DSK6713_rget(DSK6713_MISC) | 0x2); // Set McBSP1 as external
    }

    if((*MCBSPHandle)->allocated != 1)
    {
        *MCBSPHandle = MCBSP_open(DEV_NO, MCBSP_OPEN_RESET);
    }

    MCBSP_config(*MCBSPHandle, &MCBSP_SPI_CFG);                    // Config McBSP Device

    MCBSP_start(*MCBSPHandle, MCBSP_RCV_START | MCBSP_XMIT_START | MCBSP_SRGR_START | MCBSP_SRGR_FRAMESYNC, 0x00003000);

    while(!MCBSP_xrdy(*MCBSPHandle))
    {
       ;// Wait until the MCBSP is ready
    }

    MCBSP_write(*MCBSPHandle, SPI_WRITE_CONFIG);
    DSK6713_waitusec(10);
    MCBSP_read(*MCBSPHandle); // Empty receive buffer
}
