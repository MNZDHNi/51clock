#include <REGX52.H>

void Timer0_Init()
{
	TMOD &= 0xF0;
	TMOD |= 0x01;		
	TL0 = 0x66;		
	TH0 = 0xFC;
	TF0 = 0;		
	ET0	=	1;	
	EA	=	1;
	//TR0 = 1;		
}

void Timer1_Init()
{
	TMOD &= 0x0F;	
	TMOD |= 0x10;	
	TL1 = 0x66;		
	TH1 = 0xFC;		
	EA=1;
	ET1=1;
	TF1 = 0;
}

/*
void Timer0_INTP()	interrupt 1
{
	static unsigned int count=0;
	TL0 = 0x66;		
	TH0 = 0xFC;
	count++;
	if(count==1000)
	{
        //循环部分
	}
}

void Timer1_Routine() interrupt 3
{
    static unsigned int count=0;
	TL0 = 0x66;		
	TH0 = 0xFC;
	count++;
	if(count==1000)
	{
        //循环部分
	}
}
*/