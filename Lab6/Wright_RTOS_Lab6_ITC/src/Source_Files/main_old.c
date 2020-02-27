/***************************************************************************//**
 * @file
 * @brief Simple LED Blink Demo for SLSTK3402A
 *******************************************************************************
 * # License
 * <b>Copyright 2018 Silicon Laboratories Inc. www.silabs.com</b>
 *******************************************************************************
 *
 * The licensor of this software is Silicon Laboratories Inc. Your use of this
 * software is governed by the terms of Silicon Labs Master Software License
 * Agreement (MSLA) available at
 * www.silabs.com/about-us/legal/master-software-license-agreement. This
 * software is distributed to you in Source Code format and is governed by the
 * sections of the MSLA applicable to Source Code.
 *
 ******************************************************************************/

#include <stdint.h>
#include <stdbool.h>
#include "em_device.h"
#include "em_chip.h"
#include "em_emu.h"
#include "bspconfig.h"
#include "main.h"
#include "app.h"
#include "capsense.h"
#include "lab2.h"

/* Global variable definitions */
volatile uint8_t slide;	//position(s) of slider. Uses four channels
volatile uint8_t butt0=1, butt1=1;	// values go to 0 when button is pressed; 1 when unpressed. Init to 1.
volatile uint32_t msticks = 0; // gives 4 billion some milliseconds. I do not account for wrapping...

//definition of Systick IRQ Handler
void SysTick_Handler(void) {
#if !LAB2_USE_INTERRUPTS
	//ms counter for polling method delay function
	 msticks+=1;
#endif
#if LAB2_USE_INTERRUPTS
	 slide = sample_capsense();
	 decide_led_outs(butt0,butt1,slide);
#endif
}
#if !LAB2_USE_INTERRUPTS
//Delay function only for polling method
void Delay(uint32_t delay) {
	uint32_t curr;
	curr = msticks;
	while ((msticks - curr) < delay);
}
#endif

int main(void)
{
  EMU_DCDCInit_TypeDef dcdcInit = EMU_DCDCINIT_DEFAULT;
  CMU_HFXOInit_TypeDef hfxoInit = CMU_HFXOINIT_DEFAULT;

  /* Chip errata */
  CHIP_Init();

  /* Init DCDC regulator and HFXO with kit specific parameters */
  /* Init DCDC regulator and HFXO with kit specific parameters */
  /* Initialize DCDC. Always start in low-noise mode. */
  EMU_EM23Init_TypeDef em23Init = EMU_EM23INIT_DEFAULT;
  EMU_DCDCInit(&dcdcInit);
  em23Init.vScaleEM23Voltage = emuVScaleEM23_LowPower;
  EMU_EM23Init(&em23Init);
  CMU_HFXOInit(&hfxoInit);

  /* Switch HFCLK to HFRCO and disable HFRCO */
  CMU_OscillatorEnable(cmuOsc_HFRCO, true, true);
  CMU_ClockSelectSet(cmuClock_HF, cmuSelect_HFRCO);
  CMU_OscillatorEnable(cmuOsc_HFXO, false, false);

#if LAB2_USE_INTERRUPTS
  /* Setup Systick for 100ms interrupts */
  if(SysTick_Config(CMU_ClockFreqGet(cmuClock_CORE) / 10)) {
	  while(1);
  }
#endif
#if !LAB2_USE_INTERRUPTS
  /* Setup Systick for 1ms interrupts to make for an
   * accurate delay function for polling method */
  if(SysTick_Config(CMU_ClockFreqGet(cmuClock_CORE) / 1000)) {
	  while(1);
  }
#endif


  /* Call application program to open / initialize all required peripheral */
  app_peripheral_setup();

  /* Start capacitive sense buttons */
  CAPSENSE_Init();

  /* Infinite blink loop */
  while (1) {

#if !LAB2_USE_INTERRUPTS
	  slide = sample_capsense();
	  butt0 = sample_button(BSP_GPIO_PB0_PIN);
	  butt1 = sample_button(BSP_GPIO_PB1_PIN);
	  decide_led_outs(butt0, butt1, slide);
	  //delay 100ms
	  Delay(100);
#endif

#if LAB2_USE_INTERRUPTS
	  EMU_EnterEM1();
#endif

  }
  return 0;
}
