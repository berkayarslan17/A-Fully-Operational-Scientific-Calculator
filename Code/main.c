/*
 * main.c
 *
 *  Created on: 19 Ara 2020
 *      Author: Berkay Arslan / 171024030
 */

#include "stm32g0xx.h"
#include "ssdfunctions.h"
#include "keypadfunctions.h"
#include <math.h>

// Global Variables
int minus = 10;
int decimal = 11;
int I_Letter = 12;
int N_Letter = 13;
int V_Letter = 14;
int O_Letter = 15;
int u_Letter = 16;
int F_Letter = 17;
int L_Letter = 18;
int waiting_time = 0;
uint8_t press_times = 0;
_Bool prevent_bounce = 0;
int First_Digit_Value = 0;
int Second_Digit_Value = 0;
int Third_Digit_Value = 0;
int Fourth_Digit_Value = 0;

//IDLE State
_Bool IDLE_Flag = 1;
int IDLE_Transition = 0;

//Anomaly Variables
_Bool Overflow_Flag = 0;
_Bool Invalid_Flag = 0;

// Computation Variables
float counter = 0;
int Float_Number = 0;
_Bool Enter_Flag = 0;
_Bool Minus_Flag = 0;
_Bool Floating_Flag = 0;

// Addition Variables
_Bool Addition_Flag = 0;

// Subtraction Variables
_Bool Subtraction_Flag = 0;

// Multiplication Variables
_Bool Multiplication_Flag = 0;

// Division Variables
_Bool Division_Flag = 0;

// Scientific Variables
int Scientific_Flag = 0;

//Timer Flags
_Bool AdditionFunc_Flag = 0;
_Bool SubtractionFunc_Flag = 0;
_Bool MultiplicationFunc_Flag = 0;
_Bool DivisionFunc_Flag = 0;
_Bool ScientificFunc_Flag = 0;
_Bool EnterFunc_Flag = 0;

void TIM3_IRQHandler(void);
void EXTI0_1_IRQHandler(void);
void EXTI4_15_IRQHandler(void);
void Setup_TIM(uint32_t, uint32_t);
void Setup_GPIO(void);

int main(void){

    Setup_TIM(5, 20);

    Setup_GPIO();

    while(1){}

    return 0;
}

void Setup_TIM(uint32_t x, uint32_t y){

	/* Setup TIM3 */
	RCC->APBENR1 |= (1U << 1);

	TIM3->CR1 = 0;
	TIM3->CR1 |= (1U << 7);
	TIM3->CNT = 0;

	TIM3->PSC = x;
	TIM3->ARR = y;

	TIM3->DIER |= (1U << 0);
	TIM3->CR1 |= (1U << 0);

	NVIC_SetPriority(TIM3_IRQn, 1);
	NVIC_EnableIRQ(TIM3_IRQn);
}

void Setup_GPIO(void){

    // SSD SETUP
    RCC -> IOPENR |= (7U << 0); //Enabling GPIOA, GPIOB, GPIOC

    GPIOA -> MODER &= ~(15U << 2 * 0); // Clearing the bits for PA0-PA1
    GPIOA -> MODER |= (5U << 2 * 0);   // Setupping the bits for PA0-PA1

    GPIOA -> MODER &= ~(15U << 2 * 4); // Clearing the bits for PA4-PA5
    GPIOA -> MODER |= (5U << 2 * 4);   // Setupping the bits for PA4-PA5

    GPIOA -> MODER &= ~(15U << 2 * 6); // Clearing the bits for PA6-PA7
    GPIOA -> MODER |= (5U << 2 * 6);   // Setupping the bits for PA6-PA7

    GPIOA -> MODER &= ~(15U << 2 * 11); // Clearing the bits for PA11-PA12
    GPIOA -> MODER |= (5U << 2 * 11);   // Setupping the bits for PA11-PA12

    GPIOB -> MODER &= ~(3U << 2 * 3);  // Clearing the bits for PB3
    GPIOB -> MODER |= (1U << 2 * 3);   // Setupping the bits for PB3

    GPIOB -> MODER &= ~(15U << 2 * 4); // Clearing the bits for PB4-PB5
    GPIOB -> MODER |= (5U << 2 * 4);   // Setupping the bits for PB4-PB5

    GPIOB -> MODER &= ~(15U << 2 * 8); // Clearing the bits for PB8-PB9
    GPIOB -> MODER |= (5U << 2 * 8);   // Setupping the bits for PB8-PB9

    GPIOB -> MODER &= ~(3U << 2 * 2);  // Clearing the bits in PB2 for button
	GPIOB -> MODER |= (1U << 2 * 2);   // Setupping the bits for PB2 button


	GPIOC -> MODER &= ~(3U << 2 * 6); // Clearing the bits for PC6
	GPIOC -> MODER |= (1U << 2 * 6);  // Setupping the bits for on-board LED PC6

    // SET KEY PAD AS AN OUTPUT -ROW

    GPIOB -> MODER &= ~(3U << 2 * 8); // Clearing the bits for R1 -PB8
    GPIOB -> MODER |= (1U << 2 * 8);   // Setupping the bits for R1

    GPIOB -> MODER &= ~(3U << 2 * 2); // Clearing the bits for R2 -PB2
    GPIOB -> MODER |= (1U << 2 * 2);   // Setupping the bits for R2

    GPIOB -> MODER &= ~(3U << 2 * 0); // Clearing the bits for R3 -PB0
    GPIOB -> MODER |= (1U << 2 * 0);   // Setupping the bits for R3

    GPIOA -> MODER &= ~(3U << 2 * 9); // Clearing the bits for R4 -PA9
    GPIOA -> MODER |= (1U << 2 * 9);   // Setupping the bits for R4

    // SET KEY PAD AS AN INPUT -COLUMN

    GPIOA -> MODER &= ~(3U << 2 * 10); // Clearing the bits for C1 -PA10
    GPIOA -> PUPDR |= (2U << 2 * 10);   // Pull-Down mode for C1

    GPIOB -> MODER &= ~(3U << 2 * 1); // Clearing the bits for C2 -PB1
    GPIOB -> PUPDR |= (2U << 2 * 1);   // Pull-Down mode for C2

    GPIOA -> MODER &= ~(3U << 2 * 15); // Clearing the bits for C3 -PA15
    GPIOA -> PUPDR |= (2U << 2 * 15);   // Pull-Down mode for C3

    GPIOB -> MODER &= ~(3U << 2 * 7); // Clearing the bits for C4 -PB7
    GPIOB -> PUPDR |= (2U << 2 * 7);   // Pull-Down mode for C4

    // SETUP INTERRUPTS FOR INPUTS
    EXTI -> EXTICR[2] |= (0U << 8 * 2); // PA10
    EXTI -> EXTICR[0] |= (1U << 8 * 1); // PB1
    EXTI -> EXTICR[3] |= (0U << 8 * 3); // PA15
    EXTI -> EXTICR[1] |= (1U << 8 * 3); // PB7

    //RISING EDGE
    EXTI -> RTSR1 |= (1U << 10);
    EXTI -> RTSR1 |= (1U << 1);
    EXTI -> RTSR1 |= (1U << 15);
    EXTI -> RTSR1 |= (1U << 7);

    // MASK
    EXTI -> IMR1 |= (1U << 10);
    EXTI -> IMR1 |= (1U << 1);
    EXTI -> IMR1 |= (1U << 15);
    EXTI -> IMR1 |= (1U << 7);

    // NVIC
    NVIC_SetPriority(EXTI0_1_IRQn, 0);
    NVIC_EnableIRQ(EXTI0_1_IRQn);

    NVIC_SetPriority(EXTI4_15_IRQn, 0);
    NVIC_EnableIRQ(EXTI4_15_IRQn);

    setRowsKeypad();

    clearSSD();
}

