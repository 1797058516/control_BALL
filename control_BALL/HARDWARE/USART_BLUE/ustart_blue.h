#ifndef _USTART_HIM_H_
#define _USTART_HIM_H_

#include "sys.h"

void uart_init3(u32 bound);
void recetive_blue_32(u8 data);//��1���ֽ�����
void blue_control();
extern u8 blue_data[13];
#endif