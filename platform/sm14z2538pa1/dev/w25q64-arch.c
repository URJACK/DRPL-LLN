#include "contiki.h"            //contiki系统库
#include "reg.h"                //寄存器
#include "dev/w25q64-arch.h"    //W25Q64驱动
#include "dev/gpio.h"           //GPIO
#include "clock.h"              //时钟库
//FLASH类型
unsigned int SPI_FLASH_TYPE=W25Q64;
/*---------------------------------------------------------------------------*/
//us延时
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
//FLASH初始化
void SPI_Flash_Init(void)
{	
  GPIO_SET_OUTPUT(CS_PORT,CS_BIT);
  GPIO_SET_OUTPUT(DI_PORT,DI_BIT);
  GPIO_SET_OUTPUT(CLK_PORT,CLK_BIT);

  GPIO_SET_INPUT(DO_PORT,DO_BIT);

  gpio_set(CLK);
  gpio_set(DO);
  gpio_set(DI);
  gpio_set(CS);
}  
/*---------------------------------------------------------------------------*/
//SPI写
void SPI_Write(unsigned char data)
{
    unsigned char i;

    for( i = 0; i < 8 ; i++ )
    {
        if((data & 0x80)==0x80 )
           gpio_set(DI);
        else
           gpio_reset(DI);
        
        gpio_reset(CLK);
        data <<= 1;
        delay_us(4);
        gpio_set(CLK);
    }
}
/*---------------------------------------------------------------------------*/
//SPI读
unsigned char SPI_Read(void)
{
    unsigned char i;
    unsigned char temp=0;
   
    for( i = 0; i < 8 ; i++ )
    {     
        temp<<=1;
        gpio_reset(CLK);
        if( gpio_get(DO)!=0 )
            temp|=0x01;
        gpio_set(CLK);
    }
    return temp;
}
/*---------------------------------------------------------------------------*/
unsigned char SPI_Flash_ReadSR(void)   
{  
        unsigned char byte=0; 

        gpio_reset(CS);
        SPI_Write(W25X_ReadStatusReg);
        byte=SPI_Read();
        gpio_set(CS); 
	return byte;   
} 
/*---------------------------------------------------------------------------*/
void SPI_FLASH_Write_SR(unsigned char sr)   
{   
        gpio_reset(CS);
        SPI_Write(W25X_WriteStatusReg);
        SPI_Write( sr);
        gpio_set(CS);
}  
/*---------------------------------------------------------------------------*/
//SPI写使能
void SPI_FLASH_Write_Enable(void)   
{
        gpio_reset(CS);
        SPI_Write(W25X_WriteEnable);
        gpio_set(CS);
} 
/*---------------------------------------------------------------------------*/
//SPI写禁止
void SPI_FLASH_Write_Disable(void)   
{  
        gpio_reset(CS);
        SPI_Write(W25X_WriteDisable);
        gpio_set(CS);  
} 
/*---------------------------------------------------------------------------*/
//读取SPI FLASH的ID
unsigned int SPI_Flash_ReadID(void)
{
	unsigned int Temp = 0;	  
	gpio_reset(CS);				    
	SPI_Write(0x90);

	SPI_Write(0x00); 	    
	SPI_Write(0x00); 	    
	SPI_Write(0x00);

	Temp|=SPI_Read()<<8; 
	Temp|=SPI_Read();
	gpio_set(CS);	
    
	return Temp;
}   
/*---------------------------------------------------------------------------*/
//SPI FLASH读
void SPI_Flash_Read(unsigned char* pBuffer,unsigned int ReadAddr,unsigned int NumByteToRead)   
{ 
    unsigned int i;    												    
    gpio_reset(CS);

    SPI_Write(W25X_ReadData);
    SPI_Write(ReadAddr>>16);
    SPI_Write(ReadAddr>>8);
    SPI_Write(ReadAddr);
    for(i=0;i<NumByteToRead;i++)
    { 
        pBuffer[i]=SPI_Read();   
    }
	gpio_set(CS); 	      
}  
/*---------------------------------------------------------------------------*/
//FLASH写
void SPI_Flash_Write_Page(unsigned char* pBuffer,unsigned int WriteAddr,unsigned int NumByteToWrite)
{
    unsigned int i;  
    SPI_FLASH_Write_Enable();   
    gpio_reset(CS);
    SPI_Write(W25X_PageProgram);     
    SPI_Write((WriteAddr)>>16); 
    SPI_Write((WriteAddr)>>8);   
    SPI_Write(WriteAddr);   
    for(i=0;i<NumByteToWrite;i++)
      SPI_Write(pBuffer[i]);
    gpio_set(CS);
    SPI_Flash_Wait_Busy();	
} 

