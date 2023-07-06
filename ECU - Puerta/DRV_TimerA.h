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

/*******************************************************************************
 * CONSTANT AND MACRO DEFINITIONS USING #DEFINE
 ******************************************************************************/

// Timer modes
#define PWM_MODE                0
#define RTI_MODE                1
#define STOPWATCH_MODE          2

// Counter modes
#define STOP_MODE           MC_0   // Stop
#define UP_MODE             MC_1   // Up to CCR0
#define CONT_MODE           MC_2   // Up to 0xFFFF
#define UPDOWN_MODE         MC_3   // Up to CCR0 Down to 0

/*******************************************************************************
 * ENUMERATIONS AND STRUCTURES AND TYPEDEFS
 ******************************************************************************/


/*******************************************************************************
 * VARIABLE PROTOTYPES WITH GLOBAL SCOPE
 ******************************************************************************/

// Configures Timer A0 to its different uses
void TimerA0_Config( uint8_t mode , uint16_t ccr0 );

// Configures Timer A1 to its different uses
void TimerA1_Config( uint8_t mode , uint16_t ccr0 );

/*******************************************************************************
 * FUNCTION PROTOTYPES WITH GLOBAL SCOPE
 ******************************************************************************/

/*******************************************************************************
 ******************************************************************************/

#endif // _DRV_TIMERA_H_
