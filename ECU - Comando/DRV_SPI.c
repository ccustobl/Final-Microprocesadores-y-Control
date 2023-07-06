/***************************************************************************//**
 @file
 @brief
 @author  MGV
 ******************************************************************************/

/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/
#include <msp430.h>
#include <DRV_SPI.h>
#include "board.h"

/*******************************************************************************
 * CONSTANT AND MACRO DEFINITIONS USING #DEFINE
 ******************************************************************************/

#define MAX_BUFFER_SIZE     20

#define INDEX(i,max)             ((i)<0 ? (i)+(max) : (i))

/*******************************************************************************
 * ENUMERATIONS AND STRUCTURES AND TYPEDEFS
 ******************************************************************************/

/*******************************************************************************
 * VARIABLES WITH GLOBAL SCOPE
 ******************************************************************************/

/*******************************************************************************
 * FUNCTION PROTOTYPES FOR PRIVATE FUNCTIONS WITH FILE LEVEL SCOPE
 ******************************************************************************/
//Configuración de los pines de SPI, modificando PxSEL y PxSEL2
void SPI_Config_Pin(uint8_t pin);

//Transferencia de datos por SPI B
void SPI_Transmit(uint8_t *reg_data, uint8_t count);

//Recepción de datos por SPI B
void SPI_Receive(uint8_t * rxdata,uint8_t count);

//Transferencia de datos por SPI A
void SPI_Transmit_A(uint8_t *reg_data, uint8_t count);

//Recepción de datos por SPI A
void SPI_Receive_A(uint8_t * rxdata,uint8_t count);
/*******************************************************************************
 * STATIC VARIABLES AND CONST VARIABLES WITH FILE LEVEL SCOPE
 ******************************************************************************/
uint8_t ReceiveIndex = 0;
uint8_t ReceiveSum = 0;
uint8_t ReceiveBuffer[MAX_BUFFER_SIZE] = {0};

uint8_t TransmitIndex = 0;
uint8_t TransmitSum = 0;
uint8_t TransmitBuffer[MAX_BUFFER_SIZE] = {0};

uint8_t ReceiveIndex_A = 0;
uint8_t ReceiveSum_A = 0;
uint8_t ReceiveBuffer_A[MAX_BUFFER_SIZE] = {0};

uint8_t TransmitIndex_A = 0;
uint8_t TransmitSum_A = 0;
uint8_t TransmitBuffer_A[MAX_BUFFER_SIZE] = {0};

/*******************************************************************************
 *******************************************************************************
 GLOBAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/

// INCIALIZACION DEL SPI
void SPI_Init(void)
{
    //========SPI_USCI CONFIGURATION=======================
    // Configure USCI pins
    SPI_Config_Pin(MOSI);
    SPI_Config_Pin(MISO);

    //Seteo del clock para ponerlo en Master-mode
    SPI_Config_Pin(CLK);

    //CS Definition
    gpioMode(CS,OUTPUT);

    // Configure SPI - Master
    UCB0CTL1 |= UCSWRST;                     // hold SPI in reset state and configure
    UCB0CTL1 |= UCSSEL_2;                    // Select BRCLK == SMCLK
    UCB0CTL0 |= UCSYNC;                      // SPI mode on
    UCB0CTL0 |= UCMST;                       // Master mode
    UCB0CTL0 |= UCMSB | UCCKPH;              // MSB first | clock phase

//    UCB0STAT |= UCLISTEN; // Listen Mode for tests!!
    UCB0STAT &= ~UCLISTEN; // Listen Mode power off!!

    UCB0BR0 = 1;                          // No division
//    UCB0BR0 = 0x80;                          // Divide by 128
    UCB0BR1 = 0;                          // No division

    UCB0CTL1 &= ~UCSWRST;                   // Initialize eUSCI
    UC0IFG &= ~UCB0RXIFG;                 // Reset flags

    //===========================================

    IE2 |= UCB0RXIE;                                        //Habilito la recepcion siempre
}

// ----------------------------------------------------------------------------------------------------------

void SPI_ISR_Enable(void){
    IE2 |= UCB0TXIE;
    _BIS_SR(GIE);                                            // Enable General interrupts
}

// ----------------------------------------------------------------------------------------------------------

void SPI_ISR_Disable(void)
{
    IE2 &= ~UCB0TXIE;
}

// ----------------------------------------------------------------------------------------------------------

uint8_t transferSPI(uint8_t txdata)
{
    uint8_t rxdata = 0xFF;
    SPI_Transmit(&txdata,1);
    while (UCB0STAT & UCBUSY || TransmitSum != 0);
    SPI_Receive(&rxdata,1);
    return rxdata;
}

// ----------------------------------------------------------------------------------------------------------

// CS TOGGLE
void CS_OFF(void){
    gpioWrite(CS,LOW);
}

void CS_ON(void){
    gpioWrite(CS,HIGH);
}

/*******************************************************************************
 SPI - USCI_A0
 ******************************************************************************/
