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
#include "settings.h"
#include "adc.h"


void handleTare(void);
void key_display(void);
void displayPrice(void);
void addToInputPrice(void);
void displayWeight(void);
void loadMemory(void);
void clearPrice(void);
void setRecallMode(void);
void handleModeFive(void);
void setChangeMode(void);
void handleNumberInput(void);
void handleModeOne(void);
void handleModeTwo(void);
void handleModeThree(void);
void handleModeFour(int);
void handleModeSix(void);
void key_sort(unsigned char);
unsigned char* getCharArray(int);
void initializeDisplay();

unsigned char xdata hi_key_no, lo_key_no;
unsigned char xdata digitHex[] = {0xed,0xa0,0xd9,0xf8,0xb4,0x7c,0x7d,0xe0,0xfd,0xfc};	//0,1,2,3,4,5,6,7,8,9
unsigned char xdata digi_chk[] = {0x40,0xc0,0xe0,0xe8,0xe9,0xed,0xfd};
unsigned char xdata prc[] = {0x00, 0x4d,0x45, 0xd5, 0x00};
unsigned char xdata ver[] = { 0xed, 0xa2,0x00,0x45,0x5d, 0xad};
unsigned char xdata compny_name[] = { 0xb5, 0x5d,0x0d, 0x0d, 0xed, 0x10, 0x7c, 0x4d, 0xf5,0x0d};
unsigned char* xdata companyName;
unsigned char xdata blank_L[] = { 0x00,0x00,0x00,0x00,0x00,0x00};
unsigned char xdata blank_M[] = { 0x00,0x00,0x00,0x00,0x00};
unsigned char xdata bat_digi[] = { 0x00,0x1d, 0xf5, 0xfd,0x00};
unsigned char xdata bat_voltg[] = { 0xb4,0xa2, 0xa0, 0x00,0x00};
// flag to check if decimal mode activated
int xdata isDecimal = 0,afterDecimal = 0, i, j, showWeight = 1;
int xdata precision = 2, mode = 0, previousMode, isOverflow = 0, memoryAddCount = 0;
float xdata total, currentPrice, previousPrice = 0, memoryModeTotal;
unsigned char xdata key, inputPrice[7], finalDisplay[7], temp[1], savingTo = -1, loadedMemory;
unsigned char* xdata output;
float xdata weight;


void main(void)
{
    int xdata doBreak = -2, temp, timer = 0, sleepDisplay = 0, shouldPowerOff = 0;
    int aa, oo;
    Keypad_GPIO_Config();
    TM1640_GPIO_Config();
    Adc_GPIO_Config();
    TM1640_Init(DSPTM1640_DIS); 		//TM1640 initialization
    Delay_Some_Time(10);
    TM1640_U_display(prc);
    ldelay();

    TM1640_L_display(ver);
    ldelay();
    TM1640_M_display(blank_L);
    j = 1;
    while (j < 3)
    {
        key = scan_keypad();
        if (key != 'A')
        {
            doBreak = 1;
            break;
        }
        secondDelay(1);
        j++;
        
    }
    if (doBreak == 1)
    {
        temp = handleSettings();
    }else if (doBreak != 1)
    {
        initializeDisplay();
    }
    precision = loadPrecision();
    clearPrice();
    displayPrice();
    mode = UNIT_PRICE_MODE;
    setOffsetWeight(0);
    displayWeight();
    shouldPowerOff = loadPowerOffFlag();
    while(1)
    {
        weight = getWeight();
        if (weight > 0)
        {
            sleepDisplay = 0;
            timer = 0;
        }
        
        if (showWeight == 1 && sleepDisplay != 1)
        {
            displayWeight();
        }
        key = scan_keypad();
        Delay_Some_Time(5);
        if(key != 'A') {
            timer = 0;
            sleepDisplay = 0;
            if (mode == UNIT_PRICE_MODE)
            {
                handleModeOne();
            }
            else if (mode == MEMORY_SET_MODE)
            {
                handleModeTwo();
            }
            else if (mode == MEMORY_LOAD_MODE)
            {
                handleModeThree();
            }
            else if (mode == ADDITION_MODE)
            {
                handleModeFour(0);
            }
            else if (mode == CHNAGE_MODE)
            {
                handleModeFive();
            }
            
            else if (mode == RECALL_MODE)
            {
                handleModeSix();
            }
        }else
        {
            if (mode != CHNAGE_MODE && mode != RECALL_MODE && sleepDisplay != 1)
            {
                // aa = readCount();
                // oo =getOffsetCount();
                // output = getNumberDisplayFloat(aa-oo, 5, 0);
                // TM1640_M_display(output);
                displayPrice();
            }
            if (timer > 233 && shouldPowerOff == 1)
            {
                sleepDisplay = 1;
            }else if (weight == 0)
            {
                timer += 1;
            }else if (weight > 0)
            {
                timer = 0;
            }
        }
        Delay_Some_Time(10);
        if (sleepDisplay == 1)
        {
            TM1640_L_display(blank_L);
            TM1640_M_display(blank_M);
            TM1640_U_display(blank_M);
        }
    }
		
}

