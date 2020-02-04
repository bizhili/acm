/**
  ****************************(C) COPYRIGHT 2020 CCZU****************************
  * @file       usart2.c/h
  * @brief      串口2，调试串口DMA初始化，DMA发送，DMA接收
  *             
  * @note       
  * @history
  *  Version    Date            Author          Modification
  *  V1.0.0    2020/1/11          BIZHI           finished
  *
  @verbatim
  ==============================================================================

  ==============================================================================
  @endverbatim
  ****************************(C) COPYRIGHT 2020 CCZU****************************
  */
#include "include.h"
#include "usart2.h"
u16 rx_len2 = 0;                          //DMA接收数据的长度
uint8_t SendBuffer2[40]={0};                   //DMA发送缓冲区
uint8_t ReceiveBuffer2[30]={0};                //DMA接收缓冲区
u16 BufferLen2 = 30;                      //DMA缓冲区的长度
u8 rx_flag2 = 0;                         //数据帧接收标志
/**************************************
@功能             串口的初始化配置
@调用关系           
@输入参数           u32 Bound,eg:115200
@返回值                无
@说明             PA2 => USART2_TX，PA3 => USART2_RX
***************************************/
void Init_Usart2(u32 Bound)
{
    NVIC_InitTypeDef NVIC_InitStructure;      //定义中断结构体
    GPIO_InitTypeDef GPIO_InitStructure;      //定义IO初始化结构体 
    USART_InitTypeDef USART_InitStructure;    //定义串口结构体

    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE); //使能GPIOG时钟
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);//使能USART6时钟
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA1,ENABLE);          //DMA2时钟使能 
    //GPIO引脚复用映射
    GPIO_PinAFConfig(GPIOA,GPIO_PinSource2,GPIO_AF_USART2);       //引脚复用映射 PA2 => USART2_TX
    GPIO_PinAFConfig(GPIOA,GPIO_PinSource3,GPIO_AF_USART2);      //引脚复用映射 PA3 => USART2_RX
    //GPIO结构体设置
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_3;        //管脚指定  PA9  PA14
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;                  //复用功能   
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;                //推挽复用输出 
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;                  //上拉
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;             //速度50MHz   
    GPIO_Init(GPIOA, &GPIO_InitStructure);                        //初始化       
    //Usart的 NVIC 配置  
    USART_InitStructure.USART_BaudRate = Bound;                                       //波特率设置，一般设置为9600
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;                       //字长为8位数据格式
    USART_InitStructure.USART_StopBits = USART_StopBits_1;                            //一个停止位
    USART_InitStructure.USART_Parity = USART_Parity_No;                               //无奇偶校验位
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;   //无硬件数据流控制 
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;                   //收发模式   
    USART_Init(USART2,&USART_InitStructure);                                          //初始化串口
    //使能串口
    USART_Cmd(USART2, ENABLE);    
    //开启串口空闲中断
    USART_ITConfig(USART2, USART_IT_IDLE, ENABLE);    
    NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;         //串口中断通道  
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=7;   //抢占优先级3  
    NVIC_InitStructure.NVIC_IRQChannelSubPriority =0;         //子优先级3  
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;           //IRQ通道使能  
    NVIC_Init(&NVIC_InitStructure);                           //根据指定的参数初始化NVIC寄存器
}

