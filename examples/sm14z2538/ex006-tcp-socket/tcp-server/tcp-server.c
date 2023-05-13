/*
 * Copyright (c) 2012, Thingsquare, http://www.thingsquare.com/.
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

#include "contiki-net.h"      //网络通信相关函数
#include "sys/cc.h"           //C编译器相关定义

//stdio 就是指 “standard buffered input&output" 
//意思就是说带缓冲的标准输入输出！
#include <stdio.h>
//standard library标准库头文件
//常用的系统函数
#include <stdlib.h>
//字符串处理函数
#include <string.h>
//服务器端口设置
#define SERVER_PORT 80
//tcp socket对象定义
static struct tcp_socket socket;
//输入缓冲区设置400个字节
#define INPUTBUFSIZE 400
//输入缓冲区定义
static uint8_t inputbuf[INPUTBUFSIZE];
//输出缓冲区设置400个字节
#define OUTPUTBUFSIZE 400
//输出缓冲区定义
static uint8_t outputbuf[OUTPUTBUFSIZE];
//声明tcp server进程
PROCESS(tcp_server_process, "TCP echo process");
//系统初始化自动启动tcp server进程 
AUTOSTART_PROCESSES(&tcp_server_process);
//获得缓冲区的请求
static uint8_t get_received;
//发送字节
static int bytes_to_send;
/*---------------------------------------------------------------------------*/
//输入处理函数
static int
input(struct tcp_socket *s, void *ptr,
      const uint8_t *inputptr, int inputdatalen)
{
  printf("input %d bytes '%s'\n", inputdatalen, inputptr);
  if(!get_received) {
    /* See if we have a full GET request in the buffer. */
    if(strncmp((char *)inputptr, "GET /", 5) == 0 &&
       atoi((char *)&inputptr[5]) != 0) {
      bytes_to_send = atoi((char *)&inputptr[5]);
      printf("bytes_to_send %d\n", bytes_to_send);
      return 0;
    }
    printf("inputptr '%.*s'\n", inputdatalen, inputptr);
    /* Return the number of data bytes we received, to keep them all
       in the buffer. */
    return inputdatalen;
  } else {
    /* Discard everything */
    return 0; /* all data consumed */
  }
}
/*---------------------------------------------------------------------------*/
//事件处理函数
static void
event(struct tcp_socket *s, void *ptr,
      tcp_socket_event_t ev)
{
  printf("event %d\n", ev);
}
/*---------------------------------------------------------------------------*/
//tcp server进程实现
PROCESS_THREAD(tcp_server_process, ev, data)
{
  //进程开始
  PROCESS_BEGIN();
  //tcp socket注册
  tcp_socket_register(&socket, NULL,
               inputbuf, sizeof(inputbuf),
               outputbuf, sizeof(outputbuf),
               input, event);
  //tcp端口绑定
  tcp_socket_listen(&socket, SERVER_PORT);
  //输出服务器的端口号
  printf("Listening on %d\n", SERVER_PORT);
  while(1) {
    PROCESS_PAUSE();
    //如果字节大于0
    if(bytes_to_send > 0) {
      /* Send header */
      //输出调试信息到终端
      printf("sending header\n");
      //发送字符串
      tcp_socket_send_str(&socket, "HTTP/1.0 200 ok\r\nServer: Contiki tcp-socket example\r\n\r\n");

      /* Send data */
      //输出调试信息到终端
      printf("sending data\n");
      //如果字节>0
      while(bytes_to_send > 0) {
        //持续时间很短的进程
        PROCESS_PAUSE();
        int len, tosend;
        //判断发送的字节是否超出缓冲区
        tosend = MIN(bytes_to_send, sizeof(outputbuf));
        //发送数据,并返回长度
        len = tcp_socket_send(&socket, (uint8_t *)"", tosend);
        //计算字节长度
        bytes_to_send -= len;
      }
      //关闭tcp连接
      tcp_socket_close(&socket);
    }

  }
  //进程结束
  PROCESS_END();
}
/*---------------------------------------------------------------------------*/
