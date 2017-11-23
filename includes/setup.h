/*******************************************************************************
 * Projet S5
 * @file    setup.h
 * @author  Hugo Therrien
 * @date    7 novembre 2017
 * @version 0.1
 *
 * Contient les définitions de toutes les fonctions d'initilisation du DSP
 *
 ******************************************************************************/

#ifndef SETUP_H
#define SETUP_H

/*
 * @brief Routine that calls the setup functions
 */
void setup(void);

/*
 * @brief Configures timer 0 as a 10 Hz timer
 *        for more information, consult spru582b.pdf
 */
void configAndStartTimer0AsClock10Hz(void);

/*
 * @brief Configures timer 0 as a 30 Hz timer
 *        for more information, consult spru582b.pdf
 */
void configAndStartTimer0AsClock30Hz(void);

/*
 * @brief Configures timer 0 as a 300 Hz timer
 *        for more information, consult spru582b.pdf
 */
void configAndStartTimer0AsClock300Hz(void);

#endif /* SETUP_H */
