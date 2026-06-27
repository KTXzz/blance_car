//
// Created by wzx on 2026/6/9.
//

#include "APP_oled.h"
#include "oled.h"
#include <stdio.h>
#include "mpu6050.h"
#include "encoder.h"
#include "bluetooth.h" 
#include "APP_bluetooth.h"

// ===================== PID类型枚举 =====================
#define PID_BALANCE 0
#define PID_SPEED   1
#define PID_TURN    2


// 调用这个函数时，只有超过200ms才会真正刷新屏幕
void OLED_Proc(void)
{
    static uint32_t last_update = 0;
    uint32_t now = HAL_GetTick();
    if (now - last_update < 200) return;   // 不到200ms，直接退出，几乎不耗时
    last_update = now;

    // --- 以下代码只在需要刷新时才执行 ---
    // ===================== 获取蓝牙数据 =====================
    uint8_t mode = BT_APP_GetMode();
    uint8_t enable = BT_APP_GetMotorEnable();
    uint8_t pid_type = BT_APP_GetPidType();
    float data1 = BT_APP_GetData1();
    float data2 = BT_APP_GetData2();
    float data3 = BT_APP_GetData3();

    // ===================== 第一行：系统状态栏 =====================
    // MODE: 0=PID调参 1=遥控
    // ENABLE: 电机使能
    // PID_TYPE: 当前调参环

    char line[20];

    OLED_ShowString(0, 0, "                ", 16); // 清行

    if (mode == 0)
    {
        sprintf(line, "PID %s  %s",
                enable ? "ON" : "OFF",
                "BT");
    }
    else
    {
        sprintf(line, "RC  %s   %s",
                enable ? "ON" : "OFF",
                "BT");
    }

    OLED_ShowString(0, 0, line, 16);

    // ===================== PID模式界面 =====================
    if (mode == 0)
    {
        // -------- 第二行：当前PID类型 --------
        OLED_ShowString(0, 2, "                ", 16);

        if (pid_type == PID_BALANCE)
            OLED_ShowString(0, 2, "BAL PID", 16);
        else if (pid_type == PID_SPEED)
            OLED_ShowString(0, 2, "SPD PID", 16);
        else
            OLED_ShowString(0, 2, "TRN PID", 16);

        // -------- 第三行：Pid参数 --------
        OLED_ShowString(0, 4, "                ", 16);
        char buf[20];
        sprintf(buf, "%4.2f %4.2f %4.2f", data1, data2, data3);
        OLED_ShowString(0, 4, buf, 16);

        // ===== 第4行 =====
        OLED_ShowString(0,6,"                ",16);
        sprintf(buf,"Pitch:%4.1f",MPU6050_GetPitch());
        OLED_ShowString(0,6,buf,16);
    }

    // ===================== 遥控模式界面 =====================
    else
    {
        // -------- 第二行：速度  --------
        OLED_ShowString(0, 2, "                ", 16);
        char buf[20];
        sprintf(buf, "SPD:%4.1f", BT_APP_GetTargetSpeed());
        OLED_ShowString(0, 2, buf, 16);
           // -------- 第三行：转向  --------
        OLED_ShowString(0, 4, "                ", 16);
        sprintf(buf, "TRN:%4.1f", BT_APP_GetTargetTurn());
        OLED_ShowString(0, 4, buf, 16);

        // -------- 第四行：角度（可选） --------
          OLED_ShowString(0, 6, "                ", 16);
        // float angle = MPU6050_GetAngle();   // 你项目里如果是别的函数自己替换

        // sprintf(buf, "ANG:%4.1f", angle);
        // OLED_ShowString(0, 6, buf, 16);
    }
}