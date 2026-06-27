#ifndef __MPU6050_H
#define __MPU6050_H

#include "main.h"
#include <stdint.h>

/************地址************/
#define MPU6050_ADDR        (0x68<<1)

/************寄存器************/
#define MPU6050_SMPLRT_DIV      0x19
#define MPU6050_CONFIG          0x1A
#define MPU6050_GYRO_CONFIG     0x1B
#define MPU6050_ACCEL_CONFIG    0x1C
#define MPU6050_INT_ENABLE      0x38
#define MPU6050_ACCEL_XOUT_H    0x3B
#define MPU6050_GYRO_XOUT_H     0x43
#define MPU6050_PWR_MGMT_1      0x6B
#define MPU6050_WHO_AM_I        0x75

typedef struct
{
    int16_t a_x_raw,a_y_raw,a_z_raw;

    int16_t g_x_raw,g_y_raw,g_z_raw;

    float a_x,a_y,a_z;

    float g_x,g_y,g_z;

    float pitch_a,roll_a,yaw_a;

    float pitch_g,roll_g,yaw_g;

    float pitch,roll,yaw;
}MPU6050_Data_t;


HAL_StatusTypeDef MPU6050_WriteReg(uint8_t reg,uint8_t data);
HAL_StatusTypeDef MPU6050_ReadReg(uint8_t reg,uint8_t *data);
HAL_StatusTypeDef MPU6050_ReadRegs(
        uint8_t reg,
        uint8_t *buf,
        uint16_t len);

uint8_t MPU6050_Init(void);

void MPU6050_Update(void);

void MPU6050_CalibrateGyro(void);//校准

void MPU6050_CalcAccAngle(void);//加速度计算角度

void MPU6050_CalcGyroAngle(void);//陀螺仪计算角度

void MPU6050_Proc(void);

//float MPU6050_Proc(void);
float MPU6050_g_z(void);
float MPU6050_GetPitch(void);
float MPU6050_GetRoll(void);
float MPU6050_GetYaw(void);
float MPU6050_GetPitch_a(void);
float MPU6050_GetPitch_g(void);
#endif