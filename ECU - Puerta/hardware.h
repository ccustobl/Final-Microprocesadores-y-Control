/***************************************************************************//**
  @file     hardware.h
  @brief    Hardware, system and MCU registers management
  @author   Nicol�s Magliola
 ******************************************************************************/

#ifndef _HARDWARE_H_
#define _HARDWARE_H_

/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/

#include "common.h"
#include <msp430.h>


/*******************************************************************************
 * CONSTANT AND MACRO DEFINITIONS USING #DEFINE
 ******************************************************************************/

/***** SYSTEM defines *********************************************************/

#define DCOCLK_FREQ_HZ          1000000UL // 1MHz
#define MCLK_PRESCALER_2POW     0
#define SMCLK_PRESCALER_2POW    0
#define MCLK_FREQ_HZ            (DCOCLK_FREQ_HZ/(1UL<<MCLK_PRESCALER_2POW)) // 1MHz
#define SMCLK_FREQ_HZ           (DCOCLK_FREQ_HZ/(1UL<<SMCLK_PRESCALER_2POW)) // 1MHz

// Period of routine time interruption in microseconds
#define RTI_PERIOD_US           500 // us
#define MAX_RTI_CALLBACKS       6

// ISR
#define MAX_ISR_CALLBACKS       4



#define enable_interrupts()     _BIS_SR(GIE)
#define disable_interrupts()    _BIC_SR(GIE)


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

#endif // _HARDWARE_H_
