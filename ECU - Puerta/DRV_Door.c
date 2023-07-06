/***************************************************************************//**
 @file
 @brief
 @author   CCB
 ******************************************************************************/

/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/

#include "DRV_Door.h"

#include "gpio.h"
#include "board.h"

#include "APP_CAN_Format.h"
#include "APP_Data.h"

#include "DRV_TimerA.h"
#include "DRV_ADC.h"
#include "DRV_PWM.h"
#include "DRV_CAN.h"
#include "DRV_SPI.h"
#include "DRV_RTI.h"
#include "DRV_ISR.h"

/*******************************************************************************
 * CONSTANT AND MACRO DEFINITIONS USING #DEFINE
 ******************************************************************************/

/*******************************************************************************
 * ENUMERATIONS AND STRUCTURES AND TYPEDEFS
 ******************************************************************************/

/*******************************************************************************
 * VARIABLES WITH GLOBAL SCOPE
 ******************************************************************************/
char TRANSMIT[8] = {0};

/*******************************************************************************
 * FUNCTION PROTOTYPES FOR PRIVATE FUNCTIONS WITH FILE LEVEL SCOPE
 ******************************************************************************/

// Gets the measurement corresponding to the voltage pin
unsigned int ADC_Capture_Voltage(void);

// Callback that tells that a periodic update to the main ECU is needed
void UaT_RTI_Callback(void);

// Callback that tells a new CAN message has been received
void UaT_ISR_Callback(void);

/*******************************************************************************
 * STATIC VARIABLES AND CONST VARIABLES WITH FILE LEVEL SCOPE
 ******************************************************************************/

