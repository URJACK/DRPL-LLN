//引用contiki系统api
#include "contiki.h"
//引用contiki系统网络api
#include "contiki-net.h"
//ip64协议api
#include "ip64/ip64.h"
//网络协议栈api
#include "net/netstack.h"

/*---------------------------------------------------------------------------*/
//声明一个进程
PROCESS(router_node_process, "Router node");
//系统启动的时候自动启动router_node_process进程
AUTOSTART_PROCESSES(&router_node_process);
/*---------------------------------------------------------------------------*/
//进程实现
PROCESS_THREAD(router_node_process, ev, data)
{
  //进程开始
  PROCESS_BEGIN();

  /* Set us up as a RPL root node. */
  //RPL根节点初始化
  rpl_dag_root_init_dag();

  /* Initialize the IP64 module so we'll start translating packets */
  //ip64模块初始化
  ip64_init();

  /* ... and do nothing more. */
  while(1) {
  //等待事件发生
    PROCESS_WAIT_EVENT();
  }
  //进程结束
  PROCESS_END();
}
/*---------------------------------------------------------------------------*/
