/***************************************************************************//**
  @file     board.h
  @brief    Board management
  @author   CCB
 ******************************************************************************/

#ifndef _BOARD_H_
#define _BOARD_H_

/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/
#include "gpio.h"

/*******************************************************************************
 * CONSTANT AND MACRO DEFINITIONS USING #DEFINE
 ******************************************************************************/
// SPI
#define MOSI PORTNUM2PIN(1,7)
#define MISO PORTNUM2PIN(1,6)
#define CLK PORTNUM2PIN(1,5)
#define CS PORTNUM2PIN(1,2)

/******************************************************************************/
// BOTONERA
#define B_CLK       PORTNUM2PIN(1,4)
#define B_SH        PORTNUM2PIN(1,3)
//#define B_CLK_INH   PORTNUM2PIN()
#define B_REC       PORTNUM2PIN(1,1)

/******************************************************************************/
// LEDs
#define P_G  PORTNUM2PIN(1,0)
#define P_A  PORTNUM2PIN(2,6)
#define P_B  PORTNUM2PIN(2,7)

/******************************************************************************/
// On Board User LEDs
#define PIN_LED_RED         PORTNUM2PIN(1,0) // P1.0
#define PIN_LED_GREEN       PORTNUM2PIN(1,6) // P1.6
#define LED_ACTIVE          HIGH

// On Board User Switches
#define PIN_SWITCH_S2       PORTNUM2PIN(1,3) // P1.3
#define SWITCH_INPUT        INPUT_PULLUP
#define SWITCH_ACTIVE       LOW


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

#endif // _BOARD_H_
