/*
 * ssdfunctions.c
 *
 *  Created on: 19 Ara 2020
 *      Author: Berkay Arslan / 171024030
 */


// SSD Functions

#include "stm32g0xx.h"
#include "ssdfunctions.h"



void clearSSD(void)
{
    GPIOA -> ODR |= (0xFFFFU);
}

void setSSD (int number)
{
	clearSSD();
    switch(number)
    {
        case 0:
        	GPIOA -> ODR &= ~(0x1833U);
            break;
        case 1:
            GPIOA -> ODR &= ~(0x12U);
            break;
        case 2:
            GPIOA -> ODR &= ~(0x1063U);
            break;
        case 3:
            GPIOA -> ODR &= ~(0x73U);
            break;
        case 4:
            GPIOA -> ODR &= ~(0x852U);
            break;
        case 5:
            GPIOA -> ODR &= ~(0x871U);
            break;
        case 6:
            GPIOA -> ODR &= ~(0x1871U);
            break;
        case 7:
            GPIOA -> ODR &= ~(0x13U);
            break;
        case 8:
            GPIOA -> ODR &= ~(0x1873U);
            break;
        case 9:
            GPIOA -> ODR &= ~(0x873U);
        case 10: //minus
        	GPIOA -> ODR &= ~(0x40U);
        	break;
        case 11: //decimal
        	GPIOA -> ODR &= ~(0x80U);
        	break;
        case 12: // I Letter
        	GPIOA -> ODR &= ~(0x1800U);
        	break;
        case 13: // N Letter
        	GPIOA -> ODR &= ~(0x1050U);
        	break;
        case 14: // V Letter
        	GPIOA -> ODR &= ~(0x1832U);
        	break;
        case 15: // O Letter
        	GPIOA -> ODR &= ~(0x1070U);
        	break;
        case 16: // u Letter
        	GPIOA -> ODR &= ~(0x1030U);
        	break;
        case 17: // F Letter
        	GPIOA -> ODR &= ~(0x1841U);
        	break;
        case 18: // L Letter
        	GPIOA -> ODR &= ~(0x1820U);
        	break;
        default:
            break;
    }
}

void Digit_One_ON(void)
{
	GPIOB -> ODR |= (1U << 3);
}

void Digit_Two_ON(void)
{
	GPIOB -> ODR |= (1U << 9);
}

void Digit_Three_ON(void)
{
	GPIOB -> ODR |= (1U << 5);
}

void Digit_Four_ON(void)
{
	GPIOB -> ODR |= (1U << 4);
}

void Digit_One_Off(void)
{
	GPIOB -> BRR |= (1U << 3);
}

void Digit_Two_Off(void)
{
	GPIOB -> BRR |= (1U << 9);
}

void Digit_Three_Off(void)
{
	GPIOB -> BRR |= (1U << 5);
}

void Digit_Four_Off(void)
{
	GPIOB -> BRR |= (1U << 4);
}

void Digit_All_Off(void)
{
    GPIOA -> BRR |= (1U << 8);
    GPIOB -> BRR |= (1U << 9);
    GPIOB -> BRR |= (1U << 5);
    GPIOB -> BRR |= (1U << 4);
}

void delay(int delay_s)
{
	for (int i = 0; i < delay_s; ++i);
}

