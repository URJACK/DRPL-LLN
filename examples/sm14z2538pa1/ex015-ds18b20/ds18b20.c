#include "contiki.h"            //contiki系统接口
#include "dev/ds18b20-arch.h"   //ds18b20驱动接口
#include <stdio.h>              //标准输入输出设备接口

/*---------------------------------------------------------------------------*/
//进程声明
PROCESS(ds18b20_process, "ds18b20");
//系统初始化的时候自动启动这个进程
AUTOSTART_PROCESSES(&ds18b20_process);
/*---------------------------------------------------------------------------*/
//进程实现
PROCESS_THREAD(ds18b20_process, ev, data)
{
  //进程开始
  PROCESS_BEGIN();
  //定义temperature保存温度值
  unsigned int temperature;
  //定义时间定时器对象,保存温度读取的时间间隔相关参数
  static struct etimer et;
  //事件定时器的时间间隔
  etimer_set(&et, CLOCK_SECOND);
  //ds18b20初始化
  while(DS18B20_Init());
  
  while(1) {
    //等待事件定时器的事件产生
    PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));
    //复位事件定时器
    etimer_reset(&et);
    //读取温度值并保存到temperature变量中
    temperature=DS18B20_Get_Temp();	
    //将温度值输出到串口终端
    printf("temperature:%d\n",temperature);;
  }
  //进程结束
  PROCESS_END();
  /*测试*/
}
/*---------------------------------------------------------------------------*/
