/*
 * Copyright (c) 2013, Thingsquare, http://www.thingsquare.com/.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * 3. Neither the name of the copyright holder nor the names of its
 *    contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * ``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE
 * COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
 * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED
 * OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 */

#include "contiki.h"                //contiki系统api库           
#include "contiki-net.h"            //contiki网络api库
#include "contiki-lib.h"            //contiki系统lib库
#include "lib/random.h"             //随机数生成库
#include "sys/ctimer.h"             //系统回调定时器库
#include "sys/etimer.h"             //系统事件定时器库
#include "net/rpl/rpl.h"            //路由相关api
#include "net/ip/uip.h"             //uip协议api
#include "net/ipv6/uip-ds6.h"       //ipv6相关api
#include "dev/leds.h"               //LED驱动库

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
//通信端口定义
#define PORT 80
//tcp socket 数据结构体
static struct tcp_socket socket;
//输入缓冲区为400个字节
#define INPUTBUFSIZE 400
static uint8_t inputbuf[INPUTBUFSIZE];
//输出缓冲区为400个字节
#define OUTPUTBUFSIZE 400
//定义输出缓冲区
static uint8_t outputbuf[OUTPUTBUFSIZE];
//声明tcp_client_process进程
PROCESS(tcp_client_process, "TCP client example process");
//系统初始化的时候自动启动tcp_client进程
AUTOSTART_PROCESSES(&tcp_client_process);
/*---------------------------------------------------------------------------*/
//输入处理函数
static int
input(struct tcp_socket *s, void *ptr,
      const uint8_t *inputptr, int inputdatalen)
{
  printf("input %d bytes '%s' - closing socket\n", inputdatalen, inputptr);
  tcp_socket_close(s);
  return 0;
}
/*---------------------------------------------------------------------------*/
//事件函数
static void
event(struct tcp_socket *s, void *ptr,
      tcp_socket_event_t ev)
{
  if(ev == TCP_SOCKET_CONNECTED) {
    printf("Socket connected\n");
    tcp_socket_send_str(s, "Connected\n");
  } else if(ev == TCP_SOCKET_DATA_SENT) {
    printf("Socket data was sent\n");
  } else if(ev == TCP_SOCKET_CLOSED) {
    printf("Socket closed\n");
  } else if(ev == TCP_SOCKET_ABORTED) {
    printf("Socket reset\n");
  } else if(ev == TCP_SOCKET_TIMEDOUT) {
    printf("Socket timedout\n");
  }
}
/*---------------------------------------------------------------------------*/
//tcp client进程实现
PROCESS_THREAD(tcp_client_process, ev, data)
{
  //定义ip6addr变量保存ipv6地址
  uip_ip6addr_t ip6addr;
  //定义ip4addr保存ipv4地址
  uip_ip4addr_t ip4addr;
  //进程开始
  PROCESS_BEGIN();

  //uip_ipaddr(&ip4addr, 192,168,18,33);
  //ip64_addr_4to6(&ip4addr, &ip6addr);
  //设置ipv6地址
  uip_ip6addr(&ip6addr, 0xfe80, 0, 0, 0, 0x212, 0x4b00, 0x42a, 0xeae3); //server's IPv6 address
  //tcp socket 通信注册
  tcp_socket_register(&socket, NULL,
                      inputbuf, sizeof(inputbuf),
                      outputbuf, sizeof(outputbuf),
                      input, event);
  //tcp socket连接服务器
  tcp_socket_connect(&socket, &ip6addr, PORT);
  while(1) {
    //等待事件发生
    PROCESS_WAIT_EVENT();
  }
  //进程结束
  PROCESS_END();
}
/*---------------------------------------------------------------------------*/
