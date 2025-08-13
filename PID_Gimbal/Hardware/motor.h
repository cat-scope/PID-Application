#ifndef _MOTOR_H
#define _MOTOR_H
#include "ti_msp_dl_config.h"
#include "board.h"
float Position_PID_Y(float Position,float Target);
float Position_PID_X(float Position,float Target);
int PWM_limit(int in,int max,int min);
void Set_PWM(float pwma,float pwmb);

#endif
