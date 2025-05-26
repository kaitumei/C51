#ifndef __KEY_H__
#define __KEY_H__

#include <REGX52.H>

// 定义按键引脚
sbit KEY_FWD = P3^0;  // 顺时针按键（低电平有效）
sbit KEY_REV = P3^1;  // 逆时针按键（低电平有效）

// 定义按键状态码
#define NO_KEY_PRESSED -1
#define ANTICLOCKWISE_KEY_PRESSED 0
#define CLOCKWISE_KEY_PRESSED 1
#define BOTH_KEYS_PRESSED 2

// 函数声明
int get_key_status(void);
void handle_key_events(int *last_key, int current_key);
void handle_fwd_key();
void handle_rev_key();

#endif