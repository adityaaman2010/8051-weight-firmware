//
// Created by aditya on 28-08-2022.
//
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "REG_MG82FG5Bxx.h"
#include "macro.h"
#include "tm1640.h"
#include "utility.h"

unsigned char xdata alphabets[] = {
        'A', 'b', 'C', 'd', 'E', 'F', 'g', 'H', 'I',
        'J', 'K', 'L', 'O', 'P', 'S', 'U', 'Z', 't',
        'n', 'r', 'y', 'm', 'o' , '-' , '\0'
    };
unsigned char xdata alphaHex[] = {
        0xf5, 0x3d, 0x4d, 0xb9, 0x5d, 0x55, 0xf9, 0xb5, 0x05,
        0xe8, 0x15, 0x0d, 0xed, 0xd5, 0x7c, 0xad, 0xd9, 0x1d,
        0x31, 0x11, 0xbc, 0x71, 0x39, 0x8, '\0'
    };
unsigned char xdata no_digits[] = {0xed,0xa0,0xd9,0xf8,0xb4,0x7c,0x7d,0xe0,0xfd,0xfc};	//0,1,2,3,4,5,6,7,8,9
unsigned char xdata overflowHex[] = {0x10, 0x10, 0x10, 0x10, 0x10, 0x10};
unsigned char xdata final_display[7];

unsigned char* getAlphaHex()
{
    return alphaHex;
}

unsigned char* getNumberHex()
{
    return no_digits;
}

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

unsigned char* getNumberDisplayFloat(float x, int displayLength, int precision)
{
    unsigned char value[8];
    unsigned char t[8];
    int value_len = 0;
    int set_flag = 0;
    int after_display = 0;
    int xdata y, i, index, displaySingleZero;
    unsigned char a;
    if (precision == 0)
    {
        sprintf(value, "%.0f", x);
    }
    else if (precision == 1)
    {
        sprintf(value, "%.1f", x);
    }
    else if (precision == 2)
    {
        sprintf(value, "%.2f", x);
    }
    else if (precision == 3)
    {
        sprintf(value, "%.3f", x);
    }
    for(i=0; i<strlen(value);i++)
    {
        if(value[i] == '.'){
            break;
        }
        else{
            value_len++;
        }
    }
    if((value_len + precision) > (displayLength))
    {
        // Display out of bound values on display
        return overflowHex;
    }

    for (i= strlen(value) - 1; i > -1; i--)
    {
        y = strlen(value) - 1 - i;
        t[y] = value[i];
    }
    t[strlen(value)] = '\0';
    for(i=0;i < displayLength; i++)
    {
        final_display[i] = BLANK_HEX;
    }
    final_display[displayLength] = '\0';
    for(i=0;i < strlen(t); i++)
    {
        if(t[i]  == '.')
        {
            set_flag = 1;
            after_display = 1;
            continue;
        }
        index = t[i] & 0x0f;
        a = no_digits[index];
        if(after_display == 1)
        {
            if (set_flag == 1)
            {
                a = 0x02 | a;
                final_display[i-1] = a;
                set_flag = 0;
            }
            else
            {
                final_display[i-1] = a;
            }
        }
        else
        {
            final_display[i] = a;
        }
        
    }
    return final_display;
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
void displayCompanyName(unsigned char *companyName)
{
    int xdata len, j, startIndex, endIndex, temp;
    unsigned char displayArray[5];
    len = companyName[0];
    if(len < 10)
    {
        startIndex = len - 10;
        endIndex = len - 1;
    }else{
        startIndex = 0;
        endIndex = 9;
    }
    while (endIndex < len + 10)
    {
        for (j = 0; j < 5; j++)
        {
            temp = 4 - j;
            if (startIndex + j < 0 || startIndex + j >= len)
            {
                displayArray[temp] = BLANK_HEX;
            }else
            {
                displayArray[temp] = companyName[startIndex+j+1];
            }
        }
        TM1640_U_display(displayArray);
        for (j = 0; j < 5; j++)
        {
            if (endIndex - j >= len)
            {
                displayArray[j] = BLANK_HEX;
            }else
            {
                displayArray[j] = companyName[endIndex - j + 1];
            }
        }
        TM1640_M_display(displayArray);
        startIndex++;
        endIndex++;
        Delay_Some_Time(100);
    }
    
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
