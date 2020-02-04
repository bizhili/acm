#ifndef USART6_DMA_H
#define USART6_DMA_H
#include "sys.h"
#include "stm32f4xx.h"
extern u16 rx_len;
extern u8 SendBuffer[30];
extern u8 ReceiveBuffer[30];
extern u16 BufferLen;
extern u8 rx_flag; 


extern void Init_Usart6(u32 Bound);
extern void Init_Dma_Tx_Config(USART_TypeDef *Usartx,u32 Par, DMA_Stream_TypeDef *DMA_Streamx_Tx, u32 DMA_Chx,u32 Tx_Buf, u16 Tx_Size, u32 DMA_Streamx_Irqn);
extern void Init_Dma_Rx_Config(USART_TypeDef *Usartx, u32 Par, DMA_Stream_TypeDef *DMA_Streamx_Rx, u32 DMA_Chx,u32 Rx_Buf, u16 Rx_Size);
extern void Dma_Send_Enable(DMA_Stream_TypeDef *DMA_Streamx_tx,u16 ndtr);
extern void DMA2_Stream7_IRQHandler(void);
extern void USART6_IRQHandler(void);


#endif
