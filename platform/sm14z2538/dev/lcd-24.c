#include "reg.h"			//寄存器定义
#include "dev/lcd-24.h"		//液晶屏库
#include "dev/gpio.h"		//gpio库
#include "clock.h"			//时钟库
#include "dev/font.h"		//字库


#define LCD_W 240			//LCD宽度
#define LCD_H 320			//LCD高度

unsigned char bdata ,bitdata;  

unsigned int BACK_COLOR,POINT_COLOR;  

/*--------------------------------------------------------------------------*/
//us延时
void delay_us(unsigned int time)
{
  clock_delay_usec(time);
}
//ms延时
/*--------------------------------------------------------------------------*/
void delay_ms(unsigned int time)
{ 
  unsigned int i;
  for(i=0;i<time;i++)
  	clock_delay_usec(1000);
}
/*--------------------------------------------------------------------------*/
//gpio设置
static void
gpio_set(int port, int bit)
{
  GPIO_WRITE_PIN(port, bit, bit);
}
/*---------------------------------------------------------------------------*/
//gpio复位
static void
gpio_reset(int port, int bit)
{
  GPIO_WRITE_PIN(port, bit, 0);
}
/*---------------------------------------------------------------------------*/
//gpio读
static int
gpio_get(int port, int bit)
{
  return GPIO_READ_PIN(port, bit);
}
/*---------------------------------------------------------------------------*/
//LCD接口配置
void LCD_GPIO_Config( void )
{
  GPIO_SET_OUTPUT(LCD_CS_PORT,LCD_CS_BIT);
  GPIO_SET_OUTPUT(LCD_MOSI_PORT,LCD_MOSI_BIT);
  GPIO_SET_OUTPUT(LCD_SCK_PORT,LCD_SCK_BIT);
  GPIO_SET_OUTPUT(LCD_DC_PORT,LCD_DC_BIT);
  GPIO_SET_OUTPUT(LCD_RST_PORT,LCD_RST_BIT);
}
//LCD写总线
void LCD_Writ_Bus(char da)   
{	
  unsigned char i;     
  for(i=8;i>0;i--)
  {
    if((da&0x80)==0x80)
      gpio_set(LCD_MOSI);
    else
      gpio_reset(LCD_MOSI);

    gpio_reset(LCD_SCK);
    da <<= 1;
    gpio_set(LCD_SCK);
  }
}
//LCD写字节
void LCD_WR_DATA8(char da) 
{
	gpio_set(LCD_DC); 
	LCD_Writ_Bus(da);
}
//LCD写数据
void LCD_WR_DATA(int da)
{
        gpio_set(LCD_DC);
	LCD_Writ_Bus(da>>8);
	LCD_Writ_Bus(da);
}
//LCD写寄存器
void LCD_WR_REG(char reg)	 
{
        gpio_reset(LCD_DC);
	LCD_Writ_Bus(reg);
}
//LCD初始化
void Lcd_Init(void)
{
    gpio_set(LCD_CS);               
    gpio_reset(LCD_RST);   
    delay_ms(20);	
    gpio_set(LCD_RST);	
    delay_ms(20);	
    gpio_reset(LCD_CS);          

    LCD_WR_DATA8(0x39); 
    LCD_WR_DATA8(0x2C); 
    LCD_WR_DATA8(0x00); 
    LCD_WR_DATA8(0x34); 
    LCD_WR_DATA8(0x02); 

    LCD_WR_REG(0xCF);  	
    LCD_WR_DATA8(0x00); 
    LCD_WR_DATA8(0XC1); 			
    LCD_WR_DATA8(0X30); 			
 	
    LCD_WR_REG(0xE8);  				
    LCD_WR_DATA8(0x85); 			
    LCD_WR_DATA8(0x00); 			
    LCD_WR_DATA8(0x78); 			
 	
    LCD_WR_REG(0xEA);  				
    LCD_WR_DATA8(0x00); 			
    LCD_WR_DATA8(0x00); 			
 	
    LCD_WR_REG(0xED);  				
    LCD_WR_DATA8(0x64); 			
    LCD_WR_DATA8(0x03); 			
    LCD_WR_DATA8(0X12); 			
    LCD_WR_DATA8(0X81); 			

    LCD_WR_REG(0xF7);  
    LCD_WR_DATA8(0x20); 
   
    LCD_WR_REG(0xC0);               
    LCD_WR_DATA8(0x23);             
   
    LCD_WR_REG(0xC1);               
    LCD_WR_DATA8(0x10);             
 
    LCD_WR_REG(0xC5);               
    LCD_WR_DATA8(0x3E);             
    LCD_WR_DATA8(0x28); 			
 
    LCD_WR_REG(0xC7);               
    LCD_WR_DATA8(0x86);             
 
    LCD_WR_REG(0x36);               
    LCD_WR_DATA8(0x48); 	        

    LCD_WR_REG(0x3A);    			
    LCD_WR_DATA8(0x55); 			

    LCD_WR_REG(0xB1);    			
    LCD_WR_DATA8(0x00);  			
    LCD_WR_DATA8(0x18); 			
 
    LCD_WR_REG(0xB6);               
    LCD_WR_DATA8(0x08); 			
    LCD_WR_DATA8(0x82);				
    LCD_WR_DATA8(0x27);  			
 	
    LCD_WR_REG(0xF2);               
    LCD_WR_DATA8(0x00); 			
 	
    LCD_WR_REG(0x26);               
    LCD_WR_DATA8(0x01); 			
 
    LCD_WR_REG(0xE0);               
    LCD_WR_DATA8(0x0F); 	       
    LCD_WR_DATA8(0x31); 			
    LCD_WR_DATA8(0x2B); 			
    LCD_WR_DATA8(0x0C); 			
    LCD_WR_DATA8(0x0E); 			
    LCD_WR_DATA8(0x08); 			
    LCD_WR_DATA8(0x4E); 			
    LCD_WR_DATA8(0xF1); 			
    LCD_WR_DATA8(0x37); 			
    LCD_WR_DATA8(0x07); 			
    LCD_WR_DATA8(0x10); 			
    LCD_WR_DATA8(0x03); 			
    LCD_WR_DATA8(0x0E); 			
    LCD_WR_DATA8(0x09); 			
    LCD_WR_DATA8(0x00); 			
									
    LCD_WR_REG(0XE1);               
    LCD_WR_DATA8(0x00); 			
    LCD_WR_DATA8(0x0E); 			
    LCD_WR_DATA8(0x14); 			
    LCD_WR_DATA8(0x03); 			
    LCD_WR_DATA8(0x11); 			
    LCD_WR_DATA8(0x07); 			
    LCD_WR_DATA8(0x31); 			
    LCD_WR_DATA8(0xC1); 			
    LCD_WR_DATA8(0x48); 			
    LCD_WR_DATA8(0x08); 			
    LCD_WR_DATA8(0x0F); 			
    LCD_WR_DATA8(0x0C); 			
    LCD_WR_DATA8(0x31); 			
    LCD_WR_DATA8(0x36); 			
    LCD_WR_DATA8(0x0F); 			

    LCD_WR_REG(0x11);               
    delay_ms(10); 					
			
     LCD_WR_REG(0x29);              
     LCD_WR_REG(0x2C); 				
}

