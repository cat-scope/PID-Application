#include "stm32f10x.h"// Device header
#include "Delay.h"
#include <math.h>
#include <stdlib.h>
#include "OLED.h"
#include "Motor.h"
#include "Key.h"
#include "Encoder.h"
#include "Timer.h"
#include "Serial.h"
#include "PID.h"
#include "JY62.h"


uint8_t KeyNum;
int16_t leftspeed,rightspeed,angle;
//
PID_t leftmotor = {
	.kp=0.4,
	.ki=0.2,
	.kd=0.1,
	.outmax=100,
	.outmin=-100,
};
PID_t rightmotor = {
	.kp=0.4,
	.ki=0.2,
	.kd=0,
	.outmax=100,
	.outmin=-100,
};
PID_t JY62 = {
	.kp=0.6,
	.ki=0,
	.kd=0,
	.target=0,
	.outmax=50,
	.outmin=-50,
};
//pid定义
int main(void)
{
	OLED_Init();
	Motor_Init();
	Key_Init();
	Encoder_Init();
	Timer_Init();
	Serial_Init();
	JY62_Init();
	JY62_Configure();
	
//	OLED_ShowString(1, 1, "JY62 Ready    ");
//	OLED_ShowString(1, 1, "Target:");
//	OLED_ShowString(2, 1, "Actual:");
//	OLED_ShowString(3, 1, "Area:");
//	OLED_ShowString(4, 1, "Out:");




	while (1)
	{
		KeyNum = Key_GetNum();
//		/*左轮调试*/
		if (KeyNum == 1)
		{
			leftspeed += 20;
		}
//////		
////		OLED_ShowSignedNum(1, 8, leftmotor.target, 3);
////		OLED_ShowSignedNum(2, 8, leftmotor.actual, 3);
////		OLED_ShowSignedNum(3, 8, leftmotor.location, 5);
////		OLED_ShowSignedNum(4, 8, leftmotor.out, 3);
//		
//		Serial_Printf(" %d,%d\r\n " ,leftmotor.target ,leftmotor.actual);
		
		
		if (KeyNum == 1)
		{
			rightspeed += 20;
		}
		
		OLED_ShowSignedNum(1, 8, rightmotor.target, 3);
//		OLED_ShowSignedNum(2, 8, rightmotor.actual, 3);
//		OLED_ShowSignedNum(3, 8, rightmotor.location, 5);
		if(leftmotor.location >= 2500)
		{
			JY62.target = 160;
		}

		angle=jy62_yaw/100;
		OLED_ShowSignedNum(2, 8, angle, 5);
//		Serial_Printf("%d\r\n ",angle);
		
//		Serial_Printf(" %d,%d\r\n " ,rightmotor.target ,rightmotor.actual);
		Serial_Printf(" %d,%d,%d,%d\r\n " ,JY62.target ,JY62.actual,JY62.out,rightmotor.actual);
//		Delay_ms(10);

	}
}

void TIM2_IRQHandler(void)
{
	static uint16_t Count;
	if (TIM_GetITStatus(TIM2, TIM_IT_Update) == SET)
	{
		Count ++;
		if (Count >= 40)
		{
			Count = 0;
			leftmotor.actual = Encoder_leftGet();
			leftmotor.location += leftmotor.actual;
			PID_Motor_calculate(&leftmotor);
			rightmotor.actual = Encoder_rightGet();
			PID_Motor_calculate(&rightmotor);
			Motor_SetleftSpeed(leftmotor.out);
			Motor_SetrightSpeed(rightmotor.out);
			
			JY62.actual = angle;
			PID_JY62_calculate(&JY62);
			leftmotor.target = leftspeed - JY62.out;
			rightmotor.target = rightspeed + JY62.out;
		}
		TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
	}
}



		





