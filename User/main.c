#include <AMES_CLOCK.h>
#include <AMES_Delay.h>
#include <Hardware.h>
#include <AMES_USART.h>
#include <AMES_PWM.h>
#include <AMES_ACS712.h>
#include <AMES_TIMER.h>

typedef struct Moment
{
	float Mx;
	float My;
	float Mz;
}Moment_TypeDef;

typedef struct Current
{
	float Current_MOTOR1;
	float Current_MOTOR2;
	float Current_MOTOR3;
}Current_TypeDef;

static __IO unsigned char ReceiveData;
char *Cmd;
static __IO Moment_TypeDef Moment;
static __IO Current_TypeDef Current;

int main(void)
{
	TSVN_FOSC_Init();
	LED_Init();
	TSVN_USART_Init();
	TSVN_PWM_TIM5_Init(5000);
	TSVN_PWM_TIM5_Set_Duty(0, PWM_CHANNEL_1);
	TSVN_PWM_TIM5_Set_Duty(0, PWM_CHANNEL_2);
	TSVN_PWM_TIM5_Set_Duty(0, PWM_CHANNEL_3);
	TSVN_PWM_TIM5_Start();
	DIR_Init();
	TSVN_ACS712_Init();
	TSVN_TIM6_Init(2000);
	while(1)
	{
		Current.Current_MOTOR1 = TSVN_ACS712_Read(ACS_1);
		Current.Current_MOTOR2 = TSVN_ACS712_Read(ACS_2);
		Current.Current_MOTOR3 = TSVN_ACS712_Read(ACS_3);
		printf("%d\t%d\t%d\n", TSVN_ACS712_Read_ADC(ACS_1), TSVN_ACS712_Read_ADC(ACS_2),TSVN_ACS712_Read_ADC(ACS_3));
		Delay_ms(500);
	}
}
void TIM6_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM6, TIM_IT_Update) != RESET)
	{
		TIM_ClearITPendingBit(TIM6, TIM_IT_Update);
	}
}
void USART1_IRQHandler(void)
{
	if (USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)
    {
      ReceiveData =(unsigned char)USART_ReceiveData(USART1);
			if (TSVN_USART_Create_Frame(ReceiveData) == End)
				{
					Cmd = TSVN_Get_Parameters(1, TSVN_USART_Get_Frame());
					if (!strcmp("M", Cmd))
					{
						Cmd = TSVN_Get_Parameters(2, TSVN_USART_Get_Frame());
						Moment.Mx = atof(Cmd);
						Cmd = TSVN_Get_Parameters(3, TSVN_USART_Get_Frame());
						Moment.My = atof(Cmd);
						Cmd = TSVN_Get_Parameters(4, TSVN_USART_Get_Frame());
						Moment.Mz = atof(Cmd);
					}
				}
    }
}
