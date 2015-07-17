#include <Hardware.h>



void LED_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	GPIO_ResetBits(GPIOB, GPIO_Pin_8);
}
void LED_Toggle(void)
{
	static bool Status = false;
	if (Status)
		GPIO_SetBits(GPIOB, GPIO_Pin_8);
	else
		GPIO_ResetBits(GPIOB, GPIO_Pin_8);
	Status = !Status;
}
void DIR_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_10 | GPIO_Pin_11;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	GPIO_ResetBits(GPIOB, GPIO_Pin_9);
	GPIO_ResetBits(GPIOB, GPIO_Pin_10);
	GPIO_ResetBits(GPIOB, GPIO_Pin_11);
}
void DIR_Change(unsigned char Channel, unsigned char DIR)
{
	switch (Channel)
	{
		case MOTOR1: {
			if (DIR)
				GPIO_SetBits(GPIOB, GPIO_Pin_9);
			else
				GPIO_ResetBits(GPIOB, GPIO_Pin_9);
			break;
		}
		case MOTOR2: {
			if (DIR)
				GPIO_SetBits(GPIOB, GPIO_Pin_10);
			else
				GPIO_ResetBits(GPIOB, GPIO_Pin_10);
			break;
		}
		case MOTOR3: {
			if (DIR)
				GPIO_SetBits(GPIOB, GPIO_Pin_11);
			else
				GPIO_ResetBits(GPIOB, GPIO_Pin_11);
			break;
		}
	}
}
