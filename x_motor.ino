#include "WEMOS_Motor.h"

int pwm;

//Motor shield I2C Address: 0x30
//PWM frequency: 1000Hz(1kHz)
//Motor Pumpe(0x30, _MOTOR_A, 1000); //Motor A
Motor Ventil(0x30, _MOTOR_B, 1000); //Motor B

boolean gv_motor_run = false;
boolean gv_motor_dir = false;

int gv_act_pos;
int gv_dest_pos;
int gv_old_pos;
