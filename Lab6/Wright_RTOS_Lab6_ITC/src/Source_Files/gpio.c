//***********************************************************************************
// Include files
//***********************************************************************************
#include "gpio.h"
#include  <kernel/include/os.h>
#include <lab6.h>



//***********************************************************************************
// defined files
//***********************************************************************************


//***********************************************************************************
// global variables
//***********************************************************************************


//***********************************************************************************
// function prototypes
//***********************************************************************************


//***********************************************************************************
// functions
//***********************************************************************************
void gpio_open(void){

	// Set LED ports to be standard output drive with default off (cleared)
//	GPIO_DriveStrengthSet(LED0_port, gpioDriveStrengthStrongAlternateStrong);
	GPIO_DriveStrengthSet(LED0_port, gpioDriveStrengthWeakAlternateWeak);
	GPIO_PinModeSet(LED0_port, LED0_pin, gpioModePushPull, LED0_default);

//	GPIO_DriveStrengthSet(LED1_port, gpioDriveStrengthStrongAlternateStrong);
	GPIO_DriveStrengthSet(LED1_port, gpioDriveStrengthWeakAlternateWeak);
	GPIO_PinModeSet(LED1_port, LED1_pin, gpioModePushPull, LED1_default);

	// Set buttons to be inputs
	GPIO_PinModeSet(BSP_GPIO_PB0_PORT, BSP_GPIO_PB0_PIN, gpioModeInputPull, 1);
	GPIO_PinModeSet(BSP_GPIO_PB1_PORT, BSP_GPIO_PB1_PIN, gpioModeInputPull, 1);

#if LAB2_USE_INTERRUPTS
	//enable both rising and falling edge of both push buttons
	GPIO_IntConfig(BSP_GPIO_PB0_PORT, BSP_GPIO_PB0_PIN, true, true, true);
	GPIO_IntConfig(BSP_GPIO_PB1_PORT, BSP_GPIO_PB1_PIN, true, true, true);

	NVIC_ClearPendingIRQ(GPIO_EVEN_IRQn);
	NVIC_ClearPendingIRQ(GPIO_ODD_IRQn);
	// enable pushbutton interrupts in the NVIC
	NVIC_EnableIRQ(GPIO_EVEN_IRQn);
	NVIC_EnableIRQ(GPIO_ODD_IRQn);

#endif

}


void GPIO_EVEN_IRQHandler(void) {
	uint32_t interrupt_masks = GPIO_IntGet();
	RTOS_ERR err;
	GPIO_IntClear(interrupt_masks);
	if (interrupt_masks & (1 << BSP_GPIO_PB0_PIN)) {
		OSFlagPost(&button_flags,
			BUTTON_0_FLAG,
			OS_OPT_POST_FLAG_SET,
			&err);
		APP_RTOS_ASSERT_DBG((RTOS_ERR_CODE_GET(err) == RTOS_ERR_NONE), 1);
	}
}

void GPIO_ODD_IRQHandler(void) {
	uint32_t interrupt_masks = GPIO_IntGet();
	RTOS_ERR err;
	GPIO_IntClear(interrupt_masks);
	if (interrupt_masks & (1 << BSP_GPIO_PB1_PIN)) {
		// determine rising/falling edge
		OSFlagPost(&button_flags,
				BUTTON_1_FLAG,
				OS_OPT_POST_FLAG_SET,
				&err);
		APP_RTOS_ASSERT_DBG((RTOS_ERR_CODE_GET(err) == RTOS_ERR_NONE), 1);
	}
}

