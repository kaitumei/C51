#ifndef __KEY_H__
#define __KEY_H__

#include <REGX52.H>

// 定义按键引脚
sbit KEY_DIRECTION = P3^3;  // 顺逆转动按钮（低电平有效）
sbit KEY_MODE = P3^5;       // 手动自动模式切换按钮（低电平有效）

// 定义按键状态码
#define NO_KEY_PRESSED -1
#define DIRECTION_KEY_PRESSED 0
#define MODE_KEY_PRESSED 1
#define BOTH_KEYS_PRESSED 2

// 定义工作模式
#define MODE_MANUAL 0  // 手动模式
#define MODE_AUTO 1    // 自动模式

// 定义转动方向
#define DIR_CLOCKWISE 1     // 顺时针
#define DIR_ANTICLOCKWISE 0 // 逆时针

// 全局变量声明
extern unsigned char current_mode;  // 当前工作模式
extern unsigned char current_direction;  // 当前转动方向

// 函数声明
int get_key_status(void);
void handle_key_events(int *last_key, int current_key);
void handle_direction_key();  // 处理方向切换按键
void handle_mode_key();       // 处理模式切换按键
void display_mode_status();   // 在LCD上显示当前模式和方向状态
void scroll_text(unsigned char line, unsigned char column, char *text, unsigned char max_len); // 滚动显示文本

#endif