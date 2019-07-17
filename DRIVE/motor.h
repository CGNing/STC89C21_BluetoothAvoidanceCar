#ifndef __MOTOR_H
#define __MOTOR_H

#include "userdef.h"

#define SPEED_DIV 100		//最大速度值

sbit P_IN1=P0^1;
sbit P_IN2=P0^2;
sbit P_IN3=P0^3;
sbit P_IN4=P0^4;

void motor_Init(void);
void move(int l,int r);

#endif
