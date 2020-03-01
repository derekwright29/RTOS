/*******************************************************************************
 * @file
 * @brief FIFO implementation
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

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include <common/include/rtos_utils.h>

#include "fifo.h"

#define FIFO_DEPTH 10

/***************************************************************************//**
 * @brief
 *   Add a value to the FIFO
 *
 * @param value
 *   Value to add to the FIFO
 ******************************************************************************/
void InputFifo_Put(InputFifo_t *p_Fifo, InputValue_t value)
{
   return;
}

/***************************************************************************//**
 * @brief
 *   Get the next value from the FIFO
 *
 * @return
 *   Next value in the FIFO
 ******************************************************************************/
bool InputFifo_Get(InputFifo_t *p_Fifo, InputValue_t *p_value)
{
   return NULL;
}
