/*******************************************************************************
 * @file
 * @brief FIFO declarations
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
#ifndef FIFO_H_
#define FIFO_H_

#include <stdint.h>
#include <stdbool.h>
#include "buttons.h"

#define FIFO_DEPTH 10

// FIFO 1 typedef
typedef uint8_t FifoIndex_t;
typedef ButtonFlag_t InputValue_t;

typedef struct {
    FifoIndex_t  head;
    FifoIndex_t  tail;
    uint8_t 	 num_items;
    InputValue_t input[FIFO_DEPTH];
} InputFifo_t;


//***********************************************************************************
// global variables
//***********************************************************************************

/* Button Input FIFOs */
InputFifo_t FIFO_Button0;
InputFifo_t FIFO_Button1;

void InputFifo_Put(InputFifo_t *p_Fifo, InputValue_t value);
bool InputFifo_Get(InputFifo_t *p_Fifo, InputValue_t *p_value);
bool InputFifo_isEmpty(InputFifo_t *p_Fifo);
uint8_t InputFifo_getNumItems(InputFIfo_t * p_Fifo)


#define FIFO2_DEPTH 10

typedef uint8_t FifoIndex_t;
typedef ButtonFlag_t InputValue_t;

typedef struct {
    FifoIndex_t  head;
    FifoIndex_t  tail;
    uint8_t 	 num_items;
    InputValue_t input[FIFO_DEPTH];
} InputFifo_t;


//***********************************************************************************
// Duplication in order to get FIFOs of different type
//***********************************************************************************

#define FIFO2_DEPTH 20

// FIFO 2 typedef
typedef uint8_t FifoIndex2_t;
typedef int_vect_t InputValue2_t;

typedef struct {
    FifoIndex2_t  head;
    FifoIndex2_t  tail;
    uint8_t 	 num_items;
    InputValue2_t input[FIFO2_DEPTH];
} InputFifo2_t;

void InputFifo2_Put(InputFifo2_t *p_Fifo, InputValue2_t value);
bool InputFifo2_Get(InputFifo2_t *p_Fifo, InputValue2_t *p_value);
bool InputFifo2_isEmpty(InputFifo2_t *p_Fifo);
uint8_t InputFifo2_getNumItems(InputFIfo2_t * p_Fifo);




#endif /* FIFO_H_*/
