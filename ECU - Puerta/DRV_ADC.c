/***************************************************************************//**
 @file
 @brief
 @author   CCB
 ******************************************************************************/

/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/

#include <DRV_ADC.h>

#include "APP_Data.h"
#include "board.h"

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

unsigned int ADC_Meassurements[ADC_PINS_USED] = {0};

/*******************************************************************************
 *******************************************************************************
 GLOBAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/

void ADC_Init(void)
{
    // Configure ADC10
    ADC10CTL0 |= SREF_0 | ADC10SHT_0 | ADC10ON | MSC | ADC10IE;         // VR+ = AVCC and VR- = AVSS, 4 x ADC10CLKs, On/Enable, Multiple SampleConversion, Interrupt Enable
    ADC10CTL1 |= SHS_0 | ADC10DIV_0 | ADC10SSEL_3 | CONSEQ_1 | INCH_4;  // ADC10SC, ADC10 Clock Divider Select 0, SMCLK, Sequence of channels
//    ADC10CTL1 |= SHS_2 | ADC10DIV_0 | ADC10SSEL_3 | CONSEQ_1 | INCH_4;  // ADC10SC, ADC10 Clock Divider Select 0, SMCLK, Sequence of channels
    ADC10SA |= (unsigned int)ADC_Meassurements[0];
    ADC10DTC1 = 0x02;               // 2 conversions
    ADC10AE0 |= BIT_3 | BIT_4 ;     // Pin 1.3 and 1.4

    ADC10CTL0 |= ENC | ADC10SC;     // Sampling and conversion start

    RTI_Callback_Config(ADC_RTI_Callback,ADC_RTI_PERIOD);
}

void ADC_Config_InputPin(uint8_t pin)
{
    uint8_t port = PIN2PORT(pin);
    uint8_t num = PIN2NUM(pin);

    switch(port)
    {
    case 1:
        P1DIR &= ~(port<<num);
        ADC10AE0 |= (port<<num);
        break;
    case 2:
        P2DIR &= ~(port<<num);
        ADC10AE0 |= (port<<num);
        break;
    }
}

unsigned int ADC_Read(uint8_t pin)
{
    unsigned int ADC_Value;
    volatile uint8_t var = 2;

    if(pin==PIN_ADC_CURR)
    {
        var = 1;
    }
    else if(pin==PIN_ADC_VOLT)
    {
        var = 0;
    }

    ADC10CTL0 &= ~ENC;
    while (ADC10CTL1 & BUSY);
    ADC_Value = ADC_Meassurements[var];
    ADC10CTL0 |= ENC | ADC10SC;     // Sampling and conversion start

    return ADC_Value;
}

/*******************************************************************************
 *******************************************************************************
 LOCAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/

void ADC_RTI_Callback(void)
{
    ADC10CTL0 &= ~ENC;
    while (ADC10CTL1 & BUSY);               // Wait if ADC10 core is active
    ADC10SA = (unsigned int)ADC_Meassurements;
    ADC10CTL0 |= ENC + ADC10SC;             // Sampling and conversion start
}

/******************************************************************************/

// ADC interrupt service routine
#pragma vector=ADC10_VECTOR
__interrupt void ADC_ISR(void)
{
    ADC10CTL0 &= ~ADC10IFG;     // Clear ADCIFG Interrupt flag
    __bic_SR_register_on_exit(CPUOFF);
}

