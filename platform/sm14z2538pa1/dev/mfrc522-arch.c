#include "reg.h"                //寄存器定义
#include "dev/mfrc522-arch.h"   //mfrc522库
#include "dev/gpio.h"           //gpio库
#include "clock.h"              //系统时钟库

/*---------------------------------------------------------------------------*/
//设置GPIO
static void
gpio_set(int port, int bit)
{
  GPIO_WRITE_PIN(port, bit, bit);
}
/*---------------------------------------------------------------------------*/
//复位GPIO
static void
gpio_reset(int port, int bit)
{
  GPIO_WRITE_PIN(port, bit, 0);
}
/*---------------------------------------------------------------------------*/
//读取GPIO
static int
gpio_get(int port, int bit)
{
  return GPIO_READ_PIN(port, bit);
}
/*---------------------------------------------------------------------------*/
//MFRC522初始化
void RC522_Init ( void )
{
  RC522_GPIO_Config ();
	
  RC522_SPI_Config ();
}
//GPIO配置
void RC522_GPIO_Config ( void )
{
  GPIO_SET_OUTPUT(RC522_RST_PORT, RC522_RST_BIT);

  gpio_set(RC522_RST_PORT, RC522_RST_BIT);
}
//SPI配置
void RC522_SPI_Config ( void )
{
  /* Set all pins to GPIO mode */

  /* CS, MOSI, CLK are output pins */
  GPIO_SET_OUTPUT(SPI_CS_PORT, SPI_CS_BIT);
  GPIO_SET_OUTPUT(SPI_MOSI_PORT, SPI_MOSI_BIT);
  GPIO_SET_OUTPUT(SPI_SCK_PORT, SPI_SCK_BIT);

  /* MISO is an input pin */
  GPIO_SET_INPUT(SPI_MISO_PORT, SPI_MISO_BIT);

  /* The CS pin is active low, so we set it high when we haven't
     selected the chip. */
  gpio_set(SPI_CS);

  /* The CLK is active low, we set it high when we aren't using it. */
  gpio_reset(SPI_SCK); 
}

unsigned char ReadRawRC(unsigned char ucAddress)
{
  unsigned char ucAddr;
  unsigned char ucReturn=0;
  unsigned char i;
	
  ucAddr = ((ucAddress<<1)&0x7E)|0x80;

  gpio_reset(SPI_SCK);
  gpio_reset(SPI_CS);

  for(i=8;i>0;i--)
  {
    if((ucAddr&0x80)==0x80)
      gpio_set(SPI_MOSI);
    else
      gpio_reset(SPI_MOSI);

    gpio_set(SPI_SCK);
    ucAddr <<= 1;
    gpio_reset(SPI_SCK);
  }

  for(i=8;i>0;i--)
  {
    gpio_set(SPI_SCK);
    ucReturn <<= 1;
    if(gpio_get(SPI_MISO) != 0) {
      ucReturn |= 0x1;
    }
    gpio_reset(SPI_SCK);
  }
	
  gpio_set(SPI_SCK);
  gpio_set(SPI_CS);
	
  return ucReturn;
}

void WriteRawRC(unsigned char ucAddress, unsigned char ucValue)
{  
  unsigned char ucAddr,i;
	
  ucAddr=(ucAddress<<1)&0x7E;
	
  gpio_reset(SPI_SCK);
  gpio_reset(SPI_CS);

  for(i=8;i>0;i--)
  {
    if((ucAddr&0x80)==0x80)
      gpio_set(SPI_MOSI);
    else
      gpio_reset(SPI_MOSI);

    gpio_set(SPI_SCK);
    ucAddr<<=1;
    gpio_reset(SPI_SCK);
  }

  for(i=8;i>0;i--)
  {
    if((ucValue&0x80)==0x80)
      gpio_set(SPI_MOSI);
    else
      gpio_reset(SPI_MOSI);

    gpio_set(SPI_SCK);
    ucValue<<=1;
    gpio_reset(SPI_SCK);
  }
	
  gpio_set(SPI_SCK);
  gpio_set(SPI_CS);
}

void SetBitMask(unsigned char ucReg, unsigned char ucMask)  
{
    unsigned char ucTemp;
	
    ucTemp = ReadRawRC(ucReg);
	
    WriteRawRC(ucReg, ucTemp | ucMask);         // set bit mask
}

void ClearBitMask (unsigned char ucReg, unsigned char ucMask)  
{
    unsigned char ucTemp;
	
    ucTemp = ReadRawRC (ucReg);
	
    WriteRawRC(ucReg, ucTemp & ( ~ ucMask));  // clear bit mask
}

