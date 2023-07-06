/***************************************************************************//**
 @file
 @brief
 @author   CCB
 ******************************************************************************/

/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/

#include "DRV_ISR.h"

#include <msp430.h>
#include "board.h"

#include "APP_Data.h"

/*******************************************************************************
 * CONSTANT AND MACRO DEFINITIONS USING #DEFINE
 ******************************************************************************/

/*******************************************************************************
 * ENUMERATIONS AND STRUCTURES AND TYPEDEFS
 ******************************************************************************/

typedef struct
{
    void (*ISR_Callback)(void);
    uint16_t Port;
    uint16_t Number;
    uint16_t Mask;
}ISR_Callback_Struct;

/*******************************************************************************
 * VARIABLES WITH GLOBAL SCOPE
 ******************************************************************************/

/*******************************************************************************
 * FUNCTION PROTOTYPES FOR PRIVATE FUNCTIONS WITH FILE LEVEL SCOPE
 ******************************************************************************/

/*******************************************************************************
 * STATIC VARIABLES AND CONST VARIABLES WITH FILE LEVEL SCOPE
 ******************************************************************************/

static ISR_Callback_Struct ISR_Callback_List[ISR_CALLBACK_NUMBER];

/*******************************************************************************
 *******************************************************************************
 GLOBAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/

void ISR_Callback_Config (void(*funcallback)(void),uint8_t pin, uint8_t edge_selected)
{
    static unsigned int i=0;

    ISR_Callback_List[i].ISR_Callback = funcallback;
    ISR_Callback_List[i].Port =  PIN2PORT(pin);
    ISR_Callback_List[i].Number =  PIN2NUM(pin);
    ISR_Callback_List[i].Mask = 1 << ISR_Callback_List[i].Number;

    switch (ISR_Callback_List[i].Port)
    {
    case 1:
        if(edge_selected==RISING)
        {
            P1IES &= ~ISR_Callback_List[i].Mask;   // Rising Edge = 0
        }
        else // (edge_selected==FALLING)
        {
            P1IES |= ISR_Callback_List[i].Mask;    // Falling Edge = 1
        }
        P1IFG &= ~ISR_Callback_List[i].Mask;       // Clear Pending Flags
        P1IE |=  ISR_Callback_List[i].Mask;        // Enable Port1 Interrupts
        break;
    case 2:
        if(edge_selected==RISING)
        {
            P2IES &= ~ISR_Callback_List[i].Mask;   // Rising Edge = 0
        }
        else // (edge_selected==FALLING)
        {
            P2IES |= ISR_Callback_List[i].Mask;    // Falling Edge = 1
        }
        P2IFG &= ~ISR_Callback_List[i].Mask;       // Clear Pending Flags
        P2IE |=  ISR_Callback_List[i].Mask;        // Enable Port2 Interrupts
        break;
    }

    ISR_Callback_List[++i].ISR_Callback = NULL;
}



/*******************************************************************************
 *******************************************************************************
 LOCAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/

/******************************************************************************/

#pragma vector=PORT1_VECTOR             //Interrupt Service Routine (ISR) for Port 1
__interrupt void ISR_PORT1(void)
{
    uint8_t i = 0;

    while (ISR_Callback_List[i].ISR_Callback != NULL)
    {
        if(P1IFG & ISR_Callback_List[i].Mask && ISR_Callback_List[i].Port == 1)
        {
            P1IFG &= ~ISR_Callback_List[i].Mask;        // Clear Pending Flag
            ISR_Callback_List[i].ISR_Callback();        // ISR Callback
        }
        i++;
    }
}


#pragma vector=PORT2_VECTOR             //Interrupt Service Routine (ISR) for Port 2
__interrupt void ISR_PORT2(void)
{
//    gpioWrite(PORTNUM2PIN(2,3),HIGH);

    uint8_t i = 0;

    while (ISR_Callback_List[i].ISR_Callback != NULL)
    {
        if(P2IFG & ISR_Callback_List[i].Mask && ISR_Callback_List[i].Port == 2)
        {
            P2IFG &= ~ISR_Callback_List[i].Mask;        // Clear Pending Flag
            ISR_Callback_List[i].ISR_Callback();        // ISR Callback
        }
        i++;
    }
//    gpioWrite(PORTNUM2PIN(2,3),LOW);
}
