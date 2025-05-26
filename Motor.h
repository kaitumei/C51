#ifndef __MOTOR_H__
#define __MOTOR_H__

#include <REGX52.H>

// 定义电机方向
#define DIRECTION_CW   1  // 顺时针
#define DIRECTION_CCW  0  // 逆时针

// 外部变量声明
extern unsigned char step_index;

// 外部数组声明
extern code unsigned char steps[8];

// 函数声明
void step_motor(unsigned char direction);

#endif