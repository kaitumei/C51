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