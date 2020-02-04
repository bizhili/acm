#ifndef USART2_H
#define USART2_H
#include "main.h"
#include "sys.h"
#include "stm32f4xx.h"
extern uint8_t SendBuffer2[40];                   //DMA发送缓冲区
extern uint8_t ReceiveBuffer2[30];                //DMA接收缓冲区
extern void Init_Usart2(u32 Bound);
extern void Init_Dma_Tx_Config2(USART_TypeDef *Usartx,u32 Par, DMA_Stream_TypeDef *DMA_Streamx_Tx, 
                        u32 DMA_Chx,u32 Tx_Buf, u16 Tx_Size, u32 DMA_Streamx_Irqn);
extern void Init_Dma_Rx_Config2(USART_TypeDef *Usartx, u32 Par, DMA_Stream_TypeDef *DMA_Streamx_Rx, 
                        u32 DMA_Chx,u32 Rx_Buf, u16 Rx_Size);
extern void Dma_Send_Enable2(DMA_Stream_TypeDef *DMA_Streamx_tx,u16 ndtr);


#endif
