#include "UART.h"
#include <REGX52.H>
#include "Motor.h"  // 添加电机控制头文件
#include "Delay.h"  // 添加延时函数头文件

// 定义接收标志
bit uart_rx_flag = 0;  // 串口接收标志
uchar uart_rx_data = 0; // 接收到的数据

/**
 * @brief 串口初始化，设置为模式1，波特率9600
 * @param 无
 * @retval 无
 * @note 晶振11.0592MHz
 */
void UART_Init(void)
{
    // 设置串口工作模式1(8位UART)
    SCON = 0x50;    // 0101 0000 - SM0=0,SM1=1(模式1),REN=1(允许接收)
    
    // 设置定时器1为模式2(8位自动重载)
    TMOD &= 0x0F;   // 清除定时器1的模式位
    TMOD |= 0x20;   // 设置定时器1为模式2
    
    // 计算重装值以获得9600波特率(11.0592MHz晶振)
    // 9600波特率，11.0592MHz晶振，计算方法：256 - (11059200/12/32/9600)
    TH1 = 0xFD;     // 设置重装值为0xFD
    TL1 = 0xFD;
    
    // 打开定时器1
    TR1 = 1;        // 启动定时器1
    
    // 设置中断
    ES = 1;         // 开启串口中断
    EA = 1;         // 开启总中断
    
    // 初始化接收标志
    uart_rx_flag = 0;
}

/**
 * @brief 发送一个字节数据
 * @param dat 要发送的数据
 * @retval 无
 */
void UART_SendByte(uchar dat)
{
    SBUF = dat;     // 将数据写入SBUF
    while(!TI);     // 等待发送完成(TI=1)
    TI = 0;         // 清除发送完成标志
}

/**
 * @brief 发送字符串
 * @param str 要发送的字符串指针
 * @retval 无
 */
void UART_SendString(uchar *str)
{
    while(*str != '\0')
    {
        UART_SendByte(*str++);
    }
}

/**
 * @brief 接收一个字节数据(阻塞方式)
 * @param 无
 * @retval 接收到的数据
 */
uchar UART_ReceiveByte(void)
{
    while(!RI);     // 等待接收完成(RI=1)
    RI = 0;         // 清除接收完成标志
    return SBUF;    // 返回接收到的数据
}

/**
 * @brief 获取接收标志状态
 * @param 无
 * @retval 1表示有数据接收，0表示无数据
 */
bit UART_GetRxFlag(void)
{
    return uart_rx_flag;
}

/**
 * @brief 清除接收标志
 * @param 无
 * @retval 无
 */
void UART_ClearRxFlag(void)
{
    uart_rx_flag = 0;
}

/**
 * @brief 处理串口接收到的命令
 * @param cmd 接收到的命令字符
 * @retval 无
 */
void UART_ProcessCommand(uchar cmd)
{
    unsigned char i;
    switch(cmd)
    {
        case CMD_MOTOR_CW:  // 电机顺时针旋转
            // 执行24步旋转（与按键功能一致）
            for(i = 0; i < 24; i++) {
                step_motor(DIRECTION_CW);
                Delay_ms(20);  // 控制转速
            }
            UART_SendString("Motor: Clockwise\r\n");
            break;
        
        case CMD_MOTOR_CCW: // 电机逆时针旋转
            // 执行24步旋转（与按键功能一致）
            for(i = 0; i < 24; i++) {
                step_motor(DIRECTION_CCW);
                Delay_ms(20);  // 控制转速
            }
            UART_SendString("Motor: Counter-Clockwise\r\n");
            break;
        
        case CMD_MOTOR_STOP: // 电机停止
            Motor_Stop();
            UART_SendString("Motor: Stop\r\n");
            break;
        
        default:
            UART_SendString("Unknown Command\r\n");
            break;
    }
}

/**
 * @brief 串口中断服务函数
 * @note 此中断函数需要在主程序中打开相应的中断允许位
 */
void UART_Routine() interrupt 4
{
    if(RI)
    {
        // 接收到数据，保存并设置标志位
        uart_rx_data = SBUF;
        uart_rx_flag = 1;
        RI = 0;     // 清除接收中断标志
    }
}
