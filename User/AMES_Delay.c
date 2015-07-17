#include <AMES_Delay.h>

static __IO uint32_t sysTickCounter;
 
static void TimeTick_Decrement(void) {
	if (sysTickCounter != 0x00) {
		sysTickCounter--;
	}
}
void Delay_ms(uint32_t ms)
{
	sysTickCounter = ms;
	while (sysTickCounter != 0);
}
void SysTick_Handler(void) {
	TimeTick_Decrement();
}
