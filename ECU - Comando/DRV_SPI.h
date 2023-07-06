/***************************************************************************//**
  @file
  @brief
  @author   MGV
 ******************************************************************************/
#ifndef DRV_SPI_H_
#define DRV_SPI_H_

/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/
#include <stdint.h>

/*******************************************************************************
 * CONSTANT AND MACRO DEFINITIONS USING #DEFINE
 ******************************************************************************/

/*******************************************************************************
 * ENUMERATIONS AND STRUCTURES AND TYPEDEFS
 ******************************************************************************/

/*******************************************************************************
 * VARIABLE PROTOTYPES WITH GLOBAL SCOPE
 ******************************************************************************/

/*******************************************************************************
 * FUNCTION PROTOTYPES WITH GLOBAL SCOPE
 ******************************************************************************/
//Inicializaci�n y configuraci�n del SPI USCI - B0
void SPI_Init(void);

// Habilitaci�n de las interrupciones de SPI para el envi� y la recepci�n
void SPI_ISR_Enable(void);

// Inhabilitaci�n de las interrupciones de SPI para el envi� y la recepci�n
void SPI_ISR_Disable(void);

// Transferencia de datos por SPI-Transmitt y lectura por SPI-Receive
uint8_t transferSPI(uint8_t txdata);

//Chip Select OFF
void CS_OFF(void);

//Chip Select ON
void CS_ON(void);

//Inicializaci�n y configuraci�n del SPI USCI - A0
void SPI_Init_A(void);

// Habilitaci�n de las interrupciones de SPI para el envi� y la recepci�n
void SPI_ISR_Enable_A(void);

// Inhabilitaci�n de las interrupciones de SPI para el envi� y la recepci�n
void SPI_ISR_Disable_A(void);

// Transferencia de datos por SPI-Transmitt y lectura por SPI-Receive
uint8_t transferSPI_A(uint8_t txdata);

//Chip Select OFF
void CS_OFF_A(void);

//Chip Select ON
void CS_ON_A(void);
/*******************************************************************************
 ******************************************************************************/
#endif /* DRV_SPI_H_ */
