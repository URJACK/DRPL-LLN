//声明CC2538射频通道设置函数
int8_t cc2538_rf_channel_set(uint8_t channel);
//多通道设置
#define MULTICHAN_CONF_SET_CHANNEL cc2538_rf_channel_set
//声明CC2538射频读取rssi函数
int cc2538_rf_read_rssi(void);
//多通道配置读RSSI
#define MULTICHAN_CONF_READ_RSSI cc2538_rf_read_rssi
//重新定义memcpy,内存拷贝
#define w_memcpy memcpy /* Needed to make multichan.c compile */
//设置通道数
#define MULTICHAN_CONF_NR_CHANNELS    16
//关闭通道设置
#define MULTICHAN_CONF_CHANNEL_OFFSET 11

//网络协议栈Radio最大的加载长度
#define NETSTACK_RADIO_MAX_PAYLOAD_LEN 125
//网络协议栈的AES KEY 设置
#define NETSTACK_AES_KEY "thingsquare mist"
//网络寄存器配置开关打开
#undef ENERGEST_CONF_ON
#define ENERGEST_CONF_ON            1

/* Allow the device to join two multicast groups. */
#undef UIP_CONF_DS6_MADDR_NBU
#define UIP_CONF_DS6_MADDR_NBU 2

#undef NBR_TABLE_CONF_MAX_NEIGHBORS
#define NBR_TABLE_CONF_MAX_NEIGHBORS                 20

#undef UIP_CONF_DS6_ROUTE_NBU
#define UIP_CONF_DS6_ROUTE_NBU               20

#undef UIP_CONF_RECEIVE_WINDOW
#define UIP_CONF_RECEIVE_WINDOW  1000
#undef UIP_CONF_TCP_MSS
#define UIP_CONF_TCP_MSS         1000
