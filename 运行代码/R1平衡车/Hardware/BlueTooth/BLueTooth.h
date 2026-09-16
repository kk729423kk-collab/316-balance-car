#ifndef __BlueTooth_H
#define __BlueTooth_H

void BlueTooth_Init(uint32_t Baud);
void BlueTooth_send(char *ch);
uint8_t  BlueTooth_Get_State(void);
#endif
