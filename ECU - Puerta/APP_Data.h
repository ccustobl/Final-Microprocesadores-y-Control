/***************************************************************************//**
  @file
  @brief
  @author   CCB
 ******************************************************************************/

#ifndef _APP_DATA_H_
#define _APP_DATA_H_

/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/

/*******************************************************************************
 * CONSTANT AND MACRO DEFINITIONS USING #DEFINE
 ******************************************************************************/

// ISR
#define ISR_CALLBACK_NUMBER         10

// RTI
#define RTI_CALLBACK_NUMBER         10

// ADC
#define ADC_RTI_PERIOD              3000
#define ADC_PINS_USED               2

// Updates
#define UaT_RTI_PERIOD              5000 // 1000 us = 1 ms

// Lock
#define LOCK_RTI_PERIOD             2000 // 1000 us = 1 ms

// Clock
#define CLOCK_L_RTI_PERIOD          1000 // 1000 us = 1 ms

// CAN_ID Options
#define DIP_SWITCH                  0
#define SOFT                        1

// Select CAN_ID selection method
#define CAN_ID_SELECT               DIP_SWITCH

// Select ECU with Soft
#define CAN_ID_SOFT                 ECU_PUERTA_DELANTERA_IZQUIERDA


/******************************************************************************/

#if CAN_ID_SELECT == SOFT

#define DIP_SWITCH_CAPTURE          FALSE

#if CAN_ID == ECU_COMANDO

// DLC
#define DLC                                 0x02

// Data
#define RECEIVE_BYTES                       3
#define TRANSMIT_BYTES                      2


#elif CAN_ID_SOFT == ECU_PUERTA_DELANTERA_IZQUIERDA

// DLC
#define DLC                                 0x03

// Data
#define RECEIVE_BYTES                       2
#define TRANSMIT_BYTES                      3

// Info
#define NUMERO_PUERTA                       PUERTA_DELANTERA_IZQUIERDA


#elif CAN_ID_SOFT == ECU_PUERTA_DELANTERA_DERECHA

// DLC
#define DLC                                 0x03

// Data
#define RECEIVE_BYTES                       2
#define TRANSMIT_BYTES                      3

// Info
#define NUMERO_PUERTA                       PUERTA_DELANTERA_DERECHA


#elif CAN_ID_SOFT == ECU_PUERTA_TRASERA_IZQUIERDA

// DLC
#define DLC                                 0x03

// Data
#define RECEIVE_BYTES                       2
#define TRANSMIT_BYTES                      3

// Info
#define NUMERO_PUERTA                       PUERTA_TRASERA_IZQUIERDA


#elif CAN_ID_SOFT == ECU_PUERTA_TRASERA_DERECHA

// DLC
#define DLC                                 0x03

// Data
#define RECEIVE_BYTES                       2
#define TRANSMIT_BYTES                      3

// Info
#define NUMERO_PUERTA                       PUERTA_TRASERA_DERECHA


#else
#error ERROR ECU Identification Incorrectly Defined
#endif

#elif CAN_ID_SELECT == DIP_SWITCH

#define DIP_SWITCH_CAPTURE          TRUE

// DLC
#define DLC                                 0x03

// Data
#define RECEIVE_BYTES                       2
#define TRANSMIT_BYTES                      3

#endif

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

#endif // _APP_DATA_H_
