/***************************************************************************//**
  @file     rti.h
  @brief    Real Time Interrupt (RTI) driver
  @author   CCB
 ******************************************************************************/

#ifndef _DRV_RTI_H_
#define _DRV_RTI_H_

/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/
#include "common.h"
#include "hardware.h"
#include "board.h"
#include <msp430.h>
#include "APP_Data.h"

/*******************************************************************************
 * CONSTANT AND MACRO DEFINITIONS USING #DEFINE
 ******************************************************************************/

#define TIMERA1             0
#define WD                  1

#define RTI_SOURCE          TIMERA1

#if (RTI_SOURCE == WD)
#define RTI_VECTOR          WDT_VECTOR
#elif (RTI_SOURCE == TIMERA1)
#define RTI_VECTOR          TIMER1_A0_VECTOR
#else
#error RTI source incorrectly difined
#endif

// Period of RTI in us
#define MAX_RTI_CALLBACKS       6
#define RTI_PERIOD              1000 // us

#define PULSES_PER_RTI          DCOCLK_FREQ_HZ/1000000*RTI_PERIOD

// Time step in microseconds
#define TIME_STEP          1000 //us

/*******************************************************************************
 * ENUMERATIONS AND STRUCTURES AND TYPEDEFS
 ******************************************************************************/
typedef void (*rti_callback_t)(void);
/*******************************************************************************
 * VARIABLE PROTOTYPES WITH GLOBAL SCOPE
 ******************************************************************************/

/*******************************************************************************
 * FUNCTION PROTOTYPES WITH GLOBAL SCOPE
 ******************************************************************************/

// Configures the RTI Callbacks
void RTI_Init(rti_callback_t fun, uint32_t period);

/*******************************************************************************
 ******************************************************************************/

#endif // _DRV_RTI_H_
