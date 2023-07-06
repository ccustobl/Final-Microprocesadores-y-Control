/***************************************************************************//**
  @file     main.c
  @brief    Main file
  @author   MGV
 ******************************************************************************/

/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/
#include <msp430.h> 
#include <gpio.h>
#include <mcp25625.h>
#include <system.h>

#include <DRV_SPI.h>
#include <DRV_Botonera.h>
#include <DRV_CAN.h>
#include <DRV_LEDs.h>

#include <APP_CAN_Format.h>

#include "TEST.h"
#include <stdint.h>

/*******************************************************************************
 * CONSTANT AND MACRO DEFINITIONS USING #DEFINE
 ******************************************************************************/

/*******************************************************************************
 * ENUMERATIONS AND STRUCTURES AND TYPEDEFS
 ******************************************************************************/

/*******************************************************************************
 * VARIABLES WITH GLOBAL SCOPE
 ******************************************************************************/

//CAN
char RECEIVE[8];
char RECEIVE_CHECK[8];
char TRANSMIT[8];
uint16_t RX_ID;

// CLOCK
uint16_t CLOCK_L = 0;
uint16_t CLOCK_P = 0;
uint16_t CLOCK_BOTON[4];

// Puertas
char ESTADO[4];
//char PUERTAS;
int PUERTAS[4];
float TENSION;
int BLOCK_PUERTA[4];

//Botonera
char BOTONES;
int BLOCK_BOTON[4];
int CONTAR_BOTON[4];
int AUTO_BOTON[4];

//Ventana
int CONDICION_VENTANA[4] = {CERRADA};
uint16_t POSICION[4] = {0};
int EMBOLSADO[4] = {0};

uint16_t ECU_PUERTAS[4] = {ECU_PUERTA_DELANTERA_IZQUIERDA,ECU_PUERTA_DELANTERA_DERECHA,ECU_PUERTA_TRASERA_IZQUIERDA,ECU_PUERTA_TRASERA_DERECHA};
/*******************************************************************************
 * FUNCTION PROTOTYPES FOR PRIVATE FUNCTIONS WITH FILE LEVEL SCOPE
 ******************************************************************************/
/**
 * @brief Inicializa las funciones necesarias para correr la APP
 * @param void
 * @return void
 */
void Init(void);

/**
 * @brief Determina las funciones a correr continuamente en la APP
 * @param void
 * @return void
 */
void App_Run(void);

/**
 * @brief Inicialización de las funciones de software necesarias para correr la APP
 * @param void
 * @return void
 */
void App_Init(void);

/**
 * @brief Engloba las tareas a correr en la APP
 * @param void
 * @return void
 */
void App_TaskSequence(void);

/**
 * @brief Realiza la lectura periódica del CAN y corroborá si es un mensaje nuevo. De ser así reescribe la variable Global RECEIVE
 * @param void
 * @return void
 */
void App_Receive_Check(void);

/**
 * @brief Realiza el procesamiento del mensaje recibido. Es decir, determina de que puerta se envío, y modifica las variables Estado Ventanilla,
 * Estado Puerta y Tensión
 * @param void
 * @return void
 */
void App_Receive_Processing(void);

/**
 * @brief Trasnmite por CAN la puerta de destino y la acción a realizar: SUBIR, BAJAR o DETENER
 * @param uint16_t puerta_destino,uint8_t accion
 * @return void
 */
void App_Transmit_Processing(uint16_t puerta_destino,uint8_t accion);

/**
 * @brief Realiza la lectura de la botonera mediante SPI y contiene la lógica a aplicar a dicha lectura
 * @param void
 * @return void
 */
void App_Botonera_Processing(void);

/**
 * @brief Posee la lógica para aplicar el embolsado de la ventana al abrir la puerta
 * @param void
 * @return void
 */
void App_Embolsado(void);

/**
 * @brief Resetea los parametros de la botonera asociada a la puerta, Estos son: AUTO, BLOCK, CONTAR y CLOCK de cada boton
 * @param unsigned int i, siendo i la puerta a resetear
 * @return void
 */
void Reset_Boton(unsigned int i);

/**
 * @brief En cada FLAG de RTI realiza el conteo del tiempo de presión de un botón para activar la función AUTO
 * @param void
 * @return void
 */
void Contador_Auto(void);

/**
 * @brief En cada FLAG de RTI realiza el conteo de la posición de la ventana según el Estado de la Ventana
 * @param void
 * @return void
 */
void Contador_Posicion(void);

//Test

/**
 * @brief Función de Testeo que llama a las funciones guardadas en TEST.c y TEST.h
 * @param void
 * @return void
 */
