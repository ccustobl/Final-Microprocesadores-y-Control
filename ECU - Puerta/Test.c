/***************************************************************************//**
 @file
 @brief
 @author   CCB
 ******************************************************************************/

/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/

#include "Test.h"

#include "common.h"
#include "system.h"
#include "board.h"

#include "APP_CAN_Format.h"
#include "APP_Data.h"

#include "DRV_Door.h"
#include "DRV_PWM.h"
#include "DRV_Clock.h"
#include "DRV_ADC.h"
#include "DRV_CAN.h"
#include "DRV_DIP_Switch.h"
#include "mcp25625.h"

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

// Variable PWM signal test
void test_pwm(void);

// ADC capture test
uint8_t test_adc(void);

// PWM duty-cycle change using adc voltage meassurements test
void test_pwm_y_adc(void);

// Full system integration test
void test_pwm_y_adc_y_CAN(void);

// PWM duty-cycle change using adc current meassurements test
void test_pwm_y_adc_curr(void);

// CAN receive and transmit test
void test_CAN(void);

// Current peak system turn off test
void test_corte(void);

// ADC voltage capture test
void test_voltage_meassure(void);

// DIP Switch test
void test_DIP(void);

/*******************************************************************************
 * STATIC VARIABLES AND CONST VARIABLES WITH FILE LEVEL SCOPE
 ******************************************************************************/

int var1 = 2;
int var2 = 2;

char hola[8]  = {0};
char RECEIVE_test[8]  = {0};

/*******************************************************************************
 *******************************************************************************
 GLOBAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/
void Test(void)
{
    // Testeos Generales
//    test_CAN();
    test_pwm_y_adc_curr();
//    test_pwm();
//    test_adc();
//    test_pwm_y_adc();
//    test_pwm_y_adc_y_CAN();
//    test_corte();
//    test_voltage_meassure();
//    test_DIP();

    // Testeo de APP
//    test_simulacion_recepcion();
}


void Medicion_Tiempos_Init(void)
{
    gpioMode(PORTNUM2PIN(2,3),OUTPUT);
}


/*******************************************************************************
 *******************************************************************************
 LOCAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/

// DELAY
void delay_ms(unsigned int ms )
{
 unsigned int i;
 for (i = 0; i<= ms; i++)
 __delay_cycles(1000); //Built-in function that suspends the execution for 1000 cycles
}

/******************************************************************************/

void test_pwm(void)
{
    PWM_Value(80);
    delay_ms(2000);
    PWM_Value(80);
    delay_ms(2000);
    PWM_Value(60);
    delay_ms(2000);
    PWM_Value(40);
    delay_ms(2000);
}


uint8_t test_adc(void)
{
    static uint8_t ADC_Result;

//    Window_Lower();
    Window_Rise();
    ADC_Result = ADC_Read(PIN_ADC_VOLT);

    return ADC_Result;
}


void test_pwm_y_adc(void)
{
//    delay_ms(10);
    Window_Rise();
//    Window_Lower();
}

void test_pwm_y_adc_y_CAN(void)
{
    extern int CAN_RX_RECEIVED;

//    delay_ms(10);
    Window_Rise();
//    Window_Lower();
    hola[0] = 0x07;
    hola[1] = 0xAA;
//    CAN_TX_BUFFER(hola,2,CAN_ID);
    CAN_TX_BUFFER(hola,2,0x500);
    if(CAN_RX_RECEIVED == RECEIVED){
        CAN_RECEIVE(2,&(RECEIVE_test[0]));
        CAN_BITMODIFY(CANINTF,CANINTF_MASK,CANINTF_CLEAR);
        CAN_RX_RECEIVED = CLEAR;
    }
}

void test_pwm_y_adc_curr(void)
{
//    int ADC_Curr_meassure;
//    int i=0;
//
    PWM_Value(100);
//    delay_ms(500);
//    ADC_Curr_meassure = ADC_Capture_Current();
//    i +=1;
}

void test_CAN(void)
{
    hola[0] = 0x07;

    hola[1] = 0xAA;
//        CAN_TRANSMIT(hola,4,0x01);
//    CAN_TX_BUFFER(hola,2,CAN_ID);
    CAN_TX_BUFFER(hola,2,0x500);
    CAN_RECEIVE(2,&(RECEIVE_test[0]));
}

void test_corte(void)
{
    extern uint8_t WINDOW_STATUS;

    int ADC_Value;
    static int flag_1;
//    static int flag_2;
//    delay_ms(10);
    ADC_Value = ADC_Capture_Current();
    if(ADC_Value > 40)
    {
        flag_1 += 1;
        Window_Stop();
        WINDOW_STATUS = DETENIDA;
//        delay_ms(100);
    }

    if(!(WINDOW_STATUS == DETENIDA))
    {
        Window_Rise();
        gpioWrite(PIN_PWM_ENABLE,HIGH);
    }
//    else
//    {
//        flag_2 += 1;
////        delay_ms(100);
//        Window_Stop();
//        WINDOW_STATUS = DETENIDA;
//    }
}

void test_voltage_meassure(void)
{
//    int Voltage;
//    PWM_Value(100);
//    Voltage = ADC_Capture_Voltage();
}

void test_DIP(void)
{
    extern int var1;
    extern int var2;

    var1 = gpioRead(PIN_DPS_1);
    var2 = gpioRead(PIN_DPS_2);

    Window_Stop();
}
