#include <REGX52.H>
#include "Delay.h"
#include "Motor.h"

// 步进电机相序表 (半步模式)
unsigned char code steps[8] = {0x03, 0x01, 0x09, 0x08, 0x0C, 0x04, 0x06, 0x02};

// 静态变量记录当前步进索引
unsigned char step_index = 0;

/**
 * @brief 控制电机单步旋转
 * @param direction 旋转方向 (DIRECTION_CW/DIRECTION_CCW)
 */
void step_motor(unsigned char direction) {
    if (direction == DIRECTION_CW) {
        step_index = (step_index + 1) % 8;
    } else {
        step_index = (step_index + 7) % 8;
    }
    P1 = steps[step_index];
    Delay_ms(20);  // 控制步进速度
}

/**
 * @brief 设置左右电机转向
 * @param left_direction 左电机方向 (1:正转, 0:反转)
 * @param right_direction 右电机方向 (1:正转, 0:反转)
 */
void Motor_SetDirection(unsigned char left_direction, unsigned char right_direction) {
    // 设置左电机方向
    if (left_direction) {
        // 左电机正转
        P1_0 = 1;
        P1_1 = 0;
    } else {
        // 左电机反转
        P1_0 = 0;
        P1_1 = 1;
    }
    
    // 设置右电机方向
    if (right_direction) {
        // 右电机正转
        P1_2 = 1;
        P1_3 = 0;
    } else {
        // 右电机反转
        P1_2 = 0;
        P1_3 = 1;
    }
}

/**
 * @brief 停止所有电机
 */
void Motor_Stop(void) {
    // 停止左电机
    P1_0 = 0;
    P1_1 = 0;
    
    // 停止右电机
    P1_2 = 0;
    P1_3 = 0;
}