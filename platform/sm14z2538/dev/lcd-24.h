#ifndef __LCD_24_H__
#define __LCD_24_H__

#define LCD_W 240			   
#define LCD_H 320			 


#define LCD_SCK_PORT  GPIO_A_BASE
#define LCD_SCK_BIT   (1 << 2)
#define LCD_SCK       LCD_SCK_PORT, LCD_SCK_BIT


#define LCD_MOSI_PORT GPIO_A_BASE
#define LCD_MOSI_BIT  (1 << 4)
#define LCD_MOSI      LCD_MOSI_PORT, LCD_MOSI_BIT


#define LCD_MISO_PORT GPIO_A_BASE
#define LCD_MISO_BIT  (1 << 5)
#define LCD_MISO      LCD_MISO_PORT, LCD_MISO_BIT


#define LCD_DC_PORT   GPIO_A_BASE
#define LCD_DC_BIT    (1 << 6)
#define LCD_DC        LCD_DC_PORT, LCD_DC_BIT


#define LCD_CS_PORT   GPIO_A_BASE
#define LCD_CS_BIT    (1 << 7)
#define LCD_CS        LCD_CS_PORT, LCD_CS_BIT

#define LCD_RST_PORT   GPIO_B_BASE
#define LCD_RST_BIT    (1 << 0)
#define LCD_RST        LCD_RST_PORT, LCD_RST_BIT

extern  unsigned int BACK_COLOR, POINT_COLOR;  
void delay_us(unsigned int time);
void delay_ms(unsigned int time);
void LCD_GPIO_Config( void );
void LCD_Writ_Bus(char da);	
void LCD_WR_DATA8(char da);	
void LCD_WR_DATA(int da);
void LCD_WR_REG(char reg);
void Lcd_Init(void); 	
void Address_set(unsigned int x1,unsigned int y1,unsigned int x2,unsigned int y2);

void LCD_Clear(unsigned int Color);	
void LCD_DrawPoint(unsigned int x,unsigned int y); 
void LCD_Fill(unsigned int xsta,unsigned int ysta,unsigned int xend,unsigned int yend,unsigned int color);
void LCD_DrawPoint_big(unsigned int x,unsigned int y);                     
void LCD_DrawLine(unsigned int x1, unsigned int y1, unsigned int x2, unsigned int y2);	
void LCD_DrawRectangle(unsigned int x1, unsigned int y1, unsigned int x2, unsigned int y2);
void Draw_Circle(unsigned int x0,unsigned int y0,unsigned char r);	
void LCD_ShowChar(unsigned int x,unsigned int y,unsigned char num,unsigned char mode);  
unsigned long mypow(unsigned char m,unsigned char n);	
void LCD_ShowNum(unsigned int x,unsigned int y,unsigned int num,unsigned char len);    
void LCD_ShowString(unsigned int x,unsigned int y,const unsigned char *p);
void LCD_Showhanzi(unsigned int x,unsigned int y,unsigned char index);  

#define WHITE         	 0xFFFF	
#define BLACK         	 0x0000	  	 
#define BLUE         	 0x001F  	
#define BRED             0XF81F
#define GRED 		 0XFFE0
#define GBLUE		 0X07FF
#define RED           	 0xF800
#define MAGENTA       	 0xF81F
#define GREEN         	 0x07E0	
#define CYAN          	 0x7FFF
#define YELLOW        	 0xFFE0		
#define BROWN 		 0XBC40      
#define BRRED 		 0XFC07      
#define GRAY  		 0X8430      


#define DARKBLUE      	 0X01CF	    
#define LIGHTBLUE      	 0X7D7C	    
#define GRAYBLUE       	 0X5458      
 
#define LIGHTGREEN     	 0X841F     
#define LGRAY 		 0XC618      

#define LGRAYBLUE        0XA651      
#define LBBLUE           0X2B12      

#endif
