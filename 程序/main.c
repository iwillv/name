//ʵʱʱ��DS1302Ӧ�ã����B107��ʵ�鿪�������
//ʵʱ��ȡʱ��
//��ƣ���Ƭ�����¹�����--��������ƣ�2015��12��15��
#include<reg51.h>
#include<ds1302.c>
unsigned char month,year,date,sec,min,hour,week,d,i,f,d;
unsigned char cp1,cp2,cp3;													//�������
sbit P1_0 = P1^0;															//λѡ����
code unsigned char seven_seg[10] = {0xc0,0xf9,0xa4,0xb0,0x99,
										0x92,0x82,0xf8,0x80,0x90};			//����0~9����ʾ����
bit conv;
void get_ds1302_time(void)													//��ȡ1302��ʱ�����ݣ�ʱ���֡��룩������time1������
{
	d = read_ds1302_add(0x81);												//����
	sec = BCD_DEC_conv(d);													//�õ���	
	d = read_ds1302_add(0x83);												//����
	min = BCD_DEC_conv(d);													//�õ���
	d = read_ds1302_add(0x85);												//��Сʱ
	hour = BCD_DEC_conv(d);													//�õ�Сʱ
	d = read_ds1302_add(0x87);												//����
	date = BCD_DEC_conv(d);													//�õ���
   	d = read_ds1302_add(0x89);												//����
	month = BCD_DEC_conv(d);												//�õ���
	d = read_ds1302_add(0x8b);												//������
	week = BCD_DEC_conv(d);													//�õ�����
	d = read_ds1302_add(0x8d);												//����
	year = BCD_DEC_conv(d);													//�õ���		
}
void time0_isr(void) interrupt 1											//�����ж϶����������ʾ�����ݽ���ˢ��
{
	TH0 = (65536 - 2000) / 256;	
	TL0 = (65536 - 2000) % 256;
	cp1++;
	if(cp1 >= 250)															//0.5��
	{
		cp1 = 0;
		f = ~f;																//����С������˸����
		cp2++;
		if(cp2 >= 5)
		{  
			cp2 = 0;
			conv = !conv;													//����������ʾ����	
		}	
	}
	if(conv == 1)
	{
		P0 = 0xff;
		switch(cp3)
		{
				case 0:	P1_0 = 0;P0 = 0x01;P1_0 = 1;P1_0 = 0;				//P1.0�˿ڸߵ�ƽ����洢��P1.0Ϊ�͵�ƽ����洢
			        	P0 = seven_seg[sec%10];break;						//��ʾ���λ
				case 1:	
						P1_0 = 0;P0 = 0x02;P1_0 = 1;P1_0 = 0;
						P0 = seven_seg[sec/10];break;						//��ʾ��ʮλ
				case 2:	
						P1_0 = 0;P0 = 0x04;P1_0 = 1;P1_0 = 0;		
						P0 = seven_seg[min%10] & (0x7f | f);break;			//��ʾ�ָ�λ,��ʹС����������˸
				case 3:	
						P1_0 = 0;P0 = 0x08;P1_0 = 1;P1_0 = 0;
						P0 = seven_seg[min/10];break;						//��ʾ��ʮλ
				case 4:	
						P1_0 = 0;P0 = 0x10;P1_0 = 1;P1_0 = 0;			
						P0 = seven_seg[hour%10] & (0x7f | f);break;			//��ʾʱ��λ����ʹС����������˸
				case 5:	
						P1_0 = 0;P0 = 0x20;P1_0 = 1;P1_0 = 0;
						P0 = seven_seg[hour/10];break;						//��ʾʱʮλ
				case 6:	
						P1_0 = 0;P0 = 0x40;P1_0 = 1;P1_0 = 0;			
						P0 = 0xbf;break;									//��ʾ��-��
				case 7:	
						P1_0 = 0;P0 = 0x80;P1_0 = 1;P1_0 = 0;
						P0 = 0xbf;break;									//��ʾ��-��
		}
	}
	else
	{	
		P0 = 0xff;
		switch(cp3)
		{
			case 0:	
					P1_0 = 0;P0 = 0x01;P1_0 = 1;P1_0 = 0;					//P1.0�˿ڸߵ�ƽ����洢��P1.0Ϊ�͵�ƽ����洢
					P0 = seven_seg[date%10];break;							//��ʾ���λ
		 	case 1:	
					P1_0 = 0;P0 = 0x02;P1_0 = 1;P1_0 = 0;
					P0 = seven_seg[date/10];break;							//��ʾ��ʮλ
			case 2:	
					P1_0 = 0;P0 = 0x04;P1_0 = 1;P1_0 = 0;		
					P0 = seven_seg[month%10] & (0x7f | f);break;			//��ʾ�¸�λ,��ʹС����������˸
			case 3:	
					P1_0 = 0;P0 = 0x08;P1_0 = 1;P1_0 = 0;
					P0 = seven_seg[month/10];break;							//��ʾ��ʮλ
			case 4:	
					P1_0 = 0;P0 = 0x10;P1_0 = 1;P1_0 = 0;			
					P0 = seven_seg[year%10] & (0x7f | f);break;				//��ʾ���λ����ʹС����������˸
			case 5:	
					P1_0 = 0;P0 = 0x20;P1_0 = 1;P1_0 = 0;
					P0 = seven_seg[year/10];break;							//��ʾ��ʮλ
			case 6:	
					P1_0 = 0;P0 = 0x40;P1_0 = 1;P1_0 = 0;			
					P0 = seven_seg[0];break;								//��ʾ��0��
			case 7:	
					P1_0 = 0;P0 = 0x80;P1_0 = 1;P1_0 = 0;
					P0 = seven_seg[2];break;								//��ʾ��2��
		}
	}
	cp3++;
	if(cp3 >= 8)cp3 = 0;			
}
void timer0_init(void)														//Timer0��ʼ��
{
	TMOD = 0x01;
	TH0 = (65536 - 2000) / 256;	
	TL0 = (65536 - 2000) % 256;
	TR0 = 1;
	ET0 = 1;
	EA  = 1;
}
void main(void)
{
	uchar i = 13;															//����������Ҫ����ʱ�䣬�����趨Ϊ46��
	i = DEC_BCD_conv(i);
	
	write_ds1302_add_dat(0x8e,0x00);										//д���������Զ�DS1302����
	write_ds1302_add_dat(0x80,0x30);										//д�룬30��
	write_ds1302_add_dat(0x82,i);											//д�֣�46��
	write_ds1302_add_dat(0x84,0x11);										//дʱ��12ʱ
	write_ds1302_add_dat(0x86,0x28);										//д�գ�28��
	write_ds1302_add_dat(0x88,0x05);										//д�£�5�� 
	write_ds1302_add_dat(0x8a,0x03);										//д���ڣ�������
	write_ds1302_add_dat(0x8c,0x24);										//д�꣬��20��12��
	write_ds1302_add_dat(0x8e,0x80); //д����
	timer0_init();										
	while(1)
	{
		get_ds1302_time();
	}
}
