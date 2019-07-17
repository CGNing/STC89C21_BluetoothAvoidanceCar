/***************************************
作者：CGN
文件名：motor.c
创建日期：2019.4.8
描述：马达驱动函数
***************************************/

#include "reg52.h"
#include "motor.h"
#include "pwm.h"

void motor_Init(void){
	PWM_Init(SPEED_DIV);
	move(0,0);
}

void move(int l,int r){
	
	if(l>0){
		P_IN1=1;
		P_IN2=0;
	}
	else if(l<0){
		P_IN1=0;
		P_IN2=1;
		l=-l;
	}
	else{
		P_IN1=0;
		P_IN2=0;
	}
	
		if(r>0){
		P_IN3=1;
		P_IN4=0;
	}
	else if(r<0){
		P_IN3=0;
		P_IN4=1;
		r=-r;
	}
	else{
		P_IN3=0;
		P_IN4=0;
	}
	
	PWM_CH1(l);
	PWM_CH2(r);
}
