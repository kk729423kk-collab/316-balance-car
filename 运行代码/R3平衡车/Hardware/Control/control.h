#ifndef _CONTROL_H
#define _CONTROL_H
int Vertical_Pid_Value(float measure,float calcu);
int Velocity_Pid_Value(int Target_Vel,float now_velocity);
int Turn_Pid_value(int RC,float gy);
void PWM_XianFU(int max,int *value);
#endif




