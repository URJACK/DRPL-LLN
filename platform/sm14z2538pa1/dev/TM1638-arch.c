#include "contiki.h"
#include "reg.h"
#include "dev/TM1638-arch.h"
#include "dev/gpio.h"
#include "clock.h"
#include <stdio.h> /* For printf() */

unsigned char seg[8]={0,0,0,0,0,0,0,0};
/*---------------------------------------------------------------------------*/
void delay_us(unsigned int time)
{
  clock_delay_usec(time);
}
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
//数码管显示模块初始化
void TM1638_init(void)
{  
    GPIO_SET_OUTPUT(STB_PORT,STB_BIT);
    GPIO_SET_OUTPUT(CLK_PORT,CLK_BIT);
    GPIO_SET_OUTPUT(DIO_PORT,DIO_BIT);
}
/*---------------------------------------------------------------------------*/
//写
void TM1638_Write(unsigned char data)
{
    unsigned char i;  
    GPIO_SET_OUTPUT(DIO_PORT,DIO_BIT);
    for( i = 0; i < 8 ; i++ )
    {
        gpio_reset(CLK_PORT, CLK_BIT);  
        delay_us(1);
        if( data & 0x01 ) 
           gpio_set(DIO_PORT, DIO_BIT);
        else 
           gpio_reset(DIO_PORT, DIO_BIT);      
        data >>= 1;                            
        gpio_set(CLK_PORT, CLK_BIT);          
        delay_us(1);
    }
}
//读
unsigned char TM1638_Read(void)
{
    unsigned char i;
    unsigned char temp=0;  
    GPIO_SET_INPUT(DIO_PORT,DIO_BIT);
    for( i = 0; i < 8 ; i++ )
    {
        gpio_reset(CLK_PORT, CLK_BIT);  
        temp>>=1;
        delay_us(1);
        if( gpio_get(DIO_PORT, DIO_BIT)!=0 ) 
            temp|=0x80;
        gpio_set(CLK_PORT, CLK_BIT);          
        delay_us(1);
    }
    return temp;
}
//写命令
void Write_COM(unsigned char cmd)
{
    gpio_reset(STB_PORT, STB_BIT);  
    TM1638_Write(cmd);
    gpio_set(STB_PORT, STB_BIT);  
}
//写数据
void Write_DATA(unsigned char add, unsigned char data)
{
    gpio_reset(STB_PORT, STB_BIT);  
    TM1638_Write(add);
    TM1638_Write(data);
    gpio_set(STB_PORT, STB_BIT);
}
//读键值
unsigned char Read_key_A(void)
{
    unsigned char Key=0;
    unsigned char KeyCode[4];
    unsigned char i;
    gpio_reset(STB_PORT, STB_BIT);
    TM1638_Write(0x42);
    delay_us(10);
    for(i=0;i<4;i++)
        KeyCode[i]=TM1638_Read();

    gpio_set(STB_PORT, STB_BIT);

    if( KeyCode[0] > 0 ) 
    {
        if( KeyCode[0] == 0x02)
            Key = 9;
        else if( KeyCode[0] == 0x04)
            Key = 1;
        else if( KeyCode[0] == 0x20)
            Key = 10;
        else if( KeyCode[0] == 0x40)
            Key = 2;
    }
    else if( KeyCode[1] > 0 )   
    {
        if(KeyCode[1] ==0x02)
            Key = 11;
        else if(KeyCode[1] ==0x04)
            Key = 3;
        else if(KeyCode[1] ==0x20)
            Key = 12;
        else if(KeyCode[1] ==0x40)
            Key = 4;
    }
    else if( KeyCode[2] > 0 )   
    {
        if(KeyCode[2] ==0x02)
            Key = 13;
        else if(KeyCode[2] ==0x04)
            Key = 5;
        else if(KeyCode[2] ==0x20)
            Key = 14;
        else if(KeyCode[2] ==0x40)
            Key = 6;
    }
    else if( KeyCode[3] > 0 )   
    {
        if(KeyCode[3] ==0x02)
            Key = 15;
        else if(KeyCode[3] ==0x04)
            Key = 7;
        else if(KeyCode[3] ==0x20)
            Key = 16;
        else if(KeyCode[3] ==0x40)
            Key = 8;
     }
   
     return Key;    
}
/*---------------------------------------------------------------------------*/
//读键值
unsigned char Read_key_B(void)
{
    unsigned char Key=0;
    unsigned char KeyCode[4];
    unsigned char i;
    gpio_reset(STB_PORT, STB_BIT);  
    TM1638_Write(0x42);
    delay_us(10);
    for(i=0;i<4;i++) 
	KeyCode[i]=TM1638_Read();
    
    gpio_set(STB_PORT, STB_BIT);  

    if( KeyCode[0] > 0 ) 
    {
        if( KeyCode[0] == 0x01)
            Key = 1;
        else if(KeyCode[0] ==0x10)
            Key = 5;
    }   
    else if( KeyCode[1] > 0 ) 
    {
        if( KeyCode[1] == 0x01)
            Key = 2;
        else if(KeyCode[1] ==0x10)
            Key = 6;
    }   
    else if( KeyCode[2] > 0 ) 
    {
        if( KeyCode[2] == 0x01)
            Key = 3;
        else if(KeyCode[2] ==0x10)
            Key = 7;
    }   
    else if( KeyCode[3] > 0 ) 
    {
        if( KeyCode[3] == 0x01)
            Key = 4;
        else if(KeyCode[3] ==0x10)
            Key = 8;
    }   
    return Key;
}
//显示
void disp(void)
{
   unsigned char i;
   for(i=0;i<8;i++)
   {
      Write_DATA( TM1638_DIG0+i*2, seg[i]);
   }
}
/*---------------------------------------------------------------------------*/
//清空显示区
void dispclear(void)
{
    unsigned char i;
    for(i=0;i<8;i++)
    {		
      seg[i]=0x00;
    }
}
void dispclear_B(void)
{
    unsigned char i;
    for(i=0;i<8;i++)
    {		
      Write_DATA( TM1638_DIG0+i*2, 0x00);
    }
}
//清空显示LED
void dispclear_led(void)
{
    unsigned char i;
    for(i=0;i<8;i++)
    {		
      display_led(TM1638_DIG0+2*i+1,0);
    }
}
/*---------------------------------------------------------------------------*/
//显示
void display(unsigned char dig,unsigned char dat)
{
     unsigned char i;
     for (i=0;i<8;i++)
     {
	if(dat&0x01)
	{
	    seg[i]+=0x01<<dig;	
	}else
	{
	    seg[i]+=0x00;
	}
	dat=dat>>1;
     }
}
//显示LED
void display_led(unsigned char dig,unsigned char dat)
{
   Write_DATA(dig,dat);
}
/*---------------------------------------------------------------------------*/

/**
 * @}
 * @}
 */
