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

void UART8_IRQHandler(void)//����2�жϴ�����
{    
  if(USART_GetITStatus(UART8, USART_IT_RXNE) != RESET)  //�����ж�(���յ������ݱ�����0x0d 0x0a��β)
    judgeBuffer[point]=USART_ReceiveData(UART8);//(USART1->DR); //��ȡ���\յ����?
}  			

void Uart8_Init(u32 bound)
{
  NVIC_InitTypeDef NVIC_InitStructure;
  GPIO_InitTypeDef GPIO_InitStructure;
  USART_InitTypeDef USART_InitStructure;
  //1.����ʱ�Ӻͺ�GPIOʱ��ʹ�ܡ�
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE,ENABLE); //ʹ��GPIOAʱ��
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART8,ENABLE);//ʹ��USART2ʱ��
  //2.�������Ÿ�����ӳ�� 
  GPIO_PinAFConfig(GPIOE,GPIO_PinSource0,GPIO_AF_UART8); //GPIOA2����ΪUSART2
  GPIO_PinAFConfig(GPIOE,GPIO_PinSource1,GPIO_AF_UART8); //GPIOA3����ΪUSART2  
  //3.GPIO�˿�ģʽ����
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0| GPIO_Pin_1; //GPIOA2��GPIOA3��ʼ��
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//���ù���
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; //�ٶ�50MHz
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //���츴�����
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; //����
  GPIO_Init(GPIOE,&GPIO_InitStructure); //��ʼ��GPIOA2����GPIOA3
  //4.���ڲ�����ʼ�������ò����ʣ��ֳ�����żУ��Ȳ���
  USART_InitStructure.USART_BaudRate = bound;//������һ������Ϊ9600;
  USART_InitStructure.USART_WordLength = USART_WordLength_8b;//�ֳ�Ϊ8λ���ݸ�ʽ
  USART_InitStructure.USART_StopBits = USART_StopBits_1;//һ��ֹͣλ
  USART_InitStructure.USART_Parity = USART_Parity_No;//����żУ��λ
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx; //�շ�ģʽ
  USART_Init(UART8, &USART_InitStructure); //��ʼ������2����
  //5.��ʼ��NVIC
  NVIC_InitStructure.NVIC_IRQChannel = UART8_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=1;//��ռ���ȼ�0
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0; //�����ȼ�3
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQͨ��ʹ��
  NVIC_Init(&NVIC_InitStructure); //����ָ���Ĳ�����ʼ��VIC�Ĵ���
  //6.�����ж�
  USART_ITConfig(UART8, USART_IT_RXNE, ENABLE);//�����ж�  
  //7.ʹ�ܴ��� 
  USART_Cmd(UART8, ENABLE);     
}


