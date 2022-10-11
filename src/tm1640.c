//
// Created by aditya on 28-08-2022.
//
#include <STRING.H>
#include "REG_MG82FG5Bxx.h"
#include "macro.h"
#include "tm1640.h"
#include "utility.h"

unsigned char xdata alphabets[] = {
    'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'O', 'P', 'S', 'U', 'Z', 't'
    };
unsigned char xdata alphaHex[] = {
    0xf5, 0xfd, 0x4d, 0xed, 0x5d, 0x55, 0x7d, 0xb5, 0x05, 0xe8, 0x15, 0x0d, 0xed, 0xd5, 0x7c, 0xad, 0xd9, 0x1d
    };

unsigned char getHexFromAlphabet(unsigned char value)
{
    int i = 0;
    for (i = 0; i < strlen(alphabets); i++)
    {
        if (value == alphabets[i])
        {
            return alphaHex[i];
        }
}
    
}
void TM1640_Init(unsigned char InValue)
{
    //GPIO_WriteHigh(GPIO4,GPIO_PIN_1);			//P41 write high DIN
    GPIO_write_DIN_high();
    Delay_Some_Time(1);
    //GPIO_WriteHigh(GPIO4,GPIO_PIN_0);			//P40 write high SCLK
    GPIO_write_SCLK_high();

    if(InValue == DSPTM1640_DIS) //Show off
    {
        TM1640Start();//Data write mode setting
        TM1640WriteByte(DSP1640_DIS);//Display off control
        TM1640Stop();
    }
    else
    {
        TM1640Start();//Data write mode setting
        TM1640WriteByte(TM1640_FIEXD);//Data command setting address 				automatically 	increases by 1
        TM1640Stop();
        TM1640Start(); //Display brightness setting
        TM1640WriteByte(TM1640_DISPLAY); //Display command control pulse width 			14/16
        TM1640Stop();
    }
}

void TM1640Start(void)
{
    //GPIO_WriteHigh(GPIO4,GPIO_PIN_1);			//P41 write high DIN
    GPIO_write_DIN_high();
    //GPIO_WriteHigh(GPIO4,GPIO_PIN_0);			//P40 write high SCLK
    GPIO_write_SCLK_high();
    Delay_Some_Time(1);
    //GPIO_WriteLow(GPIO4,GPIO_PIN_1);	   //p41 low level DIN
    GPIO_write_DIN_low();
    Delay_Some_Time(1);
    //GPIO_WriteLow(GPIO4,GPIO_PIN_1);     //p40 low level SCLK
    GPIO_write_SCLK_low();
    Delay_Some_Time(1);
}

void TM1640WriteByte(unsigned char  date)
{
    unsigned char i;
    unsigned char  Tmp;
    Tmp=date;
    //GPIO_WriteLow(GPIO4,GPIO_PIN_1);	   //p41 low level DIN
    GPIO_write_DIN_low();
    //GPIO_WriteLow(GPIO4,GPIO_PIN_0);     //p40 low level SCLK
    GPIO_write_SCLK_low();
    for(i=0;i<8;i++)
    {
        //GPIO_WriteLow(GPIO4,GPIO_PIN_0);     //p40 low level SCLK
        GPIO_write_SCLK_low();
        Delay_Some_Time(1);
        if(Tmp&0x01)
        {
            //GPIO_WriteHigh(GPIO4,GPIO_PIN_1);			//P41 write high DIN
            GPIO_write_DIN_high();
            Delay_Some_Time(1);
        }
        else
        {
            //GPIO_WriteLow(GPIO4,GPIO_PIN_1);	   //p41 low level DIN
            GPIO_write_DIN_low();
            Delay_Some_Time(1);
        }
        //GPIO_WriteHigh(GPIO4,GPIO_PIN_0);			//P40 write high SCLK
        GPIO_write_SCLK_high();
        Tmp = Tmp>>1;
    }
    //GPIO_WriteLow(GPIO4,GPIO_PIN_0);     //p40 low level SCLK
    GPIO_write_SCLK_low();
    //GPIO_WriteLow(GPIO4,GPIO_PIN_1);	   //p41 low level DIN
    GPIO_write_DIN_low();
}

void TM1640Stop(void)
{
    //GPIO_WriteLow(GPIO4,GPIO_PIN_1);	   //p41 low level DIN
    GPIO_write_DIN_low();
    //GPIO_WriteHigh(GPIO4,GPIO_PIN_0);			//P40 write high SCLK
    GPIO_write_SCLK_high();
    Delay_Some_Time(1);
    //GPIO_WriteHigh(GPIO4,GPIO_PIN_1);
    GPIO_write_DIN_high();
    Delay_Some_Time(1);
}

void TM1640_display(unsigned char *no_digits)
{
    unsigned char  i;
    TM1640Start();
    TM1640WriteByte(0x44); //Set the starting address
    TM1640Stop();
    TM1640Start();
    TM1640WriteByte(0xcd);
    for(i=0;i<1;i++)
    {
        TM1640WriteByte(no_digits[i]);
    }
    TM1640Stop();
    TM1640Start();
    TM1640WriteByte(0x8b); //Set the starting address
    TM1640Stop();
}

void TM1640_U_display(unsigned char *utemp)
{

    TM1640Start();
    TM1640WriteByte(0x44); //Set the starting address
    TM1640Stop();
    TM1640Start();
    TM1640WriteByte(0xc5);
    TM1640WriteByte(utemp[0]);
    TM1640Stop();
    TM1640Start();
    TM1640WriteByte(0xc8);
    TM1640WriteByte(utemp[1]);
    TM1640Stop();
    TM1640Start();
    TM1640WriteByte(0xc7);
    TM1640WriteByte(utemp[2]);
    TM1640Stop();
    TM1640Start();
    TM1640WriteByte(0xca);
    TM1640WriteByte(utemp[3]);
    TM1640Stop();
    TM1640Start();
    TM1640WriteByte(0xc9);
    TM1640WriteByte(utemp[4]);
    TM1640Stop();
    TM1640Start();
    TM1640WriteByte(0x8b); //Set the starting address
    TM1640Stop();
}

