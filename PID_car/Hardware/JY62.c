#include "stm32f10x.h"                  // Device header
#include <stdio.h>
#include <stdarg.h>
#include "Delay.h"

//char Serial_RxPacket[100];	
//uint8_t Serial_RxFlag;
int16_t jy62_yaw;
	
void JY62_Init()
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	USART_InitTypeDef USART_InitStructure;
	USART_InitStructure.USART_BaudRate = 115200;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_Init(USART2, &USART_InitStructure);
	
	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_Init(&NVIC_InitStructure);
	
	USART_Cmd(USART2, ENABLE);
	

}

void JY62_Configure() {         //向JY62发送配置命令
    // 设置输出频率100Hz
    uint8_t freq_cmd[] = {0xFF, 0xAA, 0x03, 0x08, 0x64};
    
    // 设置垂直安装方向
    uint8_t orient_cmd[] = {0xFF, 0xAA, 0x02, 0x25, 0x00};
	
	// 水平校准
    uint8_t cal_cmd[] = {0xFF, 0xAA, 0x01, 0x01, 0x00};
    
    // 发送配置命令
    for(int i = 0; i < sizeof(freq_cmd); i++) {
        USART_SendData(USART2, freq_cmd[i]);
        while(USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET);
    }
	Delay_ms(100);
    
    for(int i = 0; i < sizeof(orient_cmd); i++) {
        USART_SendData(USART2, orient_cmd[i]);
        while(USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET);
    }
    Delay_ms(100);
	

    for(int i = 0; i < sizeof(cal_cmd); i++) {
        USART_SendData(USART2, cal_cmd[i]);
        while(USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET);
    }
	Delay_ms(2000);  // 校准需要2秒静止时间
	
}


//void USART2_IRQHandler(void)
//{
//	static uint8_t RxState = 0;
//	static uint8_t pRxPacket = 0;
//	if (USART_GetITStatus(USART2, USART_IT_RXNE) == SET)
//	{
//		uint8_t RxData = USART_ReceiveData(USART2);
//		
//		if (RxState == 0)
//		{
//			if (RxData == 0x55 && Serial_RxFlag == 0)
//			{
//				RxState = 1;
//				pRxPacket = 0;
//			}
//		}
//		else if (RxState == 1)
//		{
//			if (RxData == 0x53){RxState = 2;}
//				
//		}
//		else if (RxState == 2)
//		{
//			Serial_RxPacket[pRxPacket] = RxData;
//			pRxPacket ++;	
//			if(pRxPacket>=11)
//			{
//				RxState = 0;
//			}
//		}
//			
//			
//		USART_ClearITPendingBit(USART1, USART_IT_RXNE);
//	}
//}

// 数据包解析状态
int16_t NormalizeAngle(int16_t angle) {
    while(angle > 18000) angle -= 36000;
    while(angle < -18000) angle += 36000;
    return angle;
}

void USART2_IRQHandler(void) {
    static uint8_t state = 0;
    static uint8_t rx_buffer[11];
    static uint8_t data_index = 0;
    
    if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET) {
        uint8_t received = USART_ReceiveData(USART2);
        
        switch(state) {
            case 0:
                if(received == 0x55) {
                    rx_buffer[0] = received;
                    state = 1;
                }
                break;
                
            case 1:
                if(received == 0x53) {  // 角度数据包
                    rx_buffer[1] = received;
                    data_index = 2;
                    state = 2;
                } else {
                    state = 0;
                }
                break;
                
            case 2:
                rx_buffer[data_index++] = received;
                
                // 完整接收11字节数据包
                if(data_index >= 11) {
                    // 校验和验证
                    uint8_t checksum = 0;
                    for(uint8_t i = 0; i < 10; i++) {
                        checksum += rx_buffer[i];
                    }
                    
                    if(checksum == rx_buffer[10]) {
                        // 解析偏航角 (小端格式)
						int16_t yaw   = (int16_t)((rx_buffer[7] << 8) | rx_buffer[6]);  // Z轴
                        
                        // 角度归一化处理
                        yaw = NormalizeAngle(yaw);
                        
                        // 更新全局变量
                        jy62_yaw = yaw;
                    }
                    state = 0;  // 重置状态机
                }
                break;
        }
        
        USART_ClearITPendingBit(USART2, USART_IT_RXNE);
    }

}

// 角度归一化函数