void displayWeight(void)
{
    // unsigned int x = debugMehtod();
    // output = getNumberDisplayFloat(x, 5, 0);
    // TM1640_U_display(output);
    int xdata displaySingleZero;
    displaySingleZero = loadDisplayZeroFlag();
    weight = getWeight();
    if (displaySingleZero == 1 && weight == 0)
    {
        output = getNumberDisplayFloat(weight, 5, 0);
    }else
    {
        output = getNumberDisplayFloat(weight, 5, 3);
    }
    TM1640_U_display(output);
}

void loadMemory(void)
{
    float x;
    savingTo = -1;
    x = loadOnePrice(key);
    loadedMemory = key;
    if (x != -1)
    {
        // mode in which current price is immutable
        currentPrice = x;
        displayPrice();
    }
}

void handleModeFive(void)
{
    float xdata y;
    unsigned char xdata temp[6];
    if (previousMode == RECALL_MODE)
    {
        y = memoryModeTotal;
    }
    else
    {
        y = total;
    }
    if (key < 10 && strlen(inputPrice) < 5)
    {
        addToInputPrice();
        output = getNumberDisplayFloat(currentPrice, 5, 0);
        TM1640_M_display(output);
    }
    else if (key == 11)
    {
        clearPrice();
        output = getNumberDisplayFloat(currentPrice, 5, 0);
        TM1640_M_display(output);
    }
    else if(key == 13 && currentPrice > y)
    {
        for (i = 0; i < 6; i++)
        {
            temp[i] = BLANK_HEX;
        }
        temp[0] = getHexFromAlphabet('n');
        temp[1] = getHexFromAlphabet('r');
        temp[2] = getHexFromAlphabet('t');
        temp[3] = getHexFromAlphabet('E');
        temp[4] = getHexFromAlphabet('r');
        TM1640_U_display(temp);
        showWeight = 0;
        output = getNumberDisplayFloat(currentPrice - y, 5, precision);
        TM1640_M_display(output);
        for (i = 0; i < 6; i++)
        {
            temp[i] = BLANK_HEX;
        }
        TM1640_L_display(temp);
        Delay_Some_Time(10000);
        if (previousMode == RECALL_MODE)
        {
            setRecallMode();
        }
        else if (previousMode == MEMORY_LOAD_MODE)
        {
            mode = MEMORY_LOAD_MODE;
            key = loadedMemory;
            showWeight = 1;
            loadMemory();
            displayWeight();
        }
        else
        {
            clearPrice();
            displayPrice();
            showWeight = 1;
            displayWeight();
            mode = UNIT_PRICE_MODE;
        }
    }
    
}

void setChangeMode(void)
{
    float xdata y;
    unsigned char xdata temp[6];
    previousMode = mode;
    mode = CHNAGE_MODE;
    if (previousMode == RECALL_MODE)
    {
        y = memoryModeTotal;
    }
    else
    {
        y = total;
    }
    for (i = 0; i < 6; i++)
    {
        temp[i] = BLANK_HEX;
    }
    temp[0] = getHexFromAlphabet('G');
    temp[1] = getHexFromAlphabet('r');
    temp[2] = getHexFromAlphabet('A');
    temp[3] = getHexFromAlphabet('H');
    temp[4] = getHexFromAlphabet('C');
    TM1640_U_display(temp);
    showWeight = 0;
    clearPrice();
    output = getNumberDisplayFloat(0, 5, 0);
    TM1640_M_display(output);
    output = getNumberDisplayFloat(y, 6, precision);
    TM1640_L_display(output);
}

