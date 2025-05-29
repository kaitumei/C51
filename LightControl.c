#include <REGX52.H>
#include "Delay.h"
#include "Motor.h"
#include "LCD1602.h"
#include "ADC0832.h"
#include "Key.h"
#include "LightControl.h"

#define uchar unsigned char

// 全局变量定义
bit motor_running = 0;  // 电机运行状态标志
bit show_mode_flag = 0; // 显示模式标志
unsigned int mode_display_counter = 0; // 模式显示计数器
uchar adc_value = 0;    // ADC原始值
int lux_value = 0;      // 转换后的光照强度值

// 光照强度等级文本
const char *light_level_text[] = {"Low", "Medium", "High"};

/**
 * @brief 获取光照等级文本
 * @param lux 光照强度值
 * @return 对应的光照等级文本
 */
const char* get_light_level_text(int lux) {
    if (lux < NORMAL_LIGHT_MIN) return light_level_text[0]; // 0-299 Low
    else if (lux < STRONG_LIGHT) return light_level_text[1]; // 300-699 Medium
    else return light_level_text[2]; // 700+ High
}

/**
 * @brief 控制步进电机转动指定圈数
 * @param direction 转动方向
 * @param revolutions 转动圈数
 */
void rotate_motor(unsigned char direction, unsigned int revolutions) {
    unsigned int i, j;
    motor_running = 1;
    
    for(i = 0; i < revolutions; i++) {
        for(j = 0; j < STEPS_PER_REVOLUTION; j++) {
            step_motor(direction);
        }
    }
    
    motor_running = 0;
}

/**
 * @brief 显示光照强度信息
 * @param lux 光照强度值
 */
void display_light_info(int lux) {
    // 显示光照强度
    LCD_ShowString(1, 1, "Light:    Lx    ");
    LCD_ShowNum(1, 7, lux, 4);
    
    // 清空第二行
    LCD_ShowString(2, 1, "                ");
}

/**
 * @brief 根据光照强度获取光照状态
 * @param lux_value 光照强度值
 * @return 光照状态：-1(其他光照)，0(正常光照)，1(强光)
 */
int get_light_state(int lux_value) {
    if (lux_value >= STRONG_LIGHT) {
        return 1;  // 强光
    } else if (lux_value >= NORMAL_LIGHT_MIN && lux_value <= NORMAL_LIGHT_MAX) {
        return 0;  // 正常光照
    } else {
        return -1; // 其他光照（不处理）
    }
}

/**
 * @brief 更新光照显示
 * @param lux_value 光照强度值
 */
void update_light_display(int lux_value) {
    // 根据show_mode_flag决定显示模式还是光照信息
    if (show_mode_flag) {
        // 模式信息已在Key.c的display_mode_status()中显示
        // 只需计数器进行计时
        mode_display_counter++;
        if (mode_display_counter >= 10) {  // 约2秒后切回光照信息
            show_mode_flag = 0;
            mode_display_counter = 0;
            // 切回光照信息显示
            display_light_info(lux_value);
        }
    } else {
        // 显示光照信息
        LCD_ShowNum(1, 7, lux_value, 4);
    }
}

/**
 * @brief 处理光照状态变化
 * @param new_light_state 新的光照状态
 * @param current_light_state 当前光照状态(引用)
 */
void process_light_state(int new_light_state, int *current_light_state) {
    // 在自动模式下根据光照强度控制步进电机
    if (current_mode == MODE_AUTO && !motor_running && new_light_state != *current_light_state) {
        *current_light_state = new_light_state;
        
        if (new_light_state == 1) {
            // 光照强，固定顺时针转动
            rotate_motor(DIR_CLOCKWISE, REVOLUTIONS);
        } 
        else if (new_light_state == 0) {
            // 光照正常，固定逆时针转动
            rotate_motor(DIR_ANTICLOCKWISE, REVOLUTIONS);
        }
    }
} 