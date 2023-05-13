/*
 * Copyright (c) 2004, Swedish Institute of Computer Science.
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
 * 3. Neither the name of the Institute nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE INSTITUTE AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE INSTITUTE OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 *
 * This file is part of the Contiki operating system.
 *
 * Author: Adam Dunkels <adam@sics.se>
 *
 */
#ifndef CONTIKI_H_
#define CONTIKI_H_

#include "contiki-version.h"                //系统版本定义
#include "contiki-conf.h"                   //系统配置
#include "contiki-default-conf.h"           //系统默认配置

#include "sys/process.h"                    //进程
#include "sys/autostart.h"                  //自动启动进程

#include "sys/timer.h"                      //时间定时器，该模块以clock的tick作为时间精度，设定定时器，等待时间到达以后触发，需要轮询调用
#include "sys/ctimer.h"                     //该模块调用etimer，触发以后调用回调函数
#include "sys/etimer.h"                     //该模块调用timer，当定时器超时触发以后以事件的方式通知绑定定时器的线程
#include "sys/rtimer.h"                     //该模块直接调用硬件平台，设定一个硬件实时定时器，触发以后调用一个定时器回调
                                            //stimer该模块以second作为时间精度，设定定时器，等待时间到达以后触发，需要轮询调用
#include "sys/pt.h"                         //系统携程库

#include "sys/procinit.h"                   //

#include "sys/loader.h"                     //
#include "sys/clock.h"                      //系统时钟相关定义

#include "sys/energest.h"                   //

#endif /* CONTIKI_H_ */
