/***************************************************************************//**
 @file
 @brief
 @author   MGV
 ******************************************************************************/
#ifndef TEST_H_
#define TEST_H_

/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/
#include <msp430.h>
#include <stdint.h>
#include <DRV_SPI.h>
#include <DRV_Botonera.h>
#include <DRV_CAN.h>
#include <gpio.h>
#include <mcp25625.h>
#include <system.h>
#include <DRV_LEDs.h>
#include <APP_CAN_Format.h>
/*******************************************************************************
 * CONSTANT AND MACRO DEFINITIONS USING #DEFINE
 ******************************************************************************/

/*******************************************************************************
 * ENUMERATIONS AND STRUCTURES AND TYPEDEFS
 ******************************************************************************/

/*******************************************************************************
 * VARIABLE PROTOTYPES WITH GLOBAL SCOPE
 ******************************************************************************/

/*******************************************************************************
 * FUNCTION PROTOTYPES WITH GLOBAL SCOPE
 ******************************************************************************/
void test_SPI(void);
void test_SPI_A(void);
void test_CAN(void);
void test_Botonera(void);
void test_Botonera_SPI_A(void);
void test_LEDs(void);
void test_LEDs_Control(void);
void test_LEDs_RTI(void);
void test_RTI(void);

//SAMPLE
void sample_SPI(void);
void sample_CAN(void);
void sample_CAN_Mask(uint16_t ID);

//Variables de Prueba
int bufferStatus;
char data;
char data_A;
char data_B;
char DUMMY;
char prueba;

unsigned char receive_Shift;

/*******************************************************************************
 ******************************************************************************/
#endif /* TEST_H_ */
