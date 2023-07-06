/***************************************************************************//**
  @file
  @brief
  @author   MGV-CCB
 ******************************************************************************/
#ifndef _DRV_SPI_H_
#define _DRV_SPI_H_

/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/

#include <common.h>

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

void SPI_Init(void);
void SPI_ISR_Enable(void);
void SPI_ISR_Disable(void);
uint8_t transferSPI(uint8_t txdata);
void CS_OFF(void);
void CS_ON(void);

/*******************************************************************************
 ******************************************************************************/
#endif /* _DRV_SPI_H_ */
