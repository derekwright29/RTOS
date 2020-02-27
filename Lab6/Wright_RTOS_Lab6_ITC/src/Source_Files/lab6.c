/*
 * lab2.c
 *
 *  Created on: Jan 26, 2020
 *      Author: goat
 */


#include <lab6.h>


/* CSEN globals: given by class instructors */
CSEN_Init_TypeDef csenInit = CSEN_INIT_DEFAULT;
CSEN_InitMode_TypeDef csenInitMode = CSEN_INITMODE_DEFAULT;
CSEN_InputSel_TypeDef channelList[CSEN_CHANNELS] = CAPSENSE_CHANNELS;

void decide_led_outs(uint8_t button0, uint8_t button1, uint8_t capstate) {
	uint8_t buttons_only = 0;
	// no touch sensed on capsense
	if (capsense_isNeither(capstate)) {
		buttons_only = 1;
	}

	// don't take capstate value into account
	if(buttons_only) {
		if (button_isNeither(button0, button1)) {
			// same as no buttons pressed
			GPIO_PinOutClear(LED0_port, LED0_pin);
			GPIO_PinOutClear(LED1_port, LED1_pin);
		}
		else{
			if (!button0) {
				GPIO_PinOutSet(LED0_port, LED0_pin);
				GPIO_PinOutClear(LED1_port, LED1_pin);
			}
			else if (!button1) {
				GPIO_PinOutSet(LED1_port, LED1_pin);
				GPIO_PinOutClear(LED0_port, LED0_pin);
			}
		}
	}
	else {
		//means we have a one-sided capstate.
		if (button_isNeither(button0, button1)) {
			if(capsense_isLeft(capstate)) {
				GPIO_PinOutSet(LED0_port, LED0_pin);
				GPIO_PinOutClear(LED1_port, LED1_pin);
			}
			else if (capsense_isRight(capstate)) {
				GPIO_PinOutSet(LED1_port, LED1_pin);
				GPIO_PinOutClear(LED0_port, LED0_pin);
			}

		}
		else {
			if (capsense_isLeft(capstate) || !button0) {
				GPIO_PinOutSet(LED0_port, LED0_pin);
			}
			if (capsense_isRight(capstate) || !button1) {
				GPIO_PinOutSet(LED1_port, LED1_pin);
			}
		}
	}
}

uint8_t button_isNeither(uint8_t butt0, uint8_t butt1) {
	return !(butt0 ^ butt1);
}

void init_capsense() {
	/* CAPSENSE Init */
	CMU_ClockEnable(cmuClock_HFPER, true);
	CMU_ClockEnable(cmuClock_CSEN_HF, true);

	CSEN_Init(CSEN, &csenInit);
	csenInitMode.convSel = csenDMRes16;
	csenInitMode.accMode = csenAccMode4;
}

uint8_t sample_capsense() {
	uint32_t i;
	uint32_t csResult = 0;
	/* Reset CAPSENSE state variables before updating */
	for (i = CSEN_CHANNELS; i > 0; i--)
	{
	  capsenseIsPressed[i-1] = 0;
	}

	/* Loop over CAPTOUCH channels and take CSEN measurements */
	for (i = CSEN_CHANNELS; i > 0; i--)
	{
	  /* Disable CSEN before updating channel to measure */
	  CSEN_Disable(CSEN);
	  csenInitMode.singleSel = channelList[i-1];
	  CSEN_InitMode(CSEN, &csenInitMode);
	  /* Re-enable CSEN */
	  CSEN_Enable(CSEN);

	  /* Start a CAPSENSE conversion */
	  CSEN_Start(CSEN);
	  /* Wait for measurement to finish */
	  while (CSEN_IsBusy(CSEN));

	  /* Store result in local variable */
	  csResult = CSEN_DataGet(CSEN);

	  /* Update global variable */
	  if (csResult > CAPSENSE_THRESHOLD) {
		  capsenseIsPressed[i-1] = true;
	  }
	  else {
		  capsenseIsPressed[i-1] = false;
	  }
	}
	return 0;

//	CAPSENSE_Sense();
////	get four channels, store in one variable to avoid too many variables
//	return CAPSENSE_getPressed(0) | (CAPSENSE_getPressed(1) << 1) | (CAPSENSE_getPressed(2) << 2) | (CAPSENSE_getPressed(3) << 3);
}

uint8_t capsense_isLeft(uint8_t capstate) {
	return (capstate & CAPSENSE_ISLEFT_MASK ) > 0;
}

uint8_t capsense_isRight(uint8_t capstate) {
	return (capstate & CAPSENSE_ISRIGHT_MASK) > 0;
}

uint8_t capsense_isNeither(uint8_t capstate) {
	if ((capstate & CAPSENSE_ISLEFT_MASK) && (capstate & CAPSENSE_ISRIGHT_MASK)) {
		//means both sides of slider are touched
		return 1;
	}
	if (!(capstate & CAPSENSE_ISLEFT_MASK) && !(capstate & CAPSENSE_ISRIGHT_MASK)) {
		//means neither side is touched
		return 1;
	}
	return 0;
}
