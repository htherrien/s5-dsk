/********************************************************
**  Session 5 - APP6 - Téléphonie par DSP
**  Fichier SPI_driver.h
**  Auteurs : Sebastien Courtois - cous2509
**            Émile Forcier      - fore22110
**            Marc-André Roireau - roim1901
**            Hugo Therrien      - theh2203
**  Date : 13 novembre 2017
********************************************************/

#ifndef SPI_DRIVER_H
#define SPI_DRIVER_H

#define SPI_WRITE_CONFIG 0xE441
#define SPI_READ_CONFIG 0x4000
#define SPI_WRITE_DATA(data) (0x8000 | ((data) & 0x00FF))
#define SPI_READ_DATA 0x0000
#define SPI_READ_MASK 0x00FF

void SPI_init(MCBSP_Handle* MCBSPHandle, const int DEV_NO);

#endif /* SPI_DRIVER_H */
