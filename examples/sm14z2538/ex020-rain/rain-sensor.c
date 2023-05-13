#include "contiki.h"  //contiki系统定义
#include "dev/adc.h"  //adc接口
#include <stdio.h>    //标准输入输出设备接口

/*---------------------------------------------------------------------------*/
//声明进程
PROCESS(rain_process, "rain-sensor");
//系统初始化的时候自动启动这个进程
AUTOSTART_PROCESSES(&rain_process);
/*---------------------------------------------------------------------------*/
//进程实现
PROCESS_THREAD(rain_process, ev, data)
{
  //进程开始
  PROCESS_BEGIN();
  //定义value用来保存adc数据
  unsigned int value;
  //定义事件定时器,控制读取数据的时间间隔
  static struct etimer et;
  //设置定时器的时间
  etimer_set(&et, CLOCK_SECOND);
  //输出调试信息
  printf("ADC rain sens\n");
    
  while(1)
  {
    //等待定时器事件发生
    PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));
    //复位定时器
    etimer_reset(&et);
    //获得adc的数值
    value=adc_get(SOC_ADC_ADCCON_CH_AIN6, SOC_ADC_ADCCON_REF_AVDD5, SOC_ADC_ADCCON_DIV_512)>>4;                      
    // Print the result on UART
    //输出信息到串口终端
    printf("ADC readout: %d\n", value);
        
  }
  //进程结束
  PROCESS_END();
}
/*---------------------------------------------------------------------------*/
