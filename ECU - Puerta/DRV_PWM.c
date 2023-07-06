/***************************************************************************//**
 @file
 @brief
 @author   CCB
 ******************************************************************************/

/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/

#include "DRV_PWM.h"

#include <msp430.h>
#include "board.h"

#include "DRV_TimerA.h"

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
double actCCR1;

/*******************************************************************************
 *******************************************************************************
 GLOBAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/

void PWM_Init(void)
{
    // OC Settings for PWM
    DCOCTL = 0;
//    TimerA0_Config(PWM_MODE,437);     //17474 = 20ms  874=1ms
    TimerA0_Config(PWM_MODE,131);     //todo Chequear PWM Calculations
}


void PWM_Config_InputPin(uint8_t pin)
{
    uint8_t port = PIN2PORT(pin);
    uint8_t num = PIN2NUM(pin);

    switch(port)
    {
    case 1:
        P1DIR &= ~(port<<num);
        P1SEL |= (port<<num);
        P1SEL2 &= ~(port<<num);
        break;
    case 2:
        P2DIR &= ~(port<<num);
        P2SEL |= (port<<num);
        P2SEL2 &= ~(port<<num);
        break;
    }
}


void PWM_Config_OutputPin(uint8_t pin)
{
    uint8_t port = PIN2PORT(pin);
    uint8_t num = PIN2NUM(pin);

    switch(port)
    {
    case 1:
        P1DIR |= (port<<num);
        P1SEL |= (port<<num);
        P1SEL2 &= ~(port<<num);
        break;
    case 2:
        P2DIR |= (port<<num);
        P2SEL |= (port<<num);
        P2SEL2 &= ~(port<<num);
        break;
    }
}


void PWM_Value(int Percentage)
{
    volatile unsigned int dummy;
    //dummy = 437*0.01;
    //actCCR1 = dummy*Percentage;
    //todo MODIFICAMOS ESTO CON NICO PARA NO PERDER PRECISIÓN
//    dummy = Percentage * 437;
    dummy = Percentage * 131;
    dummy = dummy / 100;
    actCCR1 = dummy;
}

