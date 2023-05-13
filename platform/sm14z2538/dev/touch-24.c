#include "reg.h"
#include "dev/lcd-24.h"
#include "dev/touch-24.h"
#include "dev/gpio.h"
#include <stdio.h> /* For printf() */
#define  CMD_RDX  0xD0	 
#define  CMD_RDY  0x90

unsigned int vx = 13178,vy = 11304;	         
unsigned int chx = 449,chy = 3962; 			

struct tp_pix_  tp_pixad,tp_pixlcd;	  

/*---------------------------------------------------------------------------*/
static void
gpio_set(int port, int bit)
{
  GPIO_WRITE_PIN(port, bit, bit);
}
/*---------------------------------------------------------------------------*/
static void
gpio_reset(int port, int bit)
{
  GPIO_WRITE_PIN(port, bit, 0);
}
/*---------------------------------------------------------------------------*/
static int
gpio_get(int port, int bit)
{
  return GPIO_READ_PIN(port, bit);
}
/*---------------------------------------------------------------------------*/
unsigned char tpstate(void)
{
  return  gpio_get(TOUCH_IRQ);				
}

void spistar(void) 
{
  GPIO_SET_OUTPUT(TOUCH_CS_PORT,TOUCH_CS_BIT);
  GPIO_SET_OUTPUT(TOUCH_CLK_PORT,TOUCH_CLK_BIT);
  GPIO_SET_OUTPUT(TOUCH_DIN_PORT,TOUCH_DIN_BIT);
  
  GPIO_SET_INPUT(TOUCH_IRQ_PORT,TOUCH_IRQ_BIT); 
  GPIO_SET_INPUT(TOUCH_DO_PORT,TOUCH_DO_BIT);

  gpio_set(TOUCH_CS);			
  gpio_set(TOUCH_CLK);			
  gpio_set(TOUCH_DIN);			
  gpio_set(TOUCH_CLK);			
}

void WriteCharTo7843(unsigned char num)          
{
	unsigned char count = 0;			   
	gpio_reset(TOUCH_CLK);					   
	for(count=0;count<8;count++)   
	{
                if((num & 0x80) == 0x80)                   
                    gpio_set(TOUCH_DIN);
                else 
                    gpio_reset(TOUCH_DIN);    
		gpio_reset(TOUCH_CLK);				   
		delay_us(3);				  
		num <<= 1;				  
		gpio_set(TOUCH_CLK);
		delay_us(3);				 
	}
}

unsigned int ReadFromCharFrom7843()      
{
	unsigned char count = 0;				
	unsigned int Num = 0;					
	for(count=0;count<12;count++)	
	{
		Num <<= 1;					
		gpio_set(TOUCH_CLK); 					
		delay_us(3);					  
		gpio_reset(TOUCH_CLK); 					
		delay_us(3);					
                if(gpio_get(TOUCH_DO) != 0) {                      
                   Num |= 0x01;                                 
                }	
	}
	return(Num);					
}
		  
unsigned int ADS_Read_AD(unsigned char CMD)          
{
	unsigned int l;							
	gpio_reset(TOUCH_CS);							
	WriteCharTo7843(CMD);          
	gpio_set(TOUCH_CLK);
        delay_us(4);						
	gpio_reset(TOUCH_CLK); 						
	delay_us(4);						
	l = ReadFromCharFrom7843();		
	gpio_set(TOUCH_CS);				
	return l;					
}		   

#define READ_TIMES 15              
#define LOST_VAL 5	                
unsigned int ADS_Read_XY(unsigned char xy)
{
	unsigned int i, j;						
	unsigned int buf[READ_TIMES];			
	unsigned int sum = 0;					
	unsigned int temp;				
	for(i=0;i<READ_TIMES;i++)		
	{				 
		buf[i] = ADS_Read_AD(xy);	
	}				    
	for(i=0;i<READ_TIMES-1; i++)    
	{
		for(j=i+1;j<READ_TIMES;j++)
		{
			if(buf[i]>buf[j])      
			{
				temp = buf[i];
				buf[i] = buf[j];
				buf[j] = temp;
			}
		}
	}	  
	sum = 0;						
	for(i=LOST_VAL;i<READ_TIMES-LOST_VAL;i++) 
	{
		sum += buf[i];				
	}
	temp = sum/(READ_TIMES-2*LOST_VAL);		 
	return temp;   		
} 

unsigned char Read_ADS(unsigned int *x,unsigned int *y)
{
	unsigned int xtemp,ytemp;			 	 	  
	xtemp = ADS_Read_XY(CMD_RDX);		  
	ytemp = ADS_Read_XY(CMD_RDY);	 	 			   
	if(xtemp<100||ytemp<100)   return 0;  
	*x = xtemp;							  
	*y = ytemp;							 
	return 1;                            
}


