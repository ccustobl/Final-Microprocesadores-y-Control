/***************************************************************************//**
 @file
 @brief
 @author   CCB
 ******************************************************************************/

/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/

#include "DRV_Lock.h"

#include "gpio.h"
#include "board.h"

#include "APP_Data.h"

#include "DRV_RTI.h"

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

/*******************************************************************************
 *******************************************************************************
 GLOBAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/

void Lock_Init(uint8_t pin1,uint8_t pin2)
{
    // Initializing Pins
    Pin_Check(pin1);
    Pin_Check(pin2);

    gpioMode(pin1,INPUT);
    gpioMode(pin2,OUTPUT);

    // Periodic Checks
    RTI_Callback_Config(Lock_RTI_Callback,LOCK_RTI_PERIOD);
}


void Lock_LED_Update(void)
{
    extern int LOCKED;

    if(LOCKED==TRUE)
    {
        gpioWrite(PIN_LOCK_LED,HIGH);
    }
    else
    {
        gpioWrite(PIN_LOCK_LED,LOW);
    }
}


void Lock_SW_Check(void)
{
    extern int LOCKED;
    static int Block_Botton;

    if(Block_Botton==FALSE)
    {
        if(LOCKED == FALSE && gpioRead(PIN_LOCK_SW) == TRUE)
        {
            LOCKED = TRUE;
        }
        else if(LOCKED == TRUE && gpioRead(PIN_LOCK_SW) == TRUE)
        {
            LOCKED = FALSE;
        }

        Block_Botton = TRUE;
    }

    if(Block_Botton != gpioRead(PIN_LOCK_SW))
    {
        Block_Botton = FALSE;
    }


}

/*******************************************************************************
 *******************************************************************************
 LOCAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/

void Lock_RTI_Callback(void)
{
    extern int LOCK_CHECK;
    LOCK_CHECK = TRUE;
}

/******************************************************************************/

void Pin_Check(uint8_t pin)
{
    uint8_t port = PIN2PORT(pin);
    uint8_t num = PIN2NUM(pin);

    if((pin == PORTNUM2PIN(2,6)) || pin == PORTNUM2PIN(2,7))
    {
        switch(port)
        {
        case 1:
            BITCLR(P1SEL, num);
            BITCLR(P1SEL2, num);
            break;
        case 2:
            BITCLR(P2SEL, num);
            BITCLR(P2SEL2, num);
            break;
        }
    }
}
