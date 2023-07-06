/***************************************************************************//**
  @file     Botonera.h
  @brief    Lectura de la botonera y de los estados
  @author   MGV
 ******************************************************************************/
#ifndef DRV_BOTONERA_H_
#define DRV_BOTONERA_H_

/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/
#include <msp430.h>
#include "board.h"
#include "DRV_SPI.h"
/*******************************************************************************
 * CONSTANT AND MACRO DEFINITIONS USING #DEFINE
 ******************************************************************************/
#define PUERTA1_MASK 0x30
#define PUERTA2_MASK 0x0C
#define PUERTA3_MASK 0xC0
#define PUERTA4_MASK 0x03

#define PUERTA1_SHIFT 4
#define PUERTA2_SHIFT 2
#define PUERTA3_SHIFT 6
#define PUERTA4_SHIFT 0

#define PUERTA13_SUBIR 0x01
#define PUERTA13_BAJAR 0x02
#define PUERTA24_SUBIR 0x02
#define PUERTA24_BAJAR 0x01

#define BLOCK 1
#define FREE 0

#define AUTO_TRIGGER  500 //ms

/*******************************************************************************
 * ENUMERATIONS AND STRUCTURES AND TYPEDEFS
 ******************************************************************************/

/*******************************************************************************
 * VARIABLE PROTOTYPES WITH GLOBAL SCOPE
 ******************************************************************************/

/*******************************************************************************
 * FUNCTION PROTOTYPES WITH GLOBAL SCOPE
 ******************************************************************************/

/**
 * @brief Initializes Botonera
 * @param void
 * @return void
 */
void botonera_Init (void);

/**
 * @brief Reads the value of Botonera through SPI and returns it
 * @param pointer
 * @return void
 */
unsigned char botoneraRead_SPI_A(void);

/*******************************************************************************
 ******************************************************************************/
#endif /* DRV_BOTONERA_H_ */
