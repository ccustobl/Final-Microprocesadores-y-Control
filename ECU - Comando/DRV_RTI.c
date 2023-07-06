/***************************************************************************//**
  @file     DRV_RTI.c
  @brief    Real Time Interrupt (RTI) driver
  @author   CCB
 ******************************************************************************/

/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/
#include <DRV_RTI.h>

/*******************************************************************************
 * CONSTANT AND MACRO DEFINITIONS USING #DEFINE
 ******************************************************************************/

/*******************************************************************************
 * ENUMERATIONS AND STRUCTURES AND TYPEDEFS
 ******************************************************************************/
typedef struct
{
    rti_callback_t rtiCallback;
    uint16_t cont;
    uint16_t target;
} CallbackFun_t;
/*******************************************************************************
 * VARIABLES WITH GLOBAL SCOPE
 ******************************************************************************/

/*******************************************************************************
 * FUNCTION PROTOTYPES FOR PRIVATE FUNCTIONS WITH FILE LEVEL SCOPE
 ******************************************************************************/

/*******************************************************************************
 * ROM CONST VARIABLES WITH FILE LEVEL SCOPE
 ******************************************************************************/
static CallbackFun_t CallbackArray[MAX_RTI_CALLBACKS];
/*******************************************************************************
 * STATIC VARIABLES AND CONST VARIABLES WITH FILE LEVEL SCOPE
 ******************************************************************************/

/*******************************************************************************
 *******************************************************************************
                        GLOBAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/
void RTI_Init(rti_callback_t fun, uint32_t period)
{
    static uint8_t i = 0;
    CallbackArray[i].rtiCallback = fun; // Register callback
    CallbackArray[i].target = period/RTI_PERIOD-1;
    CallbackArray[i].cont = period/RTI_PERIOD-1;
    CallbackArray[++i].rtiCallback = NULL;
}

/*******************************************************************************
 *******************************************************************************
                        LOCAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/

#pragma vector = RTI_VECTOR //Interval timer vector location
__interrupt void RTI_IntervalTimer(void)
{
    volatile uint16_t dummy;
    dummy=(TA1IV);            // Clear Interrupt flag

    // Test RTI
//    gpioToggle(PIN_LED_RED);

    uint8_t i = 0;
    while (CallbackArray[i].rtiCallback != NULL)
    {
        if (CallbackArray[i].cont > 0)
            CallbackArray[i].cont--;
        else
        {
            CallbackArray[i].cont = CallbackArray[i].target;
            CallbackArray[i].rtiCallback();
        }
        i++;
    }
}

/******************************************************************************/
