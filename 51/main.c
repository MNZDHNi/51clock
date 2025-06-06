//#include <REGX52.H>
#include "Delay.h"
#include "Key.h"
#include "DS1302.h"
#include "LCD1602.h"
#include "Time.h"

unsigned char KeyNum, MODE, TimeSetSelect, TimeSetFlashFlag;  //键值 模式值 时间位 时间闪烁标志位

//闹钟值定义
unsigned char clock[5]; //年月日 时分秒

//秒表
struct StopTime
{
    unsigned char StopHour;
    unsigned char StopMinute;
    unsigned char StopSecond;
};
struct StopTime NowTime;
struct StopTime StopTimeArray[10];
unsigned char StopSecondCount;

/* MODE.JSON
{
    "Name":"MODE",
    "MODE":{
        "0":"TimeShow",
        "1":"TimeSet",
        "2":"AlarmSet",
        "3":"StopWatch"
    }
}
*/

void TimeShow() //MODE 0 显示时间
{
    DS1302_ReadTime();
    LCD_ShowNum(1,1,DS1302_Time[0],2);
    LCD_ShowNum(1,4,DS1302_Time[1],2);
    LCD_ShowNum(1,7,DS1302_Time[2],2);
    LCD_ShowNum(2,1,DS1302_Time[3],2);
    LCD_ShowNum(2,4,DS1302_Time[4],2);
    LCD_ShowNum(2,7,DS1302_Time[5],2);
}

void TimeSet()  //MODE 1 设置时间
{
    if(KeyNum == 2)
    {
        TimeSetSelect++;
        TimeSetSelect %= 6; //越界清零
    }
    if(KeyNum == 3)
    {
        DS1302_Time[TimeSetSelect]++;

        //输入合法性判断
        if(DS1302_Time[0] > 99) {DS1302_Time[0] = 0;}
        if(DS1302_Time[1] > 12) {DS1302_Time[1] = 1;}

        if(DS1302_Time[1] == 1 || DS1302_Time[1] == 3 || DS1302_Time[1] == 5 || DS1302_Time[1] == 7 ||
           DS1302_Time[1] == 8 || DS1302_Time[1] == 10 || DS1302_Time[1] == 12)
        {
            if(DS1302_Time[2] > 31) {DS1302_Time[2] = 1;}
        }
        else if(DS1302_Time[1] == 4 || DS1302_Time[1] == 6 || DS1302_Time[1] == 9 || DS1302_Time[1] == 11)
        {
            if(DS1302_Time[2] > 30) {DS1302_Time[2] = 1;}
        }
        else if(DS1302_Time[1] == 2)
        {
            if(DS1302_Time[0] % 4 == 0)
            {
                if(DS1302_Time[2] > 29) {DS1302_Time[2] = 1;}
            }
            else
            {
                if(DS1302_Time[2] > 28) {DS1302_Time[2] = 1;}
            }
        }

        if(DS1302_Time[3] > 23) {DS1302_Time[3] = 0;}
        if(DS1302_Time[4] > 59) {DS1302_Time[4] = 0;}
        if(DS1302_Time[5] > 59) {DS1302_Time[5] = 0;}
    }
    if(KeyNum == 4)
    {
        DS1302_Time[TimeSetSelect]--;

        //输入合法性判断
        if(DS1302_Time[0] < 0) {DS1302_Time[0] = 99;}
        if(DS1302_Time[1] < 1) {DS1302_Time[1] = 12;}

        if(DS1302_Time[1] == 1 || DS1302_Time[1] == 3 || DS1302_Time[1] == 5 || DS1302_Time[1] == 7 ||
           DS1302_Time[1] == 8 || DS1302_Time[1] == 10 || DS1302_Time[1] == 12)
        {
            if(DS1302_Time[2] < 1) {DS1302_Time[2] = 31;}
            if(DS1302_Time[2] > 31) {DS1302_Time[2] = 1;}
        }
        else if(DS1302_Time[1] == 4 || DS1302_Time[1] == 6 || DS1302_Time[1] == 9 || DS1302_Time[1] == 11)
        {
            if(DS1302_Time[2] < 1) {DS1302_Time[2] = 30;}
            if(DS1302_Time[2] > 30) {DS1302_Time[2] = 1;}
        }
        else if(DS1302_Time[1] == 2)
        {
            if(DS1302_Time[0] % 4 == 0)
            {
                if(DS1302_Time[2] < 1) {DS1302_Time[2] = 29;}
                if(DS1302_Time[2] > 29) {DS1302_Time[2] = 1;}
            }
            else
            {
                if(DS1302_Time[2] < 1) {DS1302_Time[2] = 28;}
                if(DS1302_Time[2] > 28) {DS1302_Time[2] = 1;}
            }
        }

        if(DS1302_Time[3] < 0) {DS1302_Time[3] = 23;}
        if(DS1302_Time[4] < 0) {DS1302_Time[4] = 59;}
        if(DS1302_Time[5] < 0) {DS1302_Time[5] = 59;}
    }
    
    //实时显示，未经过DS1302
    if(TimeSetSelect == 0 && TimeSetFlashFlag == 1) {LCD_ShowString(1,1,"  ");}
    else {LCD_ShowNum(1,1,DS1302_Time[0],2);}
    if(TimeSetSelect == 1 && TimeSetFlashFlag == 1) {LCD_ShowString(1,4,"  ");}
    else {LCD_ShowNum(1,4,DS1302_Time[1],2);}
    if(TimeSetSelect == 2 && TimeSetFlashFlag == 1) {LCD_ShowString(1,7,"  ");}
    else {LCD_ShowNum(1,7,DS1302_Time[2],2);}
    if(TimeSetSelect == 3 && TimeSetFlashFlag == 1) {LCD_ShowString(2,1,"  ");}
    else {LCD_ShowNum(2,1,DS1302_Time[3],2);}
    if(TimeSetSelect == 4 && TimeSetFlashFlag == 1) {LCD_ShowString(2,4,"  ");}
    else {LCD_ShowNum(2,4,DS1302_Time[4],2);}
    if(TimeSetSelect == 5 && TimeSetFlashFlag == 1) {LCD_ShowString(2,7,"  ");}
    else {LCD_ShowNum(2,7,DS1302_Time[5],2);}
    
    //LCD_ShowNum(2,10,TimeSetSelect,2);
}