void Test(void);

/*******************************************************************************
 * STATIC VARIABLES AND CONST VARIABLES WITH FILE LEVEL SCOPE
 ******************************************************************************/

/*******************************************************************************
 *******************************************************************************
                        GLOBAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/
int main(void)
{
    Init();

    for(;;){
        App_Run();
    }
}

void Init(void)
{
    System_Init_First();    //WD and CLK Inits
//    Board_Init();           //MSP430 Led and Switches Inits
    System_Init_Last();     //Enable Interrupts
    App_Init();             //App related Inits
}


void App_Run(void)
{
    // APP Task Sequence
    App_TaskSequence();

    // Test
//    Test();
}

/*******************************************************************************
 *******************************************************************************
                        LOCAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/

void App_Init(void){
    //Inicialización del SPI
    SPI_Init_A();

    //Inicialización de la botonera
    botonera_Init();

    //Inicialización del CAN
    CAN_Init();

    //Inicialización de los pines para controlar los LEDs
    DRV_LEDs_Init();

    RTI_Init(DRV_LEDs_Control,TIME_STEP); //FunCallback!!
    RTI_Init(Clock_Callback,TIME_STEP);
}

//================TESTS================

void Test(void)
{
    //    test_CAN();
    //    test_LEDs();
    //    test_LEDs_Control();
    //    test_LEDs_RTI();
    //    test_Botonera();
    //    App_Botonera_Processing();
    //    test_SPI();
    //    test_SPI_A();
    //    test_Botonera_SPI_A();

    //    sample_SPI();
    //    sample_CAN(); //0x500 SUBIR PUERTA1
        sample_CAN_Mask(0x501); //ID SUBIR PUERTA1
}

// ----------------------------------------------------------------------------------------------------------
void App_TaskSequence(void){

    // Verifico si se apreto algún botón
    App_Botonera_Processing();
    //Verifico lectura
    App_Receive_Check();

    App_Embolsado();

    // Verifico si se apreto algún botón
    if(CLOCK_L > 0){
        CLOCK_L = 0;
    }

    if(CLOCK_P > 0){
        Contador_Auto();
        Contador_Posicion();
        CLOCK_P = 0;
    }
}

// ----------------------------------------------------------------------------------------------------------
void App_Receive_Check(void){
    CAN_RECEIVE(3,&(RECEIVE_CHECK[0]));

    if(RECEIVE[0] == RECEIVE_CHECK[0]){
        if(RECEIVE[1] != RECEIVE_CHECK[1]){
            RECEIVE[1] = RECEIVE_CHECK[1];
            App_Receive_Processing();
        }
        else if(RECEIVE[2] != RECEIVE_CHECK[2]){
            RECEIVE[2] = RECEIVE_CHECK[2];
            App_Receive_Processing();
        }
    }
    else{
        RECEIVE[0] = RECEIVE_CHECK[0];
        RECEIVE[1] = RECEIVE_CHECK[1];
        RECEIVE[2] = RECEIVE_CHECK[2];
        App_Receive_Processing(); //Proceso los mensajes recibidos;
    }
}

// ----------------------------------------------------------------------------------------------------------
void App_Receive_Processing(void){
    volatile float Volt_Recieve;
    unsigned int ID_NUM;
    extern int PUERTA_SHIFT[4];
    extern char PUERTA_MASK[4];

    //Mensajes válidos
    if (RX_ID < ECU_PUERTA_DELANTERA_IZQUIERDA || RX_ID > ECU_PUERTA_TRASERA_DERECHA)
        return;
    //De los mensajes recibidos chequeo de quien fue
    ID_NUM = (RX_ID & 0x0F); //Casteo a int el final del 0x50n

    // 1°Byte es el estado de la ventana
    switch(RECEIVE[ESTADO_VENTANILLA]){
    case DETENIDA:
        ESTADO[ID_NUM-1]  = ESTADO_DETENIDO;
        break;
    case SUBIENDO:
        ESTADO[ID_NUM-1]  = ESTADO_SUBIENDO;
        break;
    case BAJANDO:
        ESTADO[ID_NUM-1]  = ESTADO_BAJANDO;
        break;
    }

    //2°Byte es el estado de la puerta
    switch(RECEIVE[ESTADO_PUERTA]){
    case PUERTA_CERRADA:
        PUERTAS[ID_NUM-1] = CERRADA;
        break;
    case PUERTA_ABIERTA:
        PUERTAS[ID_NUM-1] = ABIERTA;
        break;
    }

    // Todos modifican la tensión por lo que no importa de quien fue
    Volt_Recieve = RECEIVE[ESTADO_TENSION];
    TENSION = Volt_Recieve/10;
}
// ----------------------------------------------------------------------------------------------------------

void App_Transmit_Processing(uint16_t puerta_destino,uint8_t accion){
    TRANSMIT[DESTINATARIO] = puerta_destino;
    TRANSMIT[ACCION] = accion;
    CAN_TX_BUFFER(TRANSMIT,TRANSMIT_BYTES,CAN_ID);
}

// ----------------------------------------------------------------------------------------------------------

void App_Botonera_Processing(void){
    volatile unsigned int i;
    //Variables Externas
    extern char PUERTA_MASK[4];
    extern char BOTON_SUBIR[4];
    extern char BOTON_BAJAR[4];
    extern int PUERTA_SHIFT[4];
    extern uint16_t ECU_PUERTAS[4];

    //Variables Locales
    char Boton_Puerta;

    //Lectura de los Botones
    BOTONES = botoneraRead_SPI_A();

    for(i=1;i<=4;i++){
        Boton_Puerta = ((BOTONES & PUERTA_MASK[i-1]) >> PUERTA_SHIFT[i-1]);

        switch(ESTADO[i-1]){
        case ESTADO_DETENIDO:

            //================BOTON SUBIR================
            if((Boton_Puerta == BOTON_SUBIR[i-1]) & (BLOCK_BOTON[i-1] == FREE)){
                Reset_Boton(i);
                App_Transmit_Processing(ECU_PUERTAS[i-1],SUBIR);

                //Cancelo EMBOLSADO si toco un botón
                EMBOLSADO[i-1] = 0;
             }

            //================BOTON BAJAR================
             else if((Boton_Puerta == BOTON_BAJAR[i-1]) & (BLOCK_BOTON[i-1] == FREE)){
                 Reset_Boton(i);
                 App_Transmit_Processing(ECU_PUERTAS[i-1],BAJAR);

                 //Cancelo EMBOLSADO si toco un botón
                 EMBOLSADO[i-1] = 0;
             }

            //================Ningun BOTON================
             else if(Boton_Puerta == 0x00){
                 Reset_Boton(i);
             }

            break;
        case ESTADO_SUBIENDO:

            //================BOTON SUBIR================
            if(Boton_Puerta == BOTON_SUBIR[i-1]){
                //Cancelo EMBOLSADO si toco un botón
                EMBOLSADO[i-1] = 0;

                if(BLOCK_BOTON[i-1] == FREE){
                    BLOCK_BOTON[i-1] = BLOCK;
                    if(AUTO_BOTON[i-1] == ON){
                         App_Transmit_Processing(ECU_PUERTAS[i-1],DETENER);
                     }
                }

                // Lógica AUTO
                if(CONTAR_BOTON[i-1] == 0){
                    CONTAR_BOTON[i-1] = 1;
                }
                else{
                    if(CLOCK_BOTON[i-1] >= AUTO_TRIGGER){
                        AUTO_BOTON[i-1] = ON;
                        CONTAR_BOTON[i-1] = 0;
                    }
                }
            }

            //================BOTON BAJAR================
            else if(Boton_Puerta == BOTON_BAJAR[i-1]){
                //Cancelo EMBOLSADO si toco un botón
                EMBOLSADO[i-1] = 0;

                if(AUTO_BOTON[i-1] == OFF){
                    BLOCK_BOTON[i-1] = FREE;
                    App_Transmit_Processing(ECU_PUERTAS[i-1],BAJAR);
                }
                else{
                    BLOCK_BOTON[i-1] = BLOCK;
                    App_Transmit_Processing(ECU_PUERTAS[i-1],DETENER);
                 }
             }

            //================Ningun BOTON o AMBOS================
            else{
                if((AUTO_BOTON[i-1] == OFF) && (EMBOLSADO[i-1] == 0)){
                    App_Transmit_Processing(ECU_PUERTAS[i-1],DETENER);
                }
                //Ningun
                if(Boton_Puerta == 0x00){
                    BLOCK_BOTON[i-1] = FREE;
                }
            }
            break;
        case ESTADO_BAJANDO:

            //================BOTON SUBIR================
            if(Boton_Puerta == BOTON_SUBIR[i-1]){
                //Cancelo EMBOLSADO si toco un botón
                EMBOLSADO[i-1] = 0;

                if(AUTO_BOTON[i-1] == OFF){
                    BLOCK_BOTON[i-1] = FREE;
                    App_Transmit_Processing(ECU_PUERTAS[i-1],SUBIR);
                }
                else{
                    BLOCK_BOTON[i-1] = BLOCK;
                    App_Transmit_Processing(ECU_PUERTAS[i-1],DETENER);
                }
             }

            //================BOTON BAJAR================
             else if(Boton_Puerta == BOTON_BAJAR[i-1]){
                 //Cancelo EMBOLSADO si toco un botón
                 EMBOLSADO[i-1] = 0;

                 if(BLOCK_BOTON[i-1] == FREE){
                    BLOCK_BOTON[i-1] = BLOCK;
                    if(AUTO_BOTON[i-1] == ON){
                        App_Transmit_Processing(ECU_PUERTAS[i-1],DETENER);
                    }
                 }

                 if(CONTAR_BOTON[i-1] == 0){
                     CONTAR_BOTON[i-1] = 1;
                 }
                 else{
                     if(CLOCK_BOTON[i-1] >= AUTO_TRIGGER){
                         AUTO_BOTON[i-1] = ON;
                         CONTAR_BOTON[i-1] = 0;
                     }
                 }
             }

            //================Ningun BOTON o AMBOS================
             else{
                 if((AUTO_BOTON[i-1] == OFF) && (EMBOLSADO[i-1] == 0)){
                     App_Transmit_Processing(ECU_PUERTAS[i-1],DETENER);
                 }
                 //Ningun
                 if(Boton_Puerta == 0x00){
                     BLOCK_BOTON[i-1] = FREE;
                 }
             }
            break;
        }
    }
}

// ----------------------------------------------------------------------------------------------------------
void App_Embolsado(void){
    unsigned int i;

    for(i=1;i<=4;i++){
        switch (CONDICION_VENTANA[i-1]){
        case CERRADA:

            if((PUERTAS[i-1] == ABIERTA) && (BLOCK_PUERTA[i-1] == 0)){
                EMBOLSADO[i-1] = 1;
                BLOCK_PUERTA[i-1] = 1;
                App_Transmit_Processing(ECU_PUERTAS[i-1],BAJAR);
            }

            if((PUERTAS[i-1] == CERRADA) && (BLOCK_PUERTA[i-1] == 1)){
                BLOCK_PUERTA[i-1] = 0;
            }
            break;

        case ABIERTA:

            //Si está abierta pero se apago el embolsado salteo esta lógica
            if(EMBOLSADO[i-1] == 0){
                break;
            }
            switch (PUERTAS[i-1]){
            case ABIERTA:
                if(POSICION[i-1] >= POS_EMBOLSADO){
                    App_Transmit_Processing(ECU_PUERTAS[i-1],DETENER);
                }
                else if((POSICION[i-1] < POS_EMBOLSADO) && (ESTADO[i-1] == ESTADO_BAJANDO)){
                    App_Transmit_Processing(ECU_PUERTAS[i-1],BAJAR);
                }
                break;
            case CERRADA:
                if(POSICION[i-1] > POS_TOLERANCE){
                    App_Transmit_Processing(ECU_PUERTAS[i-1],SUBIR);
                }
            }
            break;
        }
    }
}

// ----------------------------------------------------------------------------------------------------------
void Reset_Boton(unsigned int i){
    AUTO_BOTON[i-1] = OFF;
    BLOCK_BOTON[i-1] = FREE;
    CONTAR_BOTON[i-1] = 0;
    CLOCK_BOTON[i-1] = 0;
}

// ----------------------------------------------------------------------------------------------------------
void Contador_Auto(void){
    volatile unsigned int i;

    for(i=1;i<=4;i++){
        if(CONTAR_BOTON[i-1] == 1){
            CLOCK_BOTON[i-1] += CLOCK_P;
        }
    }
}

// ----------------------------------------------------------------------------------------------------------
void Contador_Posicion(void){
     unsigned int i;
     int16_t dummy;

     for(i=1;i<=4;i++){
         switch (ESTADO[i-1]){

         case ESTADO_DETENIDO:
             if((POSICION[i-1] <= POS_TOLERANCE)){
                 CONDICION_VENTANA[i-1] = CERRADA;
                 POSICION[i-1] = 0;
             }

             break;

         case ESTADO_SUBIENDO:
             dummy = POSICION[i-1] - CLOCK_P;
             if(dummy < 0){
                 POSICION[i-1] = 0;
             }
             else{
                 POSICION[i-1] -= CLOCK_P;
             }

             break;

         case ESTADO_BAJANDO:
             POSICION[i-1] += CLOCK_P;

             if(CONDICION_VENTANA[i-1] == CERRADA){
                 CONDICION_VENTANA[i-1] = ABIERTA;
             }
             break;
         }
     }
 }
