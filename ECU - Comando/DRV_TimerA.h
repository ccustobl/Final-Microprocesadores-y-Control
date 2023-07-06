/***************************************************************************//**
  @file     timer.h
  @brief    Timer driver. Simple implementation, support multiple timers.
  @author   CCB
 ******************************************************************************/

#ifndef _DRV_TIMERA_H_
#define _DRV_TIMERA_H_

/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/
#include "common.h"
#include "hardware.h"
#include "board.h"

/*******************************************************************************
 * CONSTANT AND MACRO DEFINITIONS USING #DEFINE
 ******************************************************************************/

// Timer modes
#define PWM_MODE                0
#define RTI_MODE                1
#define STOPWATCH_MODE          2

// Counter modes
#define STOP_MODE           MC_0
#define UP_MODE             MC_1
#define CONT_MODE           MC_2
#define UPDOWN_MODE         MC_3

/*******************************************************************************
 * ENUMERATIONS AND STRUCTURES AND TYPEDEFS
 ******************************************************************************/

/*******************************************************************************
 * VARIABLE PROTOTYPES WITH GLOBAL SCOPE
 ******************************************************************************/
// Configures Timer A0 to its different uses
void TimerA0_config( uint8_t mode , uint16_t ccr0 );

// Configures Timer A1 to its different uses
void TimerA1_config( uint8_t mode , uint16_t ccr0 );

/*******************************************************************************
 * FUNCTION PROTOTYPES WITH GLOBAL SCOPE
 ******************************************************************************/

/*******************************************************************************
 ******************************************************************************/

#endif // _DRV_TIMERA_H_
