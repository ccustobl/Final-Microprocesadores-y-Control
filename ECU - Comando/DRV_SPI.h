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
//Inicialización y configuración del SPI USCI - B0
void SPI_Init(void);

// Habilitación de las interrupciones de SPI para el envió y la recepción
void SPI_ISR_Enable(void);

// Inhabilitación de las interrupciones de SPI para el envió y la recepción
void SPI_ISR_Disable(void);

// Transferencia de datos por SPI-Transmitt y lectura por SPI-Receive
uint8_t transferSPI(uint8_t txdata);

//Chip Select OFF
void CS_OFF(void);

//Chip Select ON
void CS_ON(void);

//Inicialización y configuración del SPI USCI - A0
void SPI_Init_A(void);

// Habilitación de las interrupciones de SPI para el envió y la recepción
void SPI_ISR_Enable_A(void);

// Inhabilitación de las interrupciones de SPI para el envió y la recepción
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
