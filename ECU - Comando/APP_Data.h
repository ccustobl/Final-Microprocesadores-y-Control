/***************************************************************************//**
  @file
  @brief
  @author   CCB
 ******************************************************************************/

#ifndef _APP_DATA_H_
#define _APP_DATA_H_

/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/
#include "common.h"
#include "DRV_Clock.h"
#include "hardware.h"
#include "board.h"
#include <msp430.h>

/*******************************************************************************
 * CONSTANT AND MACRO DEFINITIONS USING #DEFINE
 ******************************************************************************/

// ADC
#define ADC_RTI_PERIOD              10
#define ADC_PINS_USED               2

// Current Control
#define Current_Threshold           40

// Clock
#define CLOCK_L_RTI_PERIOD          10 // 10 x 1000 us = 10 ms
#define CLOCK_P_RTI_PERIOD          1  // 1 x 1000 us = 1 ms

//COMANDO
#define ESTADO_DETENIDO 0X00
#define ESTADO_SUBIENDO 0X01
#define ESTADO_BAJANDO  0X02

//Ventana
#define CERRADA 0
#define ABIERTA 1
#define POS_TOLERANCE 5
#define POS_EMBOLSADO 150 //todo definir cuanto tiene que ser lo que baje

/*******************************************************************************
 * ENUMERATIONS AND STRUCTURES AND TYPEDEFS
 ******************************************************************************/

/*******************************************************************************
 * VARIABLE PROTOTYPES WITH GLOBAL SCOPE
 ******************************************************************************/

/*******************************************************************************
 * FUNCTION PROTOTYPES WITH GLOBAL SCOPE
 ******************************************************************************/

/*******************************************************************************
 ******************************************************************************/

#endif // _APP_DATA_H_