void handleModeSix(void)
{
    unsigned char xdata temp[6];
    if (key != 11 && key != 13)
    {
        mode = UNIT_PRICE_MODE;
        for (i = 0; i < 6; i++)
        {
            temp[i] = BLANK_HEX;
        }
        TM1640_L_display(temp);
        temp[0] = getHexFromAlphabet('E');
        temp[1] = getHexFromAlphabet('S');
        temp[2] = getHexFromAlphabet('A');
        temp[3] = getHexFromAlphabet('r');
        temp[4] = getHexFromAlphabet('E');
        TM1640_M_display(temp);
        for (i = 0; i < 6; i++)
        {
            temp[i] = BLANK_HEX;
        }
        temp[0] = getHexFromAlphabet('n');
        temp[1] = getHexFromAlphabet('E');
        temp[2] = getHexFromAlphabet('n');
        TM1640_U_display(temp);
        Delay_Some_Time(5000);
        showWeight = 1;
        clearPrice();
        displayPrice();
        displayWeight();
    }
    else if (key == 11)
    {
        memoryModeTotal = 0;
        output = getNumberDisplayFloat(memoryModeTotal, 6, precision);
        TM1640_L_display(output);
    }
    else if (key == 13)
    {
        setChangeMode();
    }
}

void setRecallMode(void)
{
    mode = RECALL_MODE;
    key_sort(memoryAddCount);
    for (i = 0; i < 6; i++)
    {
        temp[i] = BLANK_HEX;
    }
    temp[0] = lo_key_no;
    temp[1] = hi_key_no;
    temp[3] = getHexFromAlphabet('n');
    temp[4] = getHexFromAlphabet('C');
    TM1640_M_display(temp);
    temp[0] = getHexFromAlphabet('L');
    temp[1] = getHexFromAlphabet('A');
    temp[2] = getHexFromAlphabet('t');
    temp[3] = getHexFromAlphabet('O');
    temp[4] = getHexFromAlphabet('t');
    temp[5] = BLANK_HEX;
    TM1640_U_display(temp);
    showWeight = 0;
    output = getNumberDisplayFloat(memoryModeTotal, 6, precision);
    TM1640_L_display(output);
}

void handleModeFour(int isSetting)
{
    unsigned char temp[6];
    if (isSetting == 1)
    {
        memoryAddCount = 1;
        memoryModeTotal = total;
        key_sort(memoryAddCount);
        for (i = 0; i < 6; i++)
        {
            temp[i] = BLANK_HEX;
        }
        temp[0] = lo_key_no;
        temp[1] = hi_key_no;
        temp[3] = getHexFromAlphabet('n');
        temp[4] = getHexFromAlphabet('C');
        TM1640_M_display(temp);
        for (i = 0; i < 6; i++)
        {
            temp[i] = BLANK_HEX;
        }
        temp[0] = getHexFromAlphabet('D');
        temp[1] = getHexFromAlphabet('D');
        temp[2] = getHexFromAlphabet('A');
        TM1640_U_display(temp);
        showWeight = 0;
        output = getNumberDisplayFloat(memoryModeTotal, 6, precision);
        TM1640_L_display(output);
        Delay_Some_Time(10000);
        displayPrice();
        showWeight = 1;
    }else{
        if (key < 12)
        {
            handleNumberInput();
            showWeight = 1;
        }
        else if (key > 16)
        {
            loadMemory();
            showWeight = 1;
        }
        else if (key == 12 && weight == 0)
        {
            setRecallMode();
            return;
        }
        else if (key == 12)
        {
            memoryAddCount += 1;
            memoryModeTotal += total;
            key_sort(memoryAddCount);
            for (i = 0; i < 6; i++)
            {
                temp[i] = BLANK_HEX;
            }
            temp[0] = lo_key_no;
            temp[1] = hi_key_no;
            temp[3] = getHexFromAlphabet('C');
            TM1640_M_display(temp);
            for (i = 0; i < 6; i++)
            {
                temp[i] = BLANK_HEX;
            }
            temp[0] = getHexFromAlphabet('d');
            temp[1] = getHexFromAlphabet('d');
            temp[2] = getHexFromAlphabet('A');
            TM1640_U_display(temp);
            showWeight = 0;
            output = getNumberDisplayFloat(memoryModeTotal, 6, precision);
            TM1640_L_display(output);
            Delay_Some_Time(10000);
            for (i = 0; i < 6; i++)
            {
                temp[i] = BLANK_HEX;
            }
            TM1640_L_display(temp);
            displayPrice();
            showWeight = 1;
        }
        else 
        {
            showWeight = 1;
            handleTare();
        }
    }
    displayWeight();
}

