/***************************************************************************//**
 @file
 @brief
 @author   MGV
 ******************************************************************************/

/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/
#include "TEST.h"
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
void delay_ms(unsigned int ms );
/*******************************************************************************
 * STATIC VARIABLES AND CONST VARIABLES WITH FILE LEVEL SCOPE
 ******************************************************************************/

/*******************************************************************************
 *******************************************************************************
 GLOBAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/
void test_SPI(void){
    data = transferSPI('a');
    data_A = transferSPI('u');
    data_B = transferSPI('d');

    data = transferSPI('z');
    data_A = transferSPI('g');
    data_B = transferSPI('t');
}

// ----------------------------------------------------------------------------------------------------------

void test_SPI_A(void){
    data = transferSPI_A('a');
    data_A = transferSPI_A('u');
    data_B = transferSPI_A('d');

    data = transferSPI_A('z');
    data_A = transferSPI_A('g');
    data_B = transferSPI_A('t');
}

// ----------------------------------------------------------------------------------------------------------

void test_CAN(void){
    extern char RECEIVE[8];
    extern char TRANSMIT[8];
    TRANSMIT[0] = 0x03;
    TRANSMIT[1] = 0x0F;
    CAN_TX_BUFFER(TRANSMIT,2,CAN_ID);
//    CAN_TX_BUFFER(TRANSMIT,2,0x503);
//    CAN_TX_BUFFER(TRANSMIT,2,0x500);
    CAN_RECEIVE(2,&(RECEIVE[0]));
}

// ----------------------------------------------------------------------------------------------------------

void test_Botonera_SPI_A(void){
    gpioWrite(B_SH,LOW); //Recibo
    gpioWrite(B_SH,HIGH); //Recibo
    receive_Shift = transferSPI_A(0xFF);
}

// ----------------------------------------------------------------------------------------------------------

void test_LEDs(void){
    gpioWrite(P_A, LOW);
    gpioWrite(P_B, LOW);
    gpioWrite(P_G,LOW); // Al poner en HIGH el enble tengo todos HIGH pero con el NOT todos LOW
    delay_ms(1000);
    gpioWrite(P_A, LOW);
    gpioWrite(P_B, HIGH);
    delay_ms(1000);
    gpioWrite(P_A, HIGH);
    gpioWrite(P_B, LOW);
    delay_ms(1000);
    gpioWrite(P_A, HIGH);
    gpioWrite(P_B, HIGH);
    delay_ms(1000);
    gpioWrite(P_G,HIGH);
    delay_ms(1000);
}

// ----------------------------------------------------------------------------------------------------------

void test_LEDs_Control(void){
    extern char PUERTAS;

    // Ninguna Puerta
    PUERTAS = 0x00;
    delay_ms(1000);

    // Puerta 1 Delantera Izq
    PUERTAS = 0x01;
    delay_ms(1000);
    //Puerta 2 Delantera Der
    PUERTAS = 0x02;
    delay_ms(1000);
    //Puerta 3 Trasera Izq
    PUERTAS = 0x04;
    delay_ms(1000);
    //Puerta 4 Trasera Der
    PUERTAS = 0x08;
    delay_ms(1000);

    //Todas
    PUERTAS = 0x0F;
    delay_ms(1000);

    //Combinaciones
    //Delanteras
    PUERTAS = 0x03;
    delay_ms(1000);
    //Traseras
    PUERTAS = 0x0C;
    delay_ms(1000);
    //Izquierdas
    PUERTAS = 0x05;
    delay_ms(1000);
    //Derechas
    PUERTAS = 0x0A;
    delay_ms(1000);

    //Diagonales
    PUERTAS = 0x09;
    delay_ms(1000);
    PUERTAS = 0x06;
    delay_ms(1000);

    //Ciclo
    PUERTAS = 0x0E;
    delay_ms(1000);
    PUERTAS = 0x0D;
    delay_ms(1000);
    PUERTAS = 0x07;
    delay_ms(1000);
    PUERTAS = 0x0B;
    delay_ms(1000);
}

// ----------------------------------------------------------------------------------------------------------

void test_LEDs_RTI(void){
    extern char PUERTAS;
    PUERTAS = 0x03;
}

// ----------------------------------------------------------------------------------------------------------
void test_RTI(void){

}
/*******************************************************************************
 SAMPLES
 ******************************************************************************/
void sample_SPI(void){
//    prueba = CAN_READ(TXB0CTRL); //HAY QUE AGREGARLO AL DRV_CAN.h
}

// ----------------------------------------------------------------------------------------------------------
void sample_CAN(void){
    extern char RECEIVE[8];
    extern char TRANSMIT[8];
    TRANSMIT[0] = 0x01;
    TRANSMIT[1] = 0x0F;
    CAN_TX_BUFFER(TRANSMIT,2,CAN_ID);
}

// ----------------------------------------------------------------------------------------------------------

void sample_CAN_Mask(uint16_t ID){
    extern char RECEIVE[8];
    extern char TRANSMIT[8];
    TRANSMIT[0] = 0x01;
    TRANSMIT[1] = 0x0F;
    CAN_TX_BUFFER(TRANSMIT,2,ID);
}

/*******************************************************************************
 *******************************************************************************
 LOCAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/

/******************************************************************************/

//DELAY
void delay_ms(unsigned int ms )
{
unsigned int i;
for (i = 0; i<= ms; i++)
__delay_cycles(1000); //Built-in function that suspends the execution for 1000 cycles
}
