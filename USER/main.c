/***************************************
作者：CGN
文件名：main.c
创建日期：2019.4.8
描述：main主函数
管脚：	P_CH1=P0^0;
				P_IN1=P0^1;
				P_IN2=P0^2;
				P_IN3=P0^3;
				P_IN4=P0^4;
				P_CH2=P0^5;
				P_RED1=P0^6;
				P_RED2=P0^7;
				RXD=P3^0;
				TXD=P3^1;
***************************************/

#include "reg52.h"
#include "userdef.h"
#include "delay.h"
#include "uart.h"
#include "motor.h"
#include "jdy32.h"

sbit P_RED1=P0^6;
sbit P_RED2=P0^7;

u8 RED_STATE;
u32 AVOID_CNT=0;		//避障计数器
u32 BREAK_CNT=0;		//断线计数器

u8 DATA_BUF[2];
u8 DATA_STATE;

signed char SPEED[2]={0,0};

void main(void){
	motor_Init();
	JDY32_Init();
	
	P_RED1=1;
	P_RED2=1;
	
//	delay_ms(200);
//	Uart_SendString("Start\n");
	delay_ms(200);
	
	while(1){
		//蓝牙数据读取程序
		if(UART_STATE==1){
			UART_STATE=0;
			switch(DATA_STATE){
				
				case 0:
					if(UART_DATA==0X7F)
						DATA_STATE=1;
					BREAK_CNT=1;
					break;
					
				case 1:
					DATA_BUF[0]=UART_DATA;
					BREAK_CNT=1;
					DATA_STATE=2;
					break;
				
				case 2:
					DATA_BUF[1]=UART_DATA;
					SPEED[0]=(signed char)DATA_BUF[0];
					SPEED[1]=(signed char)DATA_BUF[1];
					move(SPEED[0],SPEED[1]);
				
					Uart_SendString("ok\n");
					BREAK_CNT=1;
					AVOID_CNT=0;
					DATA_STATE=0;
					break;
				
				default:
					BREAK_CNT=1;
					DATA_STATE=0;
			}
		}
		else if(UART_STATE==2){
			UART_STATE=0;
			Uart_SendString("err\n");
		}
		
		//避障程序
		RED_STATE=((!(unsigned char)P_RED1)<<1)|(!(unsigned char)P_RED2);
		
		switch(RED_STATE){
			case 0X03:
				move(-(SPEED[0]&0X7F),(SPEED[1]&0X7F));
				AVOID_CNT=1;
				break;
			case 0X02:
				move(0,-(SPEED[0]&0X7F));
				AVOID_CNT=1;
				break;
			case 0X01:
				move(-(SPEED[0]&0X7F),0);
				AVOID_CNT=1;
				break;

		}
		if(AVOID_CNT>2000){
			AVOID_CNT=0;
			move(SPEED[0],SPEED[1]);
		}
		else if(AVOID_CNT>0)
			AVOID_CNT++;
		
		//失控保护程序
		if(BREAK_CNT>100000){
			BREAK_CNT=0;
			Uart_SendString("stop\n");
			move(0,0);
		}
		else if(BREAK_CNT>0)
			BREAK_CNT++;
	}
}
