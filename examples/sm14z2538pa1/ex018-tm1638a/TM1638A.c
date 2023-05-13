#include "contiki.h"  //contiki系统接口
#include "dev/TM1638-arch.h" //tm1638接口
//标准设备的输入输出接口
#include <stdio.h> /* For printf() */
//定义数码管段码
const unsigned char CathodeCode[ 0x11 ] = {  0X3F, 0X06, 0X5B, 0X4F, 0X66, 0X6D, 0X7D, 0X07, 0X7F, 0X6F, 0X77, 0X7C, 0X39, 0X5E, 0X79, 0X71, 0XFF  };
/*---------------------------------------------------------------------------*/
//声明进程
PROCESS(TM1638A_process, "TM1638A");
//系统初始化时自动启动进程
AUTOSTART_PROCESSES(&TM1638A_process);
/*---------------------------------------------------------------------------*/
//进程实现
PROCESS_THREAD(TM1638A_process, ev, data)
{
  //进程开始
  PROCESS_BEGIN();
  //变量定义
  unsigned int i,j,k;
  //保存按键值
  unsigned int Key=0;
  //定时器对象
  static struct etimer et;
  //设置定时器的时间
  etimer_set(&et, CLOCK_SECOND);
  //tm1638硬件初始化
  TM1638_init();
  //
  Write_COM( DISPLAY_ON );        /* 开显示 */
  Write_COM( ADDRESS_FIX_MODE );  /* 地址固定模式 */
  Write_COM( SET_PLUS_WIDTH4 );   //设置亮度
  //清屏,显示段码
  for (j=0;j<17;j++)
  {
    dispclear();
    for(i=0;i<8;i++)
    {	 		
       display(i,CathodeCode[j]);
       disp();//显示		
    }

    for(k=0;k<1000;k++)
       clock_delay_usec(1000);    
  }
  while(1)
  { //读取按键值
     Key = Read_key_A();
     //如果有按键按下,则显示按键值
     if(Key > 0)
     {
	  dispclear();
	  display(1,CathodeCode[Key/10]);//显示按键十位
	  display(0,CathodeCode[Key%10]);//显示按键个位
	  disp(); 
     }     
  }
  //进程结束
  PROCESS_END();
  /*测试*/
}
/*---------------------------------------------------------------------------*/