/*******************************************************************************
 *******************************************************************************
 GLOBAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/

void Door_Init(void)
{
    // Periodic Updates
    RTI_Callback_Config(UaT_RTI_Callback,UaT_RTI_PERIOD);//todo REVISAR

    // New Requests
    ISR_Callback_Config(UaT_ISR_Callback,PIN_CAN_ISR,FALLING);
}


void Door_OC_InputPin(uint8_t pin)
{
    gpioWrite(PIN_OC_SW,LOW);
    gpioMode(PIN_OC_SW,INPUT);
}


void PWM_Config_EnablePin(uint8_t pin)
{
    uint8_t port = PIN2PORT(pin);
    uint8_t num = PIN2NUM(pin);

    switch(port)
    {
    case 1:
        P1DIR |= (port<<num);
        break;
    case 2:
        P2DIR |= (port<<num);
        break;
    }
}

unsigned int ADC_Capture_Current(void)
{
    static unsigned int ADC_CurrValue;
    static unsigned int ADC_MeanValue;

    ADC_CurrValue = ADC_Read(PIN_ADC_CURR);
    ADC_MeanValue = ADC_Smooth(ADC_CurrValue,ADC_MeanValue,8);  // Do not use n<2

    return ADC_MeanValue;
}


void Window_Lower(void)
{
    int Percent;
    static unsigned int Voltage;

    if(ADC_Capture_Voltage()-Voltage >= 2 || ADC_Capture_Voltage()-Voltage <= 2)
    {
        Voltage = ADC_Capture_Voltage();
    }

    // 14V es 956 en el ADC, 9V es 617 en el ADC; 1V es aprox 72 en el ADC
    // si digo que a 9V uso 56% y a 14V uso 36%; mantengo 5,04V constantes en el motor
    Percent = (34552)/ADC_Capture_Voltage();    // 56*617 = 34552 //TODO NO SE CONDICE CON EL COMENTARIO DE ARRIBA. REVISAR!!

    //todo NICO SUGIERE SACAR LA RESISTENCIA DE 100 OHM PARA ALEJAR EL PUNTO DE 3.3v CUANDO TIENE 14v
    //SI HACEMOS ESTO HAY QUE REVISAR LOS ADC NUEVAMENTE

//    Percent = 4276 - (68*ADC_Capture_Voltage())/10;   //Formula Nueva: casi quema el motor

    if(Percent<=0)
    {
        Percent = 0;
    }
    else if(Percent>=100)
    {
        Percent = 100;
    }

    gpioWrite(PIN_PWM_DIR,HIGH);
    PWM_Value(100-Percent);
    gpioWrite(PIN_PWM_ENABLE,HIGH);
}


void Window_Rise(void)
{
    int Percent;

    static unsigned int Voltage;

    if(ADC_Capture_Voltage()-Voltage >= 2 || ADC_Capture_Voltage()-Voltage <= 2)
    {
        Voltage = ADC_Capture_Voltage();
    }

    // 14V es 956 en el ADC, 9V es 612 en el ADC; 1V es aprox 72 en el ADC
    // si digo que a 9V uso 56% y a 14V uso 36%; mantengo 5,04V constantes en el motor
    Percent = (34552)/ADC_Capture_Voltage();    // 56*617 = 34552 //TODO NO SE CONDICE CON EL COMENTARIO DE ARRIBA. REVISAR!!

//    Percent = 4276 - (68*ADC_Capture_Voltage())/10;   //Formula Nueva: casi quema el motor

    if(Percent<=0)
    {
        Percent = 0;
    }
    else if(Percent>=100)
    {
        Percent = 100;
    }
    gpioWrite(PIN_PWM_DIR,LOW);
    PWM_Value(Percent);
    gpioWrite(PIN_PWM_ENABLE,HIGH);
}


void Window_Stop(void)
{
    PWM_Value(0);
    gpioWrite(PIN_PWM_DIR,LOW);
    gpioWrite(PIN_PWM_ENABLE,LOW);
}


uint8_t Door_State(void)
{
    uint8_t Door_Status;
    uint8_t Door_Reading;

    Door_Reading = gpioRead(PIN_OC_SW);
    if(Door_Reading == 1)
    {
        Door_Status = PUERTA_ABIERTA;
    }
    else
    {
        Door_Status = PUERTA_CERRADA;
    }

    return Door_Status;
}


void Update_and_Transmit(void)
{
    // Variables
    extern int LOCKED;
    extern char TRANSMIT[];
    extern uint16_t CAN_ID;
    extern uint8_t WINDOW_STATUS;
    static uint8_t Door_Status;
    static uint16_t Voltage;
    static unsigned int Voltage_Conv;
    volatile uint8_t Door_Reading;

    // Updating Current Status
    Voltage = ADC_Capture_Voltage();            // Checking battery Voltage
    Door_Reading = Door_State();                 // Checking if the door is open or closed

    // Converting Voltage from ADC Value to Volts
    Voltage_Conv = (Voltage*64)/460;

    // Checking if Door is Locked
    if(LOCKED==TRUE)
    {
        if(Door_Status==PUERTA_ABIERTA)
        {
            Door_Status = Door_Reading;
        }
    }
    else if(LOCKED==FALSE)
    {
        Door_Status = Door_Reading;
    }

    // Informing Current Status
    TRANSMIT[0] = WINDOW_STATUS;
    TRANSMIT[1] = Door_Status;
    TRANSMIT[2] = Voltage_Conv;

    CAN_TX_BUFFER(TRANSMIT,TRANSMIT_BYTES,CAN_ID);
}


void Receive_Check_Update(void)
{
//    extern char * RECEIVE;
//    extern char * RECEIVE_CHECK;
    extern char RECEIVE[8];
    extern char RECEIVE_CHECK[8];
    extern uint16_t CAN_ID;
    extern int JAMMED;
    extern int ANSWER;
    volatile char data;
    volatile unsigned int i;

    CAN_RECEIVE(RECEIVE_BYTES,&(RECEIVE_CHECK[0]));
//    CAN_BITMODIFY(CANINTF,CANINTF_MASK,CANINTF_CLEAR);

    if(RECEIVE_CHECK[0]== (CAN_ID & 0xFF)) //El envío es 0x01 no 0x501, me quedo con la última parte del uint16_t
    {

//        for(i=0;i<RECEIVE_BYTES;i++)
//        {
//            data = RECEIVE_CHECK[i];
//            RECEIVE[i] = data;
//        }//Era un poco lento

        RECEIVE[0] = RECEIVE_CHECK[0];
        RECEIVE[1] = RECEIVE_CHECK[1];
        JAMMED = FALSE;
        ANSWER = FALSE;
    }
}


/*******************************************************************************
 *******************************************************************************
 LOCAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/

unsigned int ADC_Capture_Voltage(void)
{
    static unsigned int ADC_Value;

    ADC_Value = ADC_Read(PIN_ADC_VOLT);

    return ADC_Value;
}

/******************************************************************************/

void UaT_RTI_Callback(void)
{
    extern int CAN_TX_REQUESTED;
    CAN_TX_REQUESTED = REQUEST;
}

void UaT_ISR_Callback(void)
{
    extern int CAN_RX_RECEIVED;
    CAN_RX_RECEIVED = RECEIVED;
}