#define ERR_RANGE 20                     
unsigned char Read_ADS2(unsigned int *x,unsigned int *y) 
{
    unsigned int x1,y1;							 
    unsigned int x2,y2;							  
    unsigned char flag;    						
    flag = Read_ADS(&x1,&y1);   		  
    if(flag==0)   return(0);			  
    flag = Read_ADS(&x2,&y2);			 
    if(flag==0)   return(0);   			 
    if(((x2<=x1&&x1<x2+ERR_RANGE)||(x1<=x2&&x2<x1+ERR_RANGE))
    &&((y2<=y1&&y1<y2+ERR_RANGE)||(y1<=y2&&y2<y1+ERR_RANGE))) 
    {
	*x=(x1+x2)>>1;					   
        *y=(y1+y2)>>1;					   
        return 1;						   
    }
	else return 0;	  					   
} 
	   
unsigned char Read_TP_Once(void)
{
	unsigned char re = 0;								   
	unsigned int x1,y1;				
	while(re==0)								   
	{
		while(!Read_ADS2(&tp_pixad.x,&tp_pixad.y));
		delay_ms(10);							   
		while(!Read_ADS2(&x1,&y1));				   
		if(tp_pixad.x==x1&&tp_pixad.y==y1)		   
		{
			re = 1; 							   
		}
	} 
	return re;				
}

void Drow_Touch_Point(unsigned int x,unsigned int y)
{
	LCD_DrawLine(x-12,y,x+13,y);       
	LCD_DrawLine(x,y-12,x,y+13);       
	LCD_DrawPoint(x+1,y+1);			   
	LCD_DrawPoint(x-1,y+1);			   
	LCD_DrawPoint(x+1,y-1);			   
	LCD_DrawPoint(x-1,y-1);			  
}	  

unsigned char Convert_Pos(void)
{		 	 
	unsigned char l = 0; 							 
	if(Read_ADS2(&tp_pixad.x,&tp_pixad.y))	
	{
		l = 1;					
		tp_pixlcd.x=tp_pixad.x>chx?((unsigned long)tp_pixad.x-(unsigned long)chx)*1000/vx:((unsigned long)chx-(unsigned long)tp_pixad.x)*1000/vx;  
		tp_pixlcd.y=tp_pixad.y>chy?((unsigned long)tp_pixad.y-(unsigned long)chy)*1000/vy:((unsigned long)chy-(unsigned long)tp_pixad.y)*1000/vy; 
	}
	return l;
}	   

