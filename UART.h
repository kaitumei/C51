#ifndef __UART_H__
#define __UART_H__

#include <REGX52.H>

// 定义数据类型
#define uchar unsigned char
#define uint  unsigned int

// 波特率定义
#define BAUD_RATE 9600

// 串口命令定义
#define CMD_MOTOR_CW    'C'  // 电机顺时针旋转
#define CMD_MOTOR_CCW   'A'  // 电机逆时针旋转
#define CMD_MOTOR_STOP  'S'  // 电机停止

// 函数声明
void UART_Init(void);         // 串口初始化
void UART_SendByte(uchar dat); // 发送一个字节
void UART_SendString(uchar *str); // 发送字符串
uchar UART_ReceiveByte(void);  // 接收一个字节(阻塞方式)
bit UART_GetRxFlag(void);     // 获取接收标志
void UART_ClearRxFlag(void);  // 清除接收标志
void UART_ProcessCommand(uchar cmd); // 处理串口命令

#endif
