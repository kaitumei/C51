#include <REGX52.H>
#include "Delay.h"
#include "Motor.h"
#include "Key.h"
#include "LCD1602.h"
#include "LightControl.h"

// 定义全局变量
unsigned char current_mode = MODE_MANUAL;      // 默认为手动模式
unsigned char current_direction = DIR_CLOCKWISE; // 默认为顺时针方向
unsigned char scroll_position = 0;             // 滚动位置
unsigned char scroll_counter = 0;              // 滚动计数器

// 引用外部变量（定义在LightControl.c中）
extern bit show_mode_flag;
extern unsigned int mode_display_counter;

/**
 * @brief 在LCD上滚动显示文本
 * @param line 行号(1或2)
 * @param column 起始列号(1-16)
 * @param text 要显示的文本
 * @param max_len 显示区域最大长度
 */
void scroll_text(unsigned char line, unsigned char column, char *text, unsigned char max_len) {
    unsigned char i;
    unsigned char text_len = 0;
    unsigned char display_len;
    char display_text[17] = {0}; // 最大16个字符+结束符
    
    // 计算文本实际长度
    while(text[text_len] != '\0') text_len++;
    
    // 如果文本长度小于等于最大显示长度，无需滚动
    if(text_len <= max_len) {
        LCD_ShowString(line, column, text);
        return;
    }
    
    // 确定要显示的长度
    display_len = (text_len < max_len) ? text_len : max_len;
    
    // 拷贝要显示的部分
    for(i = 0; i < display_len; i++) {
        display_text[i] = text[(i + scroll_position) % text_len];
    }
    display_text[display_len] = '\0'; // 字符串结束符
    
    // 显示文本
    LCD_ShowString(line, column, display_text);
    
    // 更新滚动位置
    scroll_counter++;
    if(scroll_counter >= 2) { // 每隔一定时间滚动一次
        scroll_counter = 0;
        scroll_position = (scroll_position + 1) % text_len;
    }
}

/**
 * @brief 获取按键状态并进行消抖处理
 * @return 返回按键状态码：
 *         -1: 无按键
 *         0: 方向按键按下
 *         1: 模式按键按下
 *         2: 两个按键同时按下
 */
int get_key_status(void) {
    if (KEY_DIRECTION == 0 && KEY_MODE == 0) return BOTH_KEYS_PRESSED;  // 同时按下
    if (KEY_DIRECTION == 0) return DIRECTION_KEY_PRESSED;               // 方向键按下
    if (KEY_MODE == 0) return MODE_KEY_PRESSED;                         // 模式键按下
    return NO_KEY_PRESSED;                                              // 无按键
}

/**
 * @brief 处理方向切换按键
 */
void handle_direction_key() {
    // 切换方向
    current_direction = !current_direction;
    
    // 根据当前模式和方向执行相应操作
    if (current_mode == MODE_MANUAL) {
        unsigned char i;
        // 手动模式下执行单步操作（24步）
        for(i = 0; i < 24; i++) {
            step_motor(current_direction);
            Delay_ms(20);  // 控制转速
        }
    }
}

/**
 * @brief 处理模式切换按键
 */
void handle_mode_key() {
    // 切换模式
    current_mode = !current_mode;
    
    // 更新LCD显示模式信息
    display_mode_status();
    
    // 设置显示模式标志，使main函数显示模式信息一段时间
    show_mode_flag = 1;
    mode_display_counter = 0;
}

/**
 * @brief 在LCD上显示当前模式状态
 */
void display_mode_status() {
    // 清空LCD第一行和第二行
    LCD_ShowString(1, 1, "                ");
    LCD_ShowString(2, 1, "                ");
    
    // 居中显示模式文本
    LCD_ShowString(1, 3, "Current Mode:");
    
    if (current_mode == MODE_MANUAL) {
        LCD_ShowString(2, 5, "MANUAL");
    } else {
        LCD_ShowString(2, 6, "AUTO");
    }
}

/**
 * @brief 处理按键事件
 * @param last_key 上一次按键状态
 * @param current_key 当前按键状态
 */
void handle_key_events(int *last_key, int current_key) {
    // 如果当前没有按键按下，重置状态
    if (current_key == NO_KEY_PRESSED) {
        *last_key = NO_KEY_PRESSED;
        return;
    }

    // 如果当前两个键都按下，忽略动作
    if (current_key == BOTH_KEYS_PRESSED) {
        *last_key = BOTH_KEYS_PRESSED;
        return;
    }

    // 如果当前只有一个键按下，并且上一次未按下或不是同一种按键
    if (*last_key != current_key && *last_key != BOTH_KEYS_PRESSED) {
        // 延时5ms去抖动
        Delay_ms(5);

        // 如果当前按键仍然处于按下状态
        if (current_key == DIRECTION_KEY_PRESSED && KEY_DIRECTION == 0) {
            handle_direction_key();
        } else if (current_key == MODE_KEY_PRESSED && KEY_MODE == 0) {
            handle_mode_key();
        }
    }

    *last_key = current_key;
}