void AlarmSet() //MODE 2 设置闹钟
{
    //设置闹钟时间
    if(KeyNum == 2)
    {
        TimeSetSelect++;
        TimeSetSelect %= 6;
    }
    if(KeyNum == 3)
    {
        clock[TimeSetSelect]++;

        //输入合法性一般判断
        if(clock[0] > 99) {clock[0] = 0;}
        if(clock[1] > 12) {clock[1] = 1;}

        if(clock[1] == 1 || clock[1] == 3 || clock[1] == 5 || clock[1] == 7 ||
           clock[1] == 8 || clock[1] == 10 || clock[1] == 12)
        {
            if(clock[2] > 31) {clock[2] = 1;}
        }
        else if(clock[1] == 4 || clock[1] == 6 || clock[1] == 9 || clock[1] == 11)
        {
            if(clock[2] > 30) {clock[2] = 1;}
        }
        else if(clock[1] == 2)
        {
            if(clock[0] % 4 == 0)
            {
                if(clock[2] > 29) {clock[2] = 1;}
            }
            else
            {
                if(clock[2] > 28) {clock[2] = 1;}
            }
        }

        if(clock[3] > 23) {clock[3] = 0;}
        if(clock[4] > 59) {clock[4] = 0;}
        if(clock[5] > 59) {clock[5] = 0;}

        //闹钟时间边界判断
        if(clock[0] < DS1302_Time[0])
        {clock[0] = DS1302_Time[0];}

        if(clock[0] == DS1302_Time[0] && clock[1] < DS1302_Time[1])
        {clock[1] = DS1302_Time[1];}

        if(clock[0] == DS1302_Time[0] && clock[1] == DS1302_Time[1] &&
           clock[2] < DS1302_Time[2])
        {clock[2] = DS1302_Time[2];}

        if(clock[0] == DS1302_Time[0] && clock[1] == DS1302_Time[1] &&
           clock[2] == DS1302_Time[2] && clock[3] < DS1302_Time[3])
        {clock[3] = DS1302_Time[3];}

        if(clock[0] == DS1302_Time[0] && clock[1] == DS1302_Time[1] &&
           clock[2] == DS1302_Time[2] && clock[3] == DS1302_Time[3] &&
           clock[4] < DS1302_Time[4])
        {clock[4] = DS1302_Time[4];}

        if(clock[0] == DS1302_Time[0] && clock[1] == DS1302_Time[1] &&
           clock[2] == DS1302_Time[2] && clock[3] == DS1302_Time[3] &&
           clock[4] == DS1302_Time[4] && clock[5] < DS1302_Time[5])
        {clock[5] = DS1302_Time[5];}
    }
    if(KeyNum == 4)
    {
        clock[TimeSetSelect]--;       

        //一般边界判断
        if(clock[0] < 0) {clock[0] = 99;}
        if(clock[1] < 1) {clock[1] = 12;}

        if(clock[1] == 1 || clock[1] == 3 || clock[1] == 5 || clock[1] == 7 ||
           clock[1] == 8 || clock[1] == 10 || clock[1] == 12)
        {
            if(clock[2] < 1) {clock[2] = 31;}
            if(clock[2] > 31) {clock[2] = 1;}
        }
        else if(clock[1] == 4 || clock[1] == 6 || clock[1] == 9 || clock[1] == 11)
        {
            if(clock[2] < 1) {clock[2] = 30;}
            if(clock[2] > 30) {clock[2] = 1;}
        }
        else if(clock[1] == 2)
        {
            if(clock[0] % 4 == 0)
            {
                if(clock[2] < 1) {clock[2] = 29;}
                if(clock[2] > 29) {clock[2] = 1;}
            }
            else
            {
                if(clock[2] < 1) {clock[2] = 28;}
                if(clock[2] > 28) {clock[2] = 1;}
            }
        }

        if(clock[3] < 0) {clock[3] = 23;}
        if(clock[4] < 0) {clock[4] = 59;}
        if(clock[5] < 0) {clock[5] = 59;} 
    

        //闹钟时间边界判断
        if(clock[0] < DS1302_Time[0])
        {clock[0] = DS1302_Time[0];}

        if(clock[0] == DS1302_Time[0] && clock[1] < DS1302_Time[1])
        {clock[1] = DS1302_Time[1];}

        if(clock[0] == DS1302_Time[0] && clock[1] == DS1302_Time[1] &&
           clock[2] < DS1302_Time[2])
        {clock[2] = DS1302_Time[2];}

        if(clock[0] == DS1302_Time[0] && clock[1] == DS1302_Time[1] &&
           clock[2] == DS1302_Time[2] && clock[3] < DS1302_Time[3])
        {clock[3] = DS1302_Time[3];}

        if(clock[0] == DS1302_Time[0] && clock[1] == DS1302_Time[1] &&
           clock[2] == DS1302_Time[2] && clock[3] == DS1302_Time[3] &&
           clock[4] < DS1302_Time[4])
        {clock[4] = DS1302_Time[4];}

        if(clock[0] == DS1302_Time[0] && clock[1] == DS1302_Time[1] &&
           clock[2] == DS1302_Time[2] && clock[3] == DS1302_Time[3] &&
           clock[4] == DS1302_Time[4] && clock[5] < DS1302_Time[5])
        {clock[5] = DS1302_Time[5];}
    }

    //闹钟时间实时显示，已写入 clock[]
    if(TimeSetSelect == 0 && TimeSetFlashFlag == 1) {LCD_ShowString(1,1,"  ");}
    else {LCD_ShowNum(1,1,clock[0],2);}
    if(TimeSetSelect == 1 && TimeSetFlashFlag == 1) {LCD_ShowString(1,4,"  ");}
    else {LCD_ShowNum(1,4,clock[1],2);}
    if(TimeSetSelect == 2 && TimeSetFlashFlag == 1) {LCD_ShowString(1,7,"  ");}
    else {LCD_ShowNum(1,7,clock[2],2);}
    if(TimeSetSelect == 3 && TimeSetFlashFlag == 1) {LCD_ShowString(2,1,"  ");}
    else {LCD_ShowNum(2,1,clock[3],2);}
    if(TimeSetSelect == 4 && TimeSetFlashFlag == 1) {LCD_ShowString(2,4,"  ");}
    else {LCD_ShowNum(2,4,clock[4],2);}
    if(TimeSetSelect == 5 && TimeSetFlashFlag == 1) {LCD_ShowString(2,7,"  ");}
    else {LCD_ShowNum(2,7,clock[5],2);}
}