void set_ComputationValues(void)
{
    First_Digit_Value = 0;
    Second_Digit_Value = 0;
    Third_Digit_Value = 0;
    Fourth_Digit_Value = 0;
    press_times = 0;
    Overflow_Flag = 0;
    Invalid_Flag = 0;
}
float Last_Number(void)
{
	if(Minus_Flag == 0)
	return (1000 * First_Digit_Value + 100 * Second_Digit_Value +
			10 * Third_Digit_Value + 1 * Fourth_Digit_Value);
	else
	{
		return -(1000 * First_Digit_Value + 100 * Second_Digit_Value +
					10 * Third_Digit_Value + 1 * Fourth_Digit_Value);
	}
}
// Pozitif ve Küsüratlı sayılarda çalışırken ekrana yazmadan önce digit değerlerini ayarlama.
void swap_Digit_Positive_Values(float number)
{
    int temp = 0;
    // Sayı bir basamaklıysa
    if(number == 1)
    {
        // 1. and 4. digit swap
        temp = Fourth_Digit_Value;
        Fourth_Digit_Value = First_Digit_Value;
        First_Digit_Value = temp;

        // 2. and 4. digit swap
        temp = Fourth_Digit_Value;
        Fourth_Digit_Value = Second_Digit_Value;
        Second_Digit_Value = temp;

        // 3. and 4. digit swap
        temp = Fourth_Digit_Value;
        Fourth_Digit_Value = Third_Digit_Value;
        Third_Digit_Value = temp;
    }
    // Sayı iki basamaklıysa
    else if(number == 2)
    {
        // 1. and 3. digit swap
        temp = Third_Digit_Value;
        Third_Digit_Value = First_Digit_Value;
        First_Digit_Value = temp;

        // 2. and 4. digit swap
        temp = Fourth_Digit_Value;
        Fourth_Digit_Value = Second_Digit_Value;
        Second_Digit_Value = temp;
    }
    // Sayı üç basamaklıysa
    else
    {
        // 1. and 4. digit swap
        temp = Fourth_Digit_Value;
        Fourth_Digit_Value = First_Digit_Value;
        First_Digit_Value = temp;

        // 1. and 2. digit swap
        temp = Second_Digit_Value;
        Second_Digit_Value = First_Digit_Value;
        First_Digit_Value = temp;

        // 2. and 3. digit swap
        temp = Third_Digit_Value;
        Third_Digit_Value = Second_Digit_Value;
        Second_Digit_Value = temp;
    }

}
// Negatif ve Küsüratlı sayılarda çalışırken ekrana yazmadan önce digit değerlerini ayarlama.
void swap_Digit_Negative_Values(int number)
{
    int temp = 0;
    // Sayı 1 basamaklıysa
    if(number == 1)
    {
        // 2. and 4. digit swap
        temp = Fourth_Digit_Value;
        Fourth_Digit_Value = Second_Digit_Value;
        Second_Digit_Value = temp;

        // 3. and 4. digit swap
        temp = Fourth_Digit_Value;
        Fourth_Digit_Value = Third_Digit_Value;
        Third_Digit_Value = temp;

    }
    // Sayı 2 basamaklıysa
    else
    {
        // 2. and 3. digit swap
        temp = Third_Digit_Value;
        Third_Digit_Value = Second_Digit_Value;
        Second_Digit_Value = temp;

        // 3. and 4. digit swap
        temp = Fourth_Digit_Value;
        Fourth_Digit_Value = Third_Digit_Value;
        Third_Digit_Value = temp;
    }

}

