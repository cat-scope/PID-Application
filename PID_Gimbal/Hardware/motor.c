#include "motor.h"
#include <math.h>
#include "step_motor.h"
extern int Position1,Target1,Target2;
extern STEP_MOTOR step_motor_lr;

/***********************************************
��˾����Ȥ�Ƽ�����ݸ�����޹�˾
Ʒ�ƣ�WHEELTEC
������wheeltec.net
�Ա����̣�shop114407458.taobao.com 
����ͨ: https://minibalance.aliexpress.com/store/4455017
�汾��V1.0
�޸�ʱ�䣺2024-07-019

Brand: WHEELTEC
Website: wheeltec.net
Taobao shop: shop114407458.taobao.com 
Aliexpress: https://minibalance.aliexpress.com/store/4455017
Version: V1.0
Update��2024-07-019

All rights reserved
***********************************************/
float Position_KP=5.0,Position_KD=3.0; //����ٶ�PID����
float PID_Position[3]={5.0,0,3.0};

/*************************************************************************
�������ܣ�λ��ʽPID������
��ڲ���������������λ����Ϣ��Ŀ��λ��
����  ֵ�����PWM����
**************************************************************************/
float Position_PID_Y(float Position,float Target)
{                                             //�������
     static float Bias,Pwm,Integral_bias,Last_Bias;
     Bias=Target-Position;                                  //����ƫ��
     if(fabs(Bias) <= 4.0)		//如果误差绝对值比较小
	{
		Pwm = 0.0;				//则直接输出0，不进行调控
	}
     else if(fabs(Bias) >= 100.0)
     {
          Pwm = 0.0;				//则直接输出0，不进行调
     }
	else	
          {					
               Pwm=Position_KP*Bias/80+Position_KD*(Bias-Last_Bias)/70;       //λ��ʽPID������

               // if(Pwm > 0) {Pwm += 1;}
               // if(Pwm < 0) {Pwm -= 1;}
               Last_Bias=Bias;                                       //������һ��ƫ��
          }
        
     return Pwm;
}
/*************************************************************************
�������ܣ�λ��ʽPID������
��ڲ���������������λ����Ϣ��Ŀ��λ��
����  ֵ�����PWM����
**************************************************************************/
float Position_PID_X(float Position,float Target)
{         
     static float Bias,Pwm,Integral_bias,Last_Bias;
     Bias=Target-Position;                                  //����ƫ��
     if (fabs(Bias) <= 4.0)		//如果误差绝对值比较小
		{
			Pwm = 0.0;				//则直接输出0，不进行调控
		}
     else if(fabs(Bias) >= 120.0)
     {
          Pwm = 0.0;				//则直接输出0，不进行调
     }
	else	
          {					
               // Pwm=Position_KP*Bias/100+Position_KD*(Bias-Last_Bias)/50;       //λ��ʽPID������
               Pwm=PID_Position[0]*Bias/90+PID_Position[2]*(Bias-Last_Bias)/70;       //λ��ʽPID������

               Last_Bias=Bias;                                       //������һ��ƫ��
               // if(Pwm > 0) {Pwm += 1;}
               // if(Pwm < 0) {Pwm -= 1;}
          }
                uint8_t PID_index=0;
				if(Usart_GetRxFlag())
				{
				PID_index=Get_id_Flag();
				PID_Position[PID_index-1]=RxPacket_Data_Handle();
				}
     return Pwm;
}

int PWM_limit(int in,int max,int min)
{
    if(in>max) in=max;
    if(in<min) in=min;
    return in;
}

void Set_PWM(float pwma,float pwmb)
{
       Position1 -= pwma;
       if(pwmb>3.3)
       {step_motor_lr.remain_steps = pwmb*4.0; } 
       if(pwmb<=3.3)
       {step_motor_lr.remain_steps = pwmb*2.0;}
       Position1 = PWM_limit(Position1,1500,1420);
       step_motor_lr.remain_steps = PWM_limit(step_motor_lr.remain_steps,150,-150);
       step_motor_lr.position += step_motor_lr.remain_steps;
     //   if(step_motor_lr.position >= 250)
     //   {
     //      step_motor_lr.position = 250;
     //      step_motor_lr.remain_steps = 0;
     //   }
       DL_Timer_setCaptureCompareValue(PWM_0_INST,Position1,GPIO_PWM_0_C0_IDX);

}



