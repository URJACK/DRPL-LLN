#include "contiki.h"
#include "dev/leds.h"
#include "dev/adc.h"
#include "dev/gpio.h"

#include <stdio.h>

static struct etimer timera; //线程定时器
static struct etimer timerb;
static struct etimer timer_adc;
static int adcval = 0; //保存ADC读取的原始值

//申明
PROCESS(hello, "Hello");
PROCESS(blink, "Blink");
PROCESS(getadc, "Getadc");

//自启动
AUTOSTART_PROCESSES(&hello, &blink, &getadc);

#define HIGH 1
#define LOW 0
#define INPUT 1
#define OUTPUT 2

void ms_delay(int ms)
{
  int i, j;
  for (i = 0; i < 5; i++)
    for (j = 0; j < 5; j++)
      clock_delay_usec(200);
}

void pinInit(int port, int bit, int mode)
{
  if (mode == OUTPUT)
  {
    // GPIO_SOFTWARE_CONTROL(GPIO_PORT_TO_BASE(port), GPIO_PIN_MASK(bit));
    GPIO_SET_OUTPUT(GPIO_PORT_TO_BASE(port), GPIO_PIN_MASK(bit));
  }
  else if (mode == INPUT)
  {
    // GPIO_SOFTWARE_CONTROL(GPIO_PORT_TO_BASE(port), GPIO_PIN_MASK(bit));
    GPIO_SET_INPUT(GPIO_PORT_TO_BASE(port), GPIO_PIN_MASK(bit));
  }
}

void pinMode(int port, int bit, int mode)
{
  if (mode == HIGH)
  {
    GPIO_SET_PIN(GPIO_PORT_TO_BASE(port), GPIO_PIN_MASK(bit)); //HIGH
  }
  else if (mode == LOW)
  {
    GPIO_CLR_PIN(GPIO_PORT_TO_BASE(port), GPIO_PIN_MASK(bit)); //LOW
  }
}

static char counter = 0;

//线程1：用于串口输出
PROCESS_THREAD(hello, ev, data)
{
  PROCESS_BEGIN();
  //延时2S，两秒后会发送一个超时事件
  etimer_set(&timera, CLOCK_SECOND * 20);

  //使用该线程测试管脚
  //这里是用来测试gpio封装的函数是不是能用
  //这里的0X04表示PA3管脚，就是第三位为1（0000 0100）
  //GPIO_SET_PIN(GPIO_A_BASE,0x04);//拉高PA3管脚
  // GPIO_SET_OUTPUT(GPIO_A_BASE, 0x08);
  // GPIO_CLR_PIN(GPIO_A_BASE, 0X08);

  // pinInit(2, 2, OUTPUT);
  // pinInit(2, 3, OUTPUT);
  // pinMode(2, 2, LOW);
  // pinMode(2, 3, HIGH);

  // GPIO_SET_OUTPUT(GPIO_C_BASE, 0x03);
  // GPIO_SET_PIN(GPIO_C_BASE, 0x02);
  // GPIO_CLR_PIN(GPIO_C_BASE, 0x01);
  pinInit(2, 0, OUTPUT);

  while (1)
  {
    //等待超时时间发生
    PROCESS_WAIT_EVENT();
    if (ev == PROCESS_EVENT_TIMER)
    {
      printf("ADC val : %d  counter: %d\n", adcval, counter);
      counter = counter + 1;
      if (counter % 2 == 0)
      {
        // GPIO_SET_PIN(GPIO_B_BASE, 0x01);
        pinMode(2, 0, LOW);
        printf("TURN LOW\n");
      }
      else
      {
        // GPIO_CLR_PIN(GPIO_B_BASE, 0x01);
        pinMode(2, 0, HIGH);
        printf("TURN HIGH\n");
      }
      //重新开始计数
      etimer_reset(&timera);
    }
  }
  PROCESS_END();
}

//线程2：用于LED闪烁
PROCESS_THREAD(blink, ev, data)
{
  PROCESS_BEGIN();

  while (1)
  {
    etimer_set(&timerb, CLOCK_SECOND);
    PROCESS_WAIT_EVENT_UNTIL(ev == PROCESS_EVENT_TIMER);

    leds_toggle(LEDS_ALL); //翻转电平
    counter = counter + 1;
    if (counter % 2 == 0)
    {
      // GPIO_SET_PIN(GPIO_B_BASE, 0x01);
      printf("TURN LOW\n");
    }
    else
    {
      // GPIO_CLR_PIN(GPIO_B_BASE, 0x01);
      printf("TURN HIGH\n");
    }
    etimer_reset(&timerb);
  }
  PROCESS_END();
}

//线程3：用于AD读取
PROCESS_THREAD(getadc, ev, data)
{
  PROCESS_BEGIN();
  adc_init(); //ad初始化
  while (1)
  {
    etimer_set(&timer_adc, CLOCK_SECOND);
    PROCESS_WAIT_EVENT_UNTIL(ev == PROCESS_EVENT_TIMER);
    //读取ad值 使用了PA2通道
    adcval = adc_get(SOC_ADC_ADCCON_CH_AIN2, SOC_ADC_ADCCON_REF_INT, SOC_ADC_ADCCON_DIV_512);
    etimer_reset(&timer_adc);
  }
  PROCESS_END();
}