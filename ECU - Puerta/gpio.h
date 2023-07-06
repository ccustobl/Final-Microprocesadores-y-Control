/***************************************************************************//**
  @file     gpio.h
  @brief    Simple GPIO Pin services, similar to Arduino
  @author   Nicolás Magliola
 ******************************************************************************/

#ifndef _GPIO_H_
#define _GPIO_H_

/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/
#include "common.h"
#include "hardware.h"

/*******************************************************************************
 * CONSTANT AND MACRO DEFINITIONS USING #DEFINE
 ******************************************************************************/

#define LOW             0
#define HIGH            1

#ifndef INPUT
#define INPUT           0
#define OUTPUT          1
#define INPUT_PULLUP    2
#define INPUT_PULLDOWN  3
#endif // INPUT


// Convert port and numbber into pin ID
// Ex: P1.4 -> PORTNUM2PIN(1, 4) --> 0x14
//     P2.6 -> PORTNUM2PIN(2, 6) --> 0x26
#define PORTNUM2PIN(po, n)  ((po)<<4 | (n))
#define PIN2PORT(pi)        (((pi)>>4) & 0x03)
#define PIN2NUM(pi)         ((pi) & 0x07)


#define PIN0 0x01
#define PIN1 0x02
#define PIN2 0x04
#define PIN3 0x08
#define PIN4 0x10
#define PIN5 0x20
#define PIN6 0x40
#define PIN7 0x80

#define PIN_10 0x10
#define PIN_11 0x11
#define PIN_12 0x12
#define PIN_13 0x13
#define PIN_14 0x14
#define PIN_15 0x15
#define PIN_16 0x16
#define PIN_17 0x17
#define PIN_20 0x20
#define PIN_21 0x21
#define PIN_22 0x22
// No hay PIN 2.3
#define PIN_24 0x24
#define PIN_25 0x25
#define PIN_26 0x26
#define PIN_27 0x27
// No hay PIN 3.0
#define PIN_31 0x31
#define PIN_32 0x32

// On Board User Leds
#define RED_LED    PIN_10      //P1.0
#define GREEN_LED  PIN_11      //P1.1

// On Board User Switches
//#define SW1  B3       //P2.3
//#define SW2  PIN_27       //P2.7

// Definiciones Utiles
#define LED_PORT P1OUT
#define LED_PORT_DIR P1DIR
#define LED_PORT_FUNCTION_SELECT_0   P1SEL0
#define LED_PORT_FUNCTION_SELECT_1   P1SEL1
#define SW_PORT P2OUT
#define SW_PORT_IN P2IN
#define SW_PORT_DIR P2DIR
#define SW_PORT_PULL_ENABLE P2REN
#define SW_PORT_PULL_TYPE   P2OUT
#define PULL_UP     0x01
#define PULL_DOWN   0x00
#define PULL_ENABLE    0x01
#define PULL_DISABLE   0x00
#define mask(x)     (1<<(x))
#define set_alternate_pin_function_port1(function,pin) { P1SEL0|=((!!(function & 0x01))<<pin);\
                                                         P1SEL1|=((!!(function & 0x02))<<pin);}
#define ALT0    0
#define ALT1    1
#define ALT2    2
#define ALT3    3

/* Timer Counter Modes */
#define TIMER_STOP       MC_0   // Stop
#define TIMER_UP         MC_1   // Up to CCR0
#define TIMER_CONTINUOUS MC_2   // Up to 0xFFFF
#define TIMER_UPDOWM     MC_3   // Up to CCR0 Down to 0

// Correspondientes al ADC
#define FALSE 0
#define TRUE  1


// BITS
#define BIT_0 1
#define BIT_1 2
#define BIT_2 4
#define BIT_3 8
#define BIT_4 16
#define BIT_5 32
#define BIT_6 64
#define BIT_7 128

/*******************************************************************************
 * ENUMERATIONS AND STRUCTURES AND TYPEDEFS
 ******************************************************************************/

typedef uint8_t gpio_t;


/*******************************************************************************
 * VARIABLE PROTOTYPES WITH GLOBAL SCOPE
 ******************************************************************************/

/*******************************************************************************
 * FUNCTION PROTOTYPES WITH GLOBAL SCOPE
 ******************************************************************************/

/**
 * @brief Configures the specified pin to behave as a GPIO
 * @param pin The pin id whose mode you wish to set (according PORTNUM2PIN)
 * @param mode INPUT, OUTPUT, INPUT_PULLUP or INPUT_PULLDOWN.
 */
void gpioMode (gpio_t pin, uint8_t mode);

/**
 * @brief Writes a HIGH or a LOW value to a digital pin
 * @param pin The pin id (according PORTNUM2PIN)
 * @param value Desired value (HIGH or LOW)
 */
void gpioWrite (gpio_t pin, uint8_t value);

/**
 * @brief Toggles the value of a digital pin (HIGH<->LOW)
 * @param pin The pin id (according PORTNUM2PIN)
 */
void gpioToggle (gpio_t pin);

/**
 * @brief Reads the value from a specified digital pin, either HIGH or LOW.
 * @param pin The pin id (according PORTNUM2PIN)
 * @return HIGH or LOW
 */
uint8_t gpioRead (gpio_t pin);


/*******************************************************************************
 ******************************************************************************/

#endif // _GPIO_H_
