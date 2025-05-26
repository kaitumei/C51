#ifndef __KEY_H__
#define __KEY_H__

#include <REGX52.H>

// 定义按键引脚
sbit KEY_FWD = P3^0;  // 顺时针按键（低电平有效）
sbit KEY_REV = P3^1;  // 逆时针按键（低电平有效）
sbit KEY_UP = P3^2;    // 阈值上调按键
sbit KEY_DOWN = P3^3;  // 阈值下调按键

// 定义按键状态码
#define NO_KEY_PRESSED -1
#define ANTICLOCKWISE_KEY_PRESSED 0
#define CLOCKWISE_KEY_PRESSED 1
#define BOTH_KEYS_PRESSED 2
#define THRESHOLD_UP_KEY_PRESSED 3
#define THRESHOLD_DOWN_KEY_PRESSED 4

// 函数声明
int get_key_status(void);
void handle_key_events(int *last_key, int current_key, unsigned char *threshold);
void handle_fwd_key();
void handle_rev_key();

#endif