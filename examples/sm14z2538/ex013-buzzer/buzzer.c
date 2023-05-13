//contiki系统api
#include "contiki.h"
//buzzer蜂鸣器接口
#include "dev/buzzer-arch.h"
/*---------------------------------------------------------------------------*/
//进程声明
PROCESS(buzzer_process, "Buzzer");
//系统初始化的时候自动启动进程
AUTOSTART_PROCESSES(&buzzer_process);
/*---------------------------------------------------------------------------*/
//进程实现
PROCESS_THREAD(buzzer_process, ev, data)
{
  //蜂鸣器初始化
  buzzer_init();
  //进程结束的时候关闭蜂鸣器
  PROCESS_EXITHANDLER(beep_off(););
  //进程开始
  PROCESS_BEGIN();
  //创建一个事件定时器
  static struct etimer et;
  //设置定时器的时间
  etimer_set(&et, CLOCK_SECOND/4);

  while(1) {
    //等待定时器事件发生
    PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));
    //定时器复位
    etimer_reset(&et);
    //蜂鸣器发声
    beep_on();
    //等待定时器事件发声
    PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));
    //复位定时器
    etimer_reset(&et);
    //关闭蜂鸣器
    beep_off();
  }

  PROCESS_END();
  /*测试*/
}
/*---------------------------------------------------------------------------*/
