#ifndef _OPENMV_H_
#define _OPENMV_H_

#include "sys.h"

void Openmv_Receive_Data(u16 date);
void Openmv_Date(void);
extern u8 Openmv_x;
extern u8 Openmv_y;
void uart_init2(u32 bound);







#endif
