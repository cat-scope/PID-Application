#ifndef __PID_H
#define __PID_H

typedef struct {
	int16_t target;
	int16_t actual;
	int16_t location;
	float kp;
	float ki;
	float kd;
	int16_t Error0;
	int16_t Error1;
	int32_t ErrorInt;
	int16_t out;
	int16_t outmax;
	int16_t outmin;

} PID_t;

void PID_Motor_calculate(PID_t *p);
void PID_JY62_calculate(PID_t *p);

#endif
