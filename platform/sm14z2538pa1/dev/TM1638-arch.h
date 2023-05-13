#ifndef __TM1638_ARCH_H
#define __TM1638_ARCH_H	

#define     ADDRESS_INCREASE_MODE       0X40
#define     READ_KEY                    0X42
#define     ADDRESS_FIX_MODE            0X44

#define     TM1638_DIG0                 0XC0
#define     TM1638_DIG1                 0XC2
#define     TM1638_DIG2                 0XC4
#define     TM1638_DIG3                 0XC6
#define     TM1638_DIG4                 0XC8
#define     TM1638_DIG5                 0XCA
#define     TM1638_DIG6                 0XCC
#define     TM1638_DIG7                 0XCE

#define     DISPLAY_OFF                 0X80
#define     DISPLAY_ON                  0X88

#define     SET_PLUS_WIDTH1             0X88
#define     SET_PLUS_WIDTH2             0X89
#define     SET_PLUS_WIDTH4             0X8A
#define     SET_PLUS_WIDTH10            0X8B
#define     SET_PLUS_WIDTH11            0X8C
#define     SET_PLUS_WIDTH12            0X8D
#define     SET_PLUS_WIDTH13            0X8E
#define     SET_PLUS_WIDTH14            0X8F

#define STB_PORT GPIO_B_BASE
#define STB_BIT  (1 << 2)

#define DIO_PORT GPIO_B_BASE
#define DIO_BIT  (1 << 3)

#define CLK_PORT  GPIO_B_BASE
#define CLK_BIT  (1 << 4)

void delay_us(unsigned int time);
void TM1638_init(void);
void disp(void);
void dispclear(void);
void dispclear_led(void);
void display(unsigned char dig,unsigned char dat);
void display_led(unsigned char dig,unsigned char dat);
void TM1638_Write(unsigned char data);
unsigned char TM1638_Read(void);
void Write_COM(unsigned char cmd);
void Write_DATA(unsigned char add, unsigned char data);
unsigned char Read_key_A(void);
unsigned char Read_key_B(void);
#endif  
	 



