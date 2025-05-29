#ifndef __LIGHT_CONTROL_H__
#define __LIGHT_CONTROL_H__

#include <REGX52.H>

// 定义光照相关常量
// 假设传感器ADC值0-255对应0-1000 Lux
#define LUX_PER_ADC (1000.0 / 255.0)
// 定义ADC阈值，低于此值视为0
#define ADC_THRESHOLD 3
// 光照强度阈值
#define STRONG_LIGHT 700
#define NORMAL_LIGHT_MIN 300
#define NORMAL_LIGHT_MAX 699
// 步进电机控制参数
#define STEPS_PER_REVOLUTION 8  // 一圈步数
#define REVOLUTIONS 10

// 光照强度等级文本
extern const char *light_level_text[];

// 全局变量声明
extern bit motor_running;  // 电机运行状态标志
extern bit show_mode_flag; // 显示模式标志
extern unsigned int mode_display_counter; // 模式显示计数器

// 函数声明
const char* get_light_level_text(int lux);
void rotate_motor(unsigned char direction, unsigned int revolutions);
void display_light_info(int lux);
int get_light_state(int lux_value);
void update_light_display(int lux_value);
void process_light_state(int new_light_state, int *current_light_state);

#endif 