void TM1640_M_display(unsigned char *mtemp)
{

    TM1640Start();
    TM1640WriteByte(0x44); //Set the starting address
    TM1640Stop();
    TM1640Start();
    TM1640WriteByte(0xcb);
    TM1640WriteByte(mtemp[0]);
    TM1640Stop();
    TM1640Start();
    TM1640WriteByte(0xc1);
    TM1640WriteByte(mtemp[1]);
    TM1640Stop();
    TM1640Start();
    TM1640WriteByte(0xc4);
    TM1640WriteByte(mtemp[2]);
    TM1640Stop();
    TM1640Start();
    TM1640WriteByte(0xc3);
    TM1640WriteByte(mtemp[3]);
    TM1640Stop();
    TM1640Start();
    TM1640WriteByte(0xc6);
    TM1640WriteByte(mtemp[4]);
    TM1640Stop();
    TM1640Start();
    TM1640WriteByte(0x8b); //Set the starting address
    TM1640Stop();
}

void TM1640_UM_display(unsigned char *co_name)
{
    TM1640Start();
    TM1640WriteByte(0x44); //Set the starting address
    TM1640Stop();

    TM1640Start();
    TM1640WriteByte(0xcb);
    TM1640WriteByte(co_name[0]);
    TM1640Stop();
    Delay_Some_Time(100);

    TM1640Start();
    TM1640WriteByte(0xcb);
    TM1640WriteByte(co_name[1]);
    TM1640Stop();
    TM1640Start();
    TM1640WriteByte(0xc1);
    TM1640WriteByte(co_name[0]);
    TM1640Stop();
    Delay_Some_Time(100);

    TM1640Start();
    TM1640WriteByte(0xcb);
    TM1640WriteByte(co_name[2]);
    TM1640Stop();
    TM1640Start();
    TM1640WriteByte(0xc1);
    TM1640WriteByte(co_name[1]);
    TM1640Stop();
    TM1640Start();
    TM1640WriteByte(0xc4);
    TM1640WriteByte(co_name[0]);
    TM1640Stop();
    Delay_Some_Time(100);

    TM1640Start();
    TM1640WriteByte(0xcb);
    TM1640WriteByte(co_name[3]);
    TM1640Stop();
    TM1640Start();
    TM1640WriteByte(0xc1);
    TM1640WriteByte(co_name[2]);
    TM1640Stop();
    TM1640Start();
    TM1640WriteByte(0xc4);
    TM1640WriteByte(co_name[1]);
    TM1640Stop();
    TM1640Start();
    TM1640WriteByte(0xc3);
    TM1640WriteByte(co_name[0]);
    TM1640Stop();
    Delay_Some_Time(100);

    TM1640Start();
    TM1640WriteByte(0xcb);
    TM1640WriteByte(co_name[4]);
    TM1640Stop();
    TM1640Start();
    TM1640WriteByte(0xc1);
    TM1640WriteByte(co_name[3]);
    TM1640Stop();
    TM1640Start();
    TM1640WriteByte(0xc4);
    TM1640WriteByte(co_name[2]);
    TM1640Stop();
    TM1640Start();
    TM1640WriteByte(0xc3);
    TM1640WriteByte(co_name[1]);
    TM1640Stop();
    TM1640Start();
    TM1640WriteByte(0xc6);
    TM1640WriteByte(co_name[0]);
    TM1640Stop();
    Delay_Some_Time(100);

    TM1640Start();
    TM1640WriteByte(0xcb);
    TM1640WriteByte(co_name[5]);
    TM1640Stop();
    TM1640Start();
    TM1640WriteByte(0xc1);
    TM1640WriteByte(co_name[4]);
    TM1640Stop();
    TM1640Start();
    TM1640WriteByte(0xc4);
    TM1640WriteByte(co_name[3]);
    TM1640Stop();
    TM1640Start();
    TM1640WriteByte(0xc3);
    TM1640WriteByte(co_name[2]);
    TM1640Stop();
    TM1640Start();
    TM1640WriteByte(0xc6);
    TM1640WriteByte(co_name[1]);
    TM1640Stop();
    TM1640Start();
    TM1640WriteByte(0xc5);
    TM1640WriteByte(co_name[0]);
    TM1640Stop();
    Delay_Some_Time(100);

    TM1640Start();
    TM1640WriteByte(0xcb);
    TM1640WriteByte(co_name[6]);
    TM1640Stop();
    TM1640Start();
    TM1640WriteByte(0xc1);
    TM1640WriteByte(co_name[5]);
    TM1640Stop();
    TM1640Start();
    TM1640WriteByte(0xc4);
    TM1640WriteByte(co_name[4]);
    TM1640Stop();
    TM1640Start();
    TM1640WriteByte(0xc3);
    TM1640WriteByte(co_name[3]);
    TM1640Stop();
    TM1640Start();
    TM1640WriteByte(0xc6);
    TM1640WriteByte(co_name[2]);
    TM1640Stop();
    TM1640Start();
    TM1640WriteByte(0xc5);
    TM1640WriteByte(co_name[1]);
    TM1640Stop();
    TM1640Start();
    TM1640WriteByte(0xc8);
    TM1640WriteByte(co_name[0]);
    TM1640Stop();
    Delay_Some_Time(100);

    TM1640Start();
    TM1640WriteByte(0xcb);
    TM1640WriteByte(co_name[7]);
    TM1640Stop();
    TM1640Start();
    TM1640WriteByte(0xc1);
    TM1640WriteByte(co_name[6]);
    TM1640Stop();
    TM1640Start();
    TM1640WriteByte(0xc4);
    TM1640WriteByte(co_name[5]);
    TM1640Stop();
    TM1640Start();
    TM1640WriteByte(0xc3);
    TM1640WriteByte(co_name[4]);
    TM1640Stop();
    TM1640Start();
    TM1640WriteByte(0xc6);
    TM1640WriteByte(co_name[3]);
    TM1640Stop();
    TM1640Start();
    TM1640WriteByte(0xc5);
    TM1640WriteByte(co_name[2]);
    TM1640Stop();
    TM1640Start();
    TM1640WriteByte(0xc8);
    TM1640WriteByte(co_name[1]);
    TM1640Stop();
    TM1640Start();
    TM1640WriteByte(0xc7);
    TM1640WriteByte(co_name[0]);
    TM1640Stop();
    Delay_Some_Time(100);

    TM1640Start();
    TM1640WriteByte(0xcb);
    TM1640WriteByte(co_name[8]);
    TM1640Stop();
    TM1640Start();
    TM1640WriteByte(0xc1);
    TM1640WriteByte(co_name[7]);
    TM1640Stop();
    TM1640Start();
    TM1640WriteByte(0xc4);
    TM1640WriteByte(co_name[6]);
    TM1640Stop();
    TM1640Start();
    TM1640WriteByte(0xc3);
    TM1640WriteByte(co_name[5]);
    TM1640Stop();
    TM1640Start();
    TM1640WriteByte(0xc6);
    TM1640WriteByte(co_name[4]);
    TM1640Stop();
    TM1640Start();
    TM1640WriteByte(0xc5);
    TM1640WriteByte(co_name[3]);
    TM1640Stop();
    TM1640Start();
    TM1640WriteByte(0xc8);
    TM1640WriteByte(co_name[2]);
    TM1640Stop();
    TM1640Start();
    TM1640WriteByte(0xc7);
    TM1640WriteByte(co_name[1]);
    TM1640Stop();
    TM1640Start();
    TM1640WriteByte(0xca);
    TM1640WriteByte(co_name[0]);
    TM1640Stop();
    Delay_Some_Time(100);

    TM1640Start();
    TM1640WriteByte(0xcb);
    TM1640WriteByte(co_name[9]);
    TM1640Stop();
    TM1640Start();
    TM1640WriteByte(0xc1);
    TM1640WriteByte(co_name[8]);
    TM1640Stop();
    TM1640Start();
    TM1640WriteByte(0xc4);
    TM1640WriteByte(co_name[7]);
    TM1640Stop();
    TM1640Start();
    TM1640WriteByte(0xc3);
    TM1640WriteByte(co_name[6]);
    TM1640Stop();
    TM1640Start();
    TM1640WriteByte(0xc6);
    TM1640WriteByte(co_name[5]);
    TM1640Stop();
    TM1640Start();
    TM1640WriteByte(0xc5);
    TM1640WriteByte(co_name[4]);
    TM1640Stop();
    TM1640Start();
    TM1640WriteByte(0xc8);
    TM1640WriteByte(co_name[3]);
    TM1640Stop();
    TM1640Start();
    TM1640WriteByte(0xc7);
    TM1640WriteByte(co_name[2]);
    TM1640Stop();
    TM1640Start();
    TM1640WriteByte(0xca);
    TM1640WriteByte(co_name[1]);
    TM1640Stop();
    TM1640Start();
    TM1640WriteByte(0xc9);
    TM1640WriteByte(co_name[0]);
    TM1640Stop();
    //TM1640Start();
    //TM1640WriteByte(0x8b); //Set the starting address
    //TM1640Stop();
    Delay_Some_Time(100);

    TM1640Start();
    TM1640WriteByte(0xcb);
    TM1640WriteByte(co_name[10]);
    TM1640Stop();
    TM1640Start();
    TM1640WriteByte(0xc1);
    TM1640WriteByte(co_name[9]);
    TM1640Stop();
    TM1640Start();
    TM1640WriteByte(0xc4);
    TM1640WriteByte(co_name[8]);
    TM1640Stop();
    TM1640Start();
    TM1640WriteByte(0xc3);
    TM1640WriteByte(co_name[7]);
    TM1640Stop();
    TM1640Start();
    TM1640WriteByte(0xc6);
    TM1640WriteByte(co_name[6]);
    TM1640Stop();
    TM1640Start();
    TM1640WriteByte(0xc5);
    TM1640WriteByte(co_name[5]);
    TM1640Stop();
    TM1640Start();
    TM1640WriteByte(0xc8);
    TM1640WriteByte(co_name[4]);
    TM1640Stop();
    TM1640Start();
    TM1640WriteByte(0xc7);
    TM1640WriteByte(co_name[3]);
    TM1640Stop();
    TM1640Start();
    TM1640WriteByte(0xca);
    TM1640WriteByte(co_name[2]);
    TM1640Stop();
    TM1640Start();
    TM1640WriteByte(0xc9);
    TM1640WriteByte(co_name[1]);
    TM1640Stop();
    //TM1640Start();
    //TM1640WriteByte(0x8b); //Set the starting address
    //TM1640Stop();
    Delay_Some_Time(100);

    TM1640Start();
    TM1640WriteByte(0xcb);
    TM1640WriteByte(co_name[11]);
    TM1640Stop();
    TM1640Start();
    TM1640WriteByte(0xc1);
    TM1640WriteByte(co_name[10]);
    TM1640Stop();
    TM1640Start();
    TM1640WriteByte(0xc4);
    TM1640WriteByte(co_name[9]);
    TM1640Stop();
    TM1640Start();
    TM1640WriteByte(0xc3);
    TM1640WriteByte(co_name[8]);
    TM1640Stop();
    TM1640Start();
    TM1640WriteByte(0xc6);
    TM1640WriteByte(co_name[7]);
    TM1640Stop();
    TM1640Start();
    TM1640WriteByte(0xc5);
    TM1640WriteByte(co_name[6]);
    TM1640Stop();
    TM1640Start();
    TM1640WriteByte(0xc8);
    TM1640WriteByte(co_name[5]);
    TM1640Stop();
    TM1640Start();
    TM1640WriteByte(0xc7);
    TM1640WriteByte(co_name[4]);
    TM1640Stop();
    TM1640Start();
    TM1640WriteByte(0xca);
    TM1640WriteByte(co_name[3]);
    TM1640Stop();
    TM1640Start();
    TM1640WriteByte(0xc9);
    TM1640WriteByte(co_name[2]);
    TM1640Stop();
    //TM1640Start();
    //TM1640WriteByte(0x8b); //Set the starting address
    //TM1640Stop();
    Delay_Some_Time(100);

    TM1640Start();
    TM1640WriteByte(0xcb);
    TM1640WriteByte(co_name[12]);
    TM1640Stop();
    TM1640Start();
    TM1640WriteByte(0xc1);
    TM1640WriteByte(co_name[11]);
    TM1640Stop();
    TM1640Start();
    TM1640WriteByte(0xc4);
    TM1640WriteByte(co_name[10]);
    TM1640Stop();
    TM1640Start();
    TM1640WriteByte(0xc3);
    TM1640WriteByte(co_name[9]);
    TM1640Stop();
    TM1640Start();
    TM1640WriteByte(0xc6);
    TM1640WriteByte(co_name[8]);
    TM1640Stop();
    TM1640Start();
    TM1640WriteByte(0xc5);
    TM1640WriteByte(co_name[7]);
    TM1640Stop();
    TM1640Start();
    TM1640WriteByte(0xc8);
    TM1640WriteByte(co_name[6]);
    TM1640Stop();
    TM1640Start();
    TM1640WriteByte(0xc7);
    TM1640WriteByte(co_name[5]);
    TM1640Stop();
    TM1640Start();
    TM1640WriteByte(0xca);
    TM1640WriteByte(co_name[4]);
    TM1640Stop();
    TM1640Start();
    TM1640WriteByte(0xc9);
    TM1640WriteByte(co_name[3]);
    TM1640Stop();
    //TM1640Start();
    //TM1640WriteByte(0x8b); //Set the starting address
    //TM1640Stop();
    Delay_Some_Time(100);

    TM1640Start();
    TM1640WriteByte(0xcb);
    TM1640WriteByte(co_name[13]);
    TM1640Stop();
    TM1640Start();
    TM1640WriteByte(0xc1);
    TM1640WriteByte(co_name[12]);
    TM1640Stop();
    TM1640Start();
    TM1640WriteByte(0xc4);
    TM1640WriteByte(co_name[11]);
    TM1640Stop();
    TM1640Start();
    TM1640WriteByte(0xc3);
    TM1640WriteByte(co_name[10]);
    TM1640Stop();
    TM1640Start();
    TM1640WriteByte(0xc6);
    TM1640WriteByte(co_name[9]);
    TM1640Stop();
    TM1640Start();
    TM1640WriteByte(0xc5);
    TM1640WriteByte(co_name[8]);
    TM1640Stop();
    TM1640Start();
    TM1640WriteByte(0xc8);
    TM1640WriteByte(co_name[7]);
    TM1640Stop();
    TM1640Start();
    TM1640WriteByte(0xc7);
    TM1640WriteByte(co_name[6]);
    TM1640Stop();
    TM1640Start();
    TM1640WriteByte(0xca);
    TM1640WriteByte(co_name[5]);
    TM1640Stop();
    TM1640Start();
    TM1640WriteByte(0xc9);
    TM1640WriteByte(co_name[4]);
    TM1640Stop();
    //TM1640Start();
    //TM1640WriteByte(0x8b); //Set the starting address
    //TM1640Stop();
    Delay_Some_Time(100);

    TM1640Start();
    TM1640WriteByte(0xcb);
    TM1640WriteByte(co_name[14]);
    TM1640Stop();
    TM1640Start();
    TM1640WriteByte(0xc1);
    TM1640WriteByte(co_name[13]);
    TM1640Stop();
    TM1640Start();
    TM1640WriteByte(0xc4);
    TM1640WriteByte(co_name[12]);
    TM1640Stop();
    TM1640Start();
    TM1640WriteByte(0xc3);
    TM1640WriteByte(co_name[11]);
    TM1640Stop();
    TM1640Start();
    TM1640WriteByte(0xc6);
    TM1640WriteByte(co_name[10]);
    TM1640Stop();
    TM1640Start();
    TM1640WriteByte(0xc5);
    TM1640WriteByte(co_name[9]);
    TM1640Stop();
    TM1640Start();
    TM1640WriteByte(0xc8);
    TM1640WriteByte(co_name[8]);
    TM1640Stop();
    TM1640Start();
    TM1640WriteByte(0xc7);
    TM1640WriteByte(co_name[7]);
    TM1640Stop();
    TM1640Start();
    TM1640WriteByte(0xca);
    TM1640WriteByte(co_name[6]);
    TM1640Stop();
    TM1640Start();
    TM1640WriteByte(0xc9);
    TM1640WriteByte(co_name[5]);
    TM1640Stop();
    //TM1640Start();
    //TM1640WriteByte(0x8b); //Set the starting address
    //TM1640Stop();
    Delay_Some_Time(100);

    TM1640Start();
    TM1640WriteByte(0xcb);
    TM1640WriteByte(co_name[15]);
    TM1640Stop();
    TM1640Start();
    TM1640WriteByte(0xc1);
    TM1640WriteByte(co_name[14]);
    TM1640Stop();
    TM1640Start();
    TM1640WriteByte(0xc4);
    TM1640WriteByte(co_name[13]);
    TM1640Stop();
    TM1640Start();
    TM1640WriteByte(0xc3);
    TM1640WriteByte(co_name[12]);
    TM1640Stop();
    TM1640Start();
    TM1640WriteByte(0xc6);
    TM1640WriteByte(co_name[11]);
    TM1640Stop();
    TM1640Start();
    TM1640WriteByte(0xc5);
    TM1640WriteByte(co_name[10]);
    TM1640Stop();
    TM1640Start();
    TM1640WriteByte(0xc8);
    TM1640WriteByte(co_name[9]);
    TM1640Stop();
    TM1640Start();
    TM1640WriteByte(0xc7);
    TM1640WriteByte(co_name[8]);
    TM1640Stop();
    TM1640Start();
    TM1640WriteByte(0xca);
    TM1640WriteByte(co_name[7]);
    TM1640Stop();
    TM1640Start();
    TM1640WriteByte(0xc9);
    TM1640WriteByte(co_name[6]);
    TM1640Stop();
    //TM1640Start();
    //TM1640WriteByte(0x8b); //Set the starting address
    //TM1640Stop();
    Delay_Some_Time(100);

    TM1640Start();
    TM1640WriteByte(0xcb);
    TM1640WriteByte(co_name[16]);
    TM1640Stop();
    TM1640Start();
    TM1640WriteByte(0xc1);
    TM1640WriteByte(co_name[15]);
    TM1640Stop();
    TM1640Start();
    TM1640WriteByte(0xc4);
    TM1640WriteByte(co_name[14]);
    TM1640Stop();
    TM1640Start();
    TM1640WriteByte(0xc3);
    TM1640WriteByte(co_name[13]);
    TM1640Stop();
    TM1640Start();
    TM1640WriteByte(0xc6);
    TM1640WriteByte(co_name[12]);
    TM1640Stop();
    TM1640Start();
    TM1640WriteByte(0xc5);
    TM1640WriteByte(co_name[11]);
    TM1640Stop();
    TM1640Start();
    TM1640WriteByte(0xc8);
    TM1640WriteByte(co_name[10]);
    TM1640Stop();
    TM1640Start();
    TM1640WriteByte(0xc7);
    TM1640WriteByte(co_name[9]);
    TM1640Stop();
    TM1640Start();
    TM1640WriteByte(0xca);
    TM1640WriteByte(co_name[8]);
    TM1640Stop();
    TM1640Start();
    TM1640WriteByte(0xc9);
    TM1640WriteByte(co_name[7]);
    TM1640Stop();
    //TM1640Start();
    //TM1640WriteByte(0x8b); //Set the starting address
    //TM1640Stop();
    Delay_Some_Time(100);

    TM1640Start();
    TM1640WriteByte(0xcb);
    TM1640WriteByte(co_name[17]);
    TM1640Stop();
    TM1640Start();
    TM1640WriteByte(0xc1);
    TM1640WriteByte(co_name[16]);
    TM1640Stop();
    TM1640Start();
    TM1640WriteByte(0xc4);
    TM1640WriteByte(co_name[15]);
    TM1640Stop();
    TM1640Start();
    TM1640WriteByte(0xc3);
    TM1640WriteByte(co_name[14]);
    TM1640Stop();
    TM1640Start();
    TM1640WriteByte(0xc6);
    TM1640WriteByte(co_name[13]);
    TM1640Stop();
    TM1640Start();
    TM1640WriteByte(0xc5);
    TM1640WriteByte(co_name[12]);
    TM1640Stop();
    TM1640Start();
    TM1640WriteByte(0xc8);
    TM1640WriteByte(co_name[11]);
    TM1640Stop();
    TM1640Start();
    TM1640WriteByte(0xc7);
    TM1640WriteByte(co_name[10]);
    TM1640Stop();
    TM1640Start();
    TM1640WriteByte(0xca);
    TM1640WriteByte(co_name[9]);
    TM1640Stop();
    TM1640Start();
    TM1640WriteByte(0xc9);
    TM1640WriteByte(co_name[8]);
    TM1640Stop();
    //TM1640Start();
    //TM1640WriteByte(0x8b); //Set the starting address
    //TM1640Stop();
    Delay_Some_Time(100);

    TM1640Start();
    TM1640WriteByte(0xcb);
    TM1640WriteByte(co_name[18]);
    TM1640Stop();
    TM1640Start();
    TM1640WriteByte(0xc1);
    TM1640WriteByte(co_name[17]);
    TM1640Stop();
    TM1640Start();
    TM1640WriteByte(0xc4);
    TM1640WriteByte(co_name[16]);
    TM1640Stop();
    TM1640Start();
    TM1640WriteByte(0xc3);
    TM1640WriteByte(co_name[15]);
    TM1640Stop();
    TM1640Start();
    TM1640WriteByte(0xc6);
    TM1640WriteByte(co_name[14]);
    TM1640Stop();
    TM1640Start();
    TM1640WriteByte(0xc5);
    TM1640WriteByte(co_name[13]);
    TM1640Stop();
    TM1640Start();
    TM1640WriteByte(0xc8);
    TM1640WriteByte(co_name[12]);
    TM1640Stop();
    TM1640Start();
    TM1640WriteByte(0xc7);
    TM1640WriteByte(co_name[11]);
    TM1640Stop();
    TM1640Start();
    TM1640WriteByte(0xca);
    TM1640WriteByte(co_name[10]);
    TM1640Stop();
    TM1640Start();
    TM1640WriteByte(0xc9);
    TM1640WriteByte(co_name[9]);
    TM1640Stop();
    //TM1640Start();
    //TM1640WriteByte(0x8b); //Set the starting address
    //TM1640Stop();
    Delay_Some_Time(100);

    TM1640Start();
    TM1640WriteByte(0xcb);
    TM1640WriteByte(co_name[19]);
    TM1640Stop();
    TM1640Start();
    TM1640WriteByte(0xc1);
    TM1640WriteByte(co_name[18]);
    TM1640Stop();
    TM1640Start();
    TM1640WriteByte(0xc4);
    TM1640WriteByte(co_name[17]);
    TM1640Stop();
    TM1640Start();
    TM1640WriteByte(0xc3);
    TM1640WriteByte(co_name[16]);
    TM1640Stop();
    TM1640Start();
    TM1640WriteByte(0xc6);
    TM1640WriteByte(co_name[15]);
    TM1640Stop();
    TM1640Start();
    TM1640WriteByte(0xc5);
    TM1640WriteByte(co_name[14]);
    TM1640Stop();
    TM1640Start();
    TM1640WriteByte(0xc8);
    TM1640WriteByte(co_name[13]);
    TM1640Stop();
    TM1640Start();
    TM1640WriteByte(0xc7);
    TM1640WriteByte(co_name[12]);
    TM1640Stop();
    TM1640Start();
    TM1640WriteByte(0xca);
    TM1640WriteByte(co_name[11]);
    TM1640Stop();
    TM1640Start();
    TM1640WriteByte(0xc9);
    TM1640WriteByte(co_name[10]);
    TM1640Stop();
    TM1640Start();
    TM1640WriteByte(0x8b); //Set the starting address
    TM1640Stop();

}

