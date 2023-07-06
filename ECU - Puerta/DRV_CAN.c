/***************************************************************************//**
 @file
 @brief
 @author   MGV-CCB
 ******************************************************************************/

/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/

#include "DRV_CAN.h"

#include <msp430.h>
#include "gpio.h"
#include "board.h"

#include "APP_Data.h"
#include "APP_CAN_Format.h"

#include "DRV_SPI.h"
#include "DRV_ISR.h"
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
char lectura_TXB0D ;
char chequeo_data;

/*******************************************************************************
 * FUNCTION PROTOTYPES FOR PRIVATE FUNCTIONS WITH FILE LEVEL SCOPE
 ******************************************************************************/
//READ Instruction
char CAN_READ(char adress);
// ----------------------------------------------------------------------------------------------------------

//WRITE Instruction
void CAN_WRITE(char adress, char data);
// ----------------------------------------------------------------------------------------------------------

// Request To Send(RTS)
void CAN_RTS(int T);
// ----------------------------------------------------------------------------------------------------------

// READ STATUS Instruction
char CAN_READSTATUS(void);
// ----------------------------------------------------------------------------------------------------------

// RESET Instruction
void CAN_RESET(void);
// ----------------------------------------------------------------------------------------------------------

// READ RX ID
void CAN_READ_RX_ID(uint16_t * RX_ID);

/*******************************************************************************
 * STATIC VARIABLES AND CONST VARIABLES WITH FILE LEVEL SCOPE
 ******************************************************************************/

