/***************************************************************************//**
  @file     DRV_TimerA.c
  @brief    Timer driver.
  @author   CCB
 ******************************************************************************/

/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/

#include "DRV_TimerA.h"

#include "hardware.h"
#include "board.h"
#include "gpio.h"

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
 * ROM CONST VARIABLES WITH FILE LEVEL SCOPE
 ******************************************************************************/

/*******************************************************************************
 * STATIC VARIABLES AND CONST VARIABLES WITH FILE LEVEL SCOPE
 ******************************************************************************/

/*******************************************************************************
 *******************************************************************************
                        GLOBAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/

void TimerA0_Config( uint8_t mode , uint16_t ccr0 )
{
    // Initial settings
    TA0CTL = STOP_MODE;                         // Set mode to STOP
    switch (mode)
    {
    case PWM_MODE:
        TA0CCTL1 = OUTMOD_7 | CCIE ;                    // Output Compare Reset(CCR1)/Set(CCR0) On TA0.1 (P1.2) + CCR1 interrupt enable
        TA0CCR0 = ccr0;                                 // Set PWM period T1 = 1/(2^20/(8*2621))=20ms

//        TA0CTL = TASSEL_2 | UP_MODE | TAIE | ID_0;      //  Clk Source: SMCLK , Timer Counter: Up Continuous - divide by 1
        TA0CTL = TASSEL_2 | UP_MODE | TAIE | ID_3;      //  Clk Source: SMCLK , Timer Counter: Up Continuous - divide by 8 //todo TODAS LAS CUENTAS LAS HACEMOS CON 8 NO 1

        break;
    }
}


void TimerA1_Config( uint8_t mode , uint16_t ccr0 )
{
    // Initial settings
    TA1CTL = STOP_MODE;                     // Set mode to STOP
    switch (mode)
    {
    case RTI_MODE:
        TA1CCR0 = ccr0;                     // Set CCR0 for RTI
        TA1CCTL0 = CCIE;                    // Enable CCR0 interruptions
        TA1CTL =TASSEL_2 | UP_MODE | TAIE | ID_3;  //  Clk Source: SMCLK , Timer Counter: Up Continuous - divide by 8 //todo TODAS LAS CUENTAS LAS HACEMOS CON 8 NO 1

        break;
    }
}


/*******************************************************************************
 *******************************************************************************
                        LOCAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/

#pragma vector=TIMER0_A1_VECTOR
__interrupt void isr_myA0OCIC(void)
{
    volatile uint16_t dummy;
    extern double actCCR1;

    dummy=(TA0IV);              // Clear Interrupt flag

    switch(dummy)
    {
    case TA0IV_TACCR1:          // OC Interrupt

        // PWM
        TA0CCR1 = actCCR1;

        break;
    case TA0IV_TACCR2:          // IC Interrupt
        break;
    case TA0IV_TAIFG:           // Timer Overflow
        break;
    }
}


#pragma vector=TIMER0_A0_VECTOR
__interrupt void isr_myA0ccr(void)
{
    // NOT USED
}


#pragma vector=TIMER1_A1_VECTOR
__interrupt void isr_myA1OCIC(void)
{
    volatile uint16_t dummy;
    extern double actCCR1;

    dummy=(TA1IV);              // Clear Interrupt flag

    switch(dummy)
    {
    case TA1IV_TACCR1:          // OC Interrupt
        break;
    case TA1IV_TACCR2:          // IC Interrupt
        break;
    case TA1IV_TAIFG:           // Timer Overflow
        break;
    }
}

/******************************************************************************/