void SPI_Init_A(void)
{
    //========SPI_USCI CONFIGURATION=======================
    // Configure USCI pins
    SPI_Config_Pin(B_REC);

    //Seteo del clock para ponerlo en Master-mode
    SPI_Config_Pin(B_CLK);

    //CS Definition
    gpioMode(B_SH,OUTPUT);

    // Configure SPI - Master
    UCA0CTL1 |= UCSWRST;                     // hold SPI in reset state and configure
    UCA0CTL1 |= UCSSEL_2;                    // Select BRCLK == SMCLK
    UCA0CTL0 |= UCSYNC;                      // SPI mode on
    UCA0CTL0 |= UCMST;                       // Master mode
    UCA0CTL0 |= UCMSB | UCCKPH;              // MSB first | clock phase
    UCA0MCTL = 0;                             // No modulation must be cleared for SPI

//    UCA0STAT |= UCLISTEN; // Listen Mode for tests!!
    UCA0STAT &= ~UCLISTEN; // Listen Mode power off!!

    UCA0BR0 = 1;                            // No division
    UCA0BR1 = 0;                            // No division

    UCA0CTL1 &= ~UCSWRST;                   // Initialize eUSCI
    UC0IFG &= ~UCA0RXIFG;                 // Reset flags

    //===========================================

    IE2 |= UCA0RXIE;                                        //Habilito la recepcion siempre

}
// ----------------------------------------------------------------------------------------------------------

void SPI_ISR_Enable_A(void){
    IE2 |= UCA0TXIE;
    _BIS_SR(GIE);                                            // Enable General interrupts
}

// ----------------------------------------------------------------------------------------------------------

void SPI_ISR_Disable_A(void)
{
    IE2 &= ~UCA0TXIE;
}

// ----------------------------------------------------------------------------------------------------------

uint8_t transferSPI_A(uint8_t txdata)
{
    uint8_t rxdata = 0xFF;
    SPI_Transmit_A(&txdata,1);
    while (UCA0STAT & UCBUSY || TransmitSum_A != 0);
    SPI_Receive_A(&rxdata,1);
    return rxdata;
}

// ----------------------------------------------------------------------------------------------------------

// CS TOGGLE
void CS_OFF_A(void){
    gpioWrite(B_SH,LOW);
}

void CS_ON_A(void){
    gpioWrite(B_SH,HIGH);
}

/*******************************************************************************
 *******************************************************************************
 LOCAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/
void SPI_Config_Pin(uint8_t pin) // PXSEL | PXSEL2 = 1 | 1
{
    uint8_t port = PIN2PORT(pin);
    uint8_t num = PIN2NUM(pin);

    switch(port)
    {
    case 1:
        BITSET(P1SEL,num);
        BITSET(P1SEL2,num);
        break;
    case 2:
        BITSET(P2SEL,num);
        BITSET(P2SEL2,num);
        break;
    }
}

// ----------------------------------------------------------------------------------------------------------

void SPI_Receive(uint8_t * rxdata,uint8_t count)
{
    uint8_t i = 0;
    UC0IE |= UCB0RXIE;
    //Paso el buffer al puntero de rxdata para su uso fuera de este archivo
    while (i < count)
    {
        if (ReceiveSum != 0) //Corroboro que se cargue la totalidad de lo recivido al puntero
        {
            rxdata[i] = ReceiveBuffer[INDEX(ReceiveIndex-ReceiveSum,MAX_BUFFER_SIZE)];
            ReceiveSum--;
        }
        i++;
    }
}

// ----------------------------------------------------------------------------------------------------------

void SPI_Transmit(uint8_t *txdata, uint8_t count)
{
    uint8_t i = 0;
    //Paso el buffer al puntero de rxdata para su uso fuera de este archivo
    UC0IE &= ~UCB0TXIE;         // Disable interruptions
    while (i < count)
    {
        if (TransmitSum != MAX_BUFFER_SIZE) //Corroboro que lea la totalidad del buffer
        {
            TransmitSum++;
            TransmitBuffer[TransmitIndex++] = txdata[i++];       // Save data in buffer array
                if (TransmitIndex == MAX_BUFFER_SIZE)  // Buffer overflow
                TransmitIndex = 0;
        }

        UC0IE |= UCB0TXIE;
    }
}

/*******************************************************************************
 SPI - USCI_A0
 ******************************************************************************/
