/***************************************************************************//**
  @file     DRV_RTI.c
  @brief    Real Time Interrupt (RTI) driver
  @author   CCB
 ******************************************************************************/

/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/

#include "DRV_RTI.h"

#include "hardware.h"
#include "board.h"
#include "gpio.h"

#include "APP_Data.h"

#include "DRV_TimerA.h"

/*******************************************************************************
 * CONSTANT AND MACRO DEFINITIONS USING #DEFINE
 ******************************************************************************/

/*******************************************************************************
 * ENUMERATIONS AND STRUCTURES AND TYPEDEFS
 ******************************************************************************/

typedef struct
{
    void (*RTI_Callback)(void);
    uint16_t Counter;
    uint16_t RTI_Cycles;
}RTI_Callback_Struct;


/*******************************************************************************
 * VARIABLES WITH GLOBAL SCOPE
 ******************************************************************************/

/*******************************************************************************
 * FUNCTION PROTOTYPES FOR PRIVATE FUNCTIONS WITH FILE LEVEL SCOPE
 ******************************************************************************/

/*******************************************************************************
 * ROM CONST VARIABLES WITH FILE LEVEL SCOPE
 ******************************************************************************/

/*******************************************************************************
 * STATIC VARIABLES AND CONST VARIABLES WITH FILE LEVEL SCOPE
 ******************************************************************************/

static RTI_Callback_Struct RTI_Callback_List[RTI_CALLBACK_NUMBER];

/*******************************************************************************
 *******************************************************************************
                        GLOBAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/

void RTI_Init(void)
{
    TimerA1_Config(RTI_MODE,PULSES_PER_RTI);
}

void RTI_Callback_Config(void(*funcallback)(void),uint32_t period)
{
    static unsigned int i=0;

    RTI_Callback_List[i].RTI_Callback = funcallback;
    RTI_Callback_List[i].RTI_Cycles = period/RTI_PERIOD-1;
    RTI_Callback_List[i].Counter = RTI_Callback_List[i].RTI_Cycles;
    RTI_Callback_List[++i].RTI_Callback = NULL;
}

/*******************************************************************************
 *******************************************************************************
                        LOCAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/

#pragma vector = RTI_VECTOR //Interval timer vector location
__interrupt void RTI_IntervalTimer(void)
{
//    gpioWrite(PORTNUM2PIN(2,3),HIGH);

    volatile uint16_t dummy;
    volatile unsigned int i=0;

    dummy=(TA1IV);            // Clear Interrupt flag

    while (RTI_Callback_List[i].RTI_Callback != NULL)
    {
        if (RTI_Callback_List[i].Counter> 0)
            RTI_Callback_List[i].Counter--;
        else
        {
            RTI_Callback_List[i].Counter = RTI_Callback_List[i].RTI_Cycles;
            RTI_Callback_List[i].RTI_Callback();
        }
        i++;
    }

//    gpioWrite(PORTNUM2PIN(2,3),LOW);
}

