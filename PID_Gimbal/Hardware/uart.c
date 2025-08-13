 #include "uart.h"
#include "yb_protocol.h"


volatile uint8_t gEchoData = 0;

uint8_t Usart_RxData;
uint8_t Usart_RxFlag;

uint8_t Usart_RxPacket[100];//接受数据包
uint8_t Usart_RxPacket_Len=100;//接受数据包长度


uint8_t id_Flag;//1为Kp 2为Ki 3为Kd
uint8_t Data_BitNum=0;//数据的位数，即12.123 有6位 -12.123有7为


void uart0_init(void)
{
	//清除串口中断标志
	//Clear the serial port interrupt flag
	NVIC_ClearPendingIRQ(UART_0_INST_INT_IRQN);
	//使能串口中断
	//Enable serial port interrupt
	NVIC_EnableIRQ(UART_0_INST_INT_IRQN);
}



///////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////



void uart1_init(void)
{
	//清除串口中断标志
	//Clear the serial port interrupt flag
	NVIC_ClearPendingIRQ(UART_1_INST_INT_IRQN);
	//使能串口中断
	//Enable serial port interrupt
	NVIC_EnableIRQ(UART_1_INST_INT_IRQN);
}

//串口发送一个字节
//The serial port sends a byte
void uart1_send_char(unsigned char data)
{
	//当串口0忙的时候等待
	//Wait when serial port 0 is busy
	while( DL_UART_isBusy(UART_1_INST) == true );
	//发送
	//send
	DL_UART_Main_transmitData(UART_1_INST, data);
}

//串口发送字符串 Send string via serial port
void uart1_send_string(char* str)
{
    //当前字符串地址不在结尾 并且 字符串首地址不为空
		// The current string address is not at the end and the string first address is not empty
    while(*str!=0&&str!=0)
    {
        //发送字符串首地址中的字符，并且在发送完成之后首地址自增
		// Send the characters in the first address of the string, and increment the first address after sending.
        uart1_send_char(*str++);
    }
}


//串口的中断服务函数
//Serial port interrupt service function
void UART_1_INST_IRQHandler(void)
{
	uint8_t received = 0;
	
	//如果产生了串口中断
	//If a serial port interrupt occurs
	switch( DL_UART_getPendingInterrupt(UART_1_INST) )
	{
		case DL_UART_IIDX_RX://如果是接收中断	If it is a receive interrupt
			
			// 接收发送过来的数据保存	Receive and save the data sent
			received = DL_UART_Main_receiveData(UART_1_INST);
            // uart0_send_char(received);
            Pto_Data_Receive(received);
			
			break;
		
		default://其他的串口中断	Other serial port interrupts
			break;
	}
}



uint8_t Usart_GetRxFlag(void)
{
	if(Usart_RxFlag==1)
	{
		Usart_RxFlag=0;
		return 1;
	}
	return 0;
}



void UART_0_INST_IRQHandler(void)
{

     static uint8_t RxState=0;
	        static uint8_t pRxPacket=0;

    switch (DL_UART_Main_getPendingInterrupt(UART_0_INST)) {
        case DL_UART_MAIN_IIDX_RX:
           

		Usart_RxData=DL_UART_Main_receiveData(UART_0_INST);

		if(RxState==0&&Usart_RxData==0x23) //第一个帧头  "#"==0x23
		{
			RxState=1;
		}
		else if(RxState==1&&Usart_RxData==0x50) //第二个帧头  "P"==0x50
		{
			RxState=2;
		}
		else if(RxState==2)//确认传参的对象 即修改id_Flag
		{	
			id_Flag=Usart_RxData-48;
			RxState=3;
		}
		else if(RxState==3&&Usart_RxData==0x3D) //判断等号，也可以类比为数据开始的帧头
		{
			RxState=4;
		}
    else if(RxState==4)//开始接收传输的数据
		{	
                if(Usart_RxData==0x21)//结束的帧尾   如果没有接收到！即还有数据来，就一直接收
                        {
                    Data_BitNum=pRxPacket;//获取位数
                    pRxPacket=0;//清除索引方便下次进行接收数据
                    RxState=0;
                    Usart_RxFlag=1;
                        }
                else
                {
                    Usart_RxPacket[pRxPacket++]=Usart_RxData;//把数据放在数据包内
                }
		}
		 NVIC_ClearPendingIRQ(UART_0_INST_INT_IRQN);
	

            break;
        default:
            break;

            }
 }

          




uint8_t Get_id_Flag(void)//将获取id_Flag封装成函数
{
  uint8_t id_temp;
  id_temp=id_Flag;
  id_Flag=0;
  return id_temp;
}

float Pow_invert(uint8_t X,uint8_t n)//x除以n次10
{
  float result=X;
	while(n--)
	{
		result/=10;
	}
	return result;
}

//uint8_t Usart_RxPacket[5]={0x31,0x32,0x2E,0x31,0x33};//可以给数据包直接赋值直接调用一下换算程序，看是否输出为12.13
//Data_BitNum = 5//别忘记数据的长度也要设置
//然后直接在主程序就放  Usart_Printf("%f\n",RxPacket_Data_Handle());  Delay_ms(1000);就ok了
float RxPacket_Data_Handle(void)//数据包换算处理
{
  float Data=0.0;
  uint8_t dot_Flag=0;//小数点标志位，能区分小数点后或小数点前 0为小数点前，1为小数点后
  uint8_t dot_after_num=1;//小数点后的第几位
  int8_t minus_Flag=1;// 负号标志位 -1为是负号 1为正号
  for(uint8_t i=0;i<Data_BitNum;i++)
  {
    if(Usart_RxPacket[i]==0x2D)//如果第一位为负号
    {
      minus_Flag=-1;
      continue;//跳过本次循环 
    }
    if(dot_Flag==0)
    {
      if(Usart_RxPacket[i]==0x2E)//如果识别到小数点，则将dot_Flag置1
      {
        dot_Flag=1;
      }
      else//还没遇到小数点前的运算
      {
        Data = Data*10 + Usart_RxPacket[i]-48;
      }
    }
    else//遇到小数点后的运算
    {
      Data = Data + Pow_invert(Usart_RxPacket[i]-48,dot_after_num);
      dot_after_num++;
    }
  }
  return Data*minus_Flag;//将换算后的数据返回出来 这里乘上负号标志位
}





