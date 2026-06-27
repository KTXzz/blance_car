#ifndef __APP_bluetooth_H
#define __APP_bluetooth_H

#include "main.h"


/* 蓝牙解析后的统一数据 */
typedef struct
{
    uint8_t enable;     // 电机开关
    uint8_t mode;       // 0:PID 1:遥控
    uint8_t pid_type;   // 0/1/2

    float data1;        // KP / Speed
    float data2;        // KI / Turn
    float data3;        // KD / reserve

}BT_APP_Data_t;


/* 初始化 */
void BT_APP_Init(void);

/* APP任务 */
void BT_APP_proc(void);

/* 数据获取 */
BT_APP_Data_t* BT_APP_GetData(void);

/* 更新标志 */
uint8_t BT_APP_IsUpdated(void);

#endif