void Address_set(unsigned int x1,unsigned int y1,unsigned int x2,unsigned int y2)
{  								
	LCD_WR_REG(0x2A);			
	LCD_WR_DATA8(x1>>8);		
	LCD_WR_DATA8(x1);			
	LCD_WR_DATA8(x2>>8);		
	LCD_WR_DATA8(x2);			
  
	LCD_WR_REG(0x2B);			
	LCD_WR_DATA8(y1>>8);		
	LCD_WR_DATA8(y1);			
	LCD_WR_DATA8(y2>>8);		
	LCD_WR_DATA8(y2);			

	LCD_WR_REG(0x2C);			

}

//LCD清屏
void LCD_Clear(unsigned int Color)
{
	unsigned int i,j;						 
	Address_set(0,0,LCD_W-1,LCD_H-1);        
        for(i=0;i<LCD_W;i++)			 
	{
		for (j=0;j<LCD_H;j++)		 
		{
			LCD_WR_DATA(Color);
		}
	}
}

//LCD画点
void LCD_DrawPoint(unsigned int x,unsigned int y)
{
	Address_set(x,y,x,y);       
	LCD_WR_DATA(POINT_COLOR); 	 
}
//LCD填充

void LCD_Fill(unsigned int xsta,unsigned int ysta,unsigned int xend,unsigned int yend,unsigned int color)
{          
	unsigned int i,j; 						  
	Address_set(xsta,ysta,xend,yend); 
  	for(i=ysta;i<=yend;i++)  		{      													   	 		for(j=xsta;j<=xend;j++)		  
		{
			LCD_WR_DATA(color);       
		} 	    	
	} 					  	    
} 
//LCD画点

void LCD_DrawPoint_big(unsigned int x,unsigned int y)
{
	LCD_Fill(x-1,y-1,x+1,y+1,POINT_COLOR);	
}

//LCD画线
void LCD_DrawLine(unsigned int x1, unsigned int y1, unsigned int x2, unsigned int y2)
{
	unsigned int t; 									
	int xerr=0,yerr=0,delta_x,delta_y,distance; 
	int incx,incy,uRow,uCol; 					

	delta_x = x2-x1;                            
	delta_y = y2-y1; 						 
	uRow = x1; 	
	uCol = y1; 	
	if(delta_x>0)								
	{
		incx = 1;                              
	} 
	else if(delta_x==0)		
	{
		incx = 0;                              
	} 
	else 										
	{
		incx = -1;		
		delta_x = -delta_x;	
	} 
	if(delta_y>0)	
	{
		incy = 1;
	} 
	else if(delta_y==0)
	{
		incy = 0;                           
	} 
	else	
	{
		incy = -1;	
		delta_y = -delta_y;	
	} 
	if(delta_x>delta_y)	
	{
		distance = delta_x;                 
	} 
	else 	
	{
		distance = delta_y;	
	} 
	for(t=0;t<=distance+1;t++)               
	{  
		LCD_DrawPoint(uRow,uCol);            
		xerr += delta_x ; 
		yerr += delta_y ; 
		if(xerr>distance) 
		{ 
			xerr -= distance; 
			uRow += incx; 	
		} 
		if(yerr>distance) 
		{ 
			yerr -= distance; 
			uCol += incy; 	
		} 
	}  
}