void TM1640_digichk(unsigned char *dichk_temp)
{
    TM1640Start();
    TM1640WriteByte(0x44); //Set the starting address
    TM1640Stop();

    TM1640Start();
    TM1640WriteByte(0xc2);
    TM1640WriteByte(dichk_temp[0]);
    TM1640Stop();


    TM1640Start();
    TM1640WriteByte(0xc0);
    TM1640WriteByte(dichk_temp[0]);
    TM1640Stop();

    TM1640Start();
    TM1640WriteByte(0xcc);
    TM1640WriteByte(dichk_temp[0]);
    TM1640Stop();

    TM1640Start();
    TM1640WriteByte(0xce);
    TM1640WriteByte(dichk_temp[0]);
    TM1640Stop();

    TM1640Start();
    TM1640WriteByte(0xcf);
    TM1640WriteByte(dichk_temp[0]);
    TM1640Stop();

    TM1640Start();
    TM1640WriteByte(0xcd);
    TM1640WriteByte(dichk_temp[0]);
    TM1640Stop();

    TM1640Start();
    TM1640WriteByte(0xcb);
    TM1640WriteByte(dichk_temp[0]);
    TM1640Stop();

    TM1640Start();
    TM1640WriteByte(0xc1);
    TM1640WriteByte(dichk_temp[0]);
    TM1640Stop();

    TM1640Start();
    TM1640WriteByte(0xc4);
    TM1640WriteByte(dichk_temp[0]);
    TM1640Stop();

    TM1640Start();
    TM1640WriteByte(0xc3);
    TM1640WriteByte(dichk_temp[0]);
    TM1640Stop();

    TM1640Start();
    TM1640WriteByte(0xc6);
    TM1640WriteByte(dichk_temp[0]);
    TM1640Stop();

    TM1640Start();
    TM1640WriteByte(0xc5);
    TM1640WriteByte(dichk_temp[0]);
    TM1640Stop();

    TM1640Start();
    TM1640WriteByte(0xc8);
    TM1640WriteByte(dichk_temp[0]);
    TM1640Stop();

    TM1640Start();
    TM1640WriteByte(0xc7);
    TM1640WriteByte(dichk_temp[0]);
    TM1640Stop();

    TM1640Start();
    TM1640WriteByte(0xca);
    TM1640WriteByte(dichk_temp[0]);
    TM1640Stop();

    TM1640Start();
    TM1640WriteByte(0xc9);
    TM1640WriteByte(dichk_temp[0]);
    TM1640Stop();
    //TM1640Start();
    //TM1640WriteByte(0x8b); //Set the starting address
    //TM1640Stop();


    Delay_Some_Time(500);

    TM1640Start();
    TM1640WriteByte(0xc2);
    TM1640WriteByte(dichk_temp[1]);
    TM1640Stop();

    TM1640Start();
    TM1640WriteByte(0xc0);
    TM1640WriteByte(dichk_temp[1]);
    TM1640Stop();

    TM1640Start();
    TM1640WriteByte(0xcc);
    TM1640WriteByte(dichk_temp[1]);
    TM1640Stop();

    TM1640Start();
    TM1640WriteByte(0xce);
    TM1640WriteByte(dichk_temp[1]);
    TM1640Stop();

    TM1640Start();
    TM1640WriteByte(0xcf);
    TM1640WriteByte(dichk_temp[1]);
    TM1640Stop();

    TM1640Start();
    TM1640WriteByte(0xcd);
    TM1640WriteByte(dichk_temp[1]);
    TM1640Stop();

    TM1640Start();
    TM1640WriteByte(0xcb);
    TM1640WriteByte(dichk_temp[1]);
    TM1640Stop();

    TM1640Start();
    TM1640WriteByte(0xc1);
    TM1640WriteByte(dichk_temp[1]);
    TM1640Stop();

    TM1640Start();
    TM1640WriteByte(0xc4);
    TM1640WriteByte(dichk_temp[1]);
    TM1640Stop();

    TM1640Start();
    TM1640WriteByte(0xc3);
    TM1640WriteByte(dichk_temp[1]);
    TM1640Stop();

    TM1640Start();
    TM1640WriteByte(0xc6);
    TM1640WriteByte(dichk_temp[1]);
    TM1640Stop();

    TM1640Start();
    TM1640WriteByte(0xc5);
    TM1640WriteByte(dichk_temp[1]);
    TM1640Stop();

    TM1640Start();
    TM1640WriteByte(0xc8);
    TM1640WriteByte(dichk_temp[1]);
    TM1640Stop();

    TM1640Start();
    TM1640WriteByte(0xc7);
    TM1640WriteByte(dichk_temp[1]);
    TM1640Stop();

    TM1640Start();
    TM1640WriteByte(0xca);
    TM1640WriteByte(dichk_temp[1]);
    TM1640Stop();

    TM1640Start();
    TM1640WriteByte(0xc9);
    TM1640WriteByte(dichk_temp[1]);
    TM1640Stop();

    Delay_Some_Time(500);

    TM1640Start();
    TM1640WriteByte(0xc2);
    TM1640WriteByte(dichk_temp[2]);
    TM1640Stop();

    TM1640Start();
    TM1640WriteByte(0xc0);
    TM1640WriteByte(dichk_temp[2]);
    TM1640Stop();

    TM1640Start();
    TM1640WriteByte(0xcc);
    TM1640WriteByte(dichk_temp[2]);
    TM1640Stop();

    TM1640Start();
    TM1640WriteByte(0xce);
    TM1640WriteByte(dichk_temp[2]);
    TM1640Stop();

    TM1640Start();
    TM1640WriteByte(0xcf);
    TM1640WriteByte(dichk_temp[2]);
    TM1640Stop();

    TM1640Start();
    TM1640WriteByte(0xcd);
    TM1640WriteByte(dichk_temp[2]);
    TM1640Stop();

    TM1640Start();
    TM1640WriteByte(0xcb);
    TM1640WriteByte(dichk_temp[2]);
    TM1640Stop();

    TM1640Start();
    TM1640WriteByte(0xc1);
    TM1640WriteByte(dichk_temp[2]);
    TM1640Stop();

    TM1640Start();
    TM1640WriteByte(0xc4);
    TM1640WriteByte(dichk_temp[2]);
    TM1640Stop();

    TM1640Start();
    TM1640WriteByte(0xc3);
    TM1640WriteByte(dichk_temp[2]);
    TM1640Stop();

    TM1640Start();
    TM1640WriteByte(0xc6);
    TM1640WriteByte(dichk_temp[2]);
    TM1640Stop();

    TM1640Start();
    TM1640WriteByte(0xc5);
    TM1640WriteByte(dichk_temp[2]);
    TM1640Stop();

    TM1640Start();
    TM1640WriteByte(0xc8);
    TM1640WriteByte(dichk_temp[2]);
    TM1640Stop();

    TM1640Start();
    TM1640WriteByte(0xc7);
    TM1640WriteByte(dichk_temp[2]);
    TM1640Stop();

    TM1640Start();
    TM1640WriteByte(0xca);
    TM1640WriteByte(dichk_temp[2]);
    TM1640Stop();

    TM1640Start();
    TM1640WriteByte(0xc9);
    TM1640WriteByte(dichk_temp[2]);
    TM1640Stop();

    Delay_Some_Time(500);

    TM1640Start();
    TM1640WriteByte(0xc2);
    TM1640WriteByte(dichk_temp[3]);
    TM1640Stop();

    TM1640Start();
    TM1640WriteByte(0xc0);
    TM1640WriteByte(dichk_temp[3]);
    TM1640Stop();

    TM1640Start();
    TM1640WriteByte(0xcc);
    TM1640WriteByte(dichk_temp[3]);
    TM1640Stop();

    TM1640Start();
    TM1640WriteByte(0xce);
    TM1640WriteByte(dichk_temp[3]);
    TM1640Stop();

    TM1640Start();
    TM1640WriteByte(0xcf);
    TM1640WriteByte(dichk_temp[3]);
    TM1640Stop();

    TM1640Start();
    TM1640WriteByte(0xcd);
    TM1640WriteByte(dichk_temp[3]);
    TM1640Stop();

    TM1640Start();
    TM1640WriteByte(0xcb);
    TM1640WriteByte(dichk_temp[3]);
    TM1640Stop();

    TM1640Start();
    TM1640WriteByte(0xc1);
    TM1640WriteByte(dichk_temp[3]);
    TM1640Stop();

    TM1640Start();
    TM1640WriteByte(0xc4);
    TM1640WriteByte(dichk_temp[3]);
    TM1640Stop();

    TM1640Start();
    TM1640WriteByte(0xc3);
    TM1640WriteByte(dichk_temp[3]);
    TM1640Stop();

    TM1640Start();
    TM1640WriteByte(0xc6);
    TM1640WriteByte(dichk_temp[3]);
    TM1640Stop();

    TM1640Start();
    TM1640WriteByte(0xc5);
    TM1640WriteByte(dichk_temp[3]);
    TM1640Stop();

    TM1640Start();
    TM1640WriteByte(0xc8);
    TM1640WriteByte(dichk_temp[3]);
    TM1640Stop();

    TM1640Start();
    TM1640WriteByte(0xc7);
    TM1640WriteByte(dichk_temp[3]);
    TM1640Stop();

    TM1640Start();
    TM1640WriteByte(0xca);
    TM1640WriteByte(dichk_temp[3]);
    TM1640Stop();

    TM1640Start();
    TM1640WriteByte(0xc9);
    TM1640WriteByte(dichk_temp[3]);
    TM1640Stop();

    Delay_Some_Time(500);

    TM1640Start();
    TM1640WriteByte(0xc2);
    TM1640WriteByte(dichk_temp[4]);
    TM1640Stop();

    TM1640Start();
    TM1640WriteByte(0xc0);
    TM1640WriteByte(dichk_temp[4]);
    TM1640Stop();

    TM1640Start();
    TM1640WriteByte(0xcc);
    TM1640WriteByte(dichk_temp[4]);
    TM1640Stop();

    TM1640Start();
    TM1640WriteByte(0xce);
    TM1640WriteByte(dichk_temp[4]);
    TM1640Stop();

    TM1640Start();
    TM1640WriteByte(0xcf);
    TM1640WriteByte(dichk_temp[4]);
    TM1640Stop();

    TM1640Start();
    TM1640WriteByte(0xcd);
    TM1640WriteByte(dichk_temp[4]);
    TM1640Stop();

    TM1640Start();
    TM1640WriteByte(0xcb);
    TM1640WriteByte(dichk_temp[4]);
    TM1640Stop();

    TM1640Start();
    TM1640WriteByte(0xc1);
    TM1640WriteByte(dichk_temp[4]);
    TM1640Stop();

    TM1640Start();
    TM1640WriteByte(0xc4);
    TM1640WriteByte(dichk_temp[4]);
    TM1640Stop();

    TM1640Start();
    TM1640WriteByte(0xc3);
    TM1640WriteByte(dichk_temp[4]);
    TM1640Stop();

    TM1640Start();
    TM1640WriteByte(0xc6);
    TM1640WriteByte(dichk_temp[4]);
    TM1640Stop();

    TM1640Start();
    TM1640WriteByte(0xc5);
    TM1640WriteByte(dichk_temp[4]);
    TM1640Stop();

    TM1640Start();
    TM1640WriteByte(0xc8);
    TM1640WriteByte(dichk_temp[4]);
    TM1640Stop();

    TM1640Start();
    TM1640WriteByte(0xc7);
    TM1640WriteByte(dichk_temp[4]);
    TM1640Stop();

    TM1640Start();
    TM1640WriteByte(0xca);
    TM1640WriteByte(dichk_temp[4]);
    TM1640Stop();

    TM1640Start();
    TM1640WriteByte(0xc9);
    TM1640WriteByte(dichk_temp[4]);
    TM1640Stop();

    Delay_Some_Time(500);

    TM1640Start();
    TM1640WriteByte(0xc2);
    TM1640WriteByte(dichk_temp[5]);
    TM1640Stop();

    TM1640Start();
    TM1640WriteByte(0xc0);
    TM1640WriteByte(dichk_temp[5]);
    TM1640Stop();

    TM1640Start();
    TM1640WriteByte(0xcc);
    TM1640WriteByte(dichk_temp[5]);
    TM1640Stop();

    TM1640Start();
    TM1640WriteByte(0xce);
    TM1640WriteByte(dichk_temp[5]);
    TM1640Stop();

    TM1640Start();
    TM1640WriteByte(0xcf);
    TM1640WriteByte(dichk_temp[5]);
    TM1640Stop();

    TM1640Start();
    TM1640WriteByte(0xcd);
    TM1640WriteByte(dichk_temp[5]);
    TM1640Stop();

    TM1640Start();
    TM1640WriteByte(0xcb);
    TM1640WriteByte(dichk_temp[5]);
    TM1640Stop();

    TM1640Start();
    TM1640WriteByte(0xc1);
    TM1640WriteByte(dichk_temp[5]);
    TM1640Stop();

    TM1640Start();
    TM1640WriteByte(0xc4);
    TM1640WriteByte(dichk_temp[5]);
    TM1640Stop();

    TM1640Start();
    TM1640WriteByte(0xc3);
    TM1640WriteByte(dichk_temp[5]);
    TM1640Stop();

    TM1640Start();
    TM1640WriteByte(0xc6);
    TM1640WriteByte(dichk_temp[5]);
    TM1640Stop();

    TM1640Start();
    TM1640WriteByte(0xc5);
    TM1640WriteByte(dichk_temp[5]);
    TM1640Stop();

    TM1640Start();
    TM1640WriteByte(0xc8);
    TM1640WriteByte(dichk_temp[5]);
    TM1640Stop();

    TM1640Start();
    TM1640WriteByte(0xc7);
    TM1640WriteByte(dichk_temp[5]);
    TM1640Stop();

    TM1640Start();
    TM1640WriteByte(0xca);
    TM1640WriteByte(dichk_temp[5]);
    TM1640Stop();

    TM1640Start();
    TM1640WriteByte(0xc9);
    TM1640WriteByte(dichk_temp[5]);
    TM1640Stop();

    Delay_Some_Time(500);

    TM1640Start();
    TM1640WriteByte(0xc2);
    TM1640WriteByte(dichk_temp[6]);
    TM1640Stop();

    TM1640Start();
    TM1640WriteByte(0xc0);
    TM1640WriteByte(dichk_temp[6]);
    TM1640Stop();

    TM1640Start();
    TM1640WriteByte(0xcc);
    TM1640WriteByte(dichk_temp[6]);
    TM1640Stop();

    TM1640Start();
    TM1640WriteByte(0xce);
    TM1640WriteByte(dichk_temp[6]);
    TM1640Stop();

    TM1640Start();
    TM1640WriteByte(0xcf);
    TM1640WriteByte(dichk_temp[6]);
    TM1640Stop();

    TM1640Start();
    TM1640WriteByte(0xcd);
    TM1640WriteByte(dichk_temp[6]);
    TM1640Stop();

    TM1640Start();
    TM1640WriteByte(0xcb);
    TM1640WriteByte(dichk_temp[6]);
    TM1640Stop();

    TM1640Start();
    TM1640WriteByte(0xc1);
    TM1640WriteByte(dichk_temp[6]);
    TM1640Stop();

    TM1640Start();
    TM1640WriteByte(0xc4);
    TM1640WriteByte(dichk_temp[6]);
    TM1640Stop();

    TM1640Start();
    TM1640WriteByte(0xc3);
    TM1640WriteByte(dichk_temp[6]);
    TM1640Stop();

    TM1640Start();
    TM1640WriteByte(0xc6);
    TM1640WriteByte(dichk_temp[6]);
    TM1640Stop();

    TM1640Start();
    TM1640WriteByte(0xc5);
    TM1640WriteByte(dichk_temp[6]);
    TM1640Stop();

    TM1640Start();
    TM1640WriteByte(0xc8);
    TM1640WriteByte(dichk_temp[6]);
    TM1640Stop();

    TM1640Start();
    TM1640WriteByte(0xc7);
    TM1640WriteByte(dichk_temp[6]);
    TM1640Stop();

    TM1640Start();
    TM1640WriteByte(0xca);
    TM1640WriteByte(dichk_temp[6]);
    TM1640Stop();

    TM1640Start();
    TM1640WriteByte(0xc9);
    TM1640WriteByte(dichk_temp[6]);
    TM1640Stop();

    TM1640Start();
    TM1640WriteByte(0x8b); //Set the starting address
    TM1640Stop();
}

