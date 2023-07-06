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

#include <common.h>

/*******************************************************************************
 * CONSTANT AND MACRO DEFINITIONS USING #DEFINE
 ******************************************************************************/

#define TIMERA1                 0
#define WD                      1

#define RTI_SOURCE              TIMERA1

#if (RTI_SOURCE == WD)
#define RTI_VECTOR              WDT_VECTOR
#elif (RTI_SOURCE == TIMERA1)
#define RTI_VECTOR              TIMER1_A0_VECTOR
#else
#error RTI source incorrectly difined
#endif

// Period of RTI in us
#define RTI_PERIOD              1000 // us

#define PULSES_PER_RTI          DCOCLK_FREQ_HZ/1000000*RTI_PERIOD


/*******************************************************************************
 * ENUMERATIONS AND STRUCTURES AND TYPEDEFS
 ******************************************************************************/

/*******************************************************************************
 * VARIABLE PROTOTYPES WITH GLOBAL SCOPE
 ******************************************************************************/

/*******************************************************************************
 * FUNCTION PROTOTYPES WITH GLOBAL SCOPE
 ******************************************************************************/

// Initializes the Timer A Clock to work as RTI
void RTI_Init(void);

// Configures the RTI Callbacks
void RTI_Callback_Config(void(*funcallback)(void),uint32_t period);

/*******************************************************************************
 ******************************************************************************/

#endif // _DRV_RTI_H_
