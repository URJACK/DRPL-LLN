#include "contiki.h"                        //contiki系统api库
#include "contiki-net.h"                    //contiki网络api库
#include "contiki-lib.h"                    //contiki系统lib库
#include "lib/random.h"                     //随机数生成库
#include "sys/ctimer.h"                     //系统回调定时器库
#include "sys/etimer.h"                     //系统事件定时器库
#include "net/rpl/rpl.h"                    //路由相关api
#include "net/ip/uip.h"                     //uip网络协议api
#include "net/ipv6/uip-ds6.h"               //ipv6相关api
#include "dev/leds.h"                       //LED驱动库

#include "aes.h"                            //aes库
#include "ip64-addr.h"                      //ip64地址接口库 
#include "apps/mdns/mdns.h"                 //mdns接口
#include "apps/simple-rpl/simple-rpl.h"     //rpl相关
#include "tcp-socket.h"                     //tcp通信协议api
#include "udp-socket.h"                     //udp通信api
//#include "websocket.h"
#include "http-socket/http-socket.h"        //http socket通信api

#include <stdlib.h>                         //标准的系统相关接口
#include <stdio.h>                          //标准设备的输入输出相关接口
#include <string.h>                         //字符串处理库
#include "dev/DHT11-arch.h"                 //温湿度传感器dht11接口
#include "dev/oled.h"                       //oled显示模块接口

//定义通信端口
#define PORT 12345


//udp socket 数据对象,用来保存udp通信相关的数据结构
static struct udp_socket s;
//static uip_ipaddr_t;// addr;
//ds6配置
static struct uip_ds6_notification n;
//static uint8_t i=0; 
//定义定时器时间
#define SEND_INTERVAL		(2 * CLOCK_SECOND)
//定义定时器对象,保存定时器的相关参数
static struct etimer periodic_timer;// send_timer;

/*---------------------------------------------------------------------------*/
//声明进程
PROCESS(unicast_example_process, "Link local unicast example process");
//系统初始化的时候自动启动这个进程
AUTOSTART_PROCESSES(&unicast_example_process);


/*---------------------------------------------------------------------------*/
//路由回调函数,处理路由事件
static void
route_callback(int event, uip_ipaddr_t *route, uip_ipaddr_t *ipaddr,
               int numroutes)
{
  if(event == UIP_DS6_NOTIFICATION_DEFRT_ADD) {
    leds_off(LEDS_ALL);
    printf("Got a RPL route\n");
  }
}

/*---------------------------------------------------------------------------*/
//接收函数,输出接收到的相关信息到串口终端
static void
receiver(struct udp_socket *c,
         void *ptr,
         const uip_ipaddr_t *sender_addr,
         uint16_t sender_port,
         const uip_ipaddr_t *receiver_addr,
         uint16_t receiver_port,
         const uint8_t *data,
         uint16_t datalen)
{
  printf("Data received on port %d from port %d with length %d, '%s'\n",
         receiver_port, sender_port, datalen, data);
}
/*---------------------------------------------------------------------------*/
//进程实现
PROCESS_THREAD(unicast_example_process, ev, data)
{
  //ipv6地址设置
  uip_ip6addr_t ip6addr;
  //ipv4地址设置
  uip_ip4addr_t ip4addr;
  //进程开始
  PROCESS_BEGIN();
  //打开所有led
  leds_on(LEDS_ALL);
  //添加回调函数
  uip_ds6_notification_add(&n, route_callback);

  /* Register UDP socket callback */
  //注册udp socket 回调函数receiver,处理udp通信过程中的数据
  udp_socket_register(&s, NULL, receiver);

  /* Bind UDP socket to local port */
  //绑定udp socket 端口
  udp_socket_bind(&s, PORT);

  /* Connect UDP socket to remote port */
  //连接udp服务器
  udp_socket_connect(&s, NULL, PORT);

  //etimer_set(&et, CLOCK_SECOND*2); 

  while(1) {

    /* Set up two timers, one for keeping track of the send interval,
       which is periodic, and one for setting up a randomized send time
       within that interval. */
    //定时器时间设置
    etimer_set(&periodic_timer, SEND_INTERVAL);
    //etimer_set(&send_timer, (random_rand() % SEND_INTERVAL));
    //等待定时器时间到
    PROCESS_WAIT_UNTIL(etimer_expired(&periodic_timer));
    //定义温度和湿度变量
    unsigned char  temp[7]="Tem:", humidity[7]="Hun:";
    //添加ipv4服务器地址
    uip_ipaddr(&ip4addr, 192,168,18,93);
    //将ipv4地址转换为ipv6地址
    ip64_addr_4to6(&ip4addr, &ip6addr);
    //温湿度模块初始化
    DHT11_Inint();
    //temp="Tem:";
    temp[4] = Tem_dec+0x30;
	  temp[5] = Tem_uni+0x30;
	  temp[6] = '\0';
        //humidity="Hum:";
	  humidity[4] = Hum_dec+0x30;
	  humidity[5] = Hum_uni+0x30;
	  humidity[6] = '\0';

    //通过OLED模块显示温度和湿度
    if((Tem_data_H!=0)&&(RH_data_H!=0))
    {
      printf("Tem: %d \r\n",Tem_data_H);
	    printf("Hum: %d \r\n",RH_data_H);
	    OLED_Display_On1();
      OLED_P8x16Str(20,0,"DaLian");
      OLED_P8x16Str(20,2,"Sanmu-Link");
      OLED_P8x16Str(20,4,temp);
      OLED_P8x16Str(20,6,humidity);
    }
    //printf("Sending unicast %d\n",i);
    //i++;
    //发送温度数据到服务器
    udp_socket_sendto(&s,
                      temp, 7,
                      &ip6addr, PORT);
    //发送湿度数据到服务器
    udp_socket_sendto(&s,
                      humidity, 7,
                      &ip6addr, PORT);

    //PROCESS_WAIT_UNTIL(etimer_expired(&periodic_timer));
  }
  //进程结束
  PROCESS_END();
}
/*---------------------------------------------------------------------------*/
