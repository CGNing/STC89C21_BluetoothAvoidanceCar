/***************************************
作者：CGN
文件名：uart.c
创建日期：2019.4.8
描述：uart调用程序
***************************************/

#include "reg52.h"
#include "intrins.h"
#include "uart.h"

#if (PARITYBIT == ODD_PARITY) || (PARITYBIT == EVEN_PARITY) || (PARITYBIT == MARK_PARITY)
sbit bit9 = P2^2;           //P2.2 show UART data bit9
#endif

unsigned char UART_DATA=0;
unsigned char UART_STATE=0;

bit busy;

void Uart_Init(unsigned int BAUD){
#if (PARITYBIT == NONE_PARITY)
    SCON = 0x50;            							//8-bit variable UART
#elif (PARITYBIT == ODD_PARITY) || (PARITYBIT == EVEN_PARITY) || (PARITYBIT == MARK_PARITY)
    SCON = 0xda;            							//9-bit variable UART, parity bit initial to 1
#elif (PARITYBIT == SPACE_PARITY)
    SCON = 0xd2;            							//9-bit variable UART, parity bit initial to 0
#endif

    TMOD = TMOD &0X0F | 0x20;							//Set Timer1 as 8-bit auto reload mode
    TH1 = TL1 = -(FOSC/12/32/BAUD); 			//Set auto-reload vaule
    TR1 = 1;															//Timer1 start run
    ES = 1;																//Enable UART interrupt
    EA = 1;																//Open master interrupt switch
}

/*----------------------------
UART interrupt service routine
----------------------------*/
void Uart_Isr() interrupt 4{
    if (RI){
        RI = 0;             //Clear receive interrupt flag
			
				UART_DATA = SBUF;
				if (UART_STATE==0)
					UART_STATE=1;
				else
					UART_STATE=2;
				
//				P0 = UART_DATA;      //P0 show UART UART
#if (PARITYBIT == ODD_PARITY) || (PARITYBIT == EVEN_PARITY) || (PARITYBIT == MARK_PARITY)
        bit9 = RB8;         //P2.2 show parity bit
#endif
    }
    if (TI){
        TI = 0;             //Clear transmit interrupt flag
        busy = 0;           //Clear transmit busy flag
    }
}

/*----------------------------
Send a byte data to UART
Input: dat (data to be sent)
Output:None
----------------------------*/
void Uart_SendData(BYTE dat){
    while (busy);           //Wait for the completion of the previous data is sent
    ACC = dat;              //Calculate the even parity bit P (PSW.0)

#if (PARITYBIT == ODD_PARITY) || (PARITYBIT == EVEN_PARITY) || (PARITYBIT == MARK_PARITY)

    if (P)                  //Set the parity bit according to P
    {
#if (PARITYBIT == ODD_PARITY)
        TB8 = 0;            //Set parity bit to 0
#elif (PARITYBIT == EVEN_PARITY)
        TB8 = 1;            //Set parity bit to 1
#endif
    }
    else{
#if (PARITYBIT == ODD_PARITY)
        TB8 = 1;            //Set parity bit to 1
#elif (PARITYBIT == EVEN_PARITY)
        TB8 = 0;            //Set parity bit to 0
#endif
    }

#endif

    busy = 1;
    SBUF = ACC;             //Send data to UART buffer
}

/*----------------------------
Send a string to UART
Input: s (address of string)
Output:None
----------------------------*/
void Uart_SendString(char *s){
	while (*s)              //Check the end of the string
		Uart_SendData(*s++);       //Send current char and increment string ptr
}

void Uart_SendBytes(BYTE *s,WORD i){
	while (i--)
		Uart_SendData(*s++);
}
