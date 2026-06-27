#ifndef __BALANCE_H
#define __BALANCE_H



void Balance_Init(void);


/*
 * 平衡控制函数
 * angle: 当前车身角度
 */
void Balance_Control(float angle);


#endif