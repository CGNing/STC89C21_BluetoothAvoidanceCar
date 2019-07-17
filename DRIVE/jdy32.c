/***************************************
作者：CGN
文件名：jdy32.c
创建日期：2019.4.8
描述：JDY32蓝牙模块驱动程序
***************************************/

#include "reg52.h"
#include "jdy32.h"
#include "uart.h"

void JDY32_Init(void){
	Uart_Init(9600);
}