/**************************************
@功能             DMA的发送初始化配置
@调用关系           无
@输入参数           
    @Usartx             指定的串口，如 USART1
    @Par                DMA外设地址，如 (u32)&USART1->DR
    @DMA_Streamx_tx     代表DMA1的发送数据流，DMA1_Stream0~DMA1_Stream7，参见手册
    @DMA_Chx            代表DMA的通道选择，DMA_Channel_0~DMA_Channel_7
    @Tx_Buf             代表DMA的发送缓冲区
    @Tx_Size            代表发送数据个数
    @DMA_Streamx_Irqn   DMA数据流的中断，如 DMA2_Stream7_IRQn
@返回值                无
@说明             无
***************************************/
void Init_Dma_Tx_Config2(USART_TypeDef *Usartx,u32 Par, DMA_Stream_TypeDef *DMA_Streamx_Tx, 
                        u32 DMA_Chx,u32 Tx_Buf, u16 Tx_Size, u32 DMA_Streamx_Irqn)
{
    NVIC_InitTypeDef NVIC_InitStructure;      //定义中断结构体
    DMA_InitTypeDef DMA_InitStructure;        //定义DMA结构体
    DMA_DeInit(DMA_Streamx_Tx);                                                //将DMA的发送数据流设置为缺省值 
    while (DMA_GetCmdStatus(DMA_Streamx_Tx) != DISABLE) ;                      //等待DMA的发送数据流可配置                      

    //DMA的结构体设置
    DMA_InitStructure.DMA_Channel = DMA_Chx;                                   //通道选择  
    DMA_InitStructure.DMA_PeripheralBaseAddr = Par;                            //DMA外设地址  
    DMA_InitStructure.DMA_Memory0BaseAddr = Tx_Buf;                            //DMA 存储器0地址  
    DMA_InitStructure.DMA_DIR = DMA_DIR_MemoryToPeripheral;                    //存储器到外设模式  
    DMA_InitStructure.DMA_BufferSize = Tx_Size;                                //数据传输量   
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;           //外设非增量模式  
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;                    //存储器增量模式  
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;    //外设数据长度:8位  
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;            //存储器数据长度:8位  
    DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;                              // 使用普通模式   
    DMA_InitStructure.DMA_Priority = DMA_Priority_Medium;                      //中等优先级  
    DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;                     //初始化DMA_FIFOMode 
    DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_Full;              //FIFO 阈值选择。
    DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;                //存储器突发单次传输  
    DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;        //外设突发单次传输  
    DMA_Init(DMA_Streamx_Tx, &DMA_InitStructure);                              //初始化DMA Stream  

    //DMA 中断设置    
    NVIC_InitStructure.NVIC_IRQChannel = DMA_Streamx_Irqn;                     //DMA发送数据流中断通道
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 13;                  //抢占优先级0
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;                         //子优先级0
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;                            //IRQ通道使能 
    NVIC_Init(&NVIC_InitStructure);                                            //根据指定的参数初始化NVIC寄存器

    USART_DMACmd(Usartx,USART_DMAReq_Tx,ENABLE);                               //使能串口1的DMA发送    
    DMA_Cmd(DMA_Streamx_Tx, DISABLE);                                          //关闭DMA传输 
    DMA_ITConfig(DMA_Streamx_Tx,DMA_IT_TC,ENABLE);                             //使能DMA的发送数据流中断
}

/**************************************
@功能             DMA的接收初始化配置
@调用关系           无
@输入参数           
@Usartx             指定的串口，如 USART1
@Par                DMA外设地址，如 (u32)&USART1->DR
@DMA_Streamx_rx     代表DMA1的接收数据流，DMA1_Stream0~DMA1_Stream7，参见手册
@DMA_Chx            代表DMA的通道选择，DMA_Channel_0~DMA_Channel_7
@Rx_Buf             代表DMA的接收缓冲区
@Rx_Size            代表接收数据个数
@返回值                无
@说明             无
***************************************/
void Init_Dma_Rx_Config2(USART_TypeDef *Usartx, u32 Par, DMA_Stream_TypeDef *DMA_Streamx_Rx, 
                        u32 DMA_Chx,u32 Rx_Buf, u16 Rx_Size)
{
    DMA_InitTypeDef DMA_InitStructure;        //定义DMA结构体
    DMA_DeInit(DMA_Streamx_Rx);                                              //将DMA的接收数据流设置为缺省值
    while (DMA_GetCmdStatus(DMA_Streamx_Rx) != DISABLE);                     //等待DMA的接收数据流可配置 

    //DMA的结构体设置 
    DMA_InitStructure.DMA_Channel = DMA_Chx;                                 //通道选择  
    DMA_InitStructure.DMA_PeripheralBaseAddr = Par;                          //DMA外设地址  
    DMA_InitStructure.DMA_Memory0BaseAddr = Rx_Buf;                          //DMA 存储器0地址  
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory ;                 //外设到存储器模式  
    DMA_InitStructure.DMA_BufferSize = Rx_Size;                              //数据传输量   
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;         //外设非增量模式  
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;                  //存储器增量模式  
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;  //外设数据长度:8位  
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;          //存储器数据长度:8位  
    DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;                            // 使用普通模式   
    DMA_InitStructure.DMA_Priority = DMA_Priority_Medium;                    //中等优先级  
    DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Enable;                   //初始化DMA_FIFOMode 
    DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_Full;            //FIFO 阈值选择。
    DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;              //存储器突发单次传输  
    DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;      //外设突发单次传输  
    DMA_Init(DMA_Streamx_Rx, &DMA_InitStructure);                            //初始化DMA Stream  

    USART_DMACmd(Usartx,USART_DMAReq_Rx,ENABLE);                             //使能串口的DMA接收   
    DMA_Cmd(DMA_Streamx_Rx, ENABLE);                                         //开启DMA传输    
}

