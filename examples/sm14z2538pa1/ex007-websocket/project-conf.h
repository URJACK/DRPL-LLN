
//UIP_FALLBACK_INTERFACE重新定义
#undef UIP_FALLBACK_INTERFACE
#define UIP_FALLBACK_INTERFACE ip64_uip_fallback_interface

#define IP64_ADDRMAP_CONF_ENTRIES 64
//开启路由
#define ROUTER 1
//路由配置
#define ROUTER_CONF 1
//UIP缓冲区字节数
#undef UIP_CONF_BUFFER_SIZE
#define UIP_CONF_BUFFER_SIZE 1300
//配置TCP段的大小为1000
#undef UIP_CONF_TCP_MSS
#define UIP_CONF_TCP_MSS     1000
//配置UIP广播接收器的最大窗口
#undef UIP_CONF_RECEIVE_WINDOW
#define UIP_CONF_RECEIVE_WINDOW 1000
//Neighbor table size 
//邻居表的大小
#undef NBR_TABLE_CONF_MAX_NEIGHBORS
#define NBR_TABLE_CONF_MAX_NEIGHBORS                 64
//重新定义UIP最大的路由为64
#undef UIP_CONF_MAX_ROUTES
#define UIP_CONF_MAX_ROUTES                  64

/* Make HTTP/TCP connection way faster. */
#undef UIP_CONF_TCP_SPLIT
#define UIP_CONF_TCP_SPLIT 1
#undef UIP_SPLIT_CONF_SIZE
#define UIP_SPLIT_CONF_SIZE 8

#include "../apps/thsq-conf/thsq-conf-const.h"

#ifndef THSQ_CONF_NETSTACK
//#define THSQ_CONF_NETSTACK      THSQ_CONF_NULLRDC
#define THSQ_CONF_NETSTACK      THSQ_CONF_DROWSIE
//#define THSQ_CONF_NETSTACK      (THSQ_CONF_AES | THSQ_CONF_DROWSIE)
//CC2538射频通道设置
int8_t cc2538_rf_channel_set(uint8_t channel);
#define MULTICHAN_CONF_SET_CHANNEL(x) cc2538_rf_channel_set(x)

#define MULTICHAN_CONF_READ_RSSI(x) 0

#define MULTICHAN_CONF_NR_CHANNELS    16

#define MULTICHAN_CONF_CHANNEL_OFFSET 11

#endif /* THSQ_CONF_NETSTACK */

#include "../apps/thsq-conf/thsq-default-conf.h"

#define NETSTACK_RADIO_MAX_PAYLOAD_LEN 125

