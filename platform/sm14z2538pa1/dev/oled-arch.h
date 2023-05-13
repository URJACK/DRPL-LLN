#ifndef __OLED_ARCH_H
#define __OLED_ARCH_H	

void OLED_WrDat(unsigned char dat);	                        //OLED写数据
void OLED_WrCmd(unsigned char cmd);                         //OLED写命令
void OLED_Setpos(unsigned char y,unsigned char column);     //设置点
void OLED_Fill(unsigned char dd);	                        //填充
void OLED_Display_On(void);                                 //显示开
void OLED_Display_Off(void);                                //显示关
							   		    
void OLED_Init(void);                                       //OLED初始化

#endif  
	 



