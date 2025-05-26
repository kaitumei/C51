#include <REGX52.H>
#include "Delay.h"
#include "Motor.h"
#include "Key.h"

/**
 * @brief 获取按键状态并进行消抖处理
 * @return 返回按键状态码：
 *         -1: 无按键
 *         0: 逆时针按键按下
 *         1: 顺时针按键按下
 *         2: 两个按键同时按下
 *         3: 阈值上调按键按下
 *         4: 阈值下调按键按下
 */
int get_key_status(void) {
    if (KEY_FWD == 0 && KEY_REV == 0) return BOTH_KEYS_PRESSED;  // 同时按下
    if (KEY_FWD == 0) return CLOCKWISE_KEY_PRESSED;              // 顺时针按下
    if (KEY_REV == 0) return ANTICLOCKWISE_KEY_PRESSED;          // 逆时针按下
    if (KEY_UP == 0) return THRESHOLD_UP_KEY_PRESSED;            // 阈值上调
    if (KEY_DOWN == 0) return THRESHOLD_DOWN_KEY_PRESSED;        // 阈值下调
    return NO_KEY_PRESSED;                                       // 无按键
}

/**
 * @brief 处理顺时针按键事件
 */
void handle_fwd_key() {
    unsigned char i;
    // 连续执行24步顺时针旋转
    for(i = 0; i < 24; i++) {
        step_motor(DIRECTION_CW);  
        Delay_ms(20);  // 保持原有转速控制
    }
}

/**
 * @brief 处理逆时针按键事件
 */
void handle_rev_key() {
    unsigned char i;
    // 连续执行24步逆时针旋转
    for(i = 0; i < 24; i++) {
        step_motor(DIRECTION_CCW); 
        Delay_ms(20);  // 保持原有转速控制
    }
}

/**
 * @brief 处理按键事件
 * @param last_key 上一次按键状态
 * @param current_key 当前按键状态
 * @param threshold 当前阈值
 */
void handle_key_events(int *last_key, int current_key, unsigned char *threshold) {
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
        if (current_key == CLOCKWISE_KEY_PRESSED && KEY_FWD == 0) {
            handle_fwd_key();
        } else if (current_key == ANTICLOCKWISE_KEY_PRESSED && KEY_REV == 0) {
            handle_rev_key();
        } else if (current_key == THRESHOLD_UP_KEY_PRESSED && KEY_UP == 0) {
            if (*threshold < 255) {
                (*threshold)++;
            }
        } else if (current_key == THRESHOLD_DOWN_KEY_PRESSED && KEY_DOWN == 0) {
            if (*threshold > 0) {
                (*threshold)--;
            }
        }
    }

    *last_key = current_key;
}