// Sayıyı analiz etme fonksiyonu. counter bu fonksiyonda inceleniyor.
void Analyze_Number(void)
{
    // Sayı pozitifse
    if(counter >= 0 && counter < 10000)
    {
        // Last_Number'ı bulmak için başta değer ataması yapıldı.
    	First_Digit_Value  = counter / 1000;
    	Second_Digit_Value = (counter - First_Digit_Value * 1000) / 100;
    	Third_Digit_Value  = (counter - First_Digit_Value * 1000 - Second_Digit_Value * 100) / 10;
    	Fourth_Digit_Value = counter - First_Digit_Value * 1000 - Second_Digit_Value * 100 - Third_Digit_Value * 10;

        //Floating Point Number System koşulu Bkz: counter = 373.23 Last_Number = 373
        if(counter > Last_Number() && Last_Number() < 1000)
        {
            int Float_Number = (counter - Last_Number()) * 1000;

        	int One_Digit_Number   = 1;
            int Two_Digit_Number   = 2;
            int Three_Digit_Number = 3;

            Floating_Flag = 1;
            // 1 basamaklı küsüratlı sayı yazdırma algoritması
            if(First_Digit_Value == 0 && Second_Digit_Value == 0 && Third_Digit_Value == 0 && Fourth_Digit_Value != 0)
            {
            	First_Digit_Value  = Float_Number / 100;
                Second_Digit_Value = (Float_Number - First_Digit_Value * 100) / 10;
                Third_Digit_Value  = Float_Number - First_Digit_Value * 100 - Second_Digit_Value * 10;
                swap_Digit_Positive_Values(One_Digit_Number);
                press_times = 1;
            }
            // 2 basamaklı küsüratlı sayı yazdırma algoritması
            else if(First_Digit_Value == 0 && Second_Digit_Value == 0 && Third_Digit_Value != 0 && Fourth_Digit_Value != 0)
            {
                First_Digit_Value  = Float_Number / 100;
                Second_Digit_Value = (Float_Number - First_Digit_Value * 100) / 10;
                swap_Digit_Positive_Values(Two_Digit_Number);
                press_times = 2;
            }
            // 3 basamaklı küsüratlı sayı yazdırma algoritması
            else if (First_Digit_Value == 0 && Second_Digit_Value != 0 && Third_Digit_Value != 0 && Fourth_Digit_Value != 0)
            {
                First_Digit_Value  = Float_Number / 100;
                swap_Digit_Positive_Values(Three_Digit_Number);
                press_times = 3;
            }
            // Sayı 1'den küçükse Bkz: 0.831
            else
            {
            	First_Digit_Value  = Float_Number / 100;
            	Second_Digit_Value = (Float_Number - First_Digit_Value * 100) / 10;
            	Third_Digit_Value  = Float_Number - First_Digit_Value * 100 - Second_Digit_Value * 10;
            	swap_Digit_Positive_Values(One_Digit_Number);
            	press_times = 1;
            }

        }
        // Floating Number yoksa normal işlemine devam etsin
        else
        {
            if (First_Digit_Value != 0) press_times = 4;
            else if (Second_Digit_Value != 0) press_times = 3;
            else if (Third_Digit_Value != 0) press_times = 2;
            else press_times = 1;
        }
    }
    // Negatif Sayılarla Çalışma algoritması
    else if (counter < 0 && counter > -1000)
    {
        //Analyze Number'da eksili yazım için minus_flag 1 yapıldı.
        Minus_Flag = 1;
        // Last_Number'ı bulmak için başta değer ataması yapıldı.
        counter = -counter;
    	First_Digit_Value  = counter / 1000;
    	Second_Digit_Value = (counter - First_Digit_Value * 1000) / 100;
    	Third_Digit_Value  = (counter - First_Digit_Value * 1000 - Second_Digit_Value * 100) / 10;
    	Fourth_Digit_Value = counter - First_Digit_Value * 1000 - Second_Digit_Value * 100 - Third_Digit_Value * 10;
    	counter = -counter;
        // Floating Point Number System koşulu Bkz: counter = -373.23 Last_Number = -373
        if(counter < Last_Number())
        {
        	// Scientific modda 0 küsüratlı sayı yazdırma. Bkz: -0.88
        	if(counter > -1)
        		Float_Number = -counter * 100;
        	else
        		Float_Number = (Last_Number() - counter) * 1000;

        	float One_Digit_Number = 1;
            float Two_Digit_Number = 2;

            Floating_Flag = 1;
            // 1 basamaklı küsüratlı negatif sayı yazdırma algoritması
            if(Second_Digit_Value == 0 && Third_Digit_Value == 0 && Fourth_Digit_Value != 0)
            {
                Second_Digit_Value = Float_Number / 100;
                Third_Digit_Value  = (Float_Number - Second_Digit_Value * 100) / 10;
                swap_Digit_Negative_Values(One_Digit_Number);
                press_times = 1;
            }
            // 2 basamaklı küsüratlı negatif sayı yazdırma algoritması
            else if(Second_Digit_Value == 0 && Third_Digit_Value != 0 && Fourth_Digit_Value != 0)
            {
                Second_Digit_Value = Float_Number / 100;
                swap_Digit_Negative_Values(Two_Digit_Number);
                press_times = 2;
            }
            // Sayı -1'den büyükse Bkz: -0.78
            else
            {
            	Second_Digit_Value  = Float_Number / 100;
            	Third_Digit_Value = (Float_Number - Second_Digit_Value * 100) / 10;
            	Fourth_Digit_Value  = Float_Number - Second_Digit_Value * 100 - Third_Digit_Value * 10;
            	press_times = 1;
            }
        }
        // Floating number yoksa normal yazıma devam etsin.
        else
        {
            if (Second_Digit_Value != 0) press_times = 3;
            else if (Third_Digit_Value != 0) press_times = 2;
            else press_times = 1;
        }
    }
    // Invalid Statement
    else if (counter == -log(0))
    {	//Bu fonksiyonda zaten Minus ve Floating flag 0'a eşit.
    	//counter infinity ise invalid yazdırması lazım. O yüzden bu koşulu atadım.
    	Invalid_Flag = 1;
    }
    // Overflow Statement
    else
    {
    	// Bu fonksiyonda zaten Minus ve Floating flag 0'a eşit.
    	Overflow_Flag = 1;
    }
}

