#include "contiki.h"			//contiki系统接口
#include "dev/lcd-24.h"			//2.4寸LCD模块驱动接口
#include <stdio.h> 				//标准设备的输入输出接口
#include <stdbool.h>			//支持C99标准,布尔变量使用
#include <stdint.h>				//支持C99标准,数据类型定义
/*---------------------------------------------------------------------------*/
//进程声明
PROCESS(lcd24_process, "lcd-2.4");
//系统初始化的时候自动启动这个进程
AUTOSTART_PROCESSES(&lcd24_process);
/*---------------------------------------------------------------------------*/
//进程实现
PROCESS_THREAD(lcd24_process, ev, data)
{ //进程开始
  PROCESS_BEGIN();
  //LCD接口配置
  LCD_GPIO_Config();
  //LCD模块初始化
  Lcd_Init();
  //LCD清屏                  
  LCD_Clear(WHITE);     
  //设置背景颜色       
  BACK_COLOR = BLACK;	
  //设置前景颜色		 
  POINT_COLOR = WHITE; 		 

  while(1)
  {	
	//显示绿色	 		 	          
	LCD_Clear(GREEN);		           
	delay_ms(1000);	
	//显示蓝色	           
	LCD_Clear(BLUE);		          
	delay_ms(1000);	
	//显示abcd	       
	LCD_ShowString(1,1,"abcd");	   
	delay_ms(1000);	
	//显示数字			  
	LCD_ShowNum(20,20,123,3);		
	delay_ms(1000);		
	//画点		  
	LCD_DrawPoint_big(50,40);		   
	delay_ms(1000);		
	//画矩形	
	LCD_DrawRectangle(80,100,100,130);
	delay_ms(1000);		
	//画圆形		
	Draw_Circle(100,200,50);		 
	delay_ms(2000);				 
  }
  //进程结束
  PROCESS_END();
}
/*---------------------------------------------------------------------------*/
