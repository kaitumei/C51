#include <REGX52.H>
#include "Delay.h"
#include "Motor.h"
#include "LCD1602.h"
#include "ADC0832.h"
#include "Key.h"

#define uchar unsigned char

uchar light_value; // 光照强度变量

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
        handle_key_events(&last_key, current_key);

        // 读取光照强度
        light_value = ADC0832_ReadChannel(0); // 假设光照传感器连接到通道0

        // 在LCD1602上显示光照强度
        LCD_ShowString(1, 1, "Light:");
        LCD_ShowNum(1, 7, light_value, 3);
    }
}