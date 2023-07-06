/***************************************************************************//**
  @file
  @brief
  @author   CCB
 ******************************************************************************/

#ifndef _DRV_PWM_H_
#define _DRV_PWM_H_

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

// Initializes Timer A for PWM use (constant period and variable duty-cycle)
void PWM_Init(void);

// Configures a pin to receive PWM signals
void PWM_Config_InputPin(uint8_t pin);

// Configures a pin to output PWM signals
void PWM_Config_OutputPin(uint8_t pin);

// Updates the variable used to change Ta0CCR1, changing the duty-cycle for the PWM signal
void PWM_Value(int Percentage);

/*******************************************************************************
 ******************************************************************************/

#endif // _DRV_PWM_H_
