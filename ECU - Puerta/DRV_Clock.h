/***************************************************************************//**
  @file
  @brief
  @author   CCB
 ******************************************************************************/

#ifndef _DRV_CLOCK_H_
#define _DRV_CLOCK_H_

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

// Initializes Clocks used in the main app
void Clock_Init(void);

// RTI Callback where the Clocks go from 0 to 1, to be used in the main cycle
void Clock_L_Callback(void);

/*******************************************************************************
 ******************************************************************************/

#endif // _DRV_CLOCK_H_