void handleModeThree(void)
{
    handleTare();
    if (key == 11)
    {
        clearPrice();
        mode = UNIT_PRICE_MODE;
        displayPrice();
    }
    else if (key > 16)
    {
        mode = MEMORY_LOAD_MODE;
        loadMemory();
    }
    else if (key == 12)
    {
        mode = ADDITION_MODE;
        handleModeFour(1);
        return;
    }
    else if (key == 13)
    {
        previousMode = mode;
        setChangeMode();
    }
    
    displayWeight();
}

void handleModeTwo(void)
{
    if (key == 16 && savingTo != -1)
    {
        savePriceToMemory(savingTo, currentPrice);
        displayWeight();
        Delay_Some_Time(100);
        key = savingTo;
        mode = MEMORY_LOAD_MODE;
        loadMemory();
        showWeight = 1;
    }
    else if (key > 16)
    {
        savingTo = key;
        for(i=0;i < 5; i++)
        {
            finalDisplay[i] = BLANK_HEX;
        }
        finalDisplay[5] = '\0';
        output = getMemoryNumber(savingTo);
        finalDisplay[0] = output[1];
        finalDisplay[1] = output[0];
        finalDisplay[2] = getHexFromAlphabet('t');
        finalDisplay[3] = getHexFromAlphabet('E');
        finalDisplay[4] = getHexFromAlphabet('S');
        TM1640_U_display(finalDisplay);
        showWeight = 0;
        return;
    }
    else if (key == 11)
    {
        clearPrice();
        mode = UNIT_PRICE_MODE;
        showWeight = 0;
        displayPrice();
    }
    else
    {
        handleNumberInput();
    }
}

void handleTare(void)
{
    if (key == 14)
    {
        setOffsetWeight(0);
        weight = getWeight();
        setOffsetWeight(weight);
        weight = getWeight();
        displayWeight();
    }
    
}

void handleNumberInput(void)
{
    isOverflow = (isDecimal == 1 && strlen(inputPrice) < 6) || ((isDecimal == 0 && strlen(inputPrice) + precision < 6) && key == 10) || (isDecimal == 0 && strlen(inputPrice) + precision < 5) ? 0 : 1;
    if ((key < 11 && isOverflow == 0) || key == 11)
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
            finalDisplay[i] = BLANK_HEX;
        }
        finalDisplay[5] = '\0';
        finalDisplay[2] = getHexFromAlphabet('t');
        finalDisplay[3] = getHexFromAlphabet('E');
        finalDisplay[4] = getHexFromAlphabet('S');
        TM1640_U_display(finalDisplay);
        showWeight = 0;
        return;
    }
    else if (key > 16)
    {
        mode = MEMORY_LOAD_MODE;
        loadMemory();
        return;
    }
    else if (key == 12)
    {
        mode = ADDITION_MODE;
        handleModeFour(1);
        return;
    }else if (key == 13)
    {
        setChangeMode();
        return;
    }
    handleNumberInput();
    handleTare();
    displayWeight();
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
    unsigned char temp[6];
    weight = getWeight();
    total = currentPrice * weight;
    output = getNumberDisplayFloat(currentPrice,5, precision);
    TM1640_M_display(output);
    output = getNumberDisplayFloat(total, 6, precision);
    TM1640_L_display(output);
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
   hi_key_no = digitHex[hi_key_no];
   lo_key_no = digitHex[lo_key_no];
}

void initializeDisplay()
{
    companyName = loadCompanyName();
    TM1640_U_display(blank_L);
    TM1640_M_display(blank_L);
    TM1640_L_display(blank_L);

    TM1640_L_display(blank_L);
    TM1640_U_display(blank_L);
    TM1640_M_display(blank_L);
    ldelay();

    displayCompanyName(companyName);
    ldelay();

    TM1640_digichk(digi_chk);
    ldelay();

    TM1640_L_display(blank_L);
    TM1640_U_display(blank_L);
    TM1640_M_display(blank_L);
    ldelay();
    ldelay();
}
