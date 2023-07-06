/***************************************************************************//**
  @file
  @brief
  @author   MGV
 ******************************************************************************/

#ifndef DRV_CAN_H_
#define DRV_CAN_H_

/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/
#include <stdint.h>

/*******************************************************************************
 * CONSTANT AND MACRO DEFINITIONS USING #DEFINE
 ******************************************************************************/

//BIT MODIFY Adresses

#define BFPCTRL 0x0C
#define TXRTSCTRL 0x0D
#define CANSTAT 0x0E
#define CANCTRL 0X0F
#define TEC 0x1C
#define REC 0x1D
#define CNF3 0x28
#define CNF2 0x29
#define CNF1 0x2A
#define CANINTE 0x2B
#define CANINTF 0x2C
#define EFLG 0x2D

//Transmitt Adresses
#define TXB0CTRL 0x30
#define TXB1CTRL 0x40
#define TXB2CTRL 0x50
#define TXB0SIDH 0x31
#define TXB0SIDL 0x32
#define TXB0EID8 0x33
#define TXB1SIDH 0x41
#define TXB1SIDL 0x42
#define TXB2SIDH 0x51
#define TXB2SIDL 0x52

//Receive Adresses
#define RXB0CTRL 0x60
#define RXB1CTRL 0x70
//Acceptance Filters
#define RXF0SIDH 0x00
#define RXF0SIDL 0x01
#define RXF1SIDH 0x04
#define RXF1SIDL 0x05
//Filter Masks
#define RXM0SIDH 0x20
#define RXM0SIDL 0x21
#define RXM1SIDH 0x24
#define RXM1SIDL 0x25
//Receive Buffer Standard Identifier
#define RXB0SIDH 0x61
#define RXB0SIDL 0x62

//DLC
#define TXB0DLC 0x35
#define RXB0DLC 0x65

// OPERATION MODES
#define CONFIGURATION 0x80
#define NORMAL 0x00
#define SLEEP 0x20

//Transmit registers
#define TXB0D0 0x36
#define TXB0D1 0x37
#define TXB0D2 0x38
#define TXB0D3 0x39
#define TXB0D4 0x3A
#define TXB0D5 0x3B
#define TXB0D6 0x3C
#define TXB0D7 0x3D

#define TXB1D0 0x46
#define TXB1D1 0x47
#define TXB1D2 0x48
#define TXB1D3 0x49
#define TXB1D4 0x4A
#define TXB1D5 0x4B
#define TXB1D6 0x4C
#define TXB1D7 0x4D

#define TXB2D0 0x56
#define TXB2D1 0x57
#define TXB2D2 0x58
#define TXB2D3 0x59
#define TXB2D4 0x5A
#define TXB2D5 0x5B
#define TXB2D6 0x5C
#define TXB2D7 0x5D

//Receive registers
#define RXB0D0 0x66
#define RXB0D1 0x67
#define RXB0D2 0x68
#define RXB0D3 0x69
#define RXB0D4 0x6A
#define RXB0D5 0x6B
#define RXB0D6 0x6C
#define RXB0D7 0x6D


/*******************************************************************************
 * ENUMERATIONS AND STRUCTURES AND TYPEDEFS
 ******************************************************************************/

/*******************************************************************************
 * VARIABLE PROTOTYPES WITH GLOBAL SCOPE
 ******************************************************************************/

/*******************************************************************************
 * FUNCTION PROTOTYPES WITH GLOBAL SCOPE
 ******************************************************************************/
// INICIALIZACION DEL CAN
void CAN_Init(void);
// ----------------------------------------------------------------------------------------------------------

//BIT MODIFY Instruction
void CAN_BITMODIFY(char adress, char mask, char data);
// ----------------------------------------------------------------------------------------------------------

//Transmit
void CAN_TRANSMIT(char data[],char length,char ID);
// ----------------------------------------------------------------------------------------------------------

//LOAD TX BUFFER
void CAN_TX_BUFFER(char data[],char length,uint16_t ID);
// ----------------------------------------------------------------------------------------------------------

//Receive
void CAN_RECEIVE(char length,char * RX_CAN);
/*******************************************************************************
 ******************************************************************************/
#endif /* DRV_CAN_H_ */
