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
#include "tcp-socket.h"                     //tcp通信api
#include "udp-socket.h"                     //udp通信api
//#include "websocket.h"
#include "http-socket/http-socket.h"        //http socket通信api

#include <stdlib.h>                         //标准的输入输出设备库
#include <stdio.h>                          //标准输入输出设备库
#include <string.h>                         //字符串处理库

#define PORT 12345                          //通信端口设置
//udp数据结构
static struct udp_socket s;
//uip的ip地址
static uip_ipaddr_t addr;

static struct uip_ds6_notification n;
//定义变量i
static uint8_t i=0; 
//设置发送时间间隔
#define SEND_INTERVAL		(10 * CLOCK_SECOND)
//定义周期事件定时器,发送事件定时器
static struct etimer periodic_timer, send_timer;

/*---------------------------------------------------------------------------*/
//声明unicast_example_process 进程
PROCESS(unicast_example_process, "Link local unicast example process");
//系统初始化启动unicast_example_process进程
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
//接收函数
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
//unicast_example_process进程实现
PROCESS_THREAD(unicast_example_process, ev, data)
{
  //定义变量保存ipv6的地址
  uip_ip6addr_t ip6addr;
  //定义变量保存ipv4的地址
  uip_ip4addr_t ip4addr;
  //进程开始
  PROCESS_BEGIN();
#if 0
  /* Create a linkl-local multicast addresses. */
  uip_ip6addr(&addr, 0xff02, 0, 0, 0, 0, 0, 0x1337, 0x0001);

  /* Join local group. */
  if(uip_ds6_maddr_add(&addr) == NULL) {
    printf("Error: could not join local multicast group.\n");
  }
#endif
  //调用回调函数
  leds_on(LEDS_ALL);
  //调用回调函数
  uip_ds6_notification_add(&n, route_callback);

  /* Register UDP socket callback */
  //注册udp接收回调函数
  udp_socket_register(&s, NULL, receiver);

  /* Bind UDP socket to local port */
  //端口绑定
  udp_socket_bind(&s, PORT);

  /* Connect UDP socket to remote port */
  //连接服务器
  udp_socket_connect(&s, NULL, PORT);

  while(1) {

    /* Set up two timers, one for keeping track of the send interval,
       which is periodic, and one for setting up a randomized send time
       within that interval. */
    //设置发送事件间隔,病初始化定时器
    etimer_set(&periodic_timer, SEND_INTERVAL);
    //etimer_set(&send_timer, (random_rand() % SEND_INTERVAL));
    //等待定时时间
    PROCESS_WAIT_UNTIL(etimer_expired(&periodic_timer));
    //将ipv4地址保存到变量中
    uip_ipaddr(&ip4addr, 192,168,18,86);
    //ip64地址转换
    ip64_addr_4to6(&ip4addr, &ip6addr);
    //输出循环次数
    printf("Sending unicast %d\n",i);
    i++;
    //发送数据
    udp_socket_sendto(&s,
                      &i, 1,
                      &ip6addr, PORT);

    //PROCESS_WAIT_UNTIL(etimer_expired(&periodic_timer));
  }
  //进程结束
  PROCESS_END();
}
/*---------------------------------------------------------------------------*/
