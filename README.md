# A-Fully-Operational-Scientific-Calculator
The aim of this project is to design a fully operational scientific calculator which contains addition, subtraction, multiplication, division, scientific and trigonometric modes, negative and floating number systems, overflow and invalid statements and it will be created by using Embedded C. In order to setup the calculator, 4 digit seven segment display, 4x4 keypad and stm32g031k8 microprocessor will be used in this project.  

### Schematic Diagram

![resim](https://user-images.githubusercontent.com/44584158/110012309-75505300-7d31-11eb-91fb-ab1edb81677b.png)

### Block Diagram

![resim](https://user-images.githubusercontent.com/44584158/110012350-800ae800-7d31-11eb-8087-fb48bec9cfe1.png)

### Flow Chart

#### Main Flowchart of the Project

![resim](https://user-images.githubusercontent.com/44584158/110012444-95801200-7d31-11eb-8182-a82ee02887b0.png)

#### TIMER Handler Flowchart of the Project

![resim](https://user-images.githubusercontent.com/44584158/110012500-a0d33d80-7d31-11eb-981a-038437e1ef43.png)

#### EXTI Interrupt Handler Flowchart of the Project

![resim](https://user-images.githubusercontent.com/44584158/110012520-a761b500-7d31-11eb-8ed0-f2b6e6131c14.png)

### Parts List (w/ prices)

- 4x 330Ω resistor 0,02 TL
- 4 Digit SSD (Seven Segment Display) 7,34 TL
- 4x4 Keypad 8,12 TL
- M-M Jumpers 3,11 TL
- STM32G031K8: 120 TL
Total: 138,59 TL

### Project Setup

![resim](https://user-images.githubusercontent.com/44584158/110012620-c4968380-7d31-11eb-9ef6-ea2eb5451835.png)

It was used four 330 Ω resistor, 4 digit SSD (Seven Segment Display), 4x4 Keypad, bunch of M-M Jumpers and STM32G031K8 microprocessor in this project.

### Detailed Task List

Task 1) Turn on the SSD’s first digit. ✓
Task 2) Write all of the numbers on the first digit from 4x4 Keypad one by one. ✓
Task 3) Shift numbers sideways between digits. ✓
Task 4) Write a 4 digit number on the screen. ✓
Task 5) If the digits are already full, new number key presses should be ignored. ✓
Task 6) Add and subtract two number on the screen. ✓
Task 7) Calcutate a bunch of numbers in the same equation. ✓ 
Task 8) Write a negative number on the screen. ✓
Task 9) Add multiplication and division functions to the calculator. ✓ 
Task 10) Add Floating Number System to the calculator. ✓ 
Task 11) Add Scientific and Trigonometric modes to the calculator. ✓
Task 12) Add Overflow (ouFL) and Invalid (InV) statements to the display function. ✓ 
Task 13) When the calculator is instantiated, run the IDLE state. ✓
Task 14) If no keypad button is pressed for 10 seconds, Set all the variables to zero and run the IDLE state. ✓

### How The Algorithm Works?

