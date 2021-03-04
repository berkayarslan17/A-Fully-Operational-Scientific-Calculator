/*
 * keypadfunctions.c
 *
 *  Created on: 19 Ara 2020
 *      Author: Berkay Arslan / 171024030
 */


// Key Pad Functions

#include "stm32g0xx.h"
#include "keypadfunctions.h"

void clearRowsKeypad(void)
{
    GPIOB -> ODR &= ~(1U << 8);
    GPIOB -> ODR &= ~(1U << 2);
    GPIOB -> ODR &= ~(1U << 0);
    GPIOA -> ODR &= ~(1U << 9);
}

void setRowsKeypad(void)
{
    GPIOB -> ODR |= (1U << 8);
    GPIOB -> ODR |= (1U << 2);
    GPIOB -> ODR |= (1U << 0);
    GPIOA -> ODR |= (1U << 9);
}

