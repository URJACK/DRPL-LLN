#include "contiki.h"               //contiki系统api库  
#include "contiki-net.h"           //contiki网络api库
#include "contiki-lib.h"           //contiki系统lib库
#include "lib/random.h"            //随机数生成库
#include "sys/ctimer.h"            //系统回调定时器库
#include "sys/etimer.h"            //系统事件定时器库
#include "net/rpl/rpl.h"           //路由相关api
#include "net/ip/uip.h"            //uip协议api
#include "net/ipv6/uip-ds6.h"      //ipv6相关api
#include "dev/leds.h"              //LED驱动库

#include "aes.h"                    //aes库
#include "ip64-addr.h"              //ip64地址
#include "apps/mdns/mdns.h"         //mdns相关
#include "simple-rpl.h"             //rpl相关
#include "tcp-socket.h"             //tcp通信api
#include "udp-socket.h"             //udp通信api

//stdio 就是指 “standard buffered input&output" 
//意思就是说带缓冲的标准输入输出！
#include <stdio.h>                  
//standard library标准库头文件
//常用的系统函数
#include <stdlib.h>     
//字符串处理相关的函数            
#include <string.h>

//websocket相关的接口
#include "websocket.h"
//定义websocket对象
static struct websocket s;
//回调函数,处理websocket数据
static void callback(struct websocket *s, websocket_result r,
                     uint8_t *data, uint16_t datalen);
//设置重新连接定时器的时间
#define RECONNECT_INTERVAL 10 * CLOCK_SECOND
//重新连接定时器
static struct ctimer reconnect_timer;

/*---------------------------------------------------------------------------*/
//声明websocket进程
PROCESS(websocket_example_process, "Websocket Example");
//系统初始化的时候自动启动websocket进程
AUTOSTART_PROCESSES(&websocket_example_process);
/*---------------------------------------------------------------------------*/
//重新连接回调函数
static void
reconnect_callback(void *ptr)
{
  //填写自己的websocket服务器的地址
  websocket_open(&s, "ws://192.168.44.128:8080/",
                 "thingsquare mist", callback);
}
/*---------------------------------------------------------------------------*/
//callback是回调函数,此函数处理websocket请求
static void
callback(struct websocket *s, websocket_result r,
         uint8_t *data, uint16_t datalen)
{
  if(r == WEBSOCKET_CLOSED ||
     r == WEBSOCKET_RESET ||
     r == WEBSOCKET_HOSTNAME_NOT_FOUND ||
     r == WEBSOCKET_TIMEDOUT) {
    ctimer_set(&reconnect_timer, RECONNECT_INTERVAL, reconnect_callback, s);
  } else if(r == WEBSOCKET_CONNECTED) {
    websocket_send_str(s, "Connected");
  } else if(r == WEBSOCKET_DATA) {
    printf("websocket-example: Received data '%.*s' (len %d)\n", datalen,
           data, datalen);
  }
}
/*---------------------------------------------------------------------------*/
//websocket_example_process进程实现
PROCESS_THREAD(websocket_example_process, ev, data)
{
  //定义etimer定时器对象et,用来保存定时器信息
  static struct etimer et;
  PROCESS_BEGIN();
  //初始化ctimer定时器参数
  ctimer_set(&reconnect_timer, RECONNECT_INTERVAL, reconnect_callback, &s);

  while(1) {
    //设置定时器的时间
    etimer_set(&et, CLOCK_SECOND / 8);
    //等待定时器事件
    PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));
    //    websocket_send_str(&s, "0123456789abcdef0123456789abcdef0123456789abcdef0123456789abcdef0123456789abcdef0123456789abcdef0123456789abcdef");
    //定义buf数组,保存要发送的数据
    char buf[] = "012345678";
    //定义count计数器变量,保存计数器值
    static int count;
    //保存数据到buf中count除以9的余数转换为字符串,并且保存到buf中
    buf[0] = (count % 9) + '0';
    //计数器每循环一次增加1
    count++;
    //发送数据
    websocket_send_str(&s, buf);
  }
  //进程结束
  PROCESS_END();
}
/*---------------------------------------------------------------------------*/
