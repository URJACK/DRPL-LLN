#include "contiki.h"      //contiki系统接口
#include "dev/lcd-24.h"   //lcd液晶屏接口
#include "dev/touch-24.h" //触摸屏接口
#include "dev/font.h"     //字库
#include <stdio.h>        //标准设备的输入输出接口
/*---------------------------------------------------------------------------*/
//检测
unsigned char jiance() 
{
  if (tpstate()==0)    
  {
	BACK_COLOR = WHITE;	
	POINT_COLOR = RED;	
	point();	
	return 1;		
  }
  return 0;
}
/*---------------------------------------------------------------------------*/
//显示
void xianshi(void)
{   
  unsigned int lx,ly;	
  BACK_COLOR = WHITE;		         
  POINT_COLOR = RED;		             
  LCD_Showhanzi(10,0,0);                     
  LCD_Showhanzi(45,0,1);                      
  LCD_ShowString(10,35,"2.4 TFT SPI 240*320"); 
  LCD_ShowString(10,55,"LCD_W:");	
  LCD_ShowNum(70,55,LCD_W,3);		
  LCD_ShowString(110,55,"LCD_H:");	
  LCD_ShowNum(160,55,LCD_H,3);		
  lx = 10;			
  ly = 75;		
  LCD_ShowString(lx,ly,"VX:");	
  lx += 40;			
  LCD_ShowNum(lx,ly,vx,5);	
  lx += 60;		
  LCD_ShowString(lx,ly,"Vy:");	
  lx += 40;	
  LCD_ShowNum(lx,ly,vy,5);	
  lx = 10;	
  ly = 95; 	
  LCD_ShowString(lx,ly,"CHX:");	
  lx += 40;				
  LCD_ShowNum(lx,ly,chx,5);	
  lx += 60; 			
  LCD_ShowString(lx,ly,"CHY:");	
  lx += 40;				
  LCD_ShowNum(lx,ly,chy,5);
}
/*---------------------------------------------------------------------------*/
//显示图像
void showimage() 
{
  	int i,j,k; 		
	xianshi();                             
	for(k=3;k<7;k++)
	{
	   	for(j=0;j<6;j++)
		{	
		    Address_set(40*j,40*k,40*j+39,40*k+39);	
		    for(i=0;i<1600;i++)
			 { 			
				 LCD_WR_DATA8(image[i*2+1]);	
				 LCD_WR_DATA8(image[i*2]);
			 }	
		 }
	}
}
/*---------------------------------------------------------------------------*/
//进程声明
PROCESS(lcd24_process, "lcd-2.4");
//系统初始化自动启动进程
AUTOSTART_PROCESSES(&lcd24_process);
/*---------------------------------------------------------------------------*/
//进程实现
PROCESS_THREAD(lcd24_process, ev, data)
{
  //进程开始
  PROCESS_BEGIN();
  //启动spi
  spistar();
  //配置LCD接口	
  LCD_GPIO_Config();
  //LCD初始化
  Lcd_Init();    
  //LCD清屏             
  LCD_Clear(WHITE);
  //设置背景颜色           
  BACK_COLOR = BLACK;
  //设置前景颜色		
  POINT_COLOR = WHITE; 	
  //显示图像	
  showimage();	
  //
  Touch_Adjust();
  while(1)
  {
  //检测
	if(jiance())            
	{	
	}
  }
  //进程结束
  PROCESS_END();
}
/*---------------------------------------------------------------------------*/
