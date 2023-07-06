/***************************************************************************//**
  @file     main.c
  @brief    Main file
  @author   CCB
 ******************************************************************************/

/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/

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
#include "DRV_RTI.h"
#include "DRV_DIP_Switch.h"
#include "DRV_Lock.h"

#include "Test.h"

/*******************************************************************************
 * CONSTANT AND MACRO DEFINITIONS USING #DEFINE
 ******************************************************************************/

// Current Control
#define CURRENT_THRESHOLD_R         40
#define CURRENT_THRESHOLD_L         15

/*******************************************************************************
 * ENUMERATIONS AND STRUCTURES AND TYPEDEFS
 ******************************************************************************/

/*******************************************************************************
 * VARIABLES WITH GLOBAL SCOPE
 ******************************************************************************/

// Variables
int JAMMED = FALSE;
int ANSWER = FALSE;
uint8_t WINDOW_STATUS = DETENIDA;

// CLOCK
uint16_t CLOCK_L = 0;

// CAN
char RECEIVE[8]  = {0};
char RECEIVE_CHECK[8]  = {0};
uint16_t RX_ID;
uint16_t CAN_ID;
int CAN_RX_RECEIVED; //todo agregado de la variable global porque no puedo mandar ni recibir por CAN adentro de una interrupción
int CAN_TX_REQUESTED;

// Lock
int LOCKED = FALSE;
int LOCK_CHECK = FALSE;

/*******************************************************************************
 * FUNCTION PROTOTYPES FOR PRIVATE FUNCTIONS WITH FILE LEVEL SCOPE
 ******************************************************************************/

// All Initializations
void Init(void);

// App related Initializations
void App_Init(void);

// App continuous cycle
void App_Run(void);

// Tasks that the main program does
void App_TaskSequence(void);

/*******************************************************************************
 * STATIC VARIABLES AND CONST VARIABLES WITH FILE LEVEL SCOPE
 ******************************************************************************/

/*******************************************************************************
 *******************************************************************************
                        GLOBAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/

void main (void)
{
     Init();                // Initializations

    for(;;)
    {
        App_Run();          // App Sequence
    }
}

void Init(void)
{
    System_Init_First();    //WD and CLK Initializations
//    Board_Init();           //MSP430 Led and Switches Initializations
    System_Init_Last();     //Enable Interrupts
    App_Init();             //App related Initializations

    Medicion_Tiempos_Init();
}

void App_Run(void)
{
//    gpioWrite(PORTNUM2PIN(2,3),HIGH);
    // APP Task List
    App_TaskSequence();

    // Test
//    Test();
//    gpioWrite(PORTNUM2PIN(2,3),LOW);
}


/*******************************************************************************
 *******************************************************************************
                        LOCAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/


void App_Init(void)
{
    // RTI
    RTI_Init();                             // Initializes RTI

    //Door
    Door_Init();                            // Initializes RTI-Callback for Periodic Updates & ISR_Callback for CAN reading
    Door_OC_InputPin(PIN_OC_SW);            // Initializes Open/Close Door Button for Simulation
    Window_Stop();                          // Double checks that the window is stopped on startup

    // Clock
    Clock_Init();                           // Initializes RTI-Callback

    // PWM
    PWM_Init();
    PWM_Config_OutputPin(PIN_PWM);          // PWM Signal
    PWM_Config_EnablePin(PIN_PWM_DIR);      // Enable PWM Signals
    PWM_Config_EnablePin(PIN_PWM_ENABLE);   // Enable PWM Signals

    // ADC
    ADC_Init();
    ADC_Config_InputPin(PIN_ADC_CURR);      // 1st Position in Vector
    ADC_Config_InputPin(PIN_ADC_VOLT);      // 2nd Position in Vector

    // DIP-Switch
    DIP_Switch_Init(PIN_DPS_1,PIN_DPS_2);   // CAN_ID selection

    // CAN
    CAN_Init();                             // Initializes CAN

    // Lock
    Lock_Init(PIN_LOCK_SW,PIN_LOCK_LED);   // Initializes Door-Lock

    // Variable Initiation
    JAMMED = TRUE;
    ANSWER = TRUE;
    LOCKED = FALSE;
    LOCK_CHECK = FALSE;
}


/******************************************************************************/


void App_TaskSequence(void)
{
    // Variables
    uint8_t Current;

    //================High Frequency Tasks===============
    if(CAN_RX_RECEIVED == RECEIVED)     // Checking if there are Unread Messages
    {
        Receive_Check_Update();
        CAN_RX_RECEIVED = CLEAR;
    }
    else
    {
        CAN_CLEAR_FLAG();
    }

    if(CAN_TX_REQUESTED == REQUEST)     // Checking if it is time for a Periodic Update
    {
        Update_and_Transmit();
        CAN_TX_REQUESTED = CLEAR;
    }

    Current = ADC_Capture_Current();    // Maintaining Current under Threshold
    switch(WINDOW_STATUS){
    case SUBIENDO:
        if(Current >= CURRENT_THRESHOLD_R)
        {
            Window_Stop();
            WINDOW_STATUS = DETENIDA;
            JAMMED = TRUE;

            Update_and_Transmit();
        }
        break;
    case BAJANDO:
        if(Current >= CURRENT_THRESHOLD_L)
        {
            Window_Stop();
            WINDOW_STATUS = DETENIDA;
            JAMMED = TRUE;

            Update_and_Transmit();
        }
        break;
    }

    //==============Medium Frequency Tasks===============


    //================Low Frequency Tasks================
    if(CLOCK_L >= 1)            // Clock_L Related Tasks
    {
        CLOCK_L = 0;            //Clock Reseting

        if(JAMMED == FALSE)     // Periodic Window Status
        {
            switch(RECEIVE[1])  // Following the Instructions
            {
            case DETENER:
                Window_Stop();
                WINDOW_STATUS = DETENIDA;
                break;
            case SUBIR:
                Window_Rise();
                WINDOW_STATUS = SUBIENDO;
                break;
            case BAJAR:
                Window_Lower();
                WINDOW_STATUS = BAJANDO;
                break;
            default:
                break;
            }

            if(ANSWER == FALSE) // If there was a new Instruction, answer with New Status
            {
                Update_and_Transmit();
                ANSWER = TRUE;
            }
        }

        Lock_LED_Update();      // Lock System LED Signal
    }


    if(LOCK_CHECK==TRUE)        // Lock updating related Tasks
    {
        Lock_SW_Check();        // Lock System Check
    }
}

