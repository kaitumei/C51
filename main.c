#include <REGX52.H>
#include "Delay.h"
#include "Motor.h"
#include "LCD1602.h"
#include "ADC0832.h"
#include "Key.h"

#define uchar unsigned char

uchar light_value; // 光照强度变量
uchar threshold = 128; // 初始阈值

/**
 * @brief 主函数
 */
void main() {
    int last_key = NO_KEY_PRESSED;  // 上一次按键状态

    // 初始化ADC0832和LCD1602
    ADC0832_Init();
    LCD_Init();

    while (1) {
        int current_key = get_key_status();
        handle_key_events(&last_key, current_key, &threshold);

        // 读取光照强度
        light_value = ADC0832_ReadChannel(0); // 假设光照传感器连接到通道0

        // 根据光照强度与阈值的比较结果控制电机旋转
        if (light_value > threshold) {
            step_motor(DIRECTION_CW);
        } else {
            step_motor(DIRECTION_CCW);
        }

        // 在LCD1602上显示光照强度及阈值
        LCD_ShowString(1, 1, "Light:");
        LCD_ShowNum(1, 7, light_value, 3);
        LCD_ShowString(1, 10, "lux");

        LCD_ShowString(2, 1, "Thres:");
        LCD_ShowNum(2, 7, threshold, 3);
        LCD_ShowString(2, 10, "lux");
    }
}