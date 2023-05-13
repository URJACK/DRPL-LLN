#ifndef __MFRC522_ARCH_H
#define __MFRC522_ARCH_H	

/////////////////////////////////////////////////////////////////////
//MF522 Command
/////////////////////////////////////////////////////////////////////
#define PCD_IDLE              0x00               
#define PCD_AUTHENT           0x0E               
#define PCD_RECEIVE           0x08               
#define PCD_TRANSMIT          0x04               
#define PCD_TRANSCEIVE        0x0C               
#define PCD_RESETPHASE        0x0F               
#define PCD_CALCCRC           0x03               

/////////////////////////////////////////////////////////////////////
//Mifare_One Card Command
/////////////////////////////////////////////////////////////////////
#define PICC_REQIDL           0x26               
#define PICC_REQALL           0x52              
#define PICC_ANTICOLL1        0x93               
#define PICC_ANTICOLL2        0x95               
#define PICC_AUTHENT1A        0x60               
#define PICC_AUTHENT1B        0x61               
#define PICC_READ             0x30               
#define PICC_WRITE            0xA0               
#define PICC_DECREMENT        0xC0               
#define PICC_INCREMENT        0xC1               
#define PICC_RESTORE          0xC2               
#define PICC_TRANSFER         0xB0               
#define PICC_HALT             0x50               

/////////////////////////////////////////////////////////////////////
//The length of the MF522 FIFO define
/////////////////////////////////////////////////////////////////////
#define DEF_FIFO_LENGTH       64                 //FIFO size=64byte
#define MAXRLEN  18

/////////////////////////////////////////////////////////////////////
//MF522 Register definition
/////////////////////////////////////////////////////////////////////
// PAGE 0
#define     RFU00                 0x00    
#define     CommandReg            0x01    
#define     ComIEnReg             0x02    
#define     DivlEnReg             0x03    
#define     ComIrqReg             0x04    
#define     DivIrqReg             0x05
#define     ErrorReg              0x06    
#define     Status1Reg            0x07    
#define     Status2Reg            0x08    
#define     FIFODataReg           0x09
#define     FIFOLevelReg          0x0A
#define     WaterLevelReg         0x0B
#define     ControlReg            0x0C
#define     BitFramingReg         0x0D
#define     CollReg               0x0E
#define     RFU0F                 0x0F
// PAGE 1     
#define     RFU10                 0x10
#define     ModeReg               0x11
#define     TxModeReg             0x12
#define     RxModeReg             0x13
#define     TxControlReg          0x14
#define     TxAutoReg             0x15
#define     TxSelReg              0x16
#define     RxSelReg              0x17
#define     RxThresholdReg        0x18
#define     DemodReg              0x19
#define     RFU1A                 0x1A
#define     RFU1B                 0x1B
#define     MifareReg             0x1C
#define     RFU1D                 0x1D
#define     RFU1E                 0x1E
#define     SerialSpeedReg        0x1F
// PAGE 2    
#define     RFU20                 0x20  
#define     CRCResultRegM         0x21
#define     CRCResultRegL         0x22
#define     RFU23                 0x23
#define     ModWidthReg           0x24
#define     RFU25                 0x25
#define     RFCfgReg              0x26
#define     GsNReg                0x27
#define     CWGsCfgReg            0x28
#define     ModGsCfgReg           0x29
#define     TModeReg              0x2A
#define     TPrescalerReg         0x2B
#define     TReloadRegH           0x2C
#define     TReloadRegL           0x2D
#define     TCounterValueRegH     0x2E
#define     TCounterValueRegL     0x2F
// PAGE 3      
#define     RFU30                 0x30
#define     TestSel1Reg           0x31
#define     TestSel2Reg           0x32
#define     TestPinEnReg          0x33
#define     TestPinValueReg       0x34
#define     TestBusReg            0x35
#define     AutoTestReg           0x36
#define     VersionReg            0x37
#define     AnalogTestReg         0x38
#define     TestDAC1Reg           0x39  
#define     TestDAC2Reg           0x3A   
#define     TestADCReg            0x3B   
#define     RFU3C                 0x3C   
#define     RFU3D                 0x3D   
#define     RFU3E                 0x3E   
#define     RFU3F		  		        0x3F

/////////////////////////////////////////////////////////////////////
//Returns the error code as they communicate
/////////////////////////////////////////////////////////////////////
#define 	MI_OK                 0x26
#define 	MI_NOTAGERR           0xcc
#define 	MI_ERR                0xbb

/////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////
/* CLK = CLocK */
#define SPI_SCK_PORT  GPIO_A_BASE
#define SPI_SCK_BIT   (1 << 2)
#define SPI_SCK       SPI_SCK_PORT, SPI_SCK_BIT

/* MOSI = Master Output, Slave Input */
#define SPI_MOSI_PORT GPIO_A_BASE
#define SPI_MOSI_BIT  (1 << 4)
#define SPI_MOSI      SPI_MOSI_PORT, SPI_MOSI_BIT

/* MISO = Master Input, Slave Output */
#define SPI_MISO_PORT GPIO_A_BASE
#define SPI_MISO_BIT  (1 << 5)
#define SPI_MISO      SPI_MISO_PORT, SPI_MISO_BIT

/* CS = Chip Select */
#define SPI_CS_PORT   GPIO_B_BASE
#define SPI_CS_BIT    (1 << 5)
#define SPI_CS        SPI_CS_PORT, SPI_CS_BIT

#define RC522_RST_PORT   GPIO_B_BASE
#define RC522_RST_BIT    (1 << 4)
#define RC522_RST        RC522_RST_PORT, RC522_RST_BIT


void RC522_Init(void);
void RC522_SPI_Config(void);
void RC522_GPIO_Config(void);

unsigned char ReadRawRC(unsigned char ucAddress);
void WriteRawRC(unsigned char ucAddress, unsigned char ucValue);
void SetBitMask(unsigned char ucReg, unsigned char ucMask);
void ClearBitMask (unsigned char ucReg, unsigned char ucMask);
void PcdAntennaOn (void);
void PcdAntennaOff(void);
void PcdReset(void);
void M500PcdConfigISOType(unsigned char ucType);
char PcdComMF522(unsigned char ucCommand, unsigned char * pInData, unsigned char ucInLenByte, unsigned char * pOutData, unsigned int * pOutLenBit);
char PcdRequest (unsigned char ucReq_code, unsigned char * pTagType);
char PcdAnticoll ( unsigned char * pSnr );
void CalulateCRC ( unsigned char * pIndata, unsigned char ucLen, unsigned char * pOutData );
char PcdSelect ( unsigned char * pSnr );
char PcdAuthState ( unsigned char ucAuth_mode, unsigned char ucAddr, unsigned char * pKey, unsigned char * pSnr );
char PcdWrite ( unsigned char ucAddr, unsigned char * pData );
char PcdHalt( void );
void IC_CMT ( unsigned char * UID, unsigned char * KEY, unsigned char RW, unsigned char * Dat );

#endif 