/*******************************************************************************
 *******************************************************************************
 GLOBAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/

// INICIALIZACION DEL CAN

void CAN_Init(void){
    extern uint16_t CAN_ID;

    SPI_Init();

    CAN_RESET(); //Reseteo, debería ir a Configuration Mode

    //Chequeo que este en CONFIGURATION MODE, sino lo llevo a este modo
    if(!(CAN_READ(CANSTAT) & CANSTAT_OPMOD_CONFIGURATION)){
        CAN_BITMODIFY(CANCTRL,CANCTRL_REQOP_MASK,CANCTRL_REQOP_CONFIGURATION);
    }

    //CNF1 Configuration
    CAN_BITMODIFY(CNF1,CNF1_BRP_MASK,CNF1_BRP_3);

    //CNF2 Configuration
    CAN_BITMODIFY(CNF2,CNF2_BTLMODE_MASK,CNF2_BTLMODE_1);
    CAN_BITMODIFY(CNF2,CNF2_PRSEG_MASK,CNF2_PRSEG_7); //(PRSEG+1) x Tq
    CAN_BITMODIFY(CNF2,CNF2_PHSEG1_MASK,CNF2_PHSEG1_4); //(PHSEG1+1) x Tq
    CAN_BITMODIFY(CNF1,CNF1_SJW_MASK,CNF1_4TQ); //Set width = 4Tq

    CAN_BITMODIFY(CNF2,CNF2_SAM_MASK,CNF2_SAM_Once);

    //CNF3 Configuration
    CAN_BITMODIFY(CNF3,CNF3_SOF_MASK,CNF3_SOF_0);
    CAN_BITMODIFY(CNF3,CNF3_WAKFIL_MASK,CNF3_WAKFIL_OFF);
    CAN_BITMODIFY(CNF3,CNF3_PHSEG2_MASK,CNF3_PHSEG2_4); //(PHSEG2+1) x Tq

    //Receive Identifier - Standard Identifier 11bits
    CAN_BITMODIFY(RXF0SIDL,RXFXSIDL_EXIDE_MASK,RXFXSIDL_EXIDE_OFF); //Extended Identifier OFF

    //RXB0CTRL and RXB1CTRL  Configuration
    if(CAN_ID == ECU_COMANDO){
        CAN_BITMODIFY(RXB0CTRL,RXBXCTRL_RXM_MASK,RXBXCTRL_RXM_ALL); //Receive all messages
    }
    else{
        CAN_BITMODIFY(RXB0CTRL,RXBXCTRL_RXM_MASK,RXBXCTRL_RXM_FIL); //Receive if filter criteria is met
        // Acceptance Filters - ONLY Receive ECU(ID:0x500)
        CAN_BITMODIFY(RXF0SIDH,RXFXSIDH_ID103_MASK,RXFXSIDH_ID103_500);
        CAN_BITMODIFY(RXF0SIDL,RXFXSIDL_ID20_MASK,RXFXSIDL_ID20_500);
        //Filters Mask
        CAN_BITMODIFY(RXM0SIDH,RXMXSIDH_ID103_MASK,RXMXSIDH_ID103_ALL);
        CAN_BITMODIFY(RXM0SIDL,RXMXSIDL_ID20_MASK,RXMXSIDL_ID20_ALL);
    }

    CAN_BITMODIFY(RXB0CTRL,RXBXCTRL_BUKT_MASK,RXBXCTRL_BUKT_ON); //Rollover
//    CAN_BITMODIFY(RXB0CTRL,RXBXCTRL_BUKT_MASK,RXBXCTRL_BUKT_OFF);

    //Interrupt enable
    if(CAN_ID == ECU_COMANDO){
        CAN_BITMODIFY(CANINTE,CANINTE_MASK,CANINTE_CLEAR);
    }
    else{
        CAN_BITMODIFY(CANINTE,CANINTE_MASK,CANINTE_CLEAR);
        CAN_BITMODIFY(CANINTE,CANINTE_MASK,CANINTE_RX0IE); //Enable interrupt when message is received in RX0BD0
    }

    //Clear Flags
    CAN_BITMODIFY(CANINTF,CANINTF_MASK,CANINTF_CLEAR);

    //Define Transmitt Buffer Priorites
    CAN_BITMODIFY(TXB0CTRL,TXBXCTRL_TXP_MASK,TXBXCTRL_TXP_HIGHEST);
    CAN_BITMODIFY(TXB1CTRL,TXBXCTRL_TXP_MASK,TXBXCTRL_TXP_HINT);
    CAN_BITMODIFY(TXB2CTRL,TXBXCTRL_TXP_MASK,TXBXCTRL_TXP_LOWEST);

    //Transmitt Identifier - Standard Identifier 11bits
    CAN_BITMODIFY(TXB0SIDL,TXBXSIDL_EXIDE_MASK,TXBXSIDL_EXIDE_OFF); //Extended Identifier OFF

    //OSM
    CAN_BITMODIFY(CANCTRL,CANCTRL_OSM_MASK, CANCTRL_OSM_ON);

    //Set Normal mode
    CAN_BITMODIFY(CANCTRL,CANCTRL_REQOP_MASK,CANCTRL_REQOP_NORMAL);
    while(CAN_READ(CANSTAT) != CANSTAT_OPMOD_NORMAL){     //Chequeo que este en Normal MODE, sino lo llevo a este modo
        CAN_BITMODIFY(CANCTRL,CANCTRL_REQOP_MASK,CANCTRL_REQOP_NORMAL);
    }

    //Set Loopback mode
//    CAN_BITMODIFY(CANCTRL,CANCTRL_REQOP_MASK,CANCTRL_REQOP_LOOPBACK);
}

// ----------------------------------------------------------------------------------------------------------

//BIT MODIFY Instruction
void CAN_BITMODIFY(char adress, char mask, char data){
    CS_OFF();
    transferSPI(CANINST_BIT_MODIFY);
    transferSPI(adress);
    transferSPI(mask);
    transferSPI(data);
    while(UCB0STAT & UCBUSY);
    CS_ON();
}

// ----------------------------------------------------------------------------------------------------------

void CAN_TRANSMIT(char data[],char length,char ID){

    //todo: REVISAR CON EL TX_BUFFER. POR AHORA NO USAR!!!!!

    volatile char i;
    volatile char DLC_New;
    char stat;

    if(length <= 8){
        //Compruebo que no haya una transmision pendiente
//        while(CAN_READ(TXB0CTRL) & TXBXCTRL_TXREQ);
        stat = CAN_READSTATUS();
        while(stat &TXBXCTRL_TXREQ);

        //Configuración del DLC
        DLC_New = CAN_READ(TXB0DLC) & 0xF0; //Para no perder la info de los registros en esta posición
        DLC_New |= length;
        CAN_WRITE(TXB0DLC,length);

        // Configuración del ID
        CAN_WRITE(TXB0SIDL,0x00);
        CAN_WRITE(TXB0SIDH, ID);

        //todo: Chequer porque el envió no siempre funciona como debe.

        for(i=0; i<length; i++){
            CAN_WRITE(TXB0D0 +i,data[i]);
            chequeo_data = data[i];
            lectura_TXB0D = CAN_READ(TXB0D0 +i);
            while(!(chequeo_data & lectura_TXB0D)){
                CAN_WRITE(TXB0D0 +i,data[i]);
            }
        }

        // RTS COMMAND
        CAN_RTS(0);

        //Chequeo que se termino de mandar
        while(CAN_READ(TXB0CTRL) & TXBXCTRL_TXREQ);

        //Apago la Flag
        CAN_BITMODIFY(CANINTF, CANINTF_TX0IF, 0x00);
    }
}

// ----------------------------------------------------------------------------------------------------------

void CAN_TX_BUFFER(char data[],char length,uint16_t ID){
    volatile char i;
    volatile char DLC_New;
    uint8_t ID_H, ID_L;
    char stat;

    if(length <= 8){
        //Compruebo que no haya una transmision pendiente
        stat = CAN_READSTATUS();
        while(stat & TXBXCTRL_TXREQ){
            stat = CAN_READSTATUS();
        }

        //Configuración del DLC
        DLC_New = CAN_READ(TXB0DLC) & 0xF0; //Para no perder la info de los registros en esta posición
        DLC_New |= length;
        CAN_WRITE(TXB0DLC,length);

        // Configuración del ID
        ID_H = (ID>>3);
        ID_L = ((ID & 0x07)<<5);
        CAN_WRITE(TXB0SIDH, ID_H);
        CAN_WRITE(TXB0SIDL,ID_L);
        //todo: Corroborar carga correcta del ID

        // Carga al buffer TXB0D0

        CS_OFF();

        transferSPI(0x41);
        for(i=0; i<length; i++){
            transferSPI(data[i]);
        }

        while(UCB0STAT & UCBUSY);
        CS_ON();

        for(i=0; i<length; i++){
            chequeo_data = data[i];
            lectura_TXB0D = CAN_READ(TXB0D0 +i);
        }

        // RTS COMMAND
        CAN_RTS(0);

        //Chequeo que se termino de mandar
        while(CAN_READ(TXB0CTRL) & TXBXCTRL_TXREQ);//todo: Verificar como chequeo que se mando all

        //Apago la Flag
        CAN_BITMODIFY(CANINTF, CANINTF_TX0IF, 0x00);
   }
}

// ----------------------------------------------------------------------------------------------------------

void CAN_RECEIVE(char length,char * RX_CAN){
    volatile char i;
    volatile int RXflag = 0;
    volatile char data;
    volatile char stat;

    extern uint16_t RX_ID;

    stat = CAN_READSTATUS();

//    RXflag = CAN_READ(CANINTF) & CANINTF_RX0IF;
    RXflag = stat & CANINTF_RX0IF;
    if(RXflag == 1){ //Chequeo si hay una flag de recepción

        CS_OFF();

        transferSPI(0x92); //Ver READ RX Buffer Instruction

        for (i = 0; i < length; i++)
        {
            data = transferSPI(0xFF); // Envio para mantener el clock prendido y poder leer
            *(RX_CAN+i) = data;//todo:revisar como sumar los index
        }

        CS_ON();

        CAN_READ_RX_ID(&RX_ID);

        CAN_BITMODIFY(CANINTF, CANINTF_RX0IF,0x00); //Apago la flag

//        CAN_BITMODIFY(CANINTF,CANINTF_MASK,CANINTF_CLEAR);
    }
}

// ----------------------------------------------------------------------------------------------------------
void CAN_Config_ISRPin(uint8_t pin)
{
    gpioMode(pin,INPUT_PULLUP);
}

// ----------------------------------------------------------------------------------------------------------

void CAN_CLEAR_FLAG(void){
    if((CAN_READ(CANINTF) & CANINTF_RX0IF) != 0){
        CAN_BITMODIFY(CANINTF, CANINTF_RX0IF,0x00);
    }
//    if(CAN_READ(CANINTF)!= 0){
//        CAN_BITMODIFY(CANINTF,CANINTF_MASK,CANINTF_CLEAR);
//    }
}


/*******************************************************************************
 *******************************************************************************
 LOCAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/

// RESET Instruction
void CAN_RESET(void){
    CS_OFF();
    transferSPI(CANINST_RESET);
    CS_ON();
}

// ----------------------------------------------------------------------------------------------------------

// Request To Send(RTS)
void CAN_RTS(int T){
    CS_OFF();
    switch(T){
    case 0:
        transferSPI(CANINST_RTS_0);
    break;
    case 1:
        transferSPI(CANINST_RTS_1);
    break;
    case 2:
        transferSPI(CANINST_RTS_2);
    break;
    }
    while(UCB0STAT & UCBUSY);
    CS_ON();
}

// ----------------------------------------------------------------------------------------------------------

//READ Instruction
char CAN_READ(char adress){
    char data;
    CS_OFF();
    transferSPI(CANINST_READ);
    transferSPI(adress);
    data = transferSPI(0xFF); // Envio para mantener el clock prendido y poder leer
    while(UCB0STAT & UCBUSY); // Corroboro que el SPI termino el envío
    CS_ON();
    return data;
}

// ----------------------------------------------------------------------------------------------------------

//WRITE Instruction
void CAN_WRITE(char adress, char data){
    CS_OFF();
    transferSPI(CANINST_WRITE);
    transferSPI(adress);
    transferSPI(data);
    while(UCB0STAT & UCBUSY);
    CS_ON();
}

// ----------------------------------------------------------------------------------------------------------

// READ STATUS Instruction
char CAN_READSTATUS(void){
    char data;
    CS_OFF();
    transferSPI(CANINST_READ_STATUS);
    transferSPI(0xFF);
    data = transferSPI(0xFF);
    while(UCB0STAT & UCBUSY);
    CS_ON();
    return data;
}

// ----------------------------------------------------------------------------------------------------------

void CAN_READ_RX_ID(uint16_t * RX_ID){
    uint8_t RX_ID_H, RX_ID_L;
    uint16_t RX_ID_Compile;

    RX_ID_H = CAN_READ(RXB0SIDH);
    RX_ID_L = CAN_READ(RXB0SIDL);

    RX_ID_Compile = ((RX_ID_H<<3)|(RX_ID_L>>5));

    *RX_ID = RX_ID_Compile;
}

/* ****************************************************************************
            INTERRUPTIONS
**************************************************************************** */

/******************************************************************************/
