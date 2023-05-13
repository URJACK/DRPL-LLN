#ifndef __DS18B20_H
#define	__DS18B20_H
//io定义
#define DQ_PORT   GPIO_B_BASE
#define DQ_BIT    2
//输出
#define DS18B20_DQ_OUT(a)   if (a)	\
				GPIO_WRITE_PIN(DQ_PORT, DQ_BIT, DQ_BIT);\
		            else		\
		                GPIO_WRITE_PIN(DQ_PORT, DQ_BIT, 0)
//输入
#define DS18B20_DQ_IN      GPIO_READ_PIN(DQ_PORT,DQ_BIT)

void DS18B20_IO_OUT(void);					//输出
void DS18B20_IO_IN(void);					//输入
void DS18B20_Rst(void);						//复位
unsigned char DS18B20_Check(void);			//检查
unsigned char DS18B20_Read_Bit(void);     	//读一个位
unsigned char DS18B20_Read_Byte(void);    	//读一个字节
void DS18B20_Write_Byte(unsigned char dat);	//写字节
void DS18B20_Start(void);					//开始温度采集	 
unsigned char DS18B20_Init(void);			//初始化温度传感器
short DS18B20_Get_Temp(void);				//获得温度值


#endif

