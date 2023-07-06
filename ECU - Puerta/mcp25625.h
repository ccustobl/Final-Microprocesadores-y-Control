/***************************************************************************//**
  @file
  @brief
  @author   MGV
 ******************************************************************************/
#ifndef MCP2565_H_
#define MCP2565_H_

/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/
/*******************************************************************************
 * CONSTANT AND MACRO DEFINITIONS USING #DEFINE
 ******************************************************************************/

/************************************************************
* STANDARD BITS
************************************************************/

//CAN - SPI Instructions
#define CANINST_RESET 0xC0
#define CANINST_READ 0x03
#define CANINST_READ_RX0_BUFFER  0x92
#define CANINST_WRITE 0x02
#define CANINST_LOAD_TX0_BUFFER  0x41
#define CANINST_RTS_0 0x81
#define CANINST_RTS_1 0x82
#define CANINST_RTS_2 0x84
#define CANINST_READ_STATUS 0xA0
#define CANINST_BIT_MODIFY 0x05


// CAN CONTROL (CANCTRL)
//Request Operation
#define CANCTRL_REQOP_MASK 0xE0
#define CANCTRL_REQOP_NORMAL 0x00
#define CANCTRL_REQOP_CONFIGURATION BIT7
#define CANCTRL_REQOP_SLEEP BIT5
#define CANCTRL_REQOP_LOOPBACK BIT6

//One-Shot Mode bit
#define CANCTRL_OSM_MASK BIT3
#define CANCTRL_OSM_ON BIT3
#define CANCTRL_OSM_OFF 0x00

// ----------------------------------------------------------------------------------------------------------

// CAN STATUS (CANSTAT)
// Operation Mode
#define CANSTAT_OPMOD_MASK 0xE0
#define CANSTAT_OPMOD_NORMAL 0x00
#define CANSTAT_OPMOD_CONFIGURATION BIT7
#define CANSTAT_OPMOD_SLEEP BIT5

// ----------------------------------------------------------------------------------------------------------

//CNF1 CONTROL BITS
// Synchronisation Jump Width Length Bits
#define CNF1_SJW_MASK (BIT7|BIT6)
#define CNF1_4TQ (BIT7|BIT6)
#define CNF1_3TQ BIT7
#define CNF1_2TQ BIT6
#define CNF1_1TQ 0x00

//Baud Rate Presclaer Bits
#define CNF1_BRP_MASK 0x3F
#define CNF1_BRP_0 0x00
#define CNF1_BRP_3 0x03

// ----------------------------------------------------------------------------------------------------------

//CNF2 CONTROL BITS
//PS2 Bit Time Length
#define CNF2_BTLMODE_MASK BIT7
#define CNF2_BTLMODE_1 BIT7 // PS2 define by PHSEG2 of CNF3
#define CNF2_BTLMODE_0 0x00 // PS2 define by greater of PS1 and IPT(2Tq)

//Sample Point Configuration
#define CNF2_SAM_MASK BIT6
#define CNF2_SAM_Three BIT6 //Sample three times
#define CNF2_SAM_Once 0x00 //Sample once

//PS1 Length Bits
#define CNF2_PHSEG1_MASK 0x38
#define CNF2_PHSEG1_1 0x00
#define CNF2_PHSEG1_2 0x08
#define CNF2_PHSEG1_3 0x10
#define CNF2_PHSEG1_4 0x18
#define CNF2_PHSEG1_5 0x20
#define CNF2_PHSEG1_6 0x28
#define CNF2_PHSEG1_7 0x30
#define CNF2_PHSEG1_8 0x38

//Propagation Segment Length Bit
#define CNF2_PRSEG_MASK 0x07
#define CNF2_PRSEG_1 0x00
#define CNF2_PRSEG_2 0x01
#define CNF2_PRSEG_3 0x02
#define CNF2_PRSEG_4 0x03
#define CNF2_PRSEG_5 0x04
#define CNF2_PRSEG_6 0x05
#define CNF2_PRSEG_7 0x06
#define CNF2_PRSEG_8 0x07

// ----------------------------------------------------------------------------------------------------------

//CNF3 CONTROL BITS
// Start of Frame Signal Bit
#define CNF3_SOF_MASK BIT7
#define CNF3_SOF_1 BIT7 //Pin enabled for SOF
#define CNF3_SOF_0 0x00 //Pin enabled for clock out function

// Wake Up Filter Bit
#define CNF3_WAKFIL_MASK BIT6
#define CNF3_WAKFIL_ON BIT6 //Wake-up enabled
#define CNF3_WAKFIL_OFF 0x00 //Wake-up disabled

