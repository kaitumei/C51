#include <REGX52.h>
#include "ADC0832.h"
#include "Delay.h"

// 初始化ADC0832
void ADC0832_Init() {
    ADC_CS = 1;  // 初始状态为高电平（未选中）
    ADC_CLK = 0; // 时钟初始为低电平
}

// 读取指定通道的ADC值
unsigned int ADC0832_ReadChannel(unsigned char channel) {
    unsigned char i;
    unsigned int adc_value = 0;
    
    // 开始转换
    ADC_CS = 0;  // 选中ADC
    Delay_us(2); // 延时等待稳定
    
    // 发送起始位
    ADC_CLK = 0;
    ADC_DIO = 1;
    ADC_CLK = 1;
    Delay_us(2);
    
    // 发送SGL/DIF位（1表示单端输入）
    ADC_CLK = 0;
    ADC_DIO = 1;
    ADC_CLK = 1;
    Delay_us(2);
    
    // 发送通道选择位
    ADC_CLK = 0;
    ADC_DIO = (channel & 0x01);
    ADC_CLK = 1;
    Delay_us(2);
    
    // 释放数据线
    ADC_CLK = 0;
    ADC_DIO = 1;
    
    // 读取8位数据（MSB优先）
    for(i = 0; i < 8; i++) {
        adc_value <<= 1;
        ADC_CLK = 1;
        Delay_us(2);
        ADC_CLK = 0;
        if(ADC_DIO) adc_value |= 0x01;
        Delay_us(2);
    }
    
    // 结束转换
    ADC_CS = 1;  // 取消选中
    return adc_value;
}    