void PcdAntennaOn (void)
{
    unsigned char uc;
	
    uc = ReadRawRC(TxControlReg);
	
    if (!(uc & 0x03))
	SetBitMask(TxControlReg, 0x03);
}

void PcdAntennaOff(void)
{
  ClearBitMask(TxControlReg, 0x03);
}

void PcdReset(void)
{
  gpio_set(RC522_RST);
 
  clock_delay_usec(1);	
	
  gpio_reset(RC522_RST);
	
  clock_delay_usec(1);	
	
  gpio_set(RC522_RST);
	
  clock_delay_usec(1);	
	
  WriteRawRC(CommandReg, 0x0f);
	
  while(ReadRawRC(CommandReg) & 0x10);
	
  clock_delay_usec(1);	
	
  WriteRawRC(ModeReg, 0x3D);  
	
  WriteRawRC(TReloadRegL, 30);         
  WriteRawRC(TReloadRegH, 0);
	
  WriteRawRC(TModeReg, 0x8D);				 
	
  WriteRawRC(TPrescalerReg, 0x3E);			 
	
  WriteRawRC(TxAutoReg, 0x40);				  
}

void M500PcdConfigISOType(unsigned char ucType)
{
  if (ucType == 'A')                    
  {
    ClearBitMask(Status2Reg, 0x08);
		
    WriteRawRC(ModeReg, 0x3D);
		
    WriteRawRC(RxSelReg, 0x86);
		
    WriteRawRC(RFCfgReg, 0x7F);   
		
    WriteRawRC(TReloadRegL, 30);
		
    WriteRawRC(TReloadRegH, 0);
		
    WriteRawRC(TModeReg, 0x8D);
		
    WriteRawRC(TPrescalerReg, 0x3E);
		
    clock_delay_usec(2);	
		
    PcdAntennaOn();		
   }
}

char PcdComMF522(unsigned char ucCommand, unsigned char * pInData, unsigned char ucInLenByte, unsigned char * pOutData, unsigned int * pOutLenBit)		
{
    char cStatus = MI_ERR;
    unsigned char ucIrqEn = 0x00;
    unsigned char ucWaitFor = 0x00;
    unsigned char ucLastBits;
    unsigned ucN;
    unsigned int ul;
	
    if(ucCommand == PCD_AUTHENT){
       ucIrqEn   = 0x12;	
       ucWaitFor = 0x10;		
    }else if(ucCommand == PCD_TRANSCEIVE){
       ucIrqEn   = 0x77;		
       ucWaitFor = 0x30;	
    }
   
    WriteRawRC(ComIEnReg, ucIrqEn | 0x80);	
    ClearBitMask(ComIrqReg, 0x80);		
    WriteRawRC(CommandReg, PCD_IDLE);	
    SetBitMask(FIFOLevelReg, 0x80);		
    
    for (ul = 0; ul < ucInLenByte; ul++)
      WriteRawRC(FIFODataReg, pInData [ ul ]);    	
			
    WriteRawRC(CommandReg, ucCommand);		
   
    if(ucCommand == PCD_TRANSCEIVE)
      SetBitMask(BitFramingReg,0x80);  			
    
    ul = 1000;
		
    do 	
    {
      ucN = ReadRawRC ( ComIrqReg );			
      ul --;
    }while((ul != 0 ) && ( ! ( ucN & 0x01 ) ) && ( ! ( ucN & ucWaitFor )));
		
    ClearBitMask(BitFramingReg, 0x80);		
		
    if(ul != 0)
    {
       if ( ! ( ReadRawRC ( ErrorReg ) & 0x1B ) )			
       {
		cStatus = MI_OK;
				
	if ( ucN & ucIrqEn & 0x01 )	
	   	cStatus = MI_NOTAGERR;   
					
	if ( ucCommand == PCD_TRANSCEIVE )
	{
		ucN = ReadRawRC ( FIFOLevelReg );	
					
	  	ucLastBits = ReadRawRC ( ControlReg ) & 0x07;	
					
	if ( ucLastBits )
		* pOutLenBit = ( ucN - 1 ) * 8 + ucLastBits;   	
	else
		* pOutLenBit = ucN * 8;   
					
	if ( ucN == 0 )		
        	ucN = 1;    
					
	if ( ucN > MAXRLEN )
		ucN = MAXRLEN;   
					
	for ( ul = 0; ul < ucN; ul ++ )
		pOutData [ ul ] = ReadRawRC ( FIFODataReg );   
					
	}
					
      }
			
      else
	cStatus = MI_ERR;   
			
    }
   
   SetBitMask ( ControlReg, 0x80 );           // stop timer now
   WriteRawRC ( CommandReg, PCD_IDLE ); 
		 	
   return cStatus;	
}