//PS2 Length Bits
#define CNF3_PHSEG2_MASK 0x07
#define CNF3_PHSEG2_1 0x00
#define CNF3_PHSEG2_2 0x01
#define CNF3_PHSEG2_3 0x02
#define CNF3_PHSEG2_4 0x03
#define CNF3_PHSEG2_5 0x04
#define CNF3_PHSEG2_6 0x05
#define CNF3_PHSEG2_7 0x06
#define CNF3_PHSEG2_8 0x07

// ----------------------------------------------------------------------------------------------------------

//TXBxCTRL control bits
// Message Transmit Request Bit
#define TXBXCTRL_TXREQ BIT3

//Transmit Buffer Priority bits
#define TXBXCTRL_TXP_MASK 0x03
#define TXBXCTRL_TXP_HIGHEST 0x03
#define TXBXCTRL_TXP_HINT 0x02
#define TXBXCTRL_TXP_LINT 0x01
#define TXBXCTRL_TXP_LOWEST 0x00

// ----------------------------------------------------------------------------------------------------------

//TXBxDLC control bits
#define TXBxDLC_MASK 0x0F
#define TXBxDLC_0 0x00
#define TXBxDLC_1 0x01
#define TXBxDLC_2 0x02
#define TXBxDLC_3 0x03
#define TXBxDLC_4 0x04
#define TXBxDLC_5 0x05
#define TXBxDLC_6 0x06
#define TXBxDLC_7 0x07
#define TXBxDLC_8 0x08

// ----------------------------------------------------------------------------------------------------------
//TXB0SIDH/L, TXB1SIDH/L and TXB2SIDH/L
//Transmit Buffer Standard Frames bits
#define TXBXSIDH_xxx_MASK

//Extended Identifier
#define TXBXSIDL_EXIDE_MASK BIT3
#define TXBXSIDL_EXIDE_ON BIT3
#define TXBXSIDL_EXIDE_OFF 0x00


// ----------------------------------------------------------------------------------------------------------

//RXB0CTRL and RXB1CTRL
//Receive Buffers Operating Mode bits
#define RXBXCTRL_RXM_MASK (BIT6|BIT5)
#define RXBXCTRL_RXM_ALL (BIT6|BIT5) //Receive all messages
#define RXBXCTRL_RXM_FIL 0x00 //Receive all valid messages

//Rollover Confuguration
#define RXBXCTRL_BUKT_MASK BIT2
#define RXBXCTRL_BUKT_ON BIT2
#define RXBXCTRL_BUKT_OFF 0x00

// ----------------------------------------------------------------------------------------------------------
//RXF0SIDH/L and RXF1SIDH/L
//Receive Filters Standard Frames bits
#define RXFXSIDH_ID103_MASK 0xFF //Bits 10:3 of 11 bits identifier
#define RXFXSIDH_ID103_500 0xA0

#define RXFXSIDL_ID20_MASK 0xE0//Bits 2:0 of 11 bits identifier
#define RXFXSIDL_ID20_500 0x00

//Extended Identifier
#define RXFXSIDL_EXIDE_MASK BIT3
#define RXFXSIDL_EXIDE_ON BIT3
#define RXFXSIDL_EXIDE_OFF 0x00

// ----------------------------------------------------------------------------------------------------------
//RXM0SIDH/L and RXM1SIDH/L
#define RXMXSIDH_ID103_MASK 0xFF
#define RXMXSIDH_ID103_ALL 0xFF
#define RXMXSIDL_ID20_MASK 0xE0
#define RXMXSIDL_ID20_ALL 0xE0

// ----------------------------------------------------------------------------------------------------------

//CAN Interrupt Flag Register
#define CANINTE_MASK 0xFF
#define CANINTE_CLEAR 0x00
#define CANINTE_RX1IE BIT1
#define CANINTE_RX0IE BIT0

// ----------------------------------------------------------------------------------------------------------

//CAN Interrupt Flag Register
#define CANINTF_MASK 0xFF
#define CANINTF_ALL 0xFF
#define CANINTF_CLEAR 0x00
#define CANINTF_TX0IF BIT2
#define CANINTF_RX0IF BIT0


/*******************************************************************************
 * ENUMERATIONS AND STRUCTURES AND TYPEDEFS
 ******************************************************************************/

/*******************************************************************************
 * VARIABLE PROTOTYPES WITH GLOBAL SCOPE
 ******************************************************************************/

/*******************************************************************************
 * FUNCTION PROTOTYPES WITH GLOBAL SCOPE
 ******************************************************************************/

/*******************************************************************************
 ******************************************************************************/
#endif //MCP2565_H_
