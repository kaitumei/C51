#include <REGX52.H>
#include "Delay.h"
#include "Motor.h"
#include "LCD1602.h"
#include "ADC0832.h"
#include "Key.h"
#include "LightControl.h"
#include "UART.h"

#define uchar unsigned char

// 外部变量声明
extern uchar adc_value;    // ADC原始值 (LightControl.c)
extern int lux_value;      // 转换后的光照强度值 (LightControl.c)
extern bit uart_rx_flag;   // 串口接收标志 (UART.c)
extern uchar uart_rx_data; // 串口接收数据 (UART.c)

void main() {
    int last_key = NO_KEY_PRESSED;
    int current_light_state = -1;  // -1:初始状态, 0:正常光照, 1:强光照
    int new_light_state = -1;      // 新的光照状态
    int current_key;               // 当前按键状态变量
    
    // 初始化各模块
    ADC0832_Init();
    LCD_Init();
    UART_Init();
    
    // 初始显示光照信息
    display_light_info(0);
    
    // 发送系统启动信息
    UART_SendString("System Started\r\n");
    UART_SendString("Commands:\r\n");
    UART_SendString("C - Motor Clockwise\r\n");
    UART_SendString("A - Motor Counter-Clockwise\r\n");
    UART_SendString("S - Motor Stop\r\n");
    
    while(1) {
        // 处理串口接收到的命令
        if(UART_GetRxFlag())
        {
            UART_ProcessCommand(uart_rx_data);
            UART_ClearRxFlag();
        }
        
        // 获取当前按键状态
        current_key = get_key_status();
        
        // 处理按键事件
        handle_key_events(&last_key, current_key);
        
        // 读取并转换光照强度
        adc_value = ADC0832_ReadChannel(0);
        
        // 添加阈值判断，低于阈值时视为0
        if (adc_value < ADC_THRESHOLD) {
            adc_value = 0;
        }
        
        lux_value = (int)(adc_value * LUX_PER_ADC);
        
        // 更新显示
        update_light_display(lux_value);
        
        // 获取当前光照状态
        new_light_state = get_light_state(lux_value);
        
        // 处理光照状态
        process_light_state(new_light_state, &current_light_state);
        
        Delay_ms(200);
    }
}
