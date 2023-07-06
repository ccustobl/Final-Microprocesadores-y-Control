/***************************************************************************//**
  @file     DRV_TimerA.c
  @brief    Timer driver. Simple implementation, support multiple timers
  @author   CCB
 ******************************************************************************/

/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/
#include <DRV_TimerA.h>

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

void TimerA0_config( uint8_t mode , uint16_t ccr0 )
{
    // Initial settings
    TA0CTL = STOP_MODE;                         // Set mode to STOP
    switch (mode)
    {
    case PWM_MODE:
        TA0CCTL1 = OUTMOD_7 | CCIE ;                    // Output Compare Reset(CCR1)/Set(CCR0) On TA0.1 (P1.2) + CCR1 interrupt enable
        TA0CCR0 = ccr0;                                 // Set PWM period T1 = 1/(2^20/(8*ccr0))=x ms      si ccr0=2621 T1=20ms, si ccr0=1311 T1=10ms, si ccr0=131 T1=1ms
        TA0CCR0 = 1;
        TA0CTL = TASSEL_2 | UP_MODE | TAIE | ID_3;      //  Clk Source: SMCLK , Timer Counter: Up Continuous - divide by 8

        break;
    }
}


void TimerA1_config( uint8_t mode , uint16_t ccr0 )
{
    // Initial settings
    TA1CTL = STOP_MODE;                     // Set mode to STOP
    switch (mode)
    {
    case RTI_MODE:
        TA1CCR0 = ccr0;                     // Set CCR0 for RTI
        TA1CCTL0 = CCIE;                    // Enable CCR0 interruptions
        TA1CTL = UP_MODE | TASSEL_2 | ID_0; // Start timer + clock set to 1MHz

        break;
    }
}


/*******************************************************************************
 *******************************************************************************
                        LOCAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/

#pragma vector=TIMER0_A1_VECTOR        //Interrupt Service Routine (ISR) for overflow and CCR<1-n> Timer0A1
__interrupt void isr_myA0OCIC(void)
{
    volatile uint16_t dummy;

    dummy=(TA0IV);            // Clear Interrupt flag

    switch(dummy)
    {
    case TA0IV_TACCR1:           // OC Interrupt
        break;
    case TA0IV_TACCR2:           // IC Interrupt
        break;
    case TA0IV_TAIFG:            // Timer Overflow
        break;
    }
}


#pragma vector=TIMER0_A0_VECTOR        //Interrupt Service Routine (ISR) for CCR0 (only)
__interrupt void isr_myA0ccr(void)
{
    // NOT USED
}


#pragma vector=TIMER1_A1_VECTOR        //Interrupt Service Routine (ISR) for overflow and CCR<1-n> Timer1A1
__interrupt void isr_myA1OCIC(void)
{
    volatile uint16_t dummy;

    dummy=(TA1IV);            // Clear Interrupt flag

    switch(dummy)
    {
    case TA1IV_TACCR1:           // IC Interrupt
        break;
    case TA1IV_TACCR2:           // IC Interrupt
        break;
    case TA1IV_TAIFG:            // Timer Overflow
        break;
    }
}

/******************************************************************************/
