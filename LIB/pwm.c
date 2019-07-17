/***************************************
作者：CGN
文件名：pwm.c
创建日期：2019.4.8
描述：PWM生成函数
***************************************/

#include "reg52.h"
#include "pwm.h"

/* define variables */
u16 vaule_cycle=0;
u16 vaule_ch1=0;
u16 vaule_ch2=0;

u16 count_ch1=0;
u16 count_ch2=0;

void PWM_Init(unsigned int cycle){
	TMOD = TMOD & 0XF0 | 0X02;      //set timer0 as mode3 (8-bit)
	TH0 = TL0 = TSTEP;              //initial timer0 byte
	TR0 = 1;                        //timer0 start running
	ET0 = 1;                        //enable timer0 interrupt
	EA = 1;                         //open global interrupt switch
	vaule_cycle = cycle;            //initial counter
}

void PWM_CH1(unsigned int vaule){
	if ( vaule > vaule_cycle )
		vaule = vaule_cycle;
	vaule_ch1 = vaule;
}

void PWM_CH2(unsigned int vaule){
	if ( vaule > vaule_cycle )
		vaule = vaule_cycle;
	vaule_ch2 = vaule;
}

/* Timer0 interrupt routine */
void tm0_isr() interrupt 1
{
	count_ch1++;
	count_ch2++;
	
	if ( count_ch1 > vaule_cycle )
		count_ch1 = 0;
	if ( count_ch1 >= vaule_ch1 )
		P_CH1 = 0;
	else
		P_CH1 = 1;
	
	if ( count_ch2 > vaule_cycle )
		count_ch2 = 0;
	if ( count_ch2 >= vaule_ch2 )
		P_CH2 = 0;
	else
		P_CH2 = 1;
}
