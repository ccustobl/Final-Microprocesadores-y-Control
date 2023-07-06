/***************************************************************************//**
  @file     Botonera.c
  @brief    Lectura de la botonera y de los estados
  @author   MGV
 ******************************************************************************/

/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/
#include <DRV_Botonera.h>

/*******************************************************************************
 * CONSTANT AND MACRO DEFINITIONS USING #DEFINE
 ******************************************************************************/

/*******************************************************************************
 * ENUMERATIONS AND STRUCTURES AND TYPEDEFS
 ******************************************************************************/

/*******************************************************************************
 * VARIABLES WITH GLOBAL SCOPE
 ******************************************************************************/
int CLK_BOTONERA_INH;
int CLK_SHIFT;
int SH_SHIFT;
int SHIFT_ENDED;

char PUERTA_MASK[4] = {PUERTA1_MASK,PUERTA2_MASK,PUERTA3_MASK,PUERTA4_MASK};
int PUERTA_SHIFT[4] = {PUERTA1_SHIFT,PUERTA2_SHIFT,PUERTA3_SHIFT,PUERTA4_SHIFT};
char BOTON_SUBIR[4] = {PUERTA13_SUBIR,PUERTA24_SUBIR,PUERTA13_SUBIR,PUERTA24_SUBIR};
char BOTON_BAJAR[4] = {PUERTA13_BAJAR,PUERTA24_BAJAR,PUERTA13_BAJAR,PUERTA24_BAJAR};

/*******************************************************************************
 * FUNCTION PROTOTYPES FOR PRIVATE FUNCTIONS WITH FILE LEVEL SCOPE
 ******************************************************************************/

/*******************************************************************************
 * ROM CONST VARIABLES WITH FILE LEVEL SCOPE
 ******************************************************************************/
unsigned char receive_Shift;
/*******************************************************************************
 * STATIC VARIABLES AND CONST VARIABLES WITH FILE LEVEL SCOPE
 ******************************************************************************/

/*******************************************************************************
 *******************************************************************************
                        GLOBAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/
void botonera_Init(void){
    // PIN MODES
    gpioMode(B_CLK, OUTPUT);
    gpioMode(B_SH, OUTPUT);
//    gpioMode(B_CLK_INH, OUTPUT);//Se saco este pin por cuestión de espacio
    gpioMode(B_REC, INPUT);

    //PIN INITIAL VALUES
    gpioWrite(B_REC,LOW);
    gpioWrite(B_CLK,LOW);
//    gpioWrite(B_CLK_INH,LOW);
    gpioWrite(B_SH,HIGH);
}

// ----------------------------------------------------------------------------------------------------------
unsigned char botoneraRead_SPI_A(void){
    unsigned char receive_Shift;

    gpioWrite(B_SH,LOW); //Recibo
    gpioWrite(B_SH,HIGH); //Recibo
    receive_Shift = transferSPI_A(0xFF);

    return receive_Shift;
}

/*******************************************************************************
 *******************************************************************************
                        LOCAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/
