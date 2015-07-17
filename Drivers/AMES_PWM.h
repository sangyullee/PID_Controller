/****************************************************************************/
/*PWM library.																														*/
/*This library is copyright and Protected by AMES lab		    								*/
/*Date: 20/02/2015																													*/
/*Author: Thinh Nguyen									    																*/ 																
/****************************************************************************/

#include "stm32f10x.h" 
#include <stm32f10x_gpio.h>
#include <stm32f10x_tim.h>
#include <stdlib.h>
#include <stdio.h>

#define TIM5_CHANNEL_1_PIN GPIO_Pin_0		//PA0
#define TIM5_CHANNEL_2_PIN GPIO_Pin_1		//PA1
#define TIM5_CHANNEL_3_PIN GPIO_Pin_2		//PA2


#define TIM5_CHANNEL_PORT	GPIOA


 enum{PWM_CHANNEL_1, PWM_CHANNEL_2, PWM_CHANNEL_3};

/**************************************************************************/
/*Tan so tu 1Hz den 10Mhz*/
/**/
/**************************************************************************/
unsigned int TSVN_PWM_TIM5_Init(unsigned long Hz);
void TSVN_PWM_TIM5_Set_Duty(unsigned int Ratio, unsigned char Channel); 
void TSVN_PWM_TIM5_Start(void);
void TSVN_PWM_TIM5_Stop(void);
 
