/***************************************************************************//**
  @file
  @brief
  @author   CCB
 ******************************************************************************/

#ifndef _DRV_LOCK_H_
#define _DRV_LOCK_H_

/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/

#include "common.h"

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

// Initializes the input and output pins, and the RTI Callback used to check the Locking Status
void Lock_Init(uint8_t pin1,uint8_t pin2);

// Turns On or Off the Locked System LED
void Lock_LED_Update(void);

// RTI Callback that alerts to check Locking System Status
void Lock_RTI_Callback(void);

// Checking input pin status
void Pin_Check(uint8_t pin);

// Updating Lock System Status
void Lock_SW_Check(void);

/*******************************************************************************
 ******************************************************************************/

#endif // _DRV_LOCK_H_
