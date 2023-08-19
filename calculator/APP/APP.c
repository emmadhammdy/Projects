/*
 * APP.c
 *
 *  Created on: Aug 15, 2023
 *      Author: Emad Hamdy
 */

#include <avr/io.h>
#include <stdio.h>
#include <util/delay.h>
#include "..\HAL\lcd.h"
#include "..\HAL\keypad.h"


uint8_t operator;
uint32_t num1_adjusted;
uint32_t num2_adjusted;
uint8_t flag;
uint32_t answer;
uint32_t temp;
uint8_t ten = 1;

int performOperation(uint32_t num1, uint32_t num2, uint8_t operator)
{

	uint32_t result=0;
	switch (operator)
	{
	case '+':
		result = num1 + num2;

		break;
	case '-':
		result =  num1 - num2;

		break;
	case '*':
		result =  num1 * num2;

		break;
	case '%':
		result =  num1 / num2;

		break;
	default:
		return 0;
	}
	return result;
}

int main()
{
	LCD_init();
	uint8_t key_recieved;

	while(1)
	{
		/* Receive the pressed key from MC1 through UART */
		key_recieved = keypad_read();
		_delay_ms(100); /* Press time */
		if (key_recieved == '=')
		{
			answer=performOperation(num1_adjusted,num2_adjusted,operator);
			LCD_moveCursor(1,0);
			LCD_displayString("Result:");
			LCD_intgerToString(answer); // Display the result
		}
		else if  (key_recieved == '+' || key_recieved == '-' || key_recieved == '*' || key_recieved == '%')
		{

			operator=key_recieved;
			LCD_displayCharacter(operator);
			_delay_ms(100);
			flag++;
		}
		else if ((key_recieved >=0) && (key_recieved <= 9))
		{
			LCD_intgerToString(key_recieved);
			_delay_ms(100);
			if (flag==0)
			{
				//temp = key_recieved*ten;
				//ten = ten * 10;
				num1_adjusted = num1_adjusted * 10 + key_recieved;

			}
			else if(flag==1)
			{
				num2_adjusted = num2_adjusted * 10 + key_recieved;

			}
		}
		else
		{
			LCD_clearScreen();
			num1_adjusted=0;
			num2_adjusted=0;
			answer=0;
			flag=0;
			_delay_ms(100); /* Press time */




		}
	}
}
