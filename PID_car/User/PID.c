#include "stm32f10x.h"                  // Device header
#include "PID.h"
#include <math.h>
#include <stdlib.h>

void PID_Motor_calculate(PID_t *p)
{
	p->Error1 = p->Error0;
	p->Error0 = p->target - p->actual;
	if(fabs(p->ki)>0.0001)
			{
				p->ErrorInt += p->Error0;
			}
			else {p->ErrorInt = 0;}
		//积分限幅			
			if(p->ErrorInt > 500 ){p->ErrorInt=500;}
			if(p->ErrorInt < -500 ){p->ErrorInt=-500;}
		//			
			p->out = p->kp * p->Error0 + p->ki * p->ErrorInt + p->kd * (p->Error0 - p->Error1);
			if(p->out>p->outmax){p->out=p->outmax;}
			if(p->out<p->outmin){p->out=p->outmin;}

}

void PID_JY62_calculate(PID_t *p)
{
	p->Error1 = p->Error0;
	p->Error0 = p->target - p->actual;
  
	if(abs(p->Error0) < 5)
	{p->out = 0;}
	else{
		if(fabs(p->ki)>0.0001)
				{
					p->ErrorInt += p->Error0;
				}
				else {p->ErrorInt = 0;}
			//积分限幅			
				if(p->ErrorInt > 1000 ){p->ErrorInt=1000;}
				if(p->ErrorInt < -1000 ){p->ErrorInt=-1000;}
			//			
				p->out = p->kp * p->Error0 + p->ki * p->ErrorInt + p->kd * (p->Error0 - p->Error1);
				
		}
			if(p->out>p->outmax){p->out=p->outmax;}
			if(p->out<p->outmin){p->out=p->outmin;}

}