void TM1640_L_display(unsigned char *ltemp)
{

    TM1640Start();
    TM1640WriteByte(0x44); //Set the starting address
    TM1640Stop();
    TM1640Start();
    TM1640WriteByte(0xc2);
    TM1640WriteByte(ltemp[0]);
    TM1640Stop();
    TM1640Start();
    TM1640WriteByte(0xc0);
    TM1640WriteByte(ltemp[1]);
    TM1640Stop();
    TM1640Start();
    TM1640WriteByte(0xcc);
    TM1640WriteByte(ltemp[2]);
    TM1640Stop();
    TM1640Start();
    TM1640WriteByte(0xce);
    TM1640WriteByte(ltemp[3]);
    TM1640Stop();
    TM1640Start();
    TM1640WriteByte(0xcf);
    TM1640WriteByte(ltemp[4]);
    TM1640Stop();
    TM1640Start();
    TM1640WriteByte(0xcd);
    TM1640WriteByte(ltemp[5]);
    TM1640Stop();
    TM1640Start();
    TM1640WriteByte(0x8b); //Set the starting address
    TM1640Stop();
}

void TM1640_GPIO_Config(void)
{
    P2M0 = 0x03;
    P20 = 0;
    P21 = 0;
    PUCON0 = 0x00;
}
