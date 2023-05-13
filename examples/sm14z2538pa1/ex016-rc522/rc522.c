#include "contiki.h"              //contiki系统api
#include "dev/mfrc522-arch.h"     //mfrc522模块驱动
#include <stdio.h>                //标注设备的输入输出接口
#include <stdbool.h>              //布尔变量库，支持C99标准
/*---------------------------------------------------------------------------*/
//进程声明
PROCESS(rc522_process, "rc522");
//系统初始化的时候自动启动进程
AUTOSTART_PROCESSES(&rc522_process);
//IC卡测试
void IC_test ( void )
{
  unsigned char ucArray_ID [ 4 ];          
  unsigned char ucStatusReturn;       
  bool bFind = false;  
	
  while ( 1 )
  { 
    if ( ( ucStatusReturn = PcdRequest ( PICC_REQALL, ucArray_ID ) ) != MI_OK )
	ucStatusReturn = PcdRequest ( PICC_REQALL, ucArray_ID );	
		
    if ( ucStatusReturn != MI_OK )    
	bFind = false;
		
    if ( ( ucStatusReturn == MI_OK ) && ( bFind == false ) )
    {
	  if ( PcdAnticoll ( ucArray_ID ) == MI_OK ) 
	  {
		bFind = true;
				
		printf ( "The Card ID is: " );   
		printf ( "%02X%02X%02X%02X\n", ucArray_ID [ 0 ], ucArray_ID [ 1 ], ucArray_ID [ 2 ], ucArray_ID [ 3 ] );  
						
	   }	
     }		
  }
}

/*---------------------------------------------------------------------------*/
//进程实现
PROCESS_THREAD(rc522_process, ev, data)
{
  //进程开始
  PROCESS_BEGIN();
  //rc522硬件模块初始化
  RC522_Init ();   
	//输出信息到串口终端	
  printf ( "WF-RC522 Test\n" );	
	//
  PcdReset ();
  //卡的类型配置
  M500PcdConfigISOType ( 'A' );
	
  while ( 1 )
  {
    //ic卡测试
    IC_test ();	
  }
  //进程结束
  PROCESS_END();
  /*测试*/
}
/*---------------------------------------------------------------------------*/
