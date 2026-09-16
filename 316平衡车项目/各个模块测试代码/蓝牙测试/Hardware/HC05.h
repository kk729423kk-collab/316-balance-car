#ifndef __HC05_H
#define __HC05_H

void HC05_init(void);
void HC05_send(char *ch);
void HC05_get(char *buf);
uint8_t  BlueTooth_Get_State(void);

#endif
