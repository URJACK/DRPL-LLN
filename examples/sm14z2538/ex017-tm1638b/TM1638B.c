#include "contiki.h"
#include "dev/TM1638-arch.h"
#include <stdio.h> /* For printf() */

const unsigned char CathodeCode[ 0x11 ] = {  0X3F, 0X06, 0X5B, 0X4F, 0X66, 0X6D, 0X7D, 0X07, 0X7F, 0X6F, 0X77, 0X7C, 0X39, 0X5E, 0X79, 0X71, 0XFF  };
/*---------------------------------------------------------------------------*/
PROCESS(TM1638B_process, "TM1638B");
AUTOSTART_PROCESSES(&TM1638B_process);
/*---------------------------------------------------------------------------*/
PROCESS_THREAD(TM1638B_process, ev, data)
{
  PROCESS_BEGIN();
  
  unsigned int i,j,k;
  unsigned int Key=0;

  TM1638_init();

  Write_COM( DISPLAY_ON );        /* 开显示 */
  Write_COM( ADDRESS_FIX_MODE );  /* 地址固定模式 */
  Write_COM( SET_PLUS_WIDTH4 );   //设置亮度
  
  dispclear_led();
  for (j=0;j<17;j++)
  {
     dispclear_B();
     for(i=0;i<8;i++)
       Write_DATA( TM1638_DIG0+i*2, CathodeCode[j]);            

     for(k=0;k<1000;k++)
	delay_us(1000);
  }
  for(j=0;j<8;j++)
  {
     display_led(TM1638_DIG0+2*j+1,1);
     for(k=0;k<1000;k++)
        delay_us(1000);
  }
  dispclear_led();
  while(1) {
     Key=Read_key_B();

     if(Key > 0)
     {
	dispclear_B();
	dispclear_led();
        Write_DATA( TM1638_DIG0+12, CathodeCode[Key/10]);
        Write_DATA( TM1638_DIG0+14, CathodeCode[Key%10]);
        Key=0;
     }     
     
  }

  PROCESS_END();
  /*测试*/
}
/*---------------------------------------------------------------------------*/
