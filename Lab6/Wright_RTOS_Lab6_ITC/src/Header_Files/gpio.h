//***********************************************************************************
// Include files
//***********************************************************************************
#include "em_gpio.h"
#include "bspconfig.h"
#include  <kernel/include/os.h>

//***********************************************************************************
// defined files
//***********************************************************************************

// LED 0 pin is
#define	LED0_port		gpioPortF
#define LED0_pin		4
#define LED0_default	false 	// Default false (0) = off, true (1) = on
// LED 1 pin is
#define LED1_port		gpioPortF
#define LED1_pin		5
#define LED1_default	false	// Default false (0) = off, true (1) = on

// Flag defines
#define BUTTON_0_FLAG		0x1
#define BUTTON_1_FLAG       0x2

//***********************************************************************************
// global variables
//***********************************************************************************

/* Button 0 Event Message*/
OS_FLAG_GRP button_flags;


//***********************************************************************************
// function prototypes
//***********************************************************************************
void gpio_open(void);



/**
 * sample_button()
 * ___________________
 * @description This function returns the state of a pushbutton.
 * 				Two macro functions are provided to specifically
 * 				sample pushbutton 0 and 1.
 *
 * @parameters
 * 	- uint8_t button_pin: the pin number we want the state of.
 * 							In our case, either 6 or 7.
 * @return
 *		- uint8_t: 1 if button is UNpressed, 0 if button is PRESSED.
 */
uint8_t sample_button(uint8_t button_pin);
#define sample_button0(...)		sample_button(BSP_GPIO_PB0_PIN)
#define sample_button1(...)		sample_button(BSP_GPIO_PB1_PIN)

