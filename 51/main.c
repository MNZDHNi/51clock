#include <REGX52.H>
#include "Delay.h"
#include "Key.h"
#include "DS1302.h"
#include "LCD1602.h"
#include "Timer0.h"
#include "Timer.h"

unsigned char KeyNum, MODE, TimeSetSelect, TimeSetFlashFlag;  //键值 模式 时间位 时间闪烁标志位

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
        if(DS1302_Time[0] > 99) {DS1302_SetTime[0] = 0;}
        if(DS1302_Time[1] > 12) {DS1302_SetTime[1] = 1;}

        if(DS1302_Time[1] == 1 || DS1302_Time[1] == 3 || DS1302_Time[1] == 5 || DS1302_Time[1] == 7 ||
           DS1302_Time[1] == 8 || DS1302_Time[1] == 10 || DS1302_Time[1] == 12)
        {
            if(DS1302_Time[2] > 31) {DS1302_SetTime[2] = 1;}
        }
        else if(DS1302_Time[1] == 4 || DS1302_Time[1] == 6 || DS1302_Time[1] == 9 || DS1302_Time[1] == 11)
        {
            if(DS1302_Time[2] > 30) {DS1302_SetTime[2] = 1;}
        }
        else if(DS1302_Time[1] == 2)
        {
            if(DS1302_Time[0] % 4 == 0)
            {
                if(DS1302_Time[2] > 29) {DS1302_SetTime[2] = 1;}
            }
            else
            {
                if(DS1302_Time[2] > 28) {DS1302_SetTime[2] = 1;}
            }
        }

        if(DS1302_Time[3] > 23) {DS1302_SetTime[3] = 0;}
        if(DS1302_Time[4] > 59) {DS1302_SetTime[4] = 0;}
        if(DS1302_Time[5] > 59) {DS1302_SetTime[5] = 0;}
    }
    if(KeyNum == 4)
    {
        DS1302_Time[TimeSetSelect]--;

        //输入合法性判断
        if(DS1302_Time[0] < 0) {DS1302_SetTime[0] = 99;}
        if(DS1302_Time[1] < 1) {DS1302_SetTime[1] = 12;}

        if(DS1302_Time[1] == 1 || DS1302_Time[1] == 3 || DS1302_Time[1] == 5 || DS1302_Time[1] == 7 ||
           DS1302_Time[1] == 8 || DS1302_Time[1] == 10 || DS1302_Time[1] == 12)
        {
            if(DS1302_Time[2] < 1) {DS1302_SetTime[2] = 31;}
            if(DS1302_Time[2] > 31) {DS1302_SetTime[2] = 1;}
        }
        else if(DS1302_Time[1] == 4 || DS1302_Time[1] == 6 || DS1302_Time[1] == 9 || DS1302_Time[1] == 11)
        {
            if(DS1302_Time[2] < 1) {DS1302_SetTime[2] = 30;}
            if(DS1302_Time[2] > 30) {DS1302_SetTime[2] = 1;}
        }
        else if(DS1302_Time[1] == 2)
        {
            if(DS1302_Time[0] % 4 == 0)
            {
                if(DS1302_Time[2] < 1) {DS1302_SetTime[2] = 29;}
                if(DS1302_Time[2] > 29) {DS1302_SetTime[2] = 1;}
            }
            else
            {
                if(DS1302_Time[2] < 1) {DS1302_SetTime[2] = 28;}
                if(DS1302_Time[2] > 28) {DS1302_SetTime[2] = 1;}
            }
        }

        if(DS1302_Time[3] < 0) {DS1302_SetTime[3] = 23;}
        if(DS1302_Time[4] < 0) {DS1302_SetTime[4] = 59;}
        if(DS1302_Time[5] < 0) {DS1302_SetTime[5] = 59;}
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
    
}

void StopWatch()    //MODE 3 秒表
{
    
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
            }
            else if(MODE == 1){
                MODE = 2;
                DS1302_SetTime();
            }
            else if (MODE == 2){
                MODE = 3;
            }
            else if (MODE == 3){
                MODE = 0;
            }
        }
        
        switch(MODE)
        {
            case 0: TimeShow(); break;
            case 1: TimeSet();  break;
            case 2: break;
            case 3: break;
        }
    }
}

//中断部分
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
        TimeSetFlashFlag = !TimeSetFlashFlag;
    }
}