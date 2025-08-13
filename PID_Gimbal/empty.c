/*
 * Copyright (c) 2021, Texas Instruments Incorporated
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * *  Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * *  Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * *  Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "ti_msp_dl_config.h"
#include "led.h"
#include "board.h"
#include "motor.h"
#include "key.h"
#include "uart.h"
#include "yb_protocol.h"
#include "step_motor.h"
#include <math.h>
#include "stdlib.h"
float PWMA,PWMB;
int Actual1,Actual2,Target1=112,Target2=162;
int Position1=1400,PositionX;
STEP_MOTOR step_motor_lr = {0, 0, 0, 0};
uint8_t pin,Count_led;
uint8_t config;
int main(void)
{
    SYSCFG_DL_init();
    uart1_init();
    DL_Timer_startCounter(PWM_0_INST);//����PWM���
    NVIC_ClearPendingIRQ(UART_0_INST_INT_IRQN);//����жϱ�־λ
    NVIC_ClearPendingIRQ(TIMER_0_INST_INT_IRQN);

    // //ʹ�ܴ����ж�
    NVIC_EnableIRQ(UART_0_INST_INT_IRQN);//�����ж�
    NVIC_EnableIRQ(TIMER_0_INST_INT_IRQN);
    DL_GPIO_clearPins(GPIO_LED_PORT, GPIO_LED_PIN_2_PIN);

    // step_motor_lr.remain_steps = 2048;      //按键设置放循环里
    while (1) {
    
        // step_motor_lr.remain_steps = 2048;
        // DL_GPIO_setPins(Light_PORT, Light_PIN_1_PIN);
        // delay_ms(1000);
        //  DL_GPIO_clearPins(Light_PORT, Light_PIN_1_PIN);
     
    
         if(click())
            {
                step_motor_lr.remain_steps = -2048;
            }
        
        // Position1 += PWMA;
        // Position1 = PWM_limit(Position1,1250,750);
    //     DL_Timer_setCaptureCompareValue(PWM_0_INST,Position1,GPIO_PWM_0_C0_IDX);
    //    DL_Timer_setCaptureCompareValue(PWM_0_INST,Position2,GPIO_PWM_0_C1_IDX);
        // printf("%d,%d,%d,%f\n\r",Target1,Actual1,Position1,PWMA);
        printf("%d,%d,%d,%d\n\r",Target1,Actual1,Target2,Actual2);
        // printf("%d,%d,%d\n\r",Target2,Actual2,step_motor_lr.remain_steps);
        // printf("%d,%d,%d\n\r",Target1,Actual1,Actual2);
        // printf("%d\n\r",Position1);
    }
}

void TIMER_0_INST_IRQHandler(void)
{
    static uint16_t Count;
    int a;
    // // uint8_t config;
    if(DL_TimerA_getPendingInterrupt(TIMER_0_INST))
    {
        if(DL_TIMER_IIDX_ZERO)
        {
            // Count ++;				//计次自增

            if(step_motor_lr.remain_steps > 0)
            {
                step(&step_motor_lr, 1);
                step_motor_lr.remain_steps --;
                PositionX ++;
            }
            else if(step_motor_lr.remain_steps < 0)
            {
                step(&step_motor_lr, -1);
                step_motor_lr.remain_steps ++;
                PositionX --;
            }  
            if (config != 1) {
                PositionX=0;
            }
            if (abs(PositionX)>=160) {
           
            step_motor_lr.remain_steps = 0;
           }
            switch (config) {
            case 0:
            if((location[0]!=0) || (location[1]!=0))
            {
                config = 1;
                a = step_motor_lr.remain_steps;
                step_motor_lr.remain_steps = 0;
            }
            break;

             case 1:
            Count ++;				//计次自增
            Count_led++;
		    if (Count == 3 )		//如果计次10次，则if成立，即if每隔30ms进一次
		    {
		        Count = 0;	
                Actual1 = location[1];
                Actual2 = location[0];

                PWMA = Position_PID_Y(Actual1,Target1);
                PWMB = Position_PID_X(Actual2,Target2);
                
                Set_PWM(PWMA,PWMB);
                if((abs(Target1 - Actual1) <= 4)&&(abs(Target2 - Actual2) <= 4))
                {
                    DL_GPIO_setPins(GPIO_LED_PORT, GPIO_LED_PIN_2_PIN);

                   config = 2;

                    // if(Count_led == 300){DL_GPIO_clearPins(GPIO_LED_PORT, GPIO_LED_PIN_2_PIN);}
                }
                Set_PWM(PWMA,PWMB);
             }
            if(Count_led == 200)
            {DL_GPIO_setPins(GPIO_LED_PORT, GPIO_LED_PIN_2_PIN);}
            break;
            case 2:
            step_motor_lr.remain_steps = a;
            config = 0;
            break;
            default:break;
        }

            NVIC_ClearPendingIRQ(TIMER_0_INST_INT_IRQN);
        }

    }
}





