/***************************************************************************//**
 @file
 @brief
 @author   MGV-CCB
 ******************************************************************************/

/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/
#include "DRV_LEDs.h"
#include <msp430.h>

/*******************************************************************************
 * CONSTANT AND MACRO DEFINITIONS USING #DEFINE
 ******************************************************************************/

/*******************************************************************************
 * ENUMERATIONS AND STRUCTURES AND TYPEDEFS
 ******************************************************************************/

/*******************************************************************************
 * VARIABLES WITH GLOBAL SCOPE
 ******************************************************************************/
unsigned int LED_CYCLE;
/*******************************************************************************
 * FUNCTION PROTOTYPES FOR PRIVATE FUNCTIONS WITH FILE LEVEL SCOPE
 ******************************************************************************/
void LED_Config_Pin(uint8_t pin);
/*******************************************************************************
 * STATIC VARIABLES AND CONST VARIABLES WITH FILE LEVEL SCOPE
 ******************************************************************************/

/*******************************************************************************
 *******************************************************************************
 GLOBAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/
void DRV_LEDs_Init(void){

    LED_Config_Pin(P_A);
    LED_Config_Pin(P_B);
    LED_Config_Pin(P_G);

    gpioWrite(P_G, HIGH); // Desactivo los LEDs
    gpioWrite(P_A, LOW);
    gpioWrite(P_B, LOW);
}

// ----------------------------------------------------------------------------------------------------------

void DRV_LEDs_Control(void)
{
//    extern char PUERTAS;
    extern int PUERTAS[4];

    //Reinicio el Cycle
    if(LED_CYCLE<4){
        LED_CYCLE++;
    }
    else{
        LED_CYCLE = 1;
    }

    if(PUERTAS[0] == 0 && PUERTAS[1] == 0 && PUERTAS[2] == 0 && PUERTAS[3] == 0){
        gpioWrite(P_G, HIGH);
    }
    else{
        switch(LED_CYCLE){
        case(1):
            gpioWrite(P_G, HIGH);
            if(PUERTAS[LED_CYCLE-1]){
                gpioWrite(P_A, HIGH);
                gpioWrite(P_B, HIGH);
                gpioWrite(P_G, LOW);
            }
        break;
        case(2):
            gpioWrite(P_G, HIGH);
            if(PUERTAS[LED_CYCLE-1]){
                gpioWrite(P_A, LOW);
                gpioWrite(P_B, LOW);
                gpioWrite(P_G, LOW);
            }
        break;
        case(3):
            gpioWrite(P_G, HIGH);
            if(PUERTAS[LED_CYCLE-1]){
                gpioWrite(P_A, LOW);
                gpioWrite(P_B, HIGH);
                gpioWrite(P_G, LOW);
            }
        break;
        case(4):
            gpioWrite(P_G, HIGH);
            if(PUERTAS[LED_CYCLE-1]){
                gpioWrite(P_A, HIGH);
                gpioWrite(P_B, LOW);
                gpioWrite(P_G, LOW);
            }
        break;
        }
    }
}


/*******************************************************************************
 *******************************************************************************
 LOCAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/

void LED_Config_Pin(uint8_t pin) // PXSEL | PXSEL2 = 1 | 0
{
    uint8_t port , num;
    uint8_t P_26 = PORTNUM2PIN(2,6);
    uint8_t P_27 = PORTNUM2PIN(2,7);

    if((pin == P_26) || (pin == P_27)){
        port = PIN2PORT(pin);
        num = PIN2NUM(pin);

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
    gpioMode(pin, OUTPUT);
}

/******************************************************************************/
