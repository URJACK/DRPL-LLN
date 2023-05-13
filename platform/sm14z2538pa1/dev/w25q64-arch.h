#ifndef __W25Q64_ARCH_H
#define __W25Q64_ARCH_H

//W25Q80 ID  0XEF13
//W25Q16 ID  0XEF14
//W25Q32 ID  0XEF15
//W25Q32 ID  0XEF16	
#define W25Q80 	0XEF13 	
#define W25Q16 	0XEF14
#define W25Q32 	0XEF15
#define W25Q64 	0XEF16

extern unsigned int SPI_FLASH_TYPE;
				 
#define W25X_WriteEnable		0x06 
#define W25X_WriteDisable		0x04 
#define W25X_ReadStatusReg		0x05 
#define W25X_WriteStatusReg		0x01 
#define W25X_ReadData			0x03 
#define W25X_FastReadData		0x0B 
#define W25X_FastReadDual		0x3B 
#define W25X_PageProgram		0x02 
#define W25X_BlockErase			0xD8 
#define W25X_SectorErase		0x20 
#define W25X_ChipErase			0xC7 
#define W25X_PowerDown			0xB9 
#define W25X_ReleasePowerDown	0xAB 
#define W25X_DeviceID			0xAB 
#define W25X_ManufactDeviceID	0x90 
#define W25X_JedecDeviceID		0x9F 
                              


#define CLK_PORT  GPIO_A_BASE
#define CLK_BIT   (1 << 2)
#define CLK       CLK_PORT, CLK_BIT


#define DO_PORT GPIO_A_BASE
#define DO_BIT  (1 << 5)
#define DO      DO_PORT, DO_BIT

                              
#define DI_PORT GPIO_A_BASE
#define DI_BIT  (1 << 4)
#define DI      DI_PORT, DI_BIT


#define CS_PORT   GPIO_B_BASE
#define CS_BIT    (1 << 5)
#define CS        CS_PORT, CS_BIT
                            

void SPI_Flash_Init(void);
unsigned int  SPI_Flash_ReadID(void);  	    
unsigned char SPI_Flash_ReadSR(void);        
void SPI_FLASH_Write_SR(unsigned char sr);  
void SPI_FLASH_Write_Enable(void);  
void SPI_FLASH_Write_Disable(void);	
void SPI_Flash_Write_NoCheck(unsigned char* pBuffer,unsigned int WriteAddr,unsigned int NumByteToWrite);
void SPI_Flash_Read(unsigned char* pBuffer,unsigned int ReadAddr,unsigned int NumByteToRead);
void SPI_Flash_Write(unsigned char* pBuffer,unsigned int WriteAddr,unsigned int NumByteToWrite);
void SPI_Flash_Erase_Chip(void);  
void SPI_Flash_Erase_Sector(unsigned int Dst_Addr);
void SPI_Flash_Wait_Busy(void);  
void SPI_Flash_PowerDown(void);          
void SPI_Flash_WAKEUP(void);
void SPI_Write(unsigned char data);
unsigned char SPI_Read(void);
#endif


