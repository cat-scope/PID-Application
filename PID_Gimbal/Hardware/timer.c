// #include "ti_msp_dl_config.h"
// #include "step_motor.h"
// #include "led.h"
// // #include "uart.h"

// extern struct STEP_MOTOR step_motor_lr;
// // uint8_t config;
// void TIMER_0_INST_IRQHandler()
// {
//     // static uint16_t Count_led;
//     switch (DL_TimerA_getPendingInterrupt(TIMER_0_INST)) {
//         case DL_TIMER_IIDX_ZERO:
//             if(step_motor_lr.remain_steps > 0)
//             {
//                 step(&step_motor_lr, 1);
//                 step_motor_lr.remain_steps --;
//             }
//             else if(step_motor_lr.remain_steps < 0)
//             {
//                 step(&step_motor_lr, -1);
//                 step_motor_lr.remain_steps ++;
//             }                                                 //旋转
//         // switch (config) {
//         //     case 0:
//             // if((location[0]!=0) || (location[1]!=0))
//             // {
//             //     config = 1;
//             //     a = step_motor_lr.remain_steps;
//             //     step_motor_lr.remain_steps = 0;
//             // }
//         //     break;

//         //     case 1:
//         //     // pid
//         //     LED_ON();
//         //     Count_led ++;
//         //     if(Count_led == 1000)
//         //     {
//         //         LED_OFF();
//         //         config = 2;
//         //     }
//         //     break;
//         //     case 2:
//         //     step_motor_lr.remain_steps = a;
//         //     config = 0;
//             break;
//         //     default:break;
//         // }
        
//         default:break;
    
//     }
// }
