/***************************************************************************//**
 @file
 @brief
 @author  MGV-CCB
 ******************************************************************************/

/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/

#include "DRV_SPI.h"

#include <msp430.h>
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
void SPI_Config_Pin(uint8_t pin);
void SPI_Transmit(uint8_t *reg_data, uint8_t count);
void SPI_Receive(uint8_t * rxdata,uint8_t count);
/*******************************************************************************
 * STATIC VARIABLES AND CONST VARIABLES WITH FILE LEVEL SCOPE
 ******************************************************************************/
uint8_t ReceiveIndex = 0;
uint8_t ReceiveSum = 0;
uint8_t ReceiveBuffer[MAX_BUFFER_SIZE] = {0};

uint8_t TransmitIndex = 0;
uint8_t TransmitSum = 0;
uint8_t TransmitBuffer[MAX_BUFFER_SIZE] = {0};

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
    SPI_Config_Pin(PIN_CAN_MOSI);
    SPI_Config_Pin(PIN_CAN_MISO);

    //Seteo del clock para ponerlo en Master-mode
    SPI_Config_Pin(PIN_CAN_SCK);

    //CS Definition
    gpioMode(PIN_CAN_CS,OUTPUT);

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
//    UC0IFG &= ~UCB0RXIFG;                 // Reset flags

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
    gpioWrite(PIN_CAN_CS,LOW);
//    P2OUT &= ~BIT0;
}

void CS_ON(void){
    gpioWrite(PIN_CAN_CS,HIGH);
//    P2OUT |= BIT0;
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
//        P1SEL |= (port<<num);
//        P1SEL2 |= (port<<num);
        BITSET(P1SEL,num);
        BITSET(P1SEL2,num);
        break;
    case 2:
//        P2SEL |= (port<<num);
//        P2SEL2 |= (port<<num);
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
//            rxdata[i] = ReceiveBuffer[ReceiveIndex-ReceiveSum];
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
//    UC0IE |= UCB0TXIE;
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
}
