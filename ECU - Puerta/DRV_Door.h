/***************************************************************************//**
  @file
  @brief
  @author   CCB
 ******************************************************************************/

#ifndef _DRV_DOOR_H_
#define _DRV_DOOR_H_

/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/

#include "common.h"

/*******************************************************************************
 * CONSTANT AND MACRO DEFINITIONS USING #DEFINE
 ******************************************************************************/

// ADC
#define ADC_Smooth(cv,mv,n)         ((mv)*(n-1)/n+(cv)/n)

/*******************************************************************************
 * ENUMERATIONS AND STRUCTURES AND TYPEDEFS
 ******************************************************************************/

/*******************************************************************************
 * VARIABLE PROTOTYPES WITH GLOBAL SCOPE
 ******************************************************************************/

/*******************************************************************************
 * FUNCTION PROTOTYPES WITH GLOBAL SCOPE
 ******************************************************************************/

// Initializes RTI and ISR Callbacks needed by the Door to work
void Door_Init(void);

// Configures the pin that Enables the PWM signal to go through the H-Bridge
void PWM_Config_EnablePin(uint8_t pin);

// Configures the Open/Close pin to simulate the Door opening and closing
void Door_OC_InputPin(uint8_t pin);

// Gets the measurement corresponding to the current pin
unsigned int ADC_Capture_Current(void);

// Lowers the window
void Window_Lower(void);

// Rises the window
void Window_Rise(void);

// Stops the window
void Window_Stop(void);

// Checks if the Door is opened or closed
uint8_t Door_State(void);

// Reads the CAN Buffer, checks if the message is addressed to the Door in use and updates the Instructions
void Receive_Check_Update(void);

// Updates the Door Status (Voltage, Open/Close, and Window rising/lowering/stopped)
void Update_and_Transmit(void);




/*******************************************************************************
 ******************************************************************************/

#endif // _DRV_DOOR_H_
