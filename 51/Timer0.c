#include <REGX52.H>

void Timer0Init()
{
    TMOD &= 0xF0;
    TMOD |= 0x01;
    TL0 = 0x18;
    TH0 = 0xFC;
    TF0 = 0;
    TR0 = 1;
    ET0 = 1;
    EA = 1;
    PT0 = 0;
}

/*定时器中断函数

void Timer0_Routine() interrupt 1
{
    static unsigned int T0Count;

    //设置定时初值
    TL0 = 0x18;
    TH0 = 0xFC;

    T0Count++;
    if(T0Count >= 1000) //定时时长(ms)
    {
        T0Count = 0;
        //中断内容
    }
}

*/