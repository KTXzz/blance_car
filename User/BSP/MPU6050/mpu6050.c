//
// Created by wzx on 2026/6/8.
//

#include "mpu6050.h"
#include "i2c.h"
#include "math.h"

uint8_t rx_buf[14];
static MPU6050_Data_t mpu;
static float gx_offset = 0, gy_offset = 0, gz_offset = 0;//偏移量

HAL_StatusTypeDef MPU6050_WriteReg(uint8_t reg,uint8_t data)
{
    return HAL_I2C_Mem_Write(
            &hi2c2,
            MPU6050_ADDR,
            reg,
            I2C_MEMADD_SIZE_8BIT,
            &data,
            1,
            100);
}

HAL_StatusTypeDef MPU6050_ReadReg(uint8_t reg,uint8_t *data)
{
    return HAL_I2C_Mem_Read(
            &hi2c2,
            MPU6050_ADDR,
            reg,
            I2C_MEMADD_SIZE_8BIT,
            data,
            1,
            100);
}

HAL_StatusTypeDef MPU6050_ReadRegs(
        uint8_t reg,
        uint8_t *buf,
        uint16_t len)
{
    return HAL_I2C_Mem_Read(
            &hi2c2,
            MPU6050_ADDR,
            reg,
            I2C_MEMADD_SIZE_8BIT,
            buf,
            len,
            100);
}

uint8_t MPU6050_Init(void)
{
    uint8_t id;

    HAL_Delay(100);

    MPU6050_ReadReg(MPU6050_WHO_AM_I,&id);


    if(id != 0x68 && id != 0x70)
    {
        return 1;
    }
    MPU6050_WriteReg(MPU6050_PWR_MGMT_1,0x80);
    HAL_Delay(100);
    MPU6050_WriteReg(MPU6050_PWR_MGMT_1,0x00);
    MPU6050_WriteReg(MPU6050_GYRO_CONFIG,0x18);
    MPU6050_WriteReg(MPU6050_ACCEL_CONFIG,0x00);
        HAL_Delay(100);
        MPU6050_CalibrateGyro();
        MPU6050_Update();
        MPU6050_CalcAccAngle();
        mpu.pitch_g = mpu.pitch_a;
        mpu.roll_g  = mpu.roll_a;

        mpu.pitch = mpu.pitch_a;
        mpu.roll  = mpu.roll_a;
    return 0;
}

void MPU6050_CalibrateGyro(void)
{
    const int samples = 500;
    int32_t sum_gx = 0, sum_gy = 0, sum_gz = 0;

    for (int i = 0; i < samples; i++)
    {
        uint8_t raw[6];
        MPU6050_ReadRegs(MPU6050_GYRO_XOUT_H, raw, 6);
        sum_gx += (int16_t)((raw[0] << 8) | raw[1]);
        sum_gy += (int16_t)((raw[2] << 8) | raw[3]);
        sum_gz += (int16_t)((raw[4] << 8) | raw[5]);
        HAL_Delay(2);  // 约等 1 秒完成校准
    }
    gx_offset = (float)sum_gx / samples ;
    gy_offset = (float)sum_gy / samples ;
    gz_offset = (float)sum_gz / samples ;
}

void MPU6050_Update(void)
{
    MPU6050_ReadRegs(
            MPU6050_ACCEL_XOUT_H,
            rx_buf,
            14);

    mpu.a_x_raw =
            (int16_t)((rx_buf[0]<<8)|rx_buf[1]);

    mpu.a_y_raw =
            (int16_t)((rx_buf[2]<<8)|rx_buf[3]);

    mpu.a_z_raw =
            (int16_t)((rx_buf[4]<<8)|rx_buf[5]);

    mpu.g_x_raw =
            (int16_t)((rx_buf[8]<<8)|rx_buf[9]);

    mpu.g_y_raw =
            (int16_t)((rx_buf[10]<<8)|rx_buf[11]);

    mpu.g_z_raw =
            (int16_t)((rx_buf[12]<<8)|rx_buf[13]);

    mpu.a_x=(float)mpu.a_x_raw/16384.0f;
    mpu.a_y=(float)mpu.a_y_raw/16384.0f;
    mpu.a_z=(float)mpu.a_z_raw/16384.0f;

        mpu.g_x = (mpu.g_x_raw - gx_offset) / 16.4f;
        mpu.g_y = (mpu.g_y_raw - gy_offset) / 16.4f;
        mpu.g_z = (mpu.g_z_raw - gz_offset) / 16.4f;
}

void MPU6050_CalcAccAngle(void)//加速度计算
{
        mpu.pitch_a =
        -atan2f(mpu.a_x,mpu.a_z)*180.0f/3.1415926f;

        mpu.roll_a =
        atan2f(mpu.a_y,mpu.a_z)*180.0f/3.1415926f;
}

void MPU6050_CalcGyroAngle(void)//陀螺仪计算
{
    // pitch 角度 = 上一次角度 + y轴角速度 * dt
    mpu.pitch_g += mpu.g_y * 0.005;

    // roll 角度 = 上一次角度 + x轴角速度 * dt
    mpu.roll_g  += mpu.g_x * 0.005;

    mpu.yaw_g   += mpu.g_z * 0.005;
    // yaw 一般不用于两轮平衡车，除非有航向控
}


void MPU6050_Proc()
{
    MPU6050_Update();
    MPU6050_CalcAccAngle();
    MPU6050_CalcGyroAngle();
    const float alpha = 0.95238f;
    mpu.pitch = alpha*mpu.pitch_g + (1-alpha)*mpu.pitch_a;
    mpu.roll  = alpha*mpu.roll_g  + (1-alpha)*mpu.roll_a;
    mpu.yaw   = mpu.yaw_g;
    mpu.pitch_g = mpu.pitch;      
}


float MPU6050_g_y(void)         { return mpu.g_y; }
float MPU6050_GetPitch(void) { return mpu.pitch; }
float MPU6050_GetRoll(void)  { return mpu.roll; }
float MPU6050_GetYaw(void)   { return mpu.yaw; }
float MPU6050_GetPitch_a(void) { return mpu.pitch_a; }
float MPU6050_GetPitch_g(void) { return mpu.pitch_g; }