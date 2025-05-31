#include "reg52.h" //��Ƭ���ڲ����ܼĴ������壨�ײ�Ӳ����

typedef unsigned int u16; //�޷������Σ�0-65535
typedef unsigned char u8; //�޷����ַ��ͣ�0-255

//��������������ƽ�
sbit KEY1=P3^1;
sbit KEY2=P3^0;
sbit KEY3=P3^2;
sbit KEY4=P3^3;
sbit FMQ=P2^5;


u8 code smg_code[]={0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,0x6f,0x77,0x7c,0x39,0x5e,0x79,0x71,0x40};
u8 code smg_wei[]={0xFF,0xFB,0xF7,0xF3,0xEF,0xEB,0xE7,0xE3};
u8 smg_buf[]={12,16,1,2,3,4,5,6};
//����Ƶ���Լ�����
unsigned int code FreqTable[] = {63628,63731,63835,63928,64021,64103,64185,64260,64331,64400,64463,64528,64580,64633,64684,64732,64777,64820,64860,64898,64934,64968,65000,65030,65058,65085,65110,65134,65157,65178,65198,65217,65235,65252,65268,65283,0};
unsigned char FreqSelect,MusicSelect;
unsigned char code Music[] = {12,12,19,19,21,21,19,17,17,16,16,14,14,12};
//������ʾ����ʼֵ
u8 hour=23;
u8 minite=59;
u8 second=58;
bit bzw=0;
u8 i;

//��ʱ����
void delay (u16 i)
{
	while(i--);		//while���ִ��һ�δ��10us
}

//�������ʾ
void smgdisp()
{
	u8 i;
	smg_buf[2]=hour/10;
	smg_buf[3]=hour%10;
	smg_buf[4]=minite/10;
	smg_buf[5]=minite%10;
	smg_buf[6]=second/10;
	smg_buf[7]=second%10;
	for(i=0;i<8;i++)
	{
		P2=smg_wei[i];
		if(i==3||i==5)
			P0=smg_code[smg_buf[i]]|0x80;
		else
			P0=smg_code[smg_buf[i]];		
		delay(100);
		P0=0x00;
	}
}

//�����ж�
u8 key_scan(u8 mode)
{
	static u8 key=1;

	if(mode)key=1;//����ɨ�谴��
	if(key==1&&(KEY1==0||KEY2==0||KEY3==0||KEY4==0))//���ⰴ������
	{
		delay(1000);//����
		key=0;
		if(KEY1==0)
			return 1;
		else if(KEY2==0)
			return 2;
		else if(KEY3==0)
			return 3;
		else if(KEY4==0)
			return 4;	
	}
	else if(KEY1==1&&KEY2==1&&KEY3==1&&KEY4==1)	//�ް�������
	{
		key=1;			
	}
	return 0;		
}

//��ʱ��timer0
void Timer0Init(void)		//1����@11.0592MHz
{
	TMOD &= 0xF0;		//���ö�ʱ��ģʽΪģʽ1
	TMOD |= 0x01;		//���ö�ʱ��ģʽΪģʽ1
	TL0 = 0x66;		//���ö�ʱ��ֵ
	TH0 = 0xFC;		//���ö�ʱ��ֵ
	TF0 = 0;		//���TF0��־
	ET0	=	1;		//����T0�ж�
	EA	=	1;		//�ж��ܿ���
	//TR0 = 1;		//��ʱ��0��ʼ��ʱ
}

//��ʱ��timer1
void Timer1Init(void)		//1??@11.0592MHz
{
	TMOD &= 0x0F;		//???????
	TMOD |= 0x10;		//???????
	TL1 = 0x66;		//??????
	TH1 = 0xFC;		//??????
	EA=1;
	ET1=1;
	TF1 = 0;		//??TF1??
}


//������
void main()
{
	u8 key_value;
	Timer0Init();
	Timer1Init();
	while(1)
	{
		smgdisp();
		key_value=key_scan(1);
		if(minite==0&&second==0)
			FMQ=0;
		if(key_value==3)
		{
			second++;
			if(second>59)
			{second=0;
			minite++;
			if(minite>59)
				{minite=0;
				hour++;
				if(hour>23)
				hour=0;}
				}
		}
			if(key_value==2)
			{
				minite++;
				if(minite>59)
				{minite=0;
				hour++;
				if(hour>23)
				hour=0;}
			}
			if(key_value==1)
			{
				hour++;
				if(hour>23)
				hour=0;
			}
			if(key_value==4)
			{
				TR0=~TR0;
			}
			
			if(hour==0&&minite==0&&second==0)
			bzw=1;
			if(bzw==1)
			{
			FreqSelect = Music[MusicSelect];
			MusicSelect++;
			for(i=38;i>0;i--)
			smgdisp();
			TR1 = 0; 
			delay(400);
			TR1 = 1; 
			}
			
	}
}

void T0_INTP()	interrupt 1 //��ʱ��T0�жϳ����Զ�ִ�У��������
{
	static u16 count=0;
	TL0 = 0x66;		//�������ö�ʱ��ֵ
	TH0 = 0xFC;		//�������ö�ʱ��ֵ
	count++;
	if(count==1000)		//�ж��Ƿ�1s
	{
		count=0;
		second++;
		if(second>59)
			{second=0;minite++;}
		if(minite>59)
			{minite=0;hour++;}
		if(hour>23)
			{hour=0;}
	}
}

//��ʱ��timer1���жϺ������Զ�ִ��
void Timer1_Routine() interrupt 3
{
	if(FreqTable[FreqSelect]!=0)
	{
		TL1 = FreqTable[FreqSelect] % 256;
		TH1 = FreqTable[FreqSelect] / 256;
		FMQ= !FMQ;
	}
	else
	bzw=0;
}
