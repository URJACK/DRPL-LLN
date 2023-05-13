#include "contiki.h"              //contiki系统接口
#include "dev/step-motor-arch.h"  //步进电机驱动接口
/*---------------------------------------------------------------------------*/
//进程声明
PROCESS(step_motor_process, "step-motor");
//系统初始化的时候自动启动这个进程
AUTOSTART_PROCESSES(&step_motor_process);
/*---------------------------------------------------------------------------*/
//进程实现
PROCESS_THREAD(step_motor_process, ev, data)
{
  //进程开始
  PROCESS_BEGIN();
  //步进电机初始化
  STEP_MOTOR_init();
  
  while(1) {
    //步进电机程序
    BJ_MOTOR(10);
  }
  //进程结束
  PROCESS_END();
  /*测试*/
}
/*---------------------------------------------------------------------------*/