//LCD画矩形
void LCD_DrawRectangle(unsigned int x1, unsigned int y1, unsigned int x2, unsigned int y2)
{
	LCD_DrawLine(x1,y1,x2,y1);		 
	LCD_DrawLine(x1,y1,x1,y2);		 
	LCD_DrawLine(x1,y2,x2,y2);		 
	LCD_DrawLine(x2,y1,x2,y2);		 
}

//LCD画圆形
void Draw_Circle(unsigned int x0,unsigned int y0,unsigned char r)
{
	int a,b;					 
	int di;						  
	a = 0;						  
	b = r;	  					  
	di = 3-2*r;              
	while(a<=b)
	{		
		LCD_DrawPoint(x0+a,y0+b);            
		LCD_DrawPoint(x0-a,y0+b);                
		LCD_DrawPoint(x0+a,y0-b);            
		LCD_DrawPoint(x0-a,y0-b);             
		LCD_DrawPoint(x0+b,y0+a);  
		LCD_DrawPoint(x0-b,y0+a);          
		LCD_DrawPoint(x0+b,y0-a);  
		LCD_DrawPoint(x0-b,y0-a); 
		     
		if(di<0)	
		{
			di += 4*a+6;
		}	  	  
		else
		{
			di += 10+4*(a-b);   
			b--;
		}		              
		a++;					
		LCD_DrawPoint(x0+a,y0+b);
	}
} 

//LCD显示字符
void LCD_ShowChar(unsigned int x,unsigned int y,unsigned char num,unsigned char mode)
{
  
    unsigned char temp;							  
    unsigned char pos,t;						 
    unsigned int x0 = x;						  
    unsigned int colortemp = POINT_COLOR;      	  
    if(x>LCD_W-16||y>LCD_H-16)  return;	  
		   
	num = num-' ';                        
	Address_set(x,y,x+8-1,y+16-1);        
	if(!mode)                             
	{
		for(pos=0;pos<16;pos++)
		{ 
			temp = asc2_1608[(unsigned int)num*16+pos];
			for(t=0;t<8;t++)
		    {                 
		        if(temp&0x01)				 
				{
					POINT_COLOR = colortemp; 
				}
				else 		
				{
					POINT_COLOR = BACK_COLOR;
				}
				LCD_WR_DATA(POINT_COLOR);	 
				temp >>= 1; 				 
				x++;	
		    }
			x = x0;		
			y++;
		}	
	}
	else                                     
	{
		for(pos=0;pos<16;pos++)
		{
		    temp = asc2_1608[(unsigned int)num*16+pos];
			for(t=0;t<8;t++)
		    {                 
		        if(temp&0x01)				   
				{
					LCD_DrawPoint(x+t,y+pos);  
				}     
		        temp >>= 1; 				   
		    }
		}
	}
	POINT_COLOR = colortemp;	    	   	   
  
}   

unsigned long mypow(unsigned char m,unsigned char n)
{
	unsigned long result = 1;	 	 
	while(n--)				 
	{
		result*=m;			 
	}    
	return result;			
}			  

//LCD显示数字	 
void LCD_ShowNum(unsigned int x,unsigned int y,unsigned int num,unsigned char len)
{         	
	unsigned char t,temp;						   	
	for(t=0;t<len;t++)						 
	{
		temp = (num/mypow(10,len-t-1))%10;	 
	 	LCD_ShowChar(x+8*t,y,temp+'0',0); 	 
	}
} 
//LCD显示字符串
void LCD_ShowString(unsigned int x,unsigned int y,const unsigned char *p)
{         
	while(*p!='\0')					
       {       
	if(x>LCD_W-16)				
	{
	   x = 0;					
	   y += 16;				
	}
        if(y>LCD_H-16)				 
	{
	   y=x = 0;				 
	}
        LCD_ShowChar(x,y,*p,0);		 
        x += 8;						 
        p++;						
    }  
}
//LCD显示汉子
void LCD_Showhanzi(unsigned int x,unsigned int y,unsigned char index)	
{  
	unsigned char i,j;								
	unsigned char *temp = hanzi;    				
    Address_set(x,y,x+31,y+31);             
	temp += index*128;	
	for(j=0;j<128;j++)
	{
		for(i=0;i<8;i++)
		{ 		     
			if((*temp&(1<<i))!=0)			
			{
				LCD_WR_DATA(POINT_COLOR);	
			} 
			else
			{
				LCD_WR_DATA(BACK_COLOR);	
			}   
		}
		temp++;								
	 }
}
