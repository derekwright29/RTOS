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

#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
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
	if (!p_Fifo)	//check NULL ptr
		return;

	p_Fifo->input[p_Fifo->tail] = value;
	p_Fifo->tail++;
	p_Fifo->tail %= FIFO_DEPTH;
    if (p_Fifo->num_items < FIFO_DEPTH)
	    p_Fifo->num_items++;
	// handle overflowing into head. Lose data
	// This is ok because at that point it is outdated.
	else if (p_Fifo->num_items == FIFO_DEPTH)
	{
		// bump head if we are full
		p_Fifo->head = (p_Fifo->tail % FIFO_DEPTH);
	}
    // printf("In Put: value of head is %d\n", p_Fifo->head);
    // printf("In Put: value of tail is %d\n", p_Fifo->tail);
    // printf("In Put: value of num_items is %d\n", p_Fifo->num_items);
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
	if (!p_Fifo || !p_value || InputFifo_isEmpty(p_Fifo))
		return false;
    // printf("In Get: value of head is %d\n", p_Fifo->head);
	*p_value = p_Fifo->input[p_Fifo->head++];
	p_Fifo->num_items--;
    p_Fifo->head %= FIFO_DEPTH;
	return true;
}


/***************************************************************************//**
 * @brief
 *    Gets emptiness of FIFO
 *
 * @return
 *   boolean: 1 if FIFO is empty
 *   		  0 else
 ******************************************************************************/
bool InputFifo_isEmpty(InputFifo_t *p_Fifo)
{
	if (!p_Fifo)
		return true;
	return (p_Fifo->num_items == 0);
}