void Addition(void)
{
	//Eğer scientific mode açık değilse normal işlemine devam etsin.
	if(Scientific_Flag == 0)
	{
		if(Enter_Flag == 1)
		{
			// Eğer küsüratlı şekilde çalışılıyorsa burada herhangi bir şey yapılmasına gerek yok.
			// Enter fonksiyonunda bölünme işlemi yapılacak.
			if(Floating_Flag == 0)
			counter = Last_Number();

			Addition_Flag = 1;
			Enter_Flag = 0;
		}
		// Denklemin devamlılığına sağlamak için sıfıra basılana kadar ekrana girilen sayıları topluyor.
		else
		{
			counter += Last_Number();
			Addition_Flag = 1;

		}

		// Gösterimde eksi ve küsürat işareti gözükmesin diye Minus_Flag'i sıfıra çektim.
		Addition_Flag = 1;
		Minus_Flag = 0;
		Floating_Flag = 0;
	    set_ComputationValues();
	}
	//Scientific mod açıksa burada bir değer hesaplamasına gerek yok. Sadece Addition_Flag'i açsın.
	else
	{
		// Gösterimde eksi işareti gözükmesin diye Minus_Flag'i sıfıra çektim.
		Addition_Flag = 1;
		Minus_Flag = 0;
		Floating_Flag = 0;
	    set_ComputationValues();
	}
}

void Subtraction(void)
{
	//Eğer scientific mode açık değilse normal işlemine devam etsin.
	if(Scientific_Flag == 0)
	{
		if(Subtraction_Flag == 0 && Addition_Flag == 0 && Floating_Flag == 0)
		{
			counter = Last_Number();
			Subtraction_Flag = 1;
		}
		else if (Subtraction_Flag == 1 && Addition_Flag == 0 && Floating_Flag == 0)
		{
			counter -= Last_Number();
		}
		else if (Subtraction_Flag == 0 && Addition_Flag == 1 && Floating_Flag == 0)
		{
			counter += Last_Number();
			Subtraction_Flag = 1;
			Addition_Flag = 0;
		}
		// Floating_Flag = 1 ise
		else
		{
			Subtraction_Flag = 1;
			Floating_Flag = 0;
		}

		Subtraction_Flag = 1;
		// Gösterimde eksi işareti gözükmesin diye Minus_Flag'i sıfıra çektim.
		Minus_Flag = 0;
		set_ComputationValues();
	}
	//Scientific mod açıksa burada bir değer hesaplamasına gerek yok. Sadece Subtraction_Flag'i açsın.
	else
	{
		Subtraction_Flag = 1;
		Minus_Flag = 0;
		set_ComputationValues();
	}
}

void Multiplication(void)
{
	//Eğer scientific mode açık değilse normal işlemine devam etsin.
	if(Scientific_Flag == 0)
	{
		Multiplication_Flag = 1;
		// Eğer küsüratlı şekilde çalışılıyorsa burada herhangi bir şey yapılmasına gerek yok.
		// Enter fonksiyonunda çarpma işlemi yapılacak.
		if(Floating_Flag == 0)
		counter = Last_Number();
		set_ComputationValues();
		// Gösterimde eksi ve decimal işareti gözükmesin diye Minus_Flag'i sıfıra çektim.
		Minus_Flag = 0;
		Floating_Flag = 0;
	}
	//Scientific mod açıksa burada bir değer hesaplamasına gerek yok. Sadece Multiplication_Flag'i açsın.
	else
	{
		Multiplication_Flag = 1;
		set_ComputationValues();
		// Gösterimde eksi ve decimal işareti gözükmesin diye Minus_Flag'i sıfıra çektim.
		Minus_Flag = 0;
		Floating_Flag = 0;
	}
}

void Division(void)
{
	if(Scientific_Flag == 0)
	{
		Division_Flag = 1;
		// Eğer küsüratlı şekilde çalışılıyorsa burada herhangi bir şey yapılmasına gerek yok.
		// Enter fonksiyonunda bölünme işlemi yapılacak.
		if(Floating_Flag == 0)
		counter = Last_Number();

		set_ComputationValues();
		// Gösterimde eksi ve decimal işareti gözükmesin diye Minus_Flag'i sıfıra çektim.
		Minus_Flag = 0;
		Floating_Flag = 0;
	}
	//Scientific mod açıksa burada bir değer hesaplamasına gerek yok. Sadece Division_Flag'i açsın.
	else
	{
		Division_Flag = 1;
		set_ComputationValues();
		// Gösterimde eksi ve decimal işareti gözükmesin diye Minus_Flag'i sıfıra çektim.
		Minus_Flag = 0;
		Floating_Flag = 0;
	}
}

void Scientific_mode(void)
{
	set_ComputationValues();
	// Gösterimde eksi ve decimal işareti gözükmesin diye Minus_Flag'i sıfıra çektim.
	Minus_Flag = 0;
	Floating_Flag = 0;
	Scientific_Flag++;
	// Pi modu
	if(Scientific_Flag == 3)
	{
		Scientific_Flag = 0;
		counter = 3.141;
		Analyze_Number();
	}
}

