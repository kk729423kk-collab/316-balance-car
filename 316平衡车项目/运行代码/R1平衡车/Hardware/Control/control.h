#ifndef _CONTROL_H
#define _CONTROL_H
int Vertical_Pid_Value(float measure,float calcu);
int Velocity_Pid_Value(int Target_Vel,float now_velocity);
int turn(int encoder_bias, int gyroZ);
int XianFu(int value,int max);
#endif




