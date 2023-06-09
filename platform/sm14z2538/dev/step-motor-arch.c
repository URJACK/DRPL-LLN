/*
 * Copyright (c) 2012, Texas Instruments Incorporated - http://www.ti.com/
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * 3. Neither the name of the copyright holder nor the names of its
 *    contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * ``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE
 * COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
 * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED
 * OF THE POSSIBILITY OF SUCH DAMAGE.
 */
/**
 * \addtogroup cc2538-smartrf
 * @{
 *
 * \defgroup cc2538dk-leds SmartRF06EB LED driver
 *
 * LED driver implementation for the TI SmartRF06EB + cc2538EM
 * @{
 *
 * \file
 * LED driver implementation for the TI SmartRF06EB + cc2538EM
 */
#include "reg.h"
#include "dev/step-motor-arch.h"
#include "dev/gpio.h"
#include "clock.h"

/*---------------------------------------------------------------------------*/
void delay_ms(unsigned int time)
{
  while(time--)
  {
     clock_delay_usec(1000);
  }
}
                    
/*---------------------------------------------------------------------------*/
//步进电机初始化
void STEP_MOTOR_init(void)
{ 
  GPIO_SET_OUTPUT(IN1_PORT,IN1_BIT);
  GPIO_SET_OUTPUT(IN2_PORT,IN2_BIT);
  GPIO_SET_OUTPUT(IN3_PORT,IN3_BIT);
  GPIO_SET_OUTPUT(IN4_PORT,IN4_BIT);
}

/*---------------------------------------------------------------------------*/
void BJ_MOTOR(unsigned int speed)		//步进电机匀速转动
{
    Coil_A		        //遇到Coil_A1  用{A1=1;B1=0;C1=0;D1=0;}代替
    delay_ms(speed);    	//改变这个参数可以调整电机转速 ,
	                         //数字越小，转速越大,力矩越小   
    Coil_B
    delay_ms(speed);
    
    Coil_C
    delay_ms(speed);
    
    Coil_D
    delay_ms(speed);
}

/*---------------------------------------------------------------------------*/

/**
 * @}
 * @}
 */