/*---------------------------------------------------------------------------*/
//FLASH不检查
void SPI_Flash_Write_NoCheck(unsigned char* pBuffer,unsigned int WriteAddr,unsigned int NumByteToWrite)   
{ 			 		 
	unsigned int pageremain;	   
	pageremain=256-WriteAddr%256; 	 	    
	if(NumByteToWrite<=pageremain)pageremain=NumByteToWrite;
	while(1)
	{	   
		SPI_Flash_Write_Page(pBuffer,WriteAddr,pageremain);
		if(NumByteToWrite==pageremain)break;
	 	else 
		{
			pBuffer+=pageremain;
			WriteAddr+=pageremain;	

			NumByteToWrite-=pageremain;	
			if(NumByteToWrite>256)pageremain=256; 
			else pageremain=NumByteToWrite; 	 
		}
	};	    
} 

/*---------------------------------------------------------------------------*/
//FLASH缓冲区定义
unsigned char SPI_FLASH_BUF[4096];
//FLASH写
void SPI_Flash_Write(unsigned char* pBuffer,unsigned int WriteAddr,unsigned int NumByteToWrite)   
{ 
	unsigned int secpos;
	unsigned int secoff;
	unsigned int secremain;	   
 	unsigned int i;    

	secpos=WriteAddr/4096;
	secoff=WriteAddr%4096;
	secremain=4096-secoff;

	if(NumByteToWrite<=secremain)secremain=NumByteToWrite;
	while(1) 
	{	
		SPI_Flash_Read(SPI_FLASH_BUF,secpos*4096,4096);
		for(i=0;i<secremain;i++)
		{
			if(SPI_FLASH_BUF[secoff+i]!=0XFF)break;  	  
		}
		if(i<secremain)
		{
			SPI_Flash_Erase_Sector(secpos);
			for(i=0;i<secremain;i++)	   
			{
				SPI_FLASH_BUF[i+secoff]=pBuffer[i];	  
			}
			SPI_Flash_Write_NoCheck(SPI_FLASH_BUF,secpos*4096,4096);

		}else SPI_Flash_Write_NoCheck(pBuffer,WriteAddr,secremain);
		if(NumByteToWrite==secremain)break;
		else
		{
			secpos++;
			secoff=0;
		   	pBuffer+=secremain;  
			WriteAddr+=secremain;	   
		   	NumByteToWrite-=secremain;
			if(NumByteToWrite>4096)secremain=4096;	
			else secremain=NumByteToWrite;	
		}	 
	};	 	 
}

/*---------------------------------------------------------------------------*/
//SPI片擦除片选使能
void SPI_Flash_Erase_Chip(void)   
{                                             
    SPI_FLASH_Write_Enable();               
    SPI_Flash_Wait_Busy();   
    gpio_reset(CS);
    SPI_Write(W25X_ChipErase);       
    gpio_set(CS); 
    SPI_Flash_Wait_Busy();   	
}  
/*---------------------------------------------------------------------------*/
//扇区擦除
void SPI_Flash_Erase_Sector(unsigned int Dst_Addr)   
{   
    Dst_Addr*=4096;
    SPI_FLASH_Write_Enable();               	 
    SPI_Flash_Wait_Busy();   
    gpio_reset(CS);
    SPI_Write(W25X_SectorErase);     
    SPI_Write((Dst_Addr)>>16);      
    SPI_Write((Dst_Addr)>>8);   
    SPI_Write(Dst_Addr);
    gpio_set(CS);    	      
    SPI_Flash_Wait_Busy(); 
} 

/*---------------------------------------------------------------------------*/
void SPI_Flash_Wait_Busy(void)   
{   
   	
   while ((SPI_Flash_ReadSR()&0x01)==0x01);  
} 

/*---------------------------------------------------------------------------*/
void SPI_Flash_PowerDown(void)   
{ 
    gpio_reset(CS);
    SPI_Write(W25X_PowerDown);    
    gpio_set(CS); 
    delay_us(3); 

}   
/*---------------------------------------------------------------------------*/
void SPI_Flash_WAKEUP(void)   
{  
    gpio_reset(CS);
    SPI_Write(W25X_ReleasePowerDown);     
    gpio_set(CS);   
    delay_us(3);                   
}   






