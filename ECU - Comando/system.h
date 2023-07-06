/***************************************************************************//**
  @file     system.h
  @brief    MCU system driver
  @author   CCB
 ******************************************************************************/

#ifndef _SYSTEM_H_
#define _SYSTEM_H_

/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/
#include "common.h"
#include "board.h"
#include "hardware.h"
#include "DRV_TimerA.h"
#include "DRV_RTI.h"

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

/**
 * @brief Initialize system first: internal MCU stuffs (clock, watchdog, etc.)
 */
void System_Init_First(void);


/**
 * @brief Initialize board: MCU - board interface stuffs (pinout, etc.)
 */
void Board_Init(void);


/**
 * @brief Initialize system last: internal MCU stuffs (ISR, etc.)
 */
void System_Init_Last(void);

/*******************************************************************************
 ******************************************************************************/

#endif // _SYSTEM_H_