void StopWatch()    //MODE 3 秒表
{
    if(KeyNum == 2) //开始计时(清零并重新计时)
    {
        StopSecondCount = 0;
        NowTime.StopHour = 0;
        NowTime.StopMinute = 0;
        NowTime.StopSecond = 0;
    }
    if(KeyNum == 3) //暂停/继续
    {

    }
    if(KeyNum == 4) //回看列表 无记录不回看
    {

    }
}

void main()
{
    //初始化定时器
    Timer0Init();

    //时钟模块
    DS1302_Init();
    DS1302_SetTime();

    //LCD模块
    LCD_Init();
    LCD_ShowString(1,1,"  -  -  ");
    LCD_ShowString(2,1,"  :  :  ");

    while(1)
    {
        //change MODE
        KeyNum = Key();
        if(KeyNum == 1)
        {
            if(MODE == 0){
                MODE = 1;

                //初始化时间位
                TimeSetSelect = 0;
            }
            else if(MODE == 1){
                MODE = 2;
                
                //将新设置的时间写入DS1302
                DS1302_SetTime();

                //初始化时间位
                TimeSetSelect = 0;

                //将当前新设置的时间写入 clock[]，完成初始化
                clock[0] = DS1302_Time[0];
                clock[1] = DS1302_Time[1];
                clock[2] = DS1302_Time[2];
                clock[3] = DS1302_Time[3];
                clock[4] = DS1302_Time[4];
                clock[5] = DS1302_Time[5];
            }
            else if (MODE == 2){
                MODE = 3;

                //不需要写入 clock[] 本来就是在修改 clock[]
            }
            else if (MODE == 3){
                MODE = 0;
            }
        }
        
        //MODE 显示
        switch(MODE)
        {
            case 0: TimeShow(); break;
            case 1: TimeSet();  break;
            case 2: AlarmSet();   break;
            case 3: StopWatch();  break;
        }
    }
}

//中断部分 ******************************************************修改
void Timer0_Routine() //interrupt 1
{
    static unsigned int T0Count;

    //设置定时初值
    TL0 = 0x18;
    TH0 = 0xFC;

    T0Count++;
    if(T0Count >= 500)
    {
        T0Count = 0;
        //中断内容
        TimeSetFlashFlag = !TimeSetFlashFlag;   //控制时间闪烁
    }
}
