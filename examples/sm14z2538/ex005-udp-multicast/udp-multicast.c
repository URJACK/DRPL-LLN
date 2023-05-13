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
#include "simple-rpl.h"                     //rpl相关
#include "tcp-socket.h"                     //tcp通信api
#include "udp-socket.h"                     //udp通信api

#include <stdlib.h>                         //标准的输入输出设备库
#include <stdio.h>                          //标准输入输出设备库
#include <string.h>                         //字符串处理库

#define PORT 12345                          //通信端口设置
//udp数据结构
static struct udp_socket s;
//uip地址
static uip_ipaddr_t addr;
//uip地址
static uip_ipaddr_t addr1;
//设置发送时间间隔
#define SEND_INTERVAL		(30 * CLOCK_SECOND)
//定义周期事件定时器,发送事件定时器
static struct etimer periodic_timer, send_timer;

/*---------------------------------------------------------------------------*/
//声明multicast_example_process进程
PROCESS(multicast_example_process, "Link local multicast example process");
//系统初始化启动multicast_example_process进程
AUTOSTART_PROCESSES(&multicast_example_process);
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
//multicast_example_process进程实现
PROCESS_THREAD(multicast_example_process, ev, data)
{
  //进程开始
  PROCESS_BEGIN();

  /* Create a linkl-local multicast addresses. */
  //uip_ip6addr(&addr, 0xfe80, 0, 0, 0, 0x6062, 0x537b, 0xab9f, 0xc79b);
  //设置ipv6的地址
  uip_ip6addr(&addr, 0xfe80, 0, 0, 0, 0x212, 0x4b00, 0x42a, 0xeae3);
  uip_ip6addr(&addr1, 0xfe80, 0, 0, 0, 0x212, 0x4b00, 0x5af, 0x830e);

  /* Join local group. */
  //如果增加错误则输出错误信息
  if(uip_ds6_maddr_add(&addr) == NULL) {
    printf("Error: could not join local multicast group.\n");
  }
  //如果增加错误则输出错误信息
  if(uip_ds6_maddr_add(&addr1) == NULL) {
    printf("Error: could not join local multicast group.\n");
  }

  /* Register UDP socket callback */
  //注册 udp socket 回调函数,处理接收到的信息
  udp_socket_register(&s, NULL, receiver);

  /* Bind UDP socket to local port */
  //端口绑定
  udp_socket_bind(&s, PORT);

  /* Connect UDP socket to remote port */
  //连接udp socket 到远程服务器端口
  udp_socket_connect(&s, NULL, PORT);

  while(1) {

    /* Set up two timers, one for keeping track of the send interval,
       which is periodic, and one for setting up a randomized send time
       within that interval. */
    //设置周期定时器的时间
    etimer_set(&periodic_timer, SEND_INTERVAL);
    //设置发送定时器的时间
    etimer_set(&send_timer, (random_rand() % SEND_INTERVAL));
    //等待发送时间到
    PROCESS_WAIT_UNTIL(etimer_expired(&send_timer));
    //输出调试信息到终端
    printf("Sending multicast#1\n");
    //发送数据到远程服务器
    udp_socket_sendto(&s,
                      "nihao", 6,
                      &addr, PORT);
    //输出调试信息
    printf("Sending multicast#2\n");
    //发送数据到远程服务器
    udp_socket_sendto(&s,
                      "world", 6,
                      &addr1, PORT);
    //等待周期定时器的时间
    PROCESS_WAIT_UNTIL(etimer_expired(&periodic_timer));
  }
  //进程结束
  PROCESS_END();
}
/*---------------------------------------------------------------------------*/
