#ifndef __FIFO_H__
#define __FIFO_H__

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
 *   Get the next value from the FIFO without changing the state of the FIFO
 *
 * @return
 *   Next value in the FIFO is stored in p_value
 * 	 bool returns true if successful, false else.
 ******************************************************************************/
bool InputFifo_Peek(InputFifo_t *p_Fifo, FifoIndex_t index, InputValue_t *p_value)
{
	if (!p_Fifo || !p_value || InputFifo_isEmpty(p_Fifo))
		return false;
	if (index >= p_Fifo->head + p_Fifo->num_items)
		return false;
	*p_value = p_Fifo->input[p_Fifo->head + index];
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

/***************************************************************************//**
 * @brief
 *    Gets current number of items in FIFO
 *
 * @return
 *   boolean: 0 if FIFO is NULL/empty
 *   		  num_items else
 ******************************************************************************/
uint8_t InputFifo_getNumItems(InputFifo_t * p_Fifo) {
	if (!p_Fifo)
		return 0;
	return p_Fifo->num_items;
}

/****************************************************************************
**************************************************************************
**************************************************************************
***************************************************************************


 * Duplication in order to have FIFOs of two different types in my project
 * 
 * 
 * 
 * ***************************************************************************
 * ***************************************************************************
 * ***************************************************************************
 * ***************************************************************************
 * 
 * 
 * ****************************************************************************/
/*
 * @brief
 *   Add a value to the FIFO
 *
 * @param value
 *   Value to add to the FIFO
 ******************************************************************************/
void InputFifo2_Put(InputFifo2_t *p_Fifo, InputValue2_t value)
{
	if (!p_Fifo)	//check NULL ptr
		return;

	p_Fifo->input[p_Fifo->tail] = value;
	p_Fifo->tail++;
	p_Fifo->tail %= FIFO2_DEPTH;
    if (p_Fifo->num_items < FIFO2_DEPTH)
	    p_Fifo->num_items++;
	// handle overflowing into head. Lose data
	// This is ok because at that point it is outdated.
	else if (p_Fifo->num_items == FIFO_DEPTH)
	{
		// bump head if we are full
		p_Fifo->head = (p_Fifo->tail % FIFO_DEPTH);
	}
	return;
}

/***************************************************************************//**
 * @brief
 *   Get the next value from the FIFO
 *
 * @return
 *   Next value in the FIFO is stored in p_value
 * 	 bool returns true if successful, false else.
 ******************************************************************************/
bool InputFifo2_Get(InputFifo2_t *p_Fifo, InputValue2_t *p_value)
{
	if (!p_Fifo || !p_value || InputFifo2_isEmpty(p_Fifo))
		return false;
    // printf("In Get: value of head is %d\n", p_Fifo->head);
	*p_value = p_Fifo->input[p_Fifo->head++];
	p_Fifo->num_items--;
    p_Fifo->head %= FIFO_DEPTH;
	return true;
}

/***************************************************************************//**
 * @brief
 *   Get the next value from the FIFO without changing the state of the FIFO
 *
 * @return
 *   Next value in the FIFO is stored in p_value
 * 	 bool returns true if successful, false else.
 ******************************************************************************/
bool InputFifo2_Peek(InputFifo2_t *p_Fifo, FifoIndex2_t index, InputValue2_t *p_value)
{
	if (!p_Fifo || !p_value || InputFifo2_isEmpty(p_Fifo))
		return false;
	if (index >= p_Fifo->head + p_Fifo->num_items)
		return false;
	*p_value = p_Fifo->input[p_Fifo->head + index];
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
bool InputFifo2_isEmpty(InputFifo2_t *p_Fifo)
{
	if (!p_Fifo)
		return true;
	return (p_Fifo->num_items == 0);
}

/***************************************************************************//**
 * @brief
 *    Gets current number of items in FIFO
 *
 * @return
 *   boolean: 0 if FIFO is NULL/empty
 *   		  num_items else
 ******************************************************************************/
uint8_t InputFifo2_getNumItems(InputFifo2_t * p_Fifo) {
	if (!p_Fifo)
		return 0;
	return p_Fifo->num_items;
}

#endif
