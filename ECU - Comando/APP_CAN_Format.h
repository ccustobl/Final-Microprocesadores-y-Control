/***************************************************************************//**
  @file
  @brief
  @author   CCB
 ******************************************************************************/

#ifndef _APP_CAN_FORMAT_H_
#define _APP_CAN_FORMAT_H_

/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/

/*******************************************************************************
 * CONSTANT AND MACRO DEFINITIONS USING #DEFINE
 ******************************************************************************/

// ID
#define ECU_COMANDO                         0x500
#define ECU_PUERTA_DELANTERA_IZQUIERDA      0x501
#define ECU_PUERTA_DELANTERA_DERECHA        0x502
#define ECU_PUERTA_TRASERA_IZQUIERDA        0x503
#define ECU_PUERTA_TRASERA_DERECHA          0x504

/******************************************************************************/

// ECU en Uso
#define CAN_ID                              ECU_COMANDO

/******************************************************************************/

// Posición del mensaje para comando
#define DESTINATARIO                        0
#define ACCION                              1

// Posición del mensaje para puerta
#define ESTADO_VENTANILLA                   0
#define ESTADO_PUERTA                       1
#define ESTADO_TENSION                      2

// Estado actual de la ventana
#define DETENIDA                            0x00
#define SUBIENDO                            0x0F
#define BAJANDO                             0xF0

// Estado de puerta
#define PUERTA_CERRADA                      0x00
#define PUERTA_ABIERTA                      0x33

// Numeración de Puertas
#define PUERTA_DELANTERA_IZQUIERDA          0x01
#define PUERTA_DELANTERA_DERECHA            0x02
#define PUERTA_TRASERA_IZQUIERDA            0x03
#define PUERTA_TRASERA_DERECHA              0x04

// Estado deseado de ventanilla - Acciones
#define DETENER                             0x00
#define SUBIR                               0x0F
#define BAJAR                               0xF0

/******************************************************************************/

#if CAN_ID == ECU_COMANDO

// DLC
#define DLC                                 0x02

// Data
#define RECEIVE_BYTES                       3
#define TRANSMIT_BYTES                      2


#elif CAN_ID == ECU_PUERTA_DELANTERA_IZQUIERDA

// DLC
#define DLC                                 0x03

// Data
#define RECEIVE_BYTES                       2
#define TRANSMIT_BYTES                      3

// Info
#define NUMERO_PUERTA                       PUERTA_DELANTERA_IZQUIERDA


#elif CAN_ID == ECU_PUERTA_DELANTERA_DERECHA

// DLC
#define DLC                                 0x03

// Data
#define RECEIVE_BYTES                       2
#define TRANSMIT_BYTES                      3

// Info
#define NUMERO_PUERTA                       PUERTA_DELANTERA_DERECHA


#elif CAN_ID == ECU_PUERTA_TRASERA_IZQUIERDA

// DLC
#define DLC                                 0x03

// Data
#define RECEIVE_BYTES                       2
#define TRANSMIT_BYTES                      3

// Info
#define NUMERO_PUERTA                       PUERTA_TRASERA_IZQUIERDA


#elif CAN_ID == ECU_PUERTA_TRASERA_DERECHA

// DLC
#define DLC                                 0x03

// Data
#define RECEIVE_BYTES                       2
#define TRANSMIT_BYTES                      3

// Info
#define NUMERO_PUERTA                       PUERTA_TRASERA_DERECHA


#else
#error ERROR ECU Identification Incorrectly Defined
#endif

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

#endif // _APP_CAN_FORMAT_H_
