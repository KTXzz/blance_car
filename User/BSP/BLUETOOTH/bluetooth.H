#ifndef __BLUETOOTH_H
#define __BLUETOOTH_H

#include "main.h"

typedef struct
{
    uint8_t enable;

    uint8_t mode;

    uint8_t pid_type;

    float data1;

    float data2;

    float data3;

}BT_Data_t;

void Bluetooth_Init(void);

void Bluetooth_RxCallback(void);

uint8_t Bluetooth_IsConnected(void);

BT_Data_t* Bluetooth_GetData(void);

uint8_t Bluetooth_IsUpdated(void);

#endif