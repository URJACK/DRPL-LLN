#include "contiki.h"
#include "contiki-lib.h"
#include "contiki-net.h"

#include "dev/uart.h"
#include "dev/serial-line.h"
#include "dev/disrpl.h"

#include <string.h>

#define DEBUG DEBUG_NONE
#include "net/ip/uip-debug.h"

#define UIP_IP_BUF ((struct uip_ip_hdr *)&uip_buf[UIP_LLH_LEN])
// 网络最大节点个数
#define MAX_DATA_NODE_NUM 50
#define MAX_PAYLOAD_LEN 120

static struct uip_udp_conn *server_conn;

PROCESS(udp_server_process, "UDP server process");
AUTOSTART_PROCESSES(&resolv_process, &udp_server_process);
/*---------------------------------------------------------------------------*/
/** Network Layer */
static char msgType;                            // distributedSinkNodeHanlder using
static Node data_node_table[MAX_DATA_NODE_NUM]; // 网关节点节点存储表
static short data_node_id_cursor = 2;           // 新注册节点的id值
static short data_node_table_len = 0;           // 网关节点节点存储表的长度
/** Distributed Layer */

/*---------------------------------------------------------------------------*/
/*网关节点的处理函数*/
static uip_ipaddr_t broadcast_addr;
static char buf[MAX_PAYLOAD_LEN];
static void detect_timeout_handler()
{
  // 包装 探测报文
  buf[0] = MSG_DETECT;
  // 广播发送 探测报文
  uip_ipaddr_copy(&server_conn->ripaddr, &broadcast_addr);
  uip_udp_packet_send(server_conn, buf, 1);
  PRINTF("send Detect control message \n");
  /* Restore server connection to allow data from any node */
  memset(&server_conn->ripaddr, 0, sizeof(server_conn->ripaddr));
}
static void distributedSinkNodeHanlder(char *arr, short dataLen)
{
  // debug PRINTF("distributedSinkNodeHanlder %s %d\n", arr, dataLen);
  // 使用静态变量，预防contiki堆栈调用导致内存分配的错误。
  int i;
  char flag;
  msgType = arr[0];
  if (msgType == MSG_DETECT_RESP)
  {
    // detect_response 探测报文响应
    PRINTF("receive detect_response controll message\n");
    int newStoreNodeId = getShort(arr + 1);
    flag = TRUE;
    for (i = 0; i < data_node_table_len; ++i)
    {
      if (data_node_table[i].id == newStoreNodeId)
      {
        data_node_table[i].role = ROLE_STORE;
        flag = FALSE;
        break;
      }
    }
    if (flag)
    {
      // 未找到合适的数据节点表项
      return;
    }
    // 包装 探测响应确认报文
    buf[0] = MSG_DETECT_RESP_ACK;
    buf[1] = 0;
    // 回发 探测响应确认报文
    uip_ipaddr_copy(&server_conn->ripaddr, &UIP_IP_BUF->srcipaddr);
    uip_udp_packet_send(server_conn, buf, 1);
    PRINTF("send detect_response_ack controll message\n");
  }
  else if (msgType == MSG_REGISTER)
  {
    // register 数据节点注册
    // 依据自动id游标data_node_id_cursor，设置新注册节点的id。
    short newDataNodeId = data_node_id_cursor++;
    // 向数据节点表中增加新节点
    data_node_table[data_node_table_len].id = newDataNodeId;
    data_node_table[data_node_table_len].role = ROLE_DATA;
    uip_ipaddr_copy(&data_node_table[data_node_table_len].addr, &UIP_IP_BUF->srcipaddr);
    data_node_table_len++;
    // 包装 注册响应报文
    buf[0] = MSG_REGISTER_RESP;
    setShort(buf + 1, newDataNodeId);
    buf[3] = 0;
    // 回发 注册响应报文
    uip_ipaddr_copy(&server_conn->ripaddr, &UIP_IP_BUF->srcipaddr);
    uip_udp_packet_send(server_conn, buf, 3);
    PRINTF("received node(%d) register\n", newDataNodeId);
  }
  else if (msgType == MSG_SYNC)
  {
    // sync 节点同步
  }
  else if (msgType == MSG_DATA_READ)
  {
    // d_read 节点读取
  }
  else if (msgType == MSG_DATA_WRITE)
  {
    // d_write 节点控制
  }
  /* Restore server connection to allow data from any node */
  memset(&server_conn->ripaddr, 0, sizeof(server_conn->ripaddr));
}

static void tcpip_handler(void)
{
  if (uip_newdata())
  {
    ((char *)uip_appdata)[uip_datalen()] = 0;

    distributedSinkNodeHanlder((char *)uip_appdata, uip_datalen());
  }
}
/*---------------------------------------------------------------------------*/
static void
print_local_addresses(void)
{
  int i;
  uint8_t state;

  PRINTF("Server IPv6 addresses: ");
  for (i = 0; i < UIP_DS6_ADDR_NB; i++)
  {
    state = uip_ds6_if.addr_list[i].state;
    if (uip_ds6_if.addr_list[i].isused &&
        (state == ADDR_TENTATIVE || state == ADDR_PREFERRED))
    {
      PRINT6ADDR(&uip_ds6_if.addr_list[i].ipaddr);
      PRINTF("\n");
    }
  }
}
/*---------------------------------------------------------------------------*/
PROCESS_THREAD(udp_server_process, ev, data)
{
  static struct etimer et;
#if UIP_CONF_ROUTER
  uip_ipaddr_t ipaddr;
#endif /* UIP_CONF_ROUTER */

  PROCESS_BEGIN();
  PRINTF("UDP server started\n");

#if RESOLV_CONF_SUPPORTS_MDNS
  resolv_set_hostname("contiki-udp-server");
#endif

#if UIP_CONF_ROUTER
  uip_ip6addr(&ipaddr, 0xaaaa, 0, 0, 0, 0, 0, 0, 0);
  uip_ds6_set_addr_iid(&ipaddr, &uip_lladdr);
  uip_ds6_addr_add(&ipaddr, 0, ADDR_AUTOCONF);
#endif /* UIP_CONF_ROUTER */

  print_local_addresses();

  server_conn = udp_new(NULL, UIP_HTONS(3001), NULL);
  udp_bind(server_conn, UIP_HTONS(3000));

  uip_create_linklocal_allnodes_mcast(&broadcast_addr);
  etimer_set(&et, DETECT_INTERVAL);
  while (1)
  {
    PROCESS_YIELD();
    if (ev == tcpip_event)
    {
      tcpip_handler();
    }
    else if (ev == serial_line_event_message)
    {
      printf("data: %s\n", (char *)data);
#if DEBUG == DEBUG_PRINT
      distributedSinkNodeHanlder(data, strlen(data));
#endif
    }
    else if (etimer_expired(&et))
    {
      detect_timeout_handler();
      etimer_restart(&et);
    }
  }

  PROCESS_END();
}