//网络相关的函数
#include "contiki-net.h"
//http socket通信相关的函数
#include "http-socket.h"
//ip64转换相关的函数
#include "ip64-addr.h"
//标准的输入输出设备
#include <stdio.h>
//定义http_socket对象
static struct http_socket s;
//bytes_received保存接收的字节数
static int bytes_received = 0;

/*---------------------------------------------------------------------------*/
//声明http进程
PROCESS(http_example_process, "HTTP Example");
//系统初始化自动启动http进程
AUTOSTART_PROCESSES(&http_example_process);
/*---------------------------------------------------------------------------*/
//callback回调函数,处理http socket信息
//通过串口输出接收到的事件,数据长度,数据
static void
callback(struct http_socket *s, void *ptr,
         http_socket_event_t e,
         const uint8_t *data, uint16_t datalen)
{
  printf("callback e %d datalen %d data '%s'\n", e, datalen, data);
}
/*---------------------------------------------------------------------------*/
//http进程实现
PROCESS_THREAD(http_example_process, ev, data)
{
  //定义定时器事件定时器
  static struct etimer et;
  //ip4add保存ipv4地址
  uip_ip4addr_t ip4addr;
  //ip6addr保存ipv6地址
  uip_ip6addr_t ip6addr;
  //进程开始
  PROCESS_BEGIN();
  //定义服务器地址,地址格式为ipv4格式
  //这里可以根据自己的http服务器的地址进行修改
  uip_ipaddr(&ip4addr, 192,168,18,209);
  //把ipv4的地址转换为ipv6的地址
  ip64_addr_4to6(&ip4addr, &ip6addr);
  //更新域名服务器
  uip_nameserver_update(&ip6addr, UIP_NAMESERVER_INFINITE_LIFETIME);
  //设置定时器的时间
  etimer_set(&et, CLOCK_SECOND * 60);
  //等待定时器事件产生
  PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));
  //初始化http socket
  http_socket_init(&s);
  //发送客户端请求到服务器,利用回调函数进行处理
  http_socket_get(&s, "http://192.168.18.209:8080/", 0, 0,
                  callback, NULL);
  //设置定时器的时间
  etimer_set(&et, CLOCK_SECOND);
  while(1) {
    //等待定时器事件产生
    PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));
    //复位定时器
    etimer_reset(&et);
  }
  //进程结束
  PROCESS_END();
}
/*---------------------------------------------------------------------------*/