void SPI_Receive_A(uint8_t * rxdata,uint8_t count)
{
    uint8_t i = 0;
    UC0IE |= UCA0RXIE;
    //Paso el buffer al puntero de rxdata para su uso fuera de este archivo
    while (i < count)
    {
        if (ReceiveSum_A != 0) //Corroboro que se cargue la totalidad de lo recivido al puntero
        {
            rxdata[i] = ReceiveBuffer_A[INDEX(ReceiveIndex_A-ReceiveSum_A,MAX_BUFFER_SIZE)];
            ReceiveSum_A--;
        }
        i++;
    }
}

// ----------------------------------------------------------------------------------------------------------

void SPI_Transmit_A(uint8_t *txdata, uint8_t count)
{
    uint8_t i = 0;
    //Paso el buffer al puntero de rxdata para su uso fuera de este archivo
    UC0IE &= ~UCA0TXIE;         // Disable interruptions
    while (i < count)
    {
        if (TransmitSum_A != MAX_BUFFER_SIZE) //Corroboro que lea la totalidad del buffer
        {
            TransmitSum_A++;
            TransmitBuffer_A[TransmitIndex_A++] = txdata[i++];       // Save data in buffer array
                if (TransmitIndex_A == MAX_BUFFER_SIZE)  // Buffer overflow
                TransmitIndex_A = 0;
        }

        UC0IE |= UCA0TXIE;
    }
}

/* ****************************************************************************
            INTERRUPCIONES
**************************************************************************** */

#pragma vector = USCIAB0RX_VECTOR
__interrupt void USCIB0_RX_ISR(void)
{
    if(IFG2 & UCB0RXIFG){
        if (ReceiveSum != MAX_BUFFER_SIZE) //Corroboro que lea la totalidad del buffer
        {
            ReceiveSum++;
            ReceiveBuffer[ReceiveIndex++] = UCB0RXBUF;       // Save data in buffer array
                if (ReceiveIndex == MAX_BUFFER_SIZE)  // Buffer overflow
                ReceiveIndex = 0;
        }
        else
            UC0IE &= ~UCB0RXIE;
    }
    if(IFG2 & UCA0RXIFG){
        if (ReceiveSum_A != MAX_BUFFER_SIZE) //Corroboro que lea la totalidad del buffer
        {
            ReceiveSum_A++;
            ReceiveBuffer_A[ReceiveIndex_A++] = UCA0RXBUF;       // Save data in buffer array
                if (ReceiveIndex_A == MAX_BUFFER_SIZE)  // Buffer overflow
                ReceiveIndex_A = 0;
        }
        else
            UC0IE &= ~UCA0RXIE;
    }
}

// ----------------------------------------------------------------------------------------------------------

#pragma vector = USCIAB0TX_VECTOR
__interrupt void USCIB0_TX_ISR(void)
{
    if(IFG2 & UCB0TXIFG){
        if (TransmitSum != 0) //Corroboro que se cargue la totalidad de lo que se quiere transmitir
        {
            UCB0TXBUF = TransmitBuffer[INDEX(TransmitIndex-TransmitSum,MAX_BUFFER_SIZE)]; //
            TransmitSum--;
        }
        else
            UC0IE &= ~UCB0TXIE;
    }
    if(IFG2 & UCA0TXIFG){
        if (TransmitSum_A != 0) //Corroboro que se cargue la totalidad de lo que se quiere transmitir
        {
            UCA0TXBUF = TransmitBuffer_A[INDEX(TransmitIndex_A-TransmitSum_A,MAX_BUFFER_SIZE)]; //
            TransmitSum_A--;
        }
        else
            UC0IE &= ~UCA0TXIE;
    }
}
