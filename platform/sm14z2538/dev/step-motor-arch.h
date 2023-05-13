#ifndef __STEP_MOTOR_ARCH_H
#define	__STEP_MOTOR_ARCH_H
//#include "dev/gpio.h"

#define IN1_PORT   GPIO_B_BASE
#define IN1_BIT    2

#define IN2_PORT   GPIO_B_BASE
#define IN2_BIT    4

#define IN3_PORT   GPIO_B_BASE
#define IN3_BIT    8

#define IN4_PORT   GPIO_B_BASE
#define IN4_BIT    16

#define IN1(a)	if (a)	\
		  GPIO_WRITE_PIN(IN1_PORT, IN1_BIT, IN1_BIT);\
		else		\
		  GPIO_WRITE_PIN(IN1_PORT, IN1_BIT, 0)
                  
#define IN2(a)	if (a)	\
		  GPIO_WRITE_PIN(IN2_PORT, IN2_BIT, IN2_BIT);\
		else		\
		  GPIO_WRITE_PIN(IN2_PORT, IN2_BIT, 0)

#define IN3(a)	if (a)	\
		  GPIO_WRITE_PIN(IN3_PORT, IN3_BIT, IN3_BIT);\
		else		\
		  GPIO_WRITE_PIN(IN3_PORT, IN3_BIT, 0)

#define IN4(a)	if (a)	\
		  GPIO_WRITE_PIN(IN4_PORT, IN4_BIT, IN4_BIT);\
		else		\
		  GPIO_WRITE_PIN(IN4_PORT, IN4_BIT, 0)    

#define Coil_A {IN1(1);IN2(0);IN3(0);IN4(0);}//A相通电，其他相断电
#define Coil_B {IN1(0);IN2(1);IN3(0);IN4(0);}//B相通电，其他相断电
#define Coil_C {IN1(0);IN2(0);IN3(1);IN4(0);}//C相通电，其他相断电
#define Coil_D {IN1(0);IN2(0);IN3(0);IN4(1);}//D相通电，其他相断电
#define Coil_OFF {IN1(0);IN2(0);IN3(0);IN4(0);}//全部断电  

void STEP_MOTOR_init(void);
void BJ_MOTOR(unsigned int speed);
void delay_ms(unsigned int time);

#endif 

