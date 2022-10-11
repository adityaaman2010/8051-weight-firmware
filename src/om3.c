#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "REG_MG82FG5Bxx.H"
#include "macro.h"
#include "utility.h"
#include "tm1640.h"
#include "keypad.h"
#include "weight.h"
#include "memory.h"



void key_display(void);
void Keypad_GPIO_Config(void);
void displayPrice(void);
void addToInputPrice(void);
void displayWeight(void);
void handleModeOne(void);
void loadMemory(void);
void clearPrice(void);
void handleNumberInput(void);
void handleModeOne(void);
void handleModeTwo(void);
void handleModeThree(void);
void key_sort(unsigned char);
unsigned char* getCharArray(int);
unsigned char* getNumberDisplayFloat(float, int, int);
void initializeDisplay();

unsigned char xdata hi_key_no, lo_key_no;
unsigned char xdata overflowHex[] = {0x10, 0x10, 0x10, 0x10, 0x10, 0x10};
unsigned char xdata no_digits[] = {0xed,0xa0,0xd9,0xf8,0xb4,0x7c,0x7d,0xe0,0xfd,0xfc};	//0,1,2,3,4,5,6,7,8,9
unsigned char xdata digi_chk[] = {0x40,0xc0,0xe0,0xe8,0xe9,0xed,0xfd};
unsigned char xdata prc[] = {0x00, 0x4d,0x45, 0xd5, 0x00};
unsigned char xdata ver[] = { 0xed, 0xa2,0x00,0x45,0x5d, 0xad};
unsigned char xdata compny_name[] = { 0xb5, 0x5d,0x0d, 0x0d, 0xed, 0x10, 0x7c, 0x4d, 0xf5,0x0d,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
unsigned char xdata blank_L[] = { 0x00,0x00,0x00,0x00,0x00,0x00};
unsigned char xdata bat_digi[] = { 0x00,0x1d, 0xf5, 0xfd,0x00};
unsigned char xdata bat_voltg[] = { 0xb4,0xa2, 0xa0, 0x00,0x00};
// flag to check if decimal mode activated
int xdata isDecimal = 0,afterDecimal = 0;
int xdata precision = 2, mode = 1, isOverflow = 0;
float xdata weight, total, currentPrice;
unsigned char xdata key, inputPrice[7], temp[1], final_display[7], savingTo = -1;
unsigned char* output;


void main(void)
{
    int i;
    initializeDisplay();
    while(1)
    {
        weight = getWeight();
        key = scan_keypad();
        Delay_Some_Time(10);
        if(key != 'A') {
            if (mode == 1)
            {
                handleModeOne();
            }else if (mode == 2)
            {
                handleModeTwo();
            }else if (mode == 3)
            {
                handleModeThree();
            }
            
            
        }
        Delay_Some_Time(10);
    }
    
		
}

void displayWeight(void)
{
    output = getNumberDisplayFloat(weight, 5, precision);
    TM1640_U_display(output);
}

void loadMemory(void)
{
    float x;
    savingTo = -1;
    x = loadOnePrice(key);
    if (x != -1)
    {
        // mode in which current price is immutable
        mode = 3;
        currentPrice = x;
        displayPrice();
    }
}

void handleModeThree(void)
{
    if (key == 11)
    {
        clearPrice();
        mode = 1;
        displayPrice();
    }else if (key > 16)
    {
        loadMemory();
    }
    displayWeight();
}

void handleModeTwo(void)
{
    int i;
    if (key == 16 && savingTo != -1)
    {
        savePriceToMemory(savingTo, currentPrice);
        displayWeight();
        Delay_Some_Time(100);
        key = savingTo;
        loadMemory();
    }
    else if (key > 16)
    {
        savingTo = key;
        for(i=0;i < 5; i++)
        {
            final_display[i] = BLANK_HEX;
        }
        final_display[5] = '\0';
        output = getMemoryNumber(savingTo);
        final_display[0] = output[1];
        final_display[1] = output[0];
        final_display[2] = getHexFromAlphabet('t');
        final_display[3] = getHexFromAlphabet('E');
        final_display[4] = getHexFromAlphabet('S');
        TM1640_U_display(final_display);
        return;
    }else if (key == 11)
    {
        clearPrice();
        mode = 1;
        displayPrice();
    }else
    {
        handleNumberInput();
    }
}

void handleModeOne(void)
{
    if (key == 16)
        {
            // set mode
            mode = 2;
            clearPrice();
            displayPrice();
            for(i=0;i < 5; i++)
            {
                final_display[i] = BLANK_HEX;
            }
            final_display[5] = '\0';
            final_display[2] = getHexFromAlphabet('t');
            final_display[3] = getHexFromAlphabet('E');
            final_display[4] = getHexFromAlphabet('S');
            TM1640_U_display(final_display);
            return;
        }
        if (key > 16)
        {
            loadMemory();
            return;
        }
        handleNumberInput();
        displayWeight();
}

void handleNumberInput(void)
{
    isOverflow = (isDecimal == 1 && strlen(inputPrice) < 6) || ((isDecimal == 0 && strlen(inputPrice) + precision < 6) && key == 10) || (isDecimal == 0 && strlen(inputPrice) + precision < 5) ? 0 : 1;
    if ((key < 11 && isOverflow == 0) || key == 11)
    {
        handleModeOne();
    }
}

void handleModeOne(void)
{
    if(key == 11)
    {
        clearPrice();
    }
    else if (key < 10 &&  isDecimal == 0)
    {
        addToInputPrice();
    }
    else if (key == 10 && isDecimal == 0)
    {
        isDecimal = 1;
        return;
    }
    else if (key < 10 &&  isDecimal == 1 && afterDecimal == 0)
    {
        // first number pressed after "."
        temp[0] = '.';
        joinCharacter(inputPrice, temp);
        key = 0x30 | key;
        temp[0] = key;
        joinCharacter(inputPrice, temp);
        afterDecimal = 1;
        currentPrice = atof(inputPrice);
    }
    else if (key < 10 &&  isDecimal == 1 && afterDecimal == 1)
    {
        addToInputPrice();
    }
    displayPrice();
}

void clearPrice(void)
{
    inputPrice[0] = '\0';
    isDecimal = 0;
    afterDecimal = 0;
    currentPrice = 0.0;
}

void addToInputPrice(void)
{
    key = 0x30 | key;
    temp[0] = key;
    joinCharacter(inputPrice, temp);
    currentPrice = atof(inputPrice);
}

void displayPrice(void)
{
    total = currentPrice * weight;
    output = getNumberDisplayFloat(currentPrice,5, precision);
    TM1640_M_display(output);
    output = getNumberDisplayFloat(total, 6, precision);
    TM1640_L_display(output);
}

void Keypad_GPIO_Config(void)
{
		P1M0 = 0x1f;
		P22 = 1;
		P24 = 1;
		P26 = 1;
		P16 = 1;
		P15 = 1;
		P10 = 0;
		P11 = 0;
		P12 = 0;
		P13 = 0;
		P14 = 0;
		
}

unsigned char* getNumberDisplayFloat(float x, int displayLength, int precision)
{
    unsigned char value[8];
    unsigned char t[8];
    int value_len = 0;
    int set_flag = 0;
    int after_display = 0;
    int y, i, index;
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

void key_display(void)
{
   unsigned char temp[6];
   temp[0] = lo_key_no;
   temp[1] = hi_key_no;
   temp[2] = BLANK_HEX;
   temp[3] = BLANK_HEX;
   temp[4] = BLANK_HEX;
   temp[5] = BLANK_HEX;
   TM1640_L_display(temp);

}
void key_sort(unsigned char temp_key)
{
   if(temp_key <10)
   {
       hi_key_no = 0;
       lo_key_no = temp_key;
   }
   else
   {
       hi_key_no = temp_key/10;
       lo_key_no = temp_key%10;
   }
   hi_key_no = no_digits[hi_key_no];
   lo_key_no = no_digits[lo_key_no];
}

void initializeDisplay()
{
    TM1640_GPIO_Config();
    Keypad_GPIO_Config();
    TM1640_Init(DSPTM1640_DIS); 		//TM1640 initialization
    Delay_Some_Time(10 );

    TM1640_U_display(blank_L);
    TM1640_M_display(blank_L);
    TM1640_L_display(blank_L);

    TM1640_U_display(prc);
    ldelay();

    TM1640_L_display(ver);
    ldelay();

    TM1640_L_display(blank_L);
    TM1640_U_display(blank_L);
    TM1640_M_display(blank_L);
    ldelay();

    TM1640_UM_display(compny_name);
    ldelay();

    TM1640_digichk(digi_chk);
    ldelay();

    TM1640_L_display(blank_L);
    TM1640_U_display(blank_L);
    TM1640_M_display(blank_L);
    ldelay();
    ldelay();

    output = getNumberDisplayFloat(0, 5, 2);
    TM1640_M_display(output);
    TM1640_U_display(output);
    output = getNumberDisplayFloat(0, 6, precision);
    TM1640_L_display(output);
    inputPrice[0] = '\0';
    mode = 1;
    weight = getWeight();
    displayWeight();
}