#define tp_pianyi 50       	
#define tp_xiaozhun 1000   
void Touch_Adjust(void)
{	
	float vx1,vx2,vy1,vy2;                
	unsigned int chx1,chx2,chy1,chy2;             
	unsigned int lx,ly;				 			 
	struct tp_pixu32_ p[4];				 
	unsigned char  cnt = 0;	 					  
	POINT_COLOR=RED;                      
	BACK_COLOR = WHITE;					  
	LCD_Clear(WHITE);                      
	Drow_Touch_Point(tp_pianyi,tp_pianyi);
	while(1)
	{
		if(gpio_get(TOUCH_IRQ)==0)                     
		{
			if(Read_TP_Once())           
			{  								           		p[cnt].x = tp_pixad.x;	 
				p[cnt].y = tp_pixad.y;	 
				cnt++; 					 
			}			 
					
			if(cnt==1)
                        {	
                                LCD_Clear(WHITE);    
			 	while(!gpio_get(TOUCH_IRQ))      
				{
				}
				Drow_Touch_Point(LCD_W-tp_pianyi-1,tp_pianyi);
			}				 
			else if(cnt==2)
                        {				 
				LCD_Clear(WHITE);    
				while(!gpio_get(TOUCH_IRQ))       
				{
				}
				Drow_Touch_Point(tp_pianyi,LCD_H-tp_pianyi-1);
			}				 
			else if(cnt==3)
                        {					
				LCD_Clear(WHITE);     
				while(!gpio_get(TOUCH_IRQ))       
				{
				}
				Drow_Touch_Point(LCD_W-tp_pianyi-1,LCD_H-tp_pianyi-1);
			}				
			else if(cnt==4)
                        {	                
	    		   	LCD_Clear(WHITE);    
				while(!gpio_get(TOUCH_IRQ))      
				{
				}
			   	vx1 = p[1].x>p[0].x?(p[1].x-p[0].x+1)*1000/(LCD_W-tp_pianyi-tp_pianyi):(p[0].x-p[1].x-1)*1000/(LCD_W-tp_pianyi-tp_pianyi); 
				chx1 = p[1].x>p[0].x?p[0].x-(vx1*tp_pianyi)/1000:p[0].x+(vx1*tp_pianyi)/1000;
				vy1 = p[2].y>p[0].y?(p[2].y-p[0].y-1)*1000/(LCD_H-tp_pianyi-tp_pianyi):(p[0].y-p[2].y-1)*1000/(LCD_H-tp_pianyi-tp_pianyi); 
				chy1 = p[2].y>p[0].y?p[0].y-(vy1*tp_pianyi)/1000:p[0].y+(vy1*tp_pianyi)/1000; 
					
				vx2 = p[3].x>p[2].x?(p[3].x-p[2].x+1)*1000/(LCD_W-tp_pianyi-tp_pianyi):(p[2].x-p[3].x-1)*1000/(LCD_W-tp_pianyi-tp_pianyi); 
				chx2 = p[3].x>p[2].x?p[2].x-(vx2*tp_pianyi)/1000:p[2].x+(vx2*tp_pianyi)/1000;
				vy2 = p[3].y>p[1].y?(p[3].y-p[1].y-1)*1000/(LCD_H-tp_pianyi-tp_pianyi):(p[1].y-p[3].y-1)*1000/(LCD_H-tp_pianyi-tp_pianyi); 
				chy2 = p[3].y>p[1].y?p[1].y-(vy2*tp_pianyi)/1000:p[1].y+(vy2*tp_pianyi)/1000; 


				if((vx1>vx2&&vx1>vx2+tp_xiaozhun)||(vx1<vx2&&vx1<vx2-tp_xiaozhun)||(vy1>vy2&&vy1>vy2+tp_xiaozhun)||(vy1<vy2&&vy1<vy2-tp_xiaozhun))
				{
						cnt = 0;								   
						LCD_Clear(WHITE);                      
						Drow_Touch_Point(tp_pianyi,tp_pianyi); 
						continue;
				}
					vx=(vx1+vx2)/2;vy=(vy1+vy2)/2;			  
					chx=(chx1+chx2)/2;chy=(chy1+chy2)/2;	  
					LCD_Clear(WHITE);                          
					POINT_COLOR = BLUE;	
					BACK_COLOR = WHITE;	
			
					lx = 0;	
					ly = 50;	
					LCD_ShowString(lx,ly,"VX1:");	
					lx += 40;	
					LCD_ShowNum(lx,ly,vx1,5);	
					lx = 0;		
					ly += 20;	
					LCD_ShowString(lx,ly,"Vy1:");	
					lx += 40;		
					LCD_ShowNum(lx,ly,vy1,5);	
					lx = 0;	
					ly += 20; 		
					LCD_ShowString(lx,ly,"CHX1:");	
					lx += 40;		
					LCD_ShowNum(lx,ly,chx1,5);	
				        lx = 0;			
					ly += 20; 	
					LCD_ShowString(lx,ly,"CHY1:");	
					lx += 40;	
					LCD_ShowNum(lx,ly,chy1,5);	

					lx = 100;		
					ly = 50;		
					LCD_ShowString(lx,ly,"VX2:");	
					lx += 40;			
					LCD_ShowNum(lx,ly,vx2,5);	
					lx = 100;		
					ly += 20;				
					LCD_ShowString(lx,ly,"Vy2:");		
					lx += 40;		
					LCD_ShowNum(lx,ly,vy2,5);	
					lx = 100;	
					ly += 20; 		
					LCD_ShowString(lx,ly,"CHX2:");	
					lx += 40;		
					LCD_ShowNum(lx,ly,chx2,5);
				        lx = 100;		
					ly += 20; 		
					LCD_ShowString(lx,ly,"CHY2:");	
					lx += 40;		
					LCD_ShowNum(lx,ly,chy2,5);	
				
					lx = 50;	
					ly = 150;	
					LCD_ShowString(lx,ly,"VX:");
					lx += 40;	
					LCD_ShowNum(lx,ly,vx,5);	
					lx = 50;	
					ly += 20;	
					LCD_ShowString(lx,ly,"Vy:");	
					lx += 40;		
					LCD_ShowNum(lx,ly,vy,5);		
					lx = 50;	
					ly += 20; 				
					LCD_ShowString(lx,ly,"CHX:");	
					lx += 40;			
					LCD_ShowNum(lx,ly,chx,5);	
				        lx = 50;
					ly += 20; 				
					LCD_ShowString(lx,ly,"CHY:");	
					lx += 40;		
					LCD_ShowNum(lx,ly,chy,5);	

					lx = 30;
					ly += 30;	
					LCD_ShowString(lx,ly,"Adjust OK!  Touch Anywhere To Continue");								  
					Read_TP_Once();                           

					LCD_Clear(WHITE);                         
                                        return;
			}
		}
	} 
}

void point(void) 
{
    double t = 0;				                           
	
    while(1)
	{  	
		if(gpio_get(TOUCH_IRQ)==0)			               
		{
			t = 0;										   
			if(Convert_Pos())	                           
			{	
				LCD_ShowString(10,250,"X:");			   
				LCD_ShowNum(30,250,tp_pixad.x,4);		   
				LCD_ShowString(180,250,"Y:");			   
				LCD_ShowNum(200,250,tp_pixad.y,4);		   
				LCD_DrawPoint_big(tp_pixlcd.x,tp_pixlcd.y);
			 }
			
		}
		else
		{	
			t++;	
			if(t>65000)		
			{
				return;	
			}
		}		

	}
}
