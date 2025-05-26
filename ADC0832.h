#ifndef __ADC0832_H__
#define __ADC0832_H__

// 引脚定义
#define ADC_CS  P2_5
#define ADC_CLK P2_6
#define ADC_DIO P2_7

// 函数声明
void ADC0832_Init();
unsigned int ADC0832_ReadChannel(unsigned char channel);

#endif