#include "ti_msp_dl_config.h"
#include "step_motor.h"

//0号电机左右转动


void step( STEP_MOTOR *step_motor, int8_t dir)
{
    if(step_motor->number==0)
    {
        step_motor->current_step += 4;
        step_motor->current_step += dir;
        step_motor->current_step %= 4;
        if(step_motor->current_step == 0)
        {
            DL_GPIO_setPins(STEP_MOTOR_IN1_PORT, STEP_MOTOR_IN1_PIN);
            DL_GPIO_setPins(STEP_MOTOR_IN2_PORT, STEP_MOTOR_IN2_PIN);
            DL_GPIO_clearPins(STEP_MOTOR_IN3_PORT, STEP_MOTOR_IN3_PIN);
            DL_GPIO_clearPins(STEP_MOTOR_IN4_PORT, STEP_MOTOR_IN4_PIN);
        }
        else if(step_motor->current_step == 1)
        {
            DL_GPIO_clearPins(STEP_MOTOR_IN1_PORT, STEP_MOTOR_IN1_PIN);
            DL_GPIO_setPins(STEP_MOTOR_IN2_PORT, STEP_MOTOR_IN2_PIN);
            DL_GPIO_setPins(STEP_MOTOR_IN3_PORT, STEP_MOTOR_IN3_PIN);
            DL_GPIO_clearPins(STEP_MOTOR_IN4_PORT, STEP_MOTOR_IN4_PIN);
        }
        else if(step_motor->current_step == 2)
        {
            DL_GPIO_clearPins(STEP_MOTOR_IN1_PORT, STEP_MOTOR_IN1_PIN);
            DL_GPIO_clearPins(STEP_MOTOR_IN2_PORT, STEP_MOTOR_IN2_PIN);
            DL_GPIO_setPins(STEP_MOTOR_IN3_PORT, STEP_MOTOR_IN3_PIN);
            DL_GPIO_setPins(STEP_MOTOR_IN4_PORT, STEP_MOTOR_IN4_PIN);
        }
        else if(step_motor->current_step == 3)
        {
            DL_GPIO_setPins(STEP_MOTOR_IN1_PORT, STEP_MOTOR_IN1_PIN);
            DL_GPIO_clearPins(STEP_MOTOR_IN2_PORT, STEP_MOTOR_IN2_PIN);
            DL_GPIO_clearPins(STEP_MOTOR_IN3_PORT, STEP_MOTOR_IN3_PIN);
            DL_GPIO_setPins(STEP_MOTOR_IN4_PORT, STEP_MOTOR_IN4_PIN);
        }
    }
}
