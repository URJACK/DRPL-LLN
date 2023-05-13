#include "contiki.h"          //contiki系统定义
#include "dev/w25q64-arch.h"  //w25q64驱动接口
#include <stdio.h>            //标准输入输出函数接口
//定义文本缓冲区并初始化文本缓冲区
const uint8_t TEXT_Buffer[]={"CC2538 SPI TEST W25Q64F"};
//计算缓冲区的字节数
#define SIZE sizeof(TEXT_Buffer)
/*---------------------------------------------------------------------------*/
//声明进程
PROCESS(w25q64_process, "W25Q64");
//系统初始化的时候自动启动这个进程
AUTOSTART_PROCESSES(&w25q64_process);
/*---------------------------------------------------------------------------*/
//进程实现
PROCESS_THREAD(w25q64_process, ev, data)
{
  //进程开始
  PROCESS_BEGIN();
  //数据缓冲区
  unsigned char datatemp[SIZE];
  //flash字节
  unsigned int FLASH_SIZE;
  //事件定时器对象
  static struct etimer et;
  //设置定时器时间
  etimer_set(&et, CLOCK_SECOND);
  //初始化flash
  SPI_Flash_Init();
  //读取flsh的ID,如果读取失败则显示错误
  while(SPI_Flash_ReadID()!=W25Q64)
  {
     printf(" SPI_Flash is error!\n");
  }
  //输出flashid
  printf("SPI_Flash ID:%x\r\n\n",SPI_FLASH_TYPE);
  //设置flash字节
  FLASH_SIZE=8*1024*1024;	    
  printf("Write_Flash_Data:\r\n");
  //将数据保存到falsh中
  SPI_Flash_Write((unsigned char*)TEXT_Buffer,FLASH_SIZE-100,SIZE);
  //输出信息
  printf("%s\r\n\n", TEXT_Buffer);
  while(1) {
    //等待定时器的事件
    PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));
    //定时器复位
    etimer_reset(&et);
    //输出调试信息
    printf("Read_Flash_Data:\r\n");
    //读取flash中的内容
    SPI_Flash_Read(datatemp,FLASH_SIZE-100,SIZE);
    //打印调试信息到串口终端
    printf("%s\r\n\r\n", datatemp);
  }
  //进程结束
  PROCESS_END();
}
/*---------------------------------------------------------------------------*/
