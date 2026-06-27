#include "APP_bluetooth.h"
#include "bluetooth.h"   // BSP层
#include "pid.h"
#include "control.h"


/* APP层缓存 */
static BT_APP_Data_t bt_app_data;
static uint8_t bt_app_update = 0;


/*************************************************
 * 初始化
 *************************************************/
void BT_APP_Init(void)
{
    bt_app_update = 0;
}


/*************************************************
 * APP任务（核心）
 * 在main while(1)里调用
 *************************************************/
void BT_APP_proc(void)
{
    /* 没有新数据直接返回 */
    if(Bluetooth_IsUpdated() == 0)
        return;

    /* 从BSP获取原始数据 */
    BT_Data_t *bt = Bluetooth_GetData();


    /* ===== 1. 电机使能 ===== */
    motor_enable = (uint8_t)bt->enable;


    /* ===== 2. 模式判断 ===== */
    bt_app_data.enable   = bt->enable;
    bt_app_data.mode     = bt->mode;
    bt_app_data.pid_type = bt->pid_type;
    bt_app_data.data1    = bt->data1;
    bt_app_data.data2    = bt->data2;
    bt_app_data.data3    = bt->data3;

    bt_app_update = 1;


    /* ===== 3. PID模式 ===== */
    if(bt->mode == 0)
    {
        switch(bt->pid_type)
        {
            case 0:   // 平衡环
                balance_pid.kp = bt->data1;
                balance_pid.ki = bt->data2;
                balance_pid.kd = bt->data3;
                break;

            case 1:   // 速度环
                speed_pid.kp = bt->data1;
                speed_pid.ki = bt->data2;
                speed_pid.kd = bt->data3;
                break;

            case 2:   // 转向环
                turn_pid.kp = bt->data1;
                turn_pid.ki = bt->data2;
                turn_pid.kd = bt->data3;
                break;
        }
    }


    /* ===== 4. 遥控模式 ===== */
    else if(bt->mode == 1)
    {
        target_speed = bt->data1;
        target_turn  = bt->data2;
    }
}


/*************************************************
 * 获取数据
 *************************************************/
BT_APP_Data_t* BT_APP_GetData(void)
{
    return &bt_app_data;
}


/*************************************************
 * 便捷数据获取
 *************************************************/
uint8_t BT_APP_GetMotorEnable(void)
{
    return bt_app_data.enable;
}


uint8_t BT_APP_GetMode(void)
{
    return bt_app_data.mode;
}


uint8_t BT_APP_GetPidType(void)
{
    return bt_app_data.pid_type;
}


float BT_APP_GetData1(void)
{
    return bt_app_data.data1;
}


float BT_APP_GetData2(void)
{
    return bt_app_data.data2;
}


float BT_APP_GetData3(void)
{
    return bt_app_data.data3;
}


float BT_APP_GetTargetSpeed(void)
{
    if(bt_app_data.mode == 1)
        return bt_app_data.data1;
    return 0;
}


float BT_APP_GetTargetTurn(void)
{
    if(bt_app_data.mode == 1)
        return bt_app_data.data2;
    return 0;
}


/*************************************************
 * 更新标志
 *************************************************/
uint8_t BT_APP_IsUpdated(void)
{
    if(bt_app_update)
    {
        bt_app_update = 0;
        return 1;
    }
    return 0;
}