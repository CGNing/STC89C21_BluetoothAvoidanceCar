#ifndef __USART_H
#define __USART_H

#include "userdef.h"

/*Define UART parity mode*/
#define NONE_PARITY     0   //None parity
#define ODD_PARITY      1   //Odd parity
#define EVEN_PARITY     2   //Even parity
#define MARK_PARITY     3   //Mark parity
#define SPACE_PARITY    4   //Space parity

#define PARITYBIT NONE_PARITY   //Testing even parity

extern unsigned char UART_DATA;
extern unsigned char UART_STATE;

void Uart_Init(unsigned int BAUD);			//串口初始化
void Uart_SendData(BYTE dat);						//发送一字节数据
void Uart_SendString(char *s);					//发送字符串(以\0结尾的数组)
void Uart_SendBytes(BYTE *s,WORD i);		//发送多个字节

#endif