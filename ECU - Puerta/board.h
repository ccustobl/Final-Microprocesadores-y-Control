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


/******************************************************************************/

// PWM

// PWM Signal
#define PIN_PWM             PORTNUM2PIN(1,2) // P1.2

// PWM Direction
#define PIN_PWM_DIR         PORTNUM2PIN(2,5) // P2.5

// PWM Enable
#define PIN_PWM_ENABLE      PORTNUM2PIN(2,4) // P2.4

/******************************************************************************/

// ADC

// ADC Current Input
#define PIN_ADC_CURR        PORTNUM2PIN(1,3) // P1.3

// ADC Voltage Input
#define PIN_ADC_VOLT        PORTNUM2PIN(1,4) // P1.4

/******************************************************************************/

// CAN & SPI

// CAN ISR PIN
#define PIN_CAN_ISR         PORTNUM2PIN(2,1) // P2.1

// CAN MISO PIN
#define PIN_CAN_MISO        PORTNUM2PIN(1,6) // P1.6

// CAN MOSI PIN
#define PIN_CAN_MOSI        PORTNUM2PIN(1,7) // P1.7

// CAN SCK PIN
#define PIN_CAN_SCK         PORTNUM2PIN(1,5) // P1.5

// CAN CS PIN
#define PIN_CAN_CS          PORTNUM2PIN(2,0) // P2.0

/******************************************************************************/

// SW

// Door Open-Close SW
#define PIN_OC_SW           PORTNUM2PIN(2,2) // P2.2

/******************************************************************************/

// DIP-Switch

//BIT 0
#define PIN_DPS_1           PORTNUM2PIN(1,0) // P1.0

//BIT 1
#define PIN_DPS_2           PORTNUM2PIN(1,1) // P1.1

/******************************************************************************/

// Door Lock

// Door Lock SW
#define PIN_LOCK_SW         PORTNUM2PIN(2,6) // P2.6

// Door Lock LED
#define PIN_LOCK_LED        PORTNUM2PIN(2,7) // P2.7

/******************************************************************************/

// Basic Ports

// On Board User LEDs
#define PIN_LED_RED         PORTNUM2PIN(1,1) // P1.0
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