char PcdRequest (unsigned char ucReq_code, unsigned char * pTagType)
{
   char cStatus;  
   unsigned char ucComMF522Buf [ MAXRLEN ]; 
   unsigned int ulLen;
	
   ClearBitMask ( Status2Reg, 0x08 );	
   WriteRawRC ( BitFramingReg, 0x07 );
   SetBitMask ( TxControlReg, 0x03 );	

   ucComMF522Buf [ 0 ] = ucReq_code;	

   cStatus = PcdComMF522 ( PCD_TRANSCEIVE,ucComMF522Buf, 1, ucComMF522Buf, & ulLen );	
  
   if ( ( cStatus == MI_OK ) && ( ulLen == 0x10 ) )	
   {    
       * pTagType = ucComMF522Buf [ 0 ];
       * ( pTagType + 1 ) = ucComMF522Buf [ 1 ];
   }
	 
   else
     cStatus = MI_ERR;

   return cStatus;	 
}

char PcdAnticoll ( unsigned char * pSnr )
{
    char cStatus;
    unsigned char uc, ucSnr_check = 0;
    unsigned char ucComMF522Buf [ MAXRLEN ]; 
    unsigned int ulLen;
    
    ClearBitMask ( Status2Reg, 0x08 );		
    WriteRawRC ( BitFramingReg, 0x00);		
    ClearBitMask ( CollReg, 0x80 );	
   
    ucComMF522Buf [ 0 ] = 0x93;	
    ucComMF522Buf [ 1 ] = 0x20;
   
    cStatus = PcdComMF522 ( PCD_TRANSCEIVE, ucComMF522Buf, 2, ucComMF522Buf, & ulLen);
	
    if ( cStatus == MI_OK)		
    {
	for ( uc = 0; uc < 4; uc ++ )
       {
         * ( pSnr + uc )  = ucComMF522Buf [ uc ];	
         ucSnr_check ^= ucComMF522Buf [ uc ];
       }
			
      if ( ucSnr_check != ucComMF522Buf [ uc ] )
				cStatus = MI_ERR;    
				 
    }
    
    SetBitMask ( CollReg, 0x80 );
		
		
    return cStatus;
}

void CalulateCRC ( unsigned char * pIndata, unsigned char ucLen, unsigned char * pOutData )
{
    unsigned char uc, ucN;
	
    ClearBitMask(DivIrqReg,0x04);
	
    WriteRawRC(CommandReg,PCD_IDLE);
	
    SetBitMask(FIFOLevelReg,0x80);
	
    for ( uc = 0; uc < ucLen; uc ++)
	    WriteRawRC ( FIFODataReg, * ( pIndata + uc ) );   

    WriteRawRC ( CommandReg, PCD_CALCCRC );
	
    uc = 0xFF;
	
    do 
    {
        ucN = ReadRawRC ( DivIrqReg );
        uc --;
    } while ( ( uc != 0 ) && ! ( ucN & 0x04 ) );
		
    pOutData [ 0 ] = ReadRawRC ( CRCResultRegL );
    pOutData [ 1 ] = ReadRawRC ( CRCResultRegM );
		
		
}

char PcdSelect ( unsigned char * pSnr )
{
    char ucN;
    unsigned char uc;
    unsigned char ucComMF522Buf [ MAXRLEN ]; 
    unsigned int  ulLen;
    
    ucComMF522Buf [ 0 ] = PICC_ANTICOLL1;
    ucComMF522Buf [ 1 ] = 0x70;
    ucComMF522Buf [ 6 ] = 0;
	
    for ( uc = 0; uc < 4; uc ++ )
    {
    	ucComMF522Buf [ uc + 2 ] = * ( pSnr + uc );
    	ucComMF522Buf [ 6 ] ^= * ( pSnr + uc );
    }
		
    CalulateCRC ( ucComMF522Buf, 7, & ucComMF522Buf [ 7 ] );
  
    ClearBitMask ( Status2Reg, 0x08 );

    ucN = PcdComMF522 ( PCD_TRANSCEIVE, ucComMF522Buf, 9, ucComMF522Buf, & ulLen );
    
    if ( ( ucN == MI_OK ) && ( ulLen == 0x18 ) )
      ucN = MI_OK;  
    else
      ucN = MI_ERR;    
		
    return ucN;		
		
}

