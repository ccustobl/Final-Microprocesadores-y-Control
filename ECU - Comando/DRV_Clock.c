/***************************************************************************//**
 @file
 @brief
 @author   CCB
 ******************************************************************************/

/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/
#include "DRV_Clock.h"

/*******************************************************************************
 * CONSTANT AND MACRO DEFINITIONS USING #DEFINE
 ******************************************************************************/

/*******************************************************************************
 * ENUMERATIONS AND STRUCTURES AND TYPEDEFS
 ******************************************************************************/

/*******************************************************************************
 * VARIABLES WITH GLOBAL SCOPE
 ******************************************************************************/

/*******************************************************************************
 * FUNCTION PROTOTYPES FOR PRIVATE FUNCTIONS WITH FILE LEVEL SCOPE
 ******************************************************************************/

/*******************************************************************************
 * STATIC VARIABLES AND CONST VARIABLES WITH FILE LEVEL SCOPE
 ******************************************************************************/
static uint16_t CLOCK_L_RTI_Counter;
static uint16_t CLOCK_P_RTI_Counter;
/*******************************************************************************
 *******************************************************************************
 GLOBAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/
void Clock_Callback(void)
{
    extern uint16_t CLOCK_L;
    extern uint16_t CLOCK_P;

    if(CLOCK_L_RTI_Counter >= CLOCK_L_RTI_PERIOD)
    {
        CLOCK_L += 1;
        CLOCK_L_RTI_Counter = 0;

    }
    else
    {
        CLOCK_L_RTI_Counter += 1;
    }

    if(CLOCK_P_RTI_Counter >= CLOCK_P_RTI_PERIOD)//
    {
        CLOCK_P += 1;
        CLOCK_P_RTI_Counter = 0;
    }
    else
    {
        CLOCK_P_RTI_Counter++;
    }
}

/*******************************************************************************
 *******************************************************************************
 LOCAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/

/******************************************************************************/
