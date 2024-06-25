#include <reg51.h>
#define uchar unsigned char								//�� unsigned char �궨��Ϊ uchar
#define uint unsigned int								//�� unsigned int �궨��Ϊ uint
sbit rst = P3^4;										//���帴λ/Ƭѡ�߽�P3.4
sbit scl = P3^7;										//���崮��ʱ�����Ž�P3.7
sbit sda = P3^5;										//���崮��������������˽�P3.5
/*��DS1302��ʼ��*/
void ds1302_init(void)
{
	rst = 0;
	scl = 0;
	rst = 1;
}
/*��DS1302д1�ֽں���*/
void write_ds1302_onebyte(unsigned char dat)	   
{
	unsigned char i;
	for(i = 0;i < 8;i++)
	{
		sda = (bit)(dat & 0x01);
		scl = 0;
		scl = 1;
		dat = dat >> 1;
	}
}
/*��DS1302��һ�ֽں���*/
uchar read_ds1302_onebyte(void)		
{
	unsigned char i,dat;
	for(i = 0;i < 8;i++)
	{
		scl = 1;
		scl = 0;
		dat = dat >> 1;
		if(sda)dat = dat | 0x80;
	}
	return(dat);	
}
/*��DS1302��ĳһ��ַдһ�ֽں���*/
void write_ds1302_add_dat(unsigned char add,unsigned char dat)
{
	ds1302_init();
	write_ds1302_onebyte(add);
	write_ds1302_onebyte(dat);
	scl = 1;
	rst = 0;
}
/*��DS1302��ĳһ��ַ��һ�ֽں���*/
uchar read_ds1302_add(unsigned char add)									
{
   unsigned char dat_temp;
	ds1302_init();
	write_ds1302_onebyte(add);
	dat_temp = read_ds1302_onebyte();
	scl = 1;
	rst = 0;
	return(dat_temp);
}
/*8421BCD�뵽ʮ����ת��*/
uchar BCD_DEC_conv(unsigned char x)
{
	unsigned char dec;
	dec =  0x0f & x;
	x = x >> 4;
	dec	= dec + x * 10;
	return(dec);
}
uchar DEC_BCD_conv(unsigned char x)
{
	unsigned char bcd;
	bcd =  x % 10;
	x = x / 10;
	x = x << 4;
	bcd	= bcd | x ;
	return(bcd);
}
