#include <AMES_CLOCK.h>
#include <AMES_Delay.h>
#include <Hardware.h>
#include <AMES_USART.h>
#include <AMES_PWM.h>
#include <AMES_ACS712.h>
#include <AMES_TIMER.h>
#include <AMES_FIR.h>
#include <AMES_PID.h>
#include <stdbool.h>
#include <stdint.h>

#define CALIB_SAMPLE 200
#define MOTOR1_PWM PWM_CHANNEL_1
#define MOTOR2_PWM PWM_CHANNEL_2
#define MOTOR3_PWM PWM_CHANNEL_3

typedef struct Moment
{
	float M_MOTOR1;
	float M_MOTOR2;
	float M_MOTOR3;
}Moment_TypeDef;

typedef struct Current
{
	float Current_MOTOR1;
	float Current_MOTOR2;
	float Current_MOTOR3;
}Current_TypeDef;

typedef struct Current_Out
{
	int Current_OUT_MOTOR1;
	int Current_OUT_MOTOR2;
	int Current_OUT_MOTOR3;
}Current_Out_TypeDef;

static __IO unsigned char ReceiveData;
char *Cmd;
static __IO Moment_TypeDef Moment = {0, 0, 0};
static __IO Current_TypeDef Current;
static __IO Current_Out_TypeDef Current_Out;
volatile float SEN_CALIB[3] = {0, 0, 0};

int main(void)
{
	unsigned char i= 0;
	unsigned int PWM_Max_Value = 0;
	PIDCoff Coff_MOTOR = {0.0, 0.0, 0.0};
	
	TSVN_FOSC_Init();
	TSVN_ACS712_Init();
	TSVN_USART_Init();
	LED_Init();
	FIR_Init();
	PWM_Max_Value = TSVN_PWM_TIM5_Init(5000);
	TSVN_PWM_TIM5_Set_Duty(0, PWM_CHANNEL_1);
	TSVN_PWM_TIM5_Set_Duty(0, PWM_CHANNEL_2);
	TSVN_PWM_TIM5_Set_Duty(0, PWM_CHANNEL_3);
	TSVN_PWM_TIM5_Start();
	DIR_Init();
	PID_Mem_Create(3);
	PID_WindUp_Init(MOTOR1, PWM_Max_Value);
	PID_WindUp_Init(MOTOR2, PWM_Max_Value);
	PID_WindUp_Init(MOTOR3, PWM_Max_Value);
	
	Coff_MOTOR.Kp = 6.0;
	Coff_MOTOR.Ki = 0.01;
	Coff_MOTOR.Kd = 0.00001;
	
	PID_Init(MOTOR1, Coff_MOTOR);
	PID_Init(MOTOR2, Coff_MOTOR);
	Coff_MOTOR.Kp = 4.0;
	PID_Init(MOTOR3, Coff_MOTOR);
	
	for (i = 0; i< CALIB_SAMPLE; i++)
	{
		SEN_CALIB[0] += TSVN_ACS712_Read(ACS_1);
		SEN_CALIB[1] += TSVN_ACS712_Read(ACS_2);
		SEN_CALIB[2] += TSVN_ACS712_Read(ACS_3);
	}
	SEN_CALIB[0] /= CALIB_SAMPLE;
	SEN_CALIB[1] /= CALIB_SAMPLE;
	SEN_CALIB[2] /= CALIB_SAMPLE;
	TSVN_TIM6_Init(1000);
	while(1)
	{
		LED_Toggle();
		Delay_ms(500);
	}
}
void TIM6_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM6, TIM_IT_Update) != RESET)
	{
		TIM_ClearITPendingBit(TIM6, TIM_IT_Update);
		FIR_CollectData(SEN_MOTOR1, TSVN_ACS712_Read(ACS_1));
		Current.Current_MOTOR1 = AMES_Filter(SEN_MOTOR1) - SEN_CALIB[0];
		FIR_CollectData(SEN_MOTOR2, TSVN_ACS712_Read(ACS_2));
		Current.Current_MOTOR2 = AMES_Filter(SEN_MOTOR2) - SEN_CALIB[1];	
		FIR_CollectData(SEN_MOTOR3, TSVN_ACS712_Read(ACS_3));
		Current.Current_MOTOR3 = AMES_Filter(SEN_MOTOR3) - SEN_CALIB[2];
		//***********************MOTOR1**************************************
		Current_Out.Current_OUT_MOTOR1 = PID_Calculate(MOTOR1, Moment.M_MOTOR1, Current.Current_MOTOR1);
		if (Current_Out.Current_OUT_MOTOR1 < 0)
				DIR_Change(MOTOR1, RESERVE);
		else
				DIR_Change(MOTOR1, FORWARD);
		TSVN_PWM_TIM5_Set_Duty(abs(Current_Out.Current_OUT_MOTOR1), MOTOR1_PWM);
		//***********************MOTOR2**************************************
		Current_Out.Current_OUT_MOTOR2 = PID_Calculate(MOTOR2, Moment.M_MOTOR2, Current.Current_MOTOR2);
		if (Current_Out.Current_OUT_MOTOR2 < 0)
				DIR_Change(MOTOR2, RESERVE);
		else
				DIR_Change(MOTOR2, FORWARD);
		TSVN_PWM_TIM5_Set_Duty(abs(Current_Out.Current_OUT_MOTOR2), MOTOR2_PWM);
		//**********************MOTOR3**************************************
		Current_Out.Current_OUT_MOTOR3 = PID_Calculate(MOTOR3, Moment.M_MOTOR3, Current.Current_MOTOR3);
		if (Current_Out.Current_OUT_MOTOR3 < 0)
				DIR_Change(MOTOR3, RESERVE);
		else
				DIR_Change(MOTOR3, FORWARD);
		TSVN_PWM_TIM5_Set_Duty(abs(Current_Out.Current_OUT_MOTOR3), MOTOR3_PWM);
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
						Moment.M_MOTOR1 = atof(Cmd);
						Cmd = TSVN_Get_Parameters(3, TSVN_USART_Get_Frame());
						Moment.M_MOTOR2 = atof(Cmd);
						Cmd = TSVN_Get_Parameters(4, TSVN_USART_Get_Frame());
						Moment.M_MOTOR3 = atof(Cmd);
					}
				}
    }
}