![resim](https://user-images.githubusercontent.com/44584158/110012796-f576b880-7d31-11eb-8ee7-7b95362aae7e.png)

#### Main Function
Firstly, TIM and EXTI handlers are set in main function.

#### EXTI_IRQHandler
Secondly, when the keypad button is pressed, the program understands which column the button is in. After that, all the row pins set to LOW and respectively set to HIGH one by one and try to understand which row the button is in. Then, press_times variable up to one value more, (Press_times is used to figure out how many digits the number entered in the display function.) All the digits off, (this was used to avoid flickering.) IDLE_Flag set to zero, (When the button is pressed, it exits the IDLE State.) and IDLE_Transition set to zero. (IDLE_Transition variable holds the time when the button is not pressed up to 10 seconds.)

#### TIM_IRQHandler
In final, when the calculation flags are active, (i.e. additionFlag) the handler runs the calculation functions. In these functions, total_number variable changes depends on the buttons that’d been pressed. Then, the handler runs the Analyze_Number function and analyze the total_number value. According to its positive/negative or floating/normal types, the handler operates the display function. If the total_number variable is higher than 9999 or less than -999, then the handler runs the overflow statement. If the total_number variable is inf type, then the handler runs the invalid statement. After the analyzing, the handler runs the display function. If no button is pressed up to 10 seconds, the handler runs the IDLE state.

### Calculation Modes’ Methodology
In this section, Calculation modes’ logic will be examined .

#### Last_Number Function
The aim of the using this function is working with the number written on the SSD’s screen. For instance, the 4 digit seven segment display shows 2293 at the moment and when this function is called, it is equal to 2293.

#### Addition Function
In this function, Total_Number variable adds itself to the value that returns in the Last_Number function. After that, it calls Analyze_Number function. In some cases, addition function is bypassed. For example, if the scientific mode is active, (i.e. EA keypress for log) the calculator must bypasses the addition function and operates the Scientific mode.

#### Subtraction Function
In this function, Total_Number variable subtracts itself from the value that returns in the Last_Number function. After that, it calls Analyze_Number function. In some cases, subtraction function is bypassed. For example, if the scientific mode is active, (i.e. EB keypress for ln) the calculator must bypasses the subtraction function and operates the Scientific mode.

#### Multiplication Function
In this function, Total_Number variable multiplies itself to the value that returns in the Last_Number function. After that, it calls Analyze_Number function. In some cases, multiplication function is bypassed. For example, if the scientific mode is active, (i.e. EC keypress for sqrt) the calculator must bypasses the subtraction function and operates the Scientific mode.

#### Division Function
In this function, Total_Number variable divides itself to the value that returns in the Last_Number function. After that, it calls Analyze_Number function. In some cases, division function is bypassed. For example, if the scientific mode is active, (i.e. ED keypress for x^2) the calculator must bypasses the division function and operates the Scientific mode.

#### Scientific Mode
In this function, Scientific_Flag set one value more to arrange the trigonometric and scientific modes. If the flag is equal to one, then enter function operates the scientific functions. If the flag is equal to two, enter function runs the trigonometric functions. If the flag is equal to three, total_number equals to pi value. After that, it calls Analyze_Number function.

#### Enter Function
When the enter function is pressed, according to the calculation flags, (i.e. additionFlag, subtractionFlag…) It calculates the total number by using the Last_Number function. Then, it calls the Analyze_Number Function.

#### Analyze_Number Function
When Analyze_Number is called, Total_Number is examined in this function. According to its positive/negative or floating/normal types, the minus flag set to 0 or 1 and floatin flag set to 0 or 1 as well. The purpose here is to tell the display function what kind of number the total number is. It also inform the display function if the total number is higher than 9999/lower than -999 or it has a inf value. Thus, display function can be write the overflow (ouFL) or invalid (InV) symbols on the SSD’s screen.

#### Display_Number Function
This function is the calculator’s display function. The timer handler returns this function constantly for showing the calculated number or entered number. It considers three flags. IDLE_Flag, Minus_Flag and Floating_Flag. Its working logic can be explained as follows. Firstly when the IDLE_Flag is 0, if the minus flag is 0 and floating flag is 0, then it displays normal and positive value. If the minus flag is 1 and floating flag is 0, then it  displays normal and negative value. If the minus flag is 0 and floating flag is 1, then it displays fractional and positive value. Otherwise it displays fractional and positive value. In these conditions, if no button is pressed, the IDLE_Transition value will be increased constantly and when it reaches a 10 second delay, the IDLE_Flag will be set to one and the display function displays the IDLE state.

#### Challenges That’s Been Solved:
1) Shifting numbers sideways between digits
2) Adding Floating Number System to the calculator.
3) Writing a negative number on the screen. 

#### How Was This Challenges Solved:
1) A press function has been created. This function gets the number from keypad and according to the press times value, (Press_times is used to figure out how many digits the number entered in the display function.) and it swaps the digit values. For instance, when the number 5 is pressed on the keypad, press times equals to one and press function assign the five value to the Fourth_Digit_Value variable and number 5 is displayed on the SSD’s screen. After that, when another number is pressed, let’s say number 7. Press_times equals to two and press function assign the Fourth_Digit_Value to the Third_Digit_Value and assign number 7 to the Fourth_Digit_Value. Thus it can be showed two digit number on the SSD’s screen.
2) In Analyze_Number function, the floating flag set to one if total number has a fractional value. Thus, it directs the display function on this condition and runs the Floating Number System in display function.
3) In Analyze_Number function, the minus flag set to one if total number has a negative value. Thus, it directs the display function on this condition and runs the Negative Number System in display function.

#### Unsolved Difficulites:
1) Flickering has been observed in some calculation modes.
What is thought to be the root of the problem?
1) According to the written algorithm, it may have caused flickering because when the calculation modes are active, the display function turns off the digits one by one.



### Conclusion and Comments
In conclusion, a fully operational scientific calculator has been designed using the embedded C programming language. In the design and coding phase, experience has been gained about how the 4x4 keypad and seven segment display (SSD) will work in harmony. Information was obtained on how to get information on a 4x4 keypad. Also, great experience has been gained in developing algorithms. The working logic of TIMER and EXTI interrupt handlers is well understood and thus microprocessor dynamics has been learned better in this way. As a comment, only one problem has been unsolved in calculator. It is flickering. This is a quality requirement and it was encountered in some cases, but not in some cases. It is foreseen that the handler will exit the display function late as the source of the problem.



