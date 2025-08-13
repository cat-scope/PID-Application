#ifndef STEP_MOTOR_H_
#define STEP_MOTOR_H_

#include "ti_msp_dl_config.h"

typedef struct 
{
    //当前步
    uint8_t current_step;

    //剩余步
    int remain_steps;

    //电机编号
    uint8_t number;


    int position;

} STEP_MOTOR;

void step(STEP_MOTOR *step_motor, int8_t dir);

#endif

