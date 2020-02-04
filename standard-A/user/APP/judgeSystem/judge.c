#include "include.h"
#include "judge.h"

void Uart8_Init(u32 bound);
u8 judgeBuffer[30]={0};
u8 point=0;
void judgeTask(void *pvParameters)
{
  Uart8_Init(115200);
  while(1)
  {



    
    vTaskDelay(judgeDelayTime);
  }
}

void UART8_IRQHandler(void)//串口2中断处理函数
{    
  if(USART_GetITStatus(UART8, USART_IT_RXNE) != RESET)  //接收中断(接收到的数据必须是0x0d 0x0a结尾)
    judgeBuffer[point]=USART_ReceiveData(UART8);//(USART1->DR); //读取接蔦盏降氖?
}  			

void Uart8_Init(u32 bound)
{
  NVIC_InitTypeDef NVIC_InitStructure;
  GPIO_InitTypeDef GPIO_InitStructure;
  USART_InitTypeDef USART_InitStructure;
  //1.串口时钟和和GPIO时钟使能。
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE,ENABLE); //使能GPIOA时钟
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART8,ENABLE);//使能USART2时钟
  //2.设置引脚复用器映射 
  GPIO_PinAFConfig(GPIOE,GPIO_PinSource0,GPIO_AF_UART8); //GPIOA2复用为USART2
  GPIO_PinAFConfig(GPIOE,GPIO_PinSource1,GPIO_AF_UART8); //GPIOA3复用为USART2  
  //3.GPIO端口模式设置
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0| GPIO_Pin_1; //GPIOA2和GPIOA3初始化
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//复用功能
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; //速度50MHz
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //推挽复用输出
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; //上拉
  GPIO_Init(GPIOE,&GPIO_InitStructure); //初始化GPIOA2，和GPIOA3
  //4.串口参数初始化：设置波特率，字长，奇偶校验等参数
  USART_InitStructure.USART_BaudRate = bound;//波特率一般设置为9600;
  USART_InitStructure.USART_WordLength = USART_WordLength_8b;//字长为8位数据格式
  USART_InitStructure.USART_StopBits = USART_StopBits_1;//一个停止位
  USART_InitStructure.USART_Parity = USART_Parity_No;//无奇偶校验位
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx; //收发模式
  USART_Init(UART8, &USART_InitStructure); //初始化串口2参数
  //5.初始化NVIC
  NVIC_InitStructure.NVIC_IRQChannel = UART8_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=1;//抢占优先级0
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0; //子优先级3
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQ通道使能
  NVIC_Init(&NVIC_InitStructure); //根据指定的参数初始化VIC寄存器
  //6.开启中断
  USART_ITConfig(UART8, USART_IT_RXNE, ENABLE);//开启中断  
  //7.使能串口 
  USART_Cmd(UART8, ENABLE);     
}