void Enter(void)
{
	//Toplama
    if (Addition_Flag == 1 && Scientific_Flag == 0)
    {
        Enter_Flag = 1;
    	Addition_Flag = 0;
        counter += Last_Number();
        Analyze_Number();
    }
    //log
    else if (Addition_Flag == 1 && Scientific_Flag == 1)
    {
    	Enter_Flag = 1;
    	Addition_Flag = 0;
    	Scientific_Flag = 0;
    	counter = log10(Last_Number());
    	Analyze_Number();
    }
    //sin
    else if (Addition_Flag == 1 && Scientific_Flag == 2)
    {
        Enter_Flag = 1;
    	Addition_Flag = 0;
        Scientific_Flag = 0;
        counter = sin(Last_Number());
        Analyze_Number();
    }
    //çıkarma
    else if (Subtraction_Flag == 1 && Scientific_Flag == 0)
    {
    	Enter_Flag = 1;
    	Subtraction_Flag = 0;
    	counter -= Last_Number();
    	Analyze_Number();
    }
    //ln
    else if (Subtraction_Flag == 1 && Scientific_Flag == 1)
    {
    	Enter_Flag = 1;
    	Subtraction_Flag = 0;
    	Scientific_Flag = 0;
    	counter = log(Last_Number());
    	Analyze_Number();
    }
    //cos
    else if (Subtraction_Flag == 1 && Scientific_Flag == 2)
    {
    	Enter_Flag = 1;
    	Subtraction_Flag = 0;
    	Scientific_Flag = 0;
    	counter = cos(Last_Number());
    	Analyze_Number();
    }
    //çarpma
    else if(Multiplication_Flag == 1 && Scientific_Flag == 0)
    {
    	Enter_Flag = 1;
    	Multiplication_Flag = 0;
    	counter = counter * Last_Number();
    	Analyze_Number();
    }
    //sqrt
    else if(Multiplication_Flag == 1 && Scientific_Flag == 1)
    {
    	Enter_Flag = 1;
    	Multiplication_Flag = 0;
    	Scientific_Flag = 0;
    	counter = sqrt(Last_Number());
    	Analyze_Number();
    }
    //tan
    else if(Multiplication_Flag == 1 && Scientific_Flag == 2)
    {
    	Enter_Flag = 1;
    	Multiplication_Flag = 0;
    	Scientific_Flag = 0;
    	counter = tan(Last_Number());
    	Analyze_Number();
    }
    //bölme
    else if(Division_Flag == 1 && Scientific_Flag == 0)
    {
    	Enter_Flag = 1;
    	Division_Flag = 0;
    	counter = counter / Last_Number();
    	Analyze_Number();
    }
    //x^2
    else if(Division_Flag == 1 && Scientific_Flag == 1)
    {
    	Enter_Flag = 1;
    	Division_Flag = 0;
    	Scientific_Flag = 0;
    	counter = Last_Number() * Last_Number();
    	Analyze_Number();
    }
    //cot
    else if(Division_Flag == 1 && Scientific_Flag == 2)
    {
    	Enter_Flag = 1;
    	Division_Flag = 0;
    	Scientific_Flag = 0;
    	counter = cos(Last_Number()) / sin(Last_Number());
    	Analyze_Number();
    }
    //pi Bkz: 3.14
    else
    {
    	Enter_Flag = 1;
    	Scientific_Flag = 0;

    }
}

void Press(uint8_t press_times, int num_value)
{
    switch(press_times)
    {
    	// Ekrana bir basamaklı sayıyı gösterme
        case 1:
            Fourth_Digit_Value = num_value;
            break;
        // Ekrana İki basamaklı sayıyı gösterme
        case 2:
            Third_Digit_Value = Fourth_Digit_Value;
            Fourth_Digit_Value = num_value;
            break;
        // Ekrana üç basamaklı sayıyı gösterme
        case 3:
            Second_Digit_Value = Third_Digit_Value;
            Third_Digit_Value = Fourth_Digit_Value;
            Fourth_Digit_Value = num_value;
            break;
        // Ekrana dört basamaklı sayıyı gösterme
        case 4:
            First_Digit_Value = Second_Digit_Value;
            Second_Digit_Value = Third_Digit_Value;
            Third_Digit_Value = Fourth_Digit_Value;
            Fourth_Digit_Value = num_value;
            break;
        default:
            break;
    }
}

