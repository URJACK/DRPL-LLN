#ifndef __TOUCH_24_H__
#define __TOUCH_24_H__


#define TOUCH_CLK_PORT    GPIO_B_BASE
#define TOUCH_CLK_BIT   (1 << 1)
#define TOUCH_CLK        TOUCH_CLK_PORT, TOUCH_CLK_BIT


#define TOUCH_CS_PORT     GPIO_B_BASE
#define TOUCH_CS_BIT     (1 << 2)
#define TOUCH_CS          TOUCH_CS_PORT, TOUCH_CS_BIT


#define TOUCH_DIN_PORT    GPIO_B_BASE
#define TOUCH_DIN_BIT    (1 << 3)
#define TOUCH_DIN        TOUCH_DIN_PORT, TOUCH_DIN_BIT


#define TOUCH_DO_PORT   GPIO_B_BASE
#define TOUCH_DO_BIT    (1 << 4)
#define TOUCH_DO        TOUCH_DO_PORT, TOUCH_DO_BIT


#define TOUCH_IRQ_PORT   GPIO_B_BASE
#define TOUCH_IRQ_BIT    (1 << 5)
#define TOUCH_IRQ        TOUCH_IRQ_PORT, TOUCH_IRQ_BIT


struct tp_pix_
{
	unsigned int x;
	unsigned int y;
};
struct tp_pixu32_
{
	unsigned long x;
	unsigned long y;
};
extern struct tp_pix_  tp_pixad,tp_pixlcd;	 
extern unsigned int vx,vy;                           
extern unsigned int chx,chy;                         

unsigned char tpstate(void);						
void spistar(void);  						 
void WriteCharTo7843(unsigned char num);			
unsigned int ReadFromCharFrom7843();                
unsigned int ADS_Read_AD(unsigned char CMD);				 
unsigned int ADS_Read_XY(unsigned char xy);					 
unsigned char Read_ADS(unsigned int *x,unsigned int *y);			 
unsigned char Read_ADS2(unsigned int *x,unsigned int *y);			
unsigned char Read_TP_Once(void);					
void Drow_Touch_Point(unsigned int x,unsigned int y);		 
unsigned char Convert_Pos(void);					
void Touch_Adjust(void);					
void point(void);                          
#endif
