#include "bluetooth.h"
#include "usart.h"
#include "string.h"

#define BT_FRAME_LEN    18

static uint8_t bt_rx_buf[BT_FRAME_LEN];

static BT_Data_t bt_data;

static uint8_t bt_update_flag = 0;


/************************************************
 *
 * 蓝牙初始化
 *
 ************************************************/
void Bluetooth_Init(void)
{
    /* JDY31使能 */
    HAL_GPIO_WritePin(
            GPIOC,
            GPIO_PIN_15,
            GPIO_PIN_SET);

    /* 开启串口中断接收 */
    HAL_UART_Receive_IT(
            &huart2,
            bt_rx_buf,
            BT_FRAME_LEN);
}


/************************************************
 *
 * 蓝牙接收完成回调
 *
 ************************************************/
void Bluetooth_RxCallback(void)
{
    /* 判断包头包尾 */
    if(bt_rx_buf[0] == 0xA5 &&
       bt_rx_buf[17] == 0x5A)
 {
        bt_data.enable = bt_rx_buf[1];

        bt_data.mode = bt_rx_buf[2];

        bt_data.pid_type = bt_rx_buf[3];

        memcpy(&bt_data.data1,&bt_rx_buf[4],4);

        memcpy(&bt_data.data2,&bt_rx_buf[8],4);

        memcpy(&bt_data.data3,&bt_rx_buf[12],4);

        bt_update_flag = 1;
    }

    HAL_UART_Receive_IT(
            &huart2,
            bt_rx_buf,
            BT_FRAME_LEN);
}


/************************************************
 *
 * 获取蓝牙数据
 *
 ************************************************/
BT_Data_t* Bluetooth_GetData(void)
{
    return &bt_data;
}


/************************************************
 *
 * 判断是否收到新数据
 *
 ************************************************/
uint8_t Bluetooth_IsUpdated(void)
{
    if(bt_update_flag)
    {
        bt_update_flag = 0;

        return 1;
    }

    return 0;
}


/************************************************
 *
 * 蓝牙连接状态
 *
 ************************************************/
uint8_t Bluetooth_IsConnected(void)
{
    return HAL_GPIO_ReadPin(
            GPIOA,
            GPIO_PIN_4);
}