void Display_Number(void)
{
	//10 saniye boyunca hiçbir tuşa basılmazsa IDLE_State'e git ve her şeyi sıfırla.
	if(IDLE_Transition > 500000) {Minus_Flag = 0; Floating_Flag = 0; IDLE_Flag = 1; set_ComputationValues(); counter = 0;}

	if(IDLE_Flag == 0)
	{
	    // Pozitif ve küsüratsız sayı yazdırma algoritması
	   	if(Minus_Flag == 0 && Floating_Flag == 0)
	   	{
	       	if(Overflow_Flag == 0 && Invalid_Flag == 0)
	       	{
	       		// Hiçbir sayı döndürme
	       		if(press_times == 0)
	       		{
	       		     prevent_bounce = 0;
	       		     Digit_All_Off();
	       		     waiting_time++;
	       		}
	       		// Bir basamaklı sayı döndür
	       		else if(press_times == 1)
	       		{
	       			 IDLE_Transition++;
	       		     prevent_bounce = 0;
	       		     Digit_All_Off();
	       		     Digit_Four_ON();
	       		     setSSD((int) Fourth_Digit_Value);
	       		}
	       		// İki basamaklı sayı döndür
	       		else if(press_times == 2)
	       		{
	       			 IDLE_Transition = IDLE_Transition + 20;
	       		     prevent_bounce = 0;
	       		     Digit_All_Off();
	       		     Digit_Three_ON();
	       		     setSSD((int)Third_Digit_Value);
	       		     delay(200);
	       		     Digit_Three_Off();
	       		     Digit_Four_ON();
	       		     setSSD((int)Fourth_Digit_Value);
	       		     delay(200);
	       		}
	       		// Üç basamaklı sayı döndür
	       		else if(press_times == 3)
	       		{
	       			 IDLE_Transition = IDLE_Transition + 25;
	       		     prevent_bounce = 0;
	       		     Digit_All_Off();
	       		     Digit_Two_ON();
	       		     setSSD((int)Second_Digit_Value);
	       		     delay(200);
	       		     Digit_Two_Off();
	       		     Digit_Three_ON();
	       		     setSSD((int)Third_Digit_Value);
	       		     delay(200);
	       		     Digit_Three_Off();
	       		     Digit_Four_ON();
	       		     setSSD((int)Fourth_Digit_Value);
	       		     delay(200);
	       		 }
	       		// Dört basamaklı sayı döndür
	       		else if(press_times == 4)
	       		{
 IDLE_Transition = IDLE_Transition + 30;


	       		// Tuşa 4'ten fazla basıldığında yine 4 basamaklı sayı döndür.
	       		else
	       		{
	       		     prevent_bounce = 0;
	       		     press_times = 4;
	       		}
	       	}
	       	// OverFlow Yazımı Bkz: ouFL
	       	else if(Overflow_Flag == 1)
	       	{
	       		IDLE_Transition = IDLE_Transition + 30;
	       		prevent_bounce = 0;
	       		Digit_All_Off();
	       		Digit_One_ON();
	       		setSSD(O_Letter);
	  		    delay(200);
	  		    Digit_One_Off();
	  		    Digit_Two_ON();
	  		    setSSD(u_Letter);
	  		    delay(200);
	  		    Digit_Two_Off();
	  		    Digit_Three_ON();
	  		    setSSD(F_Letter);
	  		    delay(200);
	  		    Digit_Three_Off();
	  		    Digit_Four_ON();
	  		    setSSD(L_Letter);
	  		    delay(200);
	       	}
	       	//Invalid Yazımı Bkz: InV
	       	else
	       	{
	       		IDLE_Transition = IDLE_Transition + 25;
	       		prevent_bounce = 0;
	       		Digit_All_Off();
	       		Digit_Two_ON();
	       		setSSD(1);
	       		delay(200);
	       		Digit_Two_Off();
	       		Digit_Three_ON();
	       		setSSD(N_Letter);
	       		delay(200);
	       		Digit_Three_Off();
	       		Digit_Four_ON();
	       		setSSD(V_Letter);
	       		delay(200);
	       	}
	   	}
	   	// Negatif ve küsüratsız sayı yazdırma algoritması
	   	else if (Minus_Flag == 1 && Floating_Flag == 0)
	   	{
	   		// Hiçbir sayı döndürme
	       	if(press_times == 0)
	       	{
	       		prevent_bounce = 0;
	       	    Digit_All_Off();
	       	}
	       	// Bir basamaklı sayı döndür
	       	else if(press_times == 1)
	        {
	       		IDLE_Transition = IDLE_Transition + 20;
	       		prevent_bounce = 0;
	            Digit_All_Off();
	            Digit_Three_ON();
	            setSSD(minus);
	            delay(200);
	            Digit_Three_Off();
	            Digit_Four_ON();
	            setSSD((int)Fourth_Digit_Value);
	            delay(200);
	        }
	       	// İki basamaklı sayı döndür
	        else if(press_times == 2)
	        {
	        	IDLE_Transition = IDLE_Transition + 25;
	           	prevent_bounce = 0;
	            Digit_All_Off();
	            Digit_Two_ON();
	            setSSD(minus);
	            delay(200);
	            Digit_Two_Off();
	            Digit_Three_ON();
	            setSSD((int)Third_Digit_Value);
	            delay(200);
	            Digit_Three_Off();
	            Digit_Four_ON();
	            setSSD((int)Fourth_Digit_Value);
	            delay(200);
	        }
	       	// Üç basamaklı sayı döndür
	        else if(press_times == 3)
	        {
	        	IDLE_Transition = IDLE_Transition + 30;
	           	prevent_bounce = 0;
	            Digit_All_Off();
	            Digit_One_ON();
	            setSSD(minus);
	            delay(200);
	            Digit_One_Off();
	            Digit_Two_ON();
	            setSSD((int)Second_Digit_Value);
	            delay(200);
	            Digit_Two_Off();
	            Digit_Three_ON();
	            setSSD((int)Third_Digit_Value);
	            delay(200);
	            Digit_Three_Off();
	            Digit_Four_ON();
	            setSSD((int)Fourth_Digit_Value);
	            delay(200);
	        }
	       	// Press_times değişkeni üzerinden digit sayılarına bakıp yönlendirdim.
	        else
	        {
	           	prevent_bounce = 0;
	            if (counter < -99) press_times = 3;
	            else if (counter < -9) press_times = 2;
	            else press_times = 1;
	        }
	   	}
	    // Pozitif ve küsüratlı sayı yazdırma algoritması
	    else if(Minus_Flag == 0 && Floating_Flag == 1)
	    {
	    	// Hiçbir sayı döndürme
	       	if(press_times == 0)
	       	{
	       		prevent_bounce = 0;
	       	    Digit_All_Off();
	       	}
	       	// 1 basamaklı sayı
	       	else if(press_times == 1)
	        {
	       		IDLE_Transition = IDLE_Transition + 35;
	           	prevent_bounce = 0;
	            Digit_All_Off();
	            Digit_One_ON();
	            setSSD((int)First_Digit_Value);
	            delay(200);
	            setSSD(decimal);
	            delay(200);
	            Digit_One_Off();
	            Digit_Two_ON();
	            setSSD((int)Second_Digit_Value);
	            delay(200);
	            Digit_Two_Off();
	            Digit_Three_ON();
	            setSSD((int)Third_Digit_Value);
	            delay(200);
	            Digit_Three_Off();
	            Digit_Four_ON();
	            setSSD((int)Fourth_Digit_Value);
	            delay(200);
	        }
	        // 2 basamaklı sayı
	        else if(press_times == 2)
	        {
	        	IDLE_Transition = IDLE_Transition + 35;
	           	prevent_bounce = 0;
	            Digit_All_Off();
	            Digit_One_ON();
	            setSSD((int)First_Digit_Value);
	            delay(200);
	            Digit_One_Off();
	            Digit_Two_ON();
	            setSSD((int)Second_Digit_Value);
	            delay(200);
	            setSSD(decimal);
	            delay(200);
	            Digit_Two_Off();
	            Digit_Three_ON();
	            setSSD((int)Third_Digit_Value);
	            delay(200);
	            Digit_Three_Off();
	            Digit_Four_ON();
	            setSSD((int)Fourth_Digit_Value);
	            delay(200);
	        }
	           // 3 basamaklı sayı
	        else if(press_times == 3)
	        {
	        	IDLE_Transition = IDLE_Transition + 35;
	           	prevent_bounce = 0;
	           	Digit_All_Off();
	           	Digit_One_ON();
	           	setSSD((int)First_Digit_Value);
	           	delay(200);
	           	Digit_One_Off();
	           	Digit_Two_ON();
	           	setSSD((int)Second_Digit_Value);
	           	delay(200);
	           	Digit_Two_Off();
	           	Digit_Three_ON();
	           	setSSD((int)Third_Digit_Value);
	           	delay(200);
	           	setSSD(decimal);
	           	delay(200);
	           	Digit_Three_Off();
	           	Digit_Four_ON();
	           	setSSD((int)Fourth_Digit_Value);
	           	delay(200);
	        }
	       	// Press_times değişkeni üzerinden digit sayılarına bakıp yönlendirdim.
	        else
	        {
	           	prevent_bounce = 0;
	            if (counter < 10) press_times = 1;
	            else if (counter < 100) press_times = 2;
	            else press_times = 3;
	        }
	    }
	    // Negatif ve küsüratlı sayı yazdırma algoritması
	    else
	    {
	        // 1 basamaklı sayı
	        if(press_times == 1)
	        {
	        	IDLE_Transition = IDLE_Transition + 35;
	           	prevent_bounce = 0;
	            Digit_All_Off();
	            Digit_One_ON();
	            setSSD(minus);
	            delay(200);
	            Digit_One_Off();
	            Digit_Two_ON();
	            setSSD((int)Second_Digit_Value);
	            delay(200);
	            setSSD(decimal);
	            delay(200);
	            Digit_Two_Off();
	            Digit_Three_ON();
	            setSSD((int)Third_Digit_Value);
	            delay(200);
	            Digit_Three_Off();
	            Digit_Four_ON();
	            setSSD((int)Fourth_Digit_Value);
	            delay(200);
	        }
	        // 2 basamaklı sayı
	        else if(press_times == 2)
	        {
	        	IDLE_Transition = IDLE_Transition + 35;
	           	prevent_bounce = 0;
	            Digit_All_Off();
	            Digit_One_ON();
	            setSSD(minus);
	            delay(200);
	            Digit_One_Off();
	            Digit_Two_ON();
	            setSSD((int)Second_Digit_Value);
	            delay(200);
	            Digit_Two_Off();
	            Digit_Three_ON();
	            setSSD((int)Third_Digit_Value);
	            delay(200);
	            setSSD(decimal);
	            delay(200);
	            Digit_Three_Off();
	            Digit_Four_ON();
	            setSSD((int)Fourth_Digit_Value);
	            delay(200);
	        }
	        else
	        {
	        	prevent_bounce = 0;
	            if (counter > -10) press_times = 1;
	            else press_times = 2;
	        }
	     }
		}
		//IDLE State
	    else
	    {
	        prevent_bounce = 0;
	        Digit_All_Off();
	        Digit_One_ON();
	        setSSD(1);
	        delay(200);
	        Digit_One_Off();
	        Digit_Two_ON();
	        setSSD(7);
	        delay(200);
	        Digit_Two_Off();
	        Digit_Three_ON();
	        setSSD(3);
	        delay(200);
	        Digit_Three_Off();
	        Digit_Four_ON();
	        setSSD(0);
	        delay(200);
	    }
}

void TIM3_IRQHandler(void)
{
	// Sayıyı bu fonksiyonda yazdırıyorum.
	Display_Number();
	// Interrupt flagleri sayesinde bütün yükü TIMER handler'ına verdim.
	if(AdditionFunc_Flag == 1) {Addition(); AdditionFunc_Flag = 0;}
	if(SubtractionFunc_Flag == 1) {Subtraction(); SubtractionFunc_Flag = 0;}
	if(MultiplicationFunc_Flag == 1) {Multiplication(); MultiplicationFunc_Flag = 0;}
	if(DivisionFunc_Flag == 1) {Division(); DivisionFunc_Flag = 0;}
	if(EnterFunc_Flag == 1) {Enter(); EnterFunc_Flag = 0;}
	if(ScientificFunc_Flag == 1) {Scientific_mode(); ScientificFunc_Flag = 0;}

	TIM3->SR &= ~(1U << 0);
}

/* Press_times, display fonksiyonunda girilen sayının kaç basamaklı olduğunu anlayabilmek için kullanıldı.
 * Digit_All_Off(), flickering'i engellemek için kullanıldı.
 * prevent_bounce, bouncing'i engellemek için kullanıldı.
 * IDLE_Flag, tuşa basıldığı zaman IDLE State'den çıkılmasını sağlar.
 * IDLE_Transition ise 10 saniyeye kadar basılmadığındaki süreyi tutan zamanı sıfırlar.
 */
void EXTI0_1_IRQHandler(void)
{
    // interrupts from PB1
    clearRowsKeypad();
    GPIOB -> ODR ^= (1U << 8);  //PB8
    if((GPIOB ->IDR & (1U << 1)) && prevent_bounce == 0)
    {delay(10000); press_times++; Press(press_times, 2); Digit_All_Off(); prevent_bounce = 1; IDLE_Flag = 0; IDLE_Transition = 0;}
    GPIOB -> ODR ^= (1U << 8);  //PB8

    GPIOB -> ODR ^= (1U << 2);  //PB2
    if((GPIOB ->IDR & (1U << 1)) && prevent_bounce == 0)
    {delay(10000); press_times++; Press(press_times, 5); Digit_All_Off(); prevent_bounce = 1; IDLE_Flag = 0; IDLE_Transition = 0;}
    GPIOB -> ODR ^= (1U << 2);  //PB2

    GPIOB -> ODR ^= (1U << 0);  //PB0
    if((GPIOB ->IDR & (1U << 1)) && prevent_bounce == 0)
    {delay(10000); press_times++; Press(press_times, 8); Digit_All_Off(); prevent_bounce = 1; IDLE_Flag = 0; IDLE_Transition = 0;}
    GPIOB -> ODR ^= (1U << 0);  //PB0

    GPIOA -> ODR ^= (1U << 9);  //PA9
    if((GPIOB ->IDR & (1U << 1)) && prevent_bounce == 0)
    {delay(10000); press_times++; Press(press_times, 0); Digit_All_Off(); prevent_bounce = 1; IDLE_Flag = 0; IDLE_Transition = 0;}
    GPIOA -> ODR ^= (1U << 9);  //PA9

    EXTI -> RPR1 |= (1U << 1);
    setRowsKeypad();
}

void EXTI4_15_IRQHandler(void)
{
    // interrupts from PA10
    if((GPIOA ->IDR & (1U << 10)))
    {
        clearRowsKeypad();
        GPIOB -> ODR ^= (1U << 8);  //PB8
        if((GPIOA ->IDR & (1U << 10)) && prevent_bounce == 0)
        {delay(10000); press_times++; Press(press_times, 1); Digit_All_Off(); prevent_bounce = 1; IDLE_Flag = 0; IDLE_Transition = 0;}
        GPIOB -> ODR ^= (1U << 8);  //PB8

        GPIOB -> ODR ^= (1U << 2);  //PB2
        if((GPIOA ->IDR & (1U << 10)) && prevent_bounce == 0)
        {delay(10000); press_times++; Press(press_times, 4); Digit_All_Off(); prevent_bounce = 1; IDLE_Flag = 0; IDLE_Transition = 0;}
        GPIOB -> ODR ^= (1U << 2);  //PB2

        GPIOB -> ODR ^= (1U << 0);  //PB0
        if((GPIOA ->IDR & (1U << 10)) && prevent_bounce == 0)
        {delay(10000); press_times++; Press(press_times, 7); Digit_All_Off(); prevent_bounce = 1; IDLE_Flag = 0; IDLE_Transition = 0;}
        GPIOB -> ODR ^= (1U << 0);  //PB0

        GPIOA -> ODR ^= (1U << 9);  //PA9
        if((GPIOA ->IDR & (1U << 10)) && prevent_bounce == 0)
        {delay(10000); EnterFunc_Flag = 1; Digit_All_Off(); prevent_bounce = 1; IDLE_Flag = 0;} //star
        GPIOA -> ODR ^= (1U << 9);  //PA9

        EXTI -> RPR1 |= (1U << 10);
        setRowsKeypad();
    }

    // interrupts from PA15
    if((GPIOA ->IDR & (1U << 15)))
    {
        clearRowsKeypad();
        GPIOB -> ODR ^= (1U << 8);  //PB8
        if((GPIOA ->IDR & (1U << 15)) && prevent_bounce == 0)
        {delay(10000); press_times++; Press(press_times, 3); Digit_All_Off(); prevent_bounce = 1; IDLE_Flag = 0; IDLE_Transition = 0;}
        GPIOB -> ODR ^= (1U << 8);  //PB8

        GPIOB -> ODR ^= (1U << 2);  //PB2
        if((GPIOA ->IDR & (1U << 15)) && prevent_bounce == 0)
        {delay(10000); press_times++; Press(press_times, 6); Digit_All_Off(); prevent_bounce = 1; IDLE_Flag = 0; IDLE_Transition = 0;}
        GPIOB -> ODR ^= (1U << 2);  //PB2

        GPIOB -> ODR ^= (1U << 0);  //PB0
        if((GPIOA ->IDR & (1U << 15)) && prevent_bounce == 0)
        {delay(10000); press_times++; Press(press_times, 9); Digit_All_Off(); prevent_bounce = 1; IDLE_Flag = 0; IDLE_Transition = 0;}
        GPIOB -> ODR ^= (1U << 0);  //PB0

        GPIOA -> ODR ^= (1U << 9);  //PA9
        if((GPIOA ->IDR & (1U << 15)) && prevent_bounce == 0)
        {delay(10000); ScientificFunc_Flag = 1; Digit_All_Off(); prevent_bounce = 1; IDLE_Flag = 0; IDLE_Transition = 0;}
        GPIOA -> ODR ^= (1U << 9);  //PA9

        EXTI -> RPR1 |= (1U << 15);
        setRowsKeypad();
    }

    // interrupts from PB7
    if(GPIOB ->IDR & (1U << 7))
    {
        clearRowsKeypad();
        GPIOB -> ODR ^= (1U << 8);  //PB8
        if((GPIOB ->IDR & (1U << 7)) && prevent_bounce == 0)
        {delay(10000); AdditionFunc_Flag = 1; Digit_All_Off(); prevent_bounce = 1; IDLE_Flag = 0; IDLE_Transition = 0;}
        GPIOB -> ODR ^= (1U << 8);  //PB8

        GPIOB -> ODR ^= (1U << 2);  //PB2
        if((GPIOB ->IDR & (1U << 7)) && prevent_bounce == 0)
        {delay(10000); SubtractionFunc_Flag = 1; Digit_All_Off(); prevent_bounce = 1; IDLE_Flag = 0; IDLE_Transition = 0;}
        GPIOB -> ODR ^= (1U << 2);  //PB2

        GPIOB -> ODR ^= (1U << 0);  //PB0
        if((GPIOB ->IDR & (1U << 7)) && prevent_bounce == 0)
        {delay(10000); MultiplicationFunc_Flag = 1; Digit_All_Off(); prevent_bounce = 1; IDLE_Flag = 0; IDLE_Transition = 0;}
        GPIOB -> ODR ^= (1U << 0);  //PB0

        GPIOA -> ODR ^= (1U << 9);  //PA9
        if((GPIOB ->IDR & (1U << 7)) && prevent_bounce == 0)
        {delay(10000); DivisionFunc_Flag = 1; Digit_All_Off(); prevent_bounce = 1; IDLE_Flag = 0; IDLE_Transition = 0;}
        GPIOA -> ODR ^= (1U << 9);  //PA9

        EXTI -> RPR1 |= (1U << 7);
        setRowsKeypad();
    }
}


