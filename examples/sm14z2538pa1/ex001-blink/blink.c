//引用系统api
#include "contiki.h"
//引用LED驱动
#include "dev/leds.h"
/*---------------------------------------------------------------------------*/
//声明进程
PROCESS(blink_process, "Blink");
//系统初始化的时候自动启动blink_process进程
AUTOSTART_PROCESSES(&blink_process);
/*---------------------------------------------------------------------------*/
//blink_process进程实现
PROCESS_THREAD(blink_process, ev, data)
{
  //进程推出后关闭所有的LED设备
  PROCESS_EXITHANDLER(leds_off(LEDS_ALL););
  //进程开始
  PROCESS_BEGIN();
  //创建定时器事件对象
  static struct etimer et;
  //设置定时器时间,CLOCK_SECOND时间为1秒钟
  etimer_set(&et, CLOCK_SECOND);
  //进程循环
  static char i = 0;
  while (1)
  {
    //等待定时器事件发生
    PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));
    //重置定时器
    etimer_reset(&et);
    //点亮所有LED
    //LEDS_ALL表示所有的LED灯D1-D4
    //LEDS_ORANGE表示蓝色LED灯D1
    //LEDS_GREEN表示绿色LED灯D2
    //LEDS_YELLOW表示黄色LED灯D3
    //LEDS_RED表示红色LED灯D4
    //leds_on(i);
    leds_set(1<<i);
    printf("I'm ffz;\n");
    i++;
    if(i>3)
      i = 0;
    //等待定时器时间发生
    // PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));
    //复位定时器
    // etimer_reset(&et);
    //关闭所有LED设备
    // leds_off(0x08);
  }
  //进程结束
  PROCESS_END();
  /*测试*/
}
/*---------------------------------------------------------------------------*/
