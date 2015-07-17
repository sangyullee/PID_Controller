/****************************************************************************/
/*Delta Hardware library.																														*/
/*This library is copyright and Protected by AMES lab		    								*/
/*Date: 07/06/2015																													*/
/*Author: Thinh Nguyen										    															*/ 																
/****************************************************************************/

#include <stm32f10x_gpio.h>
#include <stm32f10x_rcc.h>
#include <stdbool.h>

enum  {MOTOR1, MOTOR2, MOTOR3};

void LED_Init(void);
void LED_Toggle(void);

void DIR_Init(void);
void DIR_Change(unsigned char Channel, unsigned char DIR);


