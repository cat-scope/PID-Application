#ifndef	__USART_H__
#define __USART_H__

#include "ti_msp_dl_config.h"
#include "stdio.h"


void uart0_init(void);


void uart1_init(void);
void uart1_send_char(unsigned char data);
void uart1_send_string(char* str);


uint8_t Usart_GetRxFlag(void);
void UART_0_INST_IRQHandler(void);
uint8_t Get_id_Flag(void);
float Pow_invert(uint8_t X,uint8_t n);
float RxPacket_Data_Handle(void);

#endif