/**************************************
@功能             DMA的发送使能
@调用关系           无
@输入参数           
@DMA_Streamx_tx     代表DMA的发送数据流，DMA1_Stream0~DMA1_Stream7 / DMA2_Stream0~DMA2_Stream7
@ndtr               代表DMA的发送数据个数
@返回值                无
@说明             无
***************************************/
void Dma_Send_Enable2(DMA_Stream_TypeDef *DMA_Streamx_tx,u16 ndtr)
{
    DMA_Cmd(DMA_Streamx_tx, DISABLE);                         //关闭DMA传输     
    while (DMA_GetCmdStatus(DMA_Streamx_tx) != DISABLE);        //确保DMA可以被设置        
    DMA_SetCurrDataCounter(DMA_Streamx_tx,ndtr);              //数据传输量   
    DMA_Cmd(DMA_Streamx_tx, ENABLE);                          //开启DMA传输 
}

/**************************************
@功能             DMA的发送完成中断函数
@调用关系           无
@输入参数           无
@返回值                无
@说明             无
***************************************/

void DMA1_Stream6_IRQHandler(void)  
{
    if(DMA_GetFlagStatus(DMA1_Stream6,DMA_FLAG_TCIF6)!=RESET)     //等待DMA2_Steam7传输完成  
    { 
        DMA_ClearFlag(DMA1_Stream6,DMA_FLAG_TCIF6);           //清除DMA2_Steam7传输完成标志 
        DMA_Cmd(DMA1_Stream6,DISABLE);                        //关闭DMA           
        DMA_ClearITPendingBit(DMA1_Stream6, DMA_IT_TCIF6);    //清除中断屏蔽位     
    }  
}  

/**************************************
@功能             串口2的空闲中断函数
@调用关系           无
@输入参数           无
@返回值             无
@说明               无
***************************************/
void USART2_IRQHandler(void)  
{  
    u16 i;
    led_red_toggle();
    if(USART_GetITStatus(USART2,USART_IT_IDLE) != RESET)            //如果接到一帧数据 
    {
        DMA_Cmd(DMA1_Stream5, DISABLE);                             //关闭DMA,防止处理其间有数据  
      
        //先读SR，再读DR，是为了清除IDLE中断
        i = USART2->SR;                                             //读SR寄存器
        i = USART2->DR;                                             //读DR寄存器
      
        rx_len2 = 6+BufferLen2 - DMA_GetCurrDataCounter(DMA1_Stream5);  //获得这一帧数据的长度//很奇怪要加6？2020/1/9
      
        if(rx_len2 !=0&&rx_len2<30)                                              //这一帧数据长度不为0
            rx_flag2 = 1;
        if(rx_flag2)    
        {
        rx_flag2 = 0;
        }
        for(i=0;i<rx_len2;i++)                                   //将接收缓冲区清零
        {
          ReceiveBuffer2[i]=0;
        }
        
        //清除DMA2_Steam5传输完成标志 
        DMA_ClearFlag(DMA1_Stream5,DMA_FLAG_TCIF5 | DMA_FLAG_FEIF5 | DMA_FLAG_DMEIF5 | DMA_FLAG_TEIF5 | DMA_FLAG_HTIF5);      
        DMA_SetCurrDataCounter(DMA1_Stream5, BufferLen2);            //设置DMA接收单元的长度
        DMA_Cmd(DMA1_Stream5, ENABLE);                              //打开DMA 
    }  
}