char PcdAuthState ( unsigned char ucAuth_mode, unsigned char ucAddr, unsigned char * pKey, unsigned char * pSnr )
{
    char cStatus;
    unsigned char uc, ucComMF522Buf [ MAXRLEN ];
    unsigned int ulLen;
	
    ucComMF522Buf [ 0 ] = ucAuth_mode;
    ucComMF522Buf [ 1 ] = ucAddr;
	
    for ( uc = 0; uc < 6; uc ++ )
	    ucComMF522Buf [ uc + 2 ] = * ( pKey + uc );   
	
    for ( uc = 0; uc < 6; uc ++ )
	    ucComMF522Buf [ uc + 8 ] = * ( pSnr + uc );   

    cStatus = PcdComMF522 ( PCD_AUTHENT, ucComMF522Buf, 12, ucComMF522Buf, & ulLen );
	
    if ( ( cStatus != MI_OK ) || ( ! ( ReadRawRC ( Status2Reg ) & 0x08 ) ) )
      cStatus = MI_ERR;   
    
    return cStatus;		
		
}

char PcdWrite ( unsigned char ucAddr, unsigned char * pData )
{
    char cStatus;
    unsigned char uc, ucComMF522Buf [ MAXRLEN ];
    unsigned int ulLen;
     
    ucComMF522Buf [ 0 ] = PICC_WRITE;
    ucComMF522Buf [ 1 ] = ucAddr;
	
    CalulateCRC ( ucComMF522Buf, 2, & ucComMF522Buf [ 2 ] );
 
    cStatus = PcdComMF522 ( PCD_TRANSCEIVE, ucComMF522Buf, 4, ucComMF522Buf, & ulLen );

    if ( ( cStatus != MI_OK ) || ( ulLen != 4 ) || ( ( ucComMF522Buf [ 0 ] & 0x0F ) != 0x0A ) )
      cStatus = MI_ERR;   
        
    if ( cStatus == MI_OK )
    {
			//memcpy(ucComMF522Buf, pData, 16);
      for ( uc = 0; uc < 16; uc ++ )
			  ucComMF522Buf [ uc ] = * ( pData + uc );  
			
      CalulateCRC ( ucComMF522Buf, 16, & ucComMF522Buf [ 16 ] );

      cStatus = PcdComMF522 ( PCD_TRANSCEIVE, ucComMF522Buf, 18, ucComMF522Buf, & ulLen );
			
			if ( ( cStatus != MI_OK ) || ( ulLen != 4 ) || ( ( ucComMF522Buf [ 0 ] & 0x0F ) != 0x0A ) )
        cStatus = MI_ERR;   
			
    } 
		
		
    return cStatus;		
}

char PcdRead ( unsigned char ucAddr, unsigned char * pData )
{
    char cStatus;
    unsigned char uc, ucComMF522Buf [ MAXRLEN ]; 
    unsigned int ulLen;
    

    ucComMF522Buf [ 0 ] = PICC_READ;
    ucComMF522Buf [ 1 ] = ucAddr;
	
    CalulateCRC ( ucComMF522Buf, 2, & ucComMF522Buf [ 2 ] );
   
    cStatus = PcdComMF522 ( PCD_TRANSCEIVE, ucComMF522Buf, 4, ucComMF522Buf, & ulLen );
	
    if ( ( cStatus == MI_OK ) && ( ulLen == 0x90 ) )
    {
			for ( uc = 0; uc < 16; uc ++ )
        * ( pData + uc ) = ucComMF522Buf [ uc ];   
    }
		
    else
      cStatus = MI_ERR;   
    
		
    return cStatus;		
		
}

char PcdHalt( void )
{
    unsigned char ucComMF522Buf [ MAXRLEN ]; 
    unsigned int  ulLen;
  

  ucComMF522Buf [ 0 ] = PICC_HALT;
  ucComMF522Buf [ 1 ] = 0;
	
  CalulateCRC ( ucComMF522Buf, 2, & ucComMF522Buf [ 2 ] );
 	PcdComMF522 ( PCD_TRANSCEIVE, ucComMF522Buf, 4, ucComMF522Buf, & ulLen );

  return MI_OK;
	
}


void IC_CMT ( unsigned char * UID, unsigned char * KEY, unsigned char RW, unsigned char * Dat )
{
  unsigned char ucArray_ID [4] = {0};
  
	
  PcdRequest ( 0x52, ucArray_ID );

  PcdAnticoll ( ucArray_ID );
  
  PcdSelect ( UID );
  
  PcdAuthState ( 0x60, 0x10, KEY, UID );
	

   if ( RW )
    PcdRead ( 0x10, Dat );
   
   else 
     PcdWrite ( 0x10, Dat );
   
   PcdHalt ();
}

