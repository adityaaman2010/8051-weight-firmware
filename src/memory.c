//
// Created by aditya on 11-10-2022.
//
// #pragma SRC
#include "memory.h"
#include "REG_MG82FG5Bxx.h"
#include "macro.h"
#include "tm1640.h"
#include "utility.h"


float xdata memoryOne = 0.0;
float xdata memoryTwo = 0.0;
float xdata memoryThree = 0.0;
float xdata memoryFour = 0.0;
float xdata memoryFive = 0.0;
float xdata memorySix = 0.0;
float xdata memorySeven = 0.0;
int xdata l;
unsigned char xdata settingsStarter = 0x62, scrollingName[21], scrollingStart = 0x1D, password[4];


union f  {
  float          f;          /* Floating-point value */
  int           in;  
  unsigned long ul;          /* Unsigned long value */
};



void loadPricesFromMemory(void)
{
    memoryOne = loadOnePrice(17);
    memoryTwo = loadOnePrice(18);
    memoryThree = loadOnePrice(19);
    memoryFour = loadOnePrice(20);
    memoryFive = loadOnePrice(21);
    memorySix = loadOnePrice(22);
    memorySeven = loadOnePrice(23);

}

float getPriceFromMemory(unsigned char key)
{
    float result;
    switch (key)
    {
    case 17:
        result = memoryOne;
        break;
    case 18:
        result = memoryTwo;
        break;
    case 19:
        result = memoryThree;
        break;
    case 20:
        result = memoryFour;
        break;
    case 21:
        result = memoryFive;
        break;
    case 22:
        result = memorySix;
        break;
    case 23:
        result = memorySeven;
        break;
    default:
        result = -1;
        break;
    }
    return result;
}

unsigned char* getMemoryNumber(unsigned char key)
{
    unsigned char result[3];
    switch (key)
    {
    case 17:
        result[0] = 0xed;
        result[1] = 0xa0;
        break;
    case 18:
        result[0] = 0xed;
        result[1] = 0xd9;
        break;
    case 19:
        result[0] = 0xed;
        result[1] = 0xf8;
        break;
    case 20:
        result[0] = 0xed;
        result[1] = 0xb4;
        break;
    case 21:
        result[0] = 0xed;
        result[1] = 0x7c;
        break;
    case 22:
        result[0] = 0xed;
        result[1] = 0x7d;
        break;
    case 23:
        result[0] = 0xed;
        result[1] = 0xe0;
        break;
    default:
        result[0] = 0xed;
        break;
    }
    result[2] = '\0';
    return result;
}

float loadOnePrice(unsigned char key)
{
    unsigned char start_add, end_add;
    float readWrite;
    union f x;
    union {
        float a;
        unsigned char bytes[4];
    } thing;
    switch (key){
    case 17:
        start_add = 0x76;
        end_add = 0x00;
        break;
    case 18:
        start_add = 0x76;
        end_add = 0x04;
        break;
    case 19:
        start_add = 0x76;
        end_add = 0x08;
        break;
    case 20:
        start_add = 0x76;
        end_add = 0x0B;
        break;
    case 21:
        start_add = 0x76;
        end_add = 0x10;
        break;
    case 22:
        start_add = 0x76;
        end_add = 0x14;
        break;
    case 23:
        start_add = 0x76;
        end_add = 0x18;
        break;
    }
    CKCON1 = 0x0B;
    ISPCR = 0x80;
    IFMT = 0x01;
    IFADRH = start_add;
    for (l = 0; l < 4; l++)
    {
        IFADRL = end_add + l;
        SCMD = 0x46;
        SCMD = 0x0B9;
        thing.bytes[l] = IFD;
    }
    IFMT = 0x00;
    ISPCR = 0x00;
    x.f = thing.a;
    if (x.ul == NaN){
        return 0.0;
    }
    return thing.a;
}

int loadPrecision(void)
{
    return loadInteger(0x76, settingsStarter);
}

void savePrecision(int value)
{
    saveInteger(0x76, settingsStarter, value);
}

int loadDisplayZeroFlag(void)
{
    return loadInteger(0x76, settingsStarter + 2);
}

void saveDisplayZeroFlag(int value)
{
    saveInteger(0x76, settingsStarter + 2, value);
}

int loadPowerOffFlag(void)
{
    return loadInteger(0x76, settingsStarter + 4);
}

void savePowerOffFlag(int value)
{
    saveInteger(0x76, settingsStarter + 4, value);
}

int loadBuzzerFlag(void)
{
    return loadInteger(0x76, settingsStarter + 6);
}

void saveBuzzerFlag(int value)
{
    saveInteger(0x76, settingsStarter + 6, value);
}

unsigned char* loadPassword(void)
{
    password[0] = loadCharacter(0x76, settingsStarter + 8);
    password[1] = loadCharacter(0x76, settingsStarter + 9);
    password[2] = loadCharacter(0x76, settingsStarter + 10);
    password[3] = loadCharacter(0x76, settingsStarter + 11);
    if (password[0] == 0xff)
    {
        password[0] = 0xd9;
        password[1] = 0xed;
        password[2] = 0xa0;
        password[3] = 0xed;
    }
    return password;
}



void savePassword(unsigned char* value)
{
    saveCharacter(0x76, settingsStarter + 8, value[0]);
    saveCharacter(0x76, settingsStarter + 9, value[1]);
    saveCharacter(0x76, settingsStarter + 10, value[2]);
    saveCharacter(0x76, settingsStarter + 11, value[3]);
}

void saveAutoZeroTracking(float value)
{
    saveFloat(0x76, settingsStarter + 12, value);
}

float loadAutoZeroTracking(void)
{
    return loadFloat(0x76, settingsStarter + 12);
}

void saveCapacityAndResolution(float* value)
{
    int xdata i;
    unsigned char xdata address[] = { 0x00, 0x05, 0x10, 0x20, 0x30, 0x40, 0x50 };
    for(i=0; i < ((int)(2*value[0]) + 1); i++)
    {
        saveFloat(0x79, address[i], value[i]);
    }
}

float loadCapacityAndResolution(int i)
{
    unsigned char xdata address[] = { 0x00, 0x05, 0x10, 0x20, 0x30, 0x40, 0x50 };
    return loadFloat(0x79, address[i]);
}

unsigned char* loadCompanyName(void)
{
    for (l = 0; l < 21; l++)
    {
        scrollingName[l] = loadCharacter(0x76, scrollingStart + l);
    }
    if (scrollingName[0] == 0xff)
    {
        for (l = 1; l < 11; l++)
        {
            scrollingName[l] = 0xed;
        }
        scrollingName[0] = 10;
    }
    return scrollingName;
}

void saveCompanyName(unsigned char* value)
{
    for(l = 0; l < value[0]+1; l++)
    {
        saveCharacter(0x76, scrollingStart+l, value[l]);
    }
}

void saveWeightCalibration(float value)
{
    saveFloat(0x76, 0x3D, value);
}

float getWeightCalibration(void)
{
    return loadFloat(0x76, 0x3D);
}

void saveCharacter(unsigned char start_add, unsigned char end_add, unsigned char value)
{
    CKCON1 = 0x0B;
    ISPCR = 0x80;
    IFMT = 0x02 ; 
    IFADRH = start_add;
    IFADRL = end_add;
    IFD = value;
    SCMD = 0x46;
    SCMD = 0x0B9;
    IFMT = 0x00;
    ISPCR = 0x00;
}

unsigned char loadCharacter(unsigned char start_add, unsigned char end_add)
{
    unsigned char xdata result;
    CKCON1 = 0x0B;
    ISPCR = 0x80;
    IFMT = 0x01;
    IFADRH = start_add;
    IFADRL = end_add;
    SCMD = 0x46;
    SCMD = 0x0B9;
    result = IFD;
    IFMT = 0x00;
    ISPCR = 0x00;
    return result;
}

void saveInteger(unsigned char start_add, unsigned char end_add, int toSave)
{
    union {
        int a;
        unsigned char bytes[2];
    } thing;
    thing.a = toSave;
    CKCON1 = 0x0B;
    ISPCR = 0x80;
    IFMT = 0x02 ; 
    IFADRH = start_add;
    for (l = 0; l < 2; l++)
    {
        IFADRL = end_add + l;
        IFD = thing.bytes[l] ;
        SCMD = 0x46;
        SCMD = 0x0B9;
    }
    IFMT = 0x00;
    ISPCR = 0x00;
}

int loadInteger(unsigned char start_add, unsigned char end_add)
{
    union f x;
    union {
        int a;
        unsigned char bytes[2];
    } thing;
    CKCON1 = 0x0B;
    ISPCR = 0x80;
    IFMT = 0x01;
    IFADRH = start_add;
    for (l = 0; l < 2; l++)
    {
        IFADRL = end_add + l;
        SCMD = 0x46;
        SCMD = 0x0B9;
        thing.bytes[l] = IFD;
    }
    IFMT = 0x00;
    ISPCR = 0x00;
    x.in = thing.a;
    if (x.ul == NaN){
        return 0;
    }
    return thing.a;
}

float loadFloat(unsigned char start_add, unsigned char end_add)
{
    union f x;
    union {
        float a;
        unsigned char bytes[4];
    } thing;
    CKCON1 = 0x0B;
    ISPCR = 0x80;
    IFMT = 0x01;
    IFADRH = start_add;
    for (l = 0; l < 4; l++)
    {
        IFADRL = end_add + l;
        SCMD = 0x46;
        SCMD = 0x0B9;
        thing.bytes[l] = IFD;
    }
    IFMT = 0x00;
    ISPCR = 0x00;
    x.f = thing.a;
    if (x.ul == NaN){
        return 0;
    }
    return thing.a;
}

void saveFloat(unsigned char start_add, unsigned char end_add, float toSave)
{
    union {
        float a;
        unsigned char bytes[4];
    } thing;
    thing.a = toSave;
    CKCON1 = 0x0B;
    ISPCR = 0x80;
    IFMT = 0x02 ; 
    IFADRH = start_add;
    for (l = 0; l < 4; l++)
    {
        IFADRL = end_add + l;
        IFD = thing.bytes[l] ;
        SCMD = 0x46;
        SCMD = 0x0B9;
    }
    IFMT = 0x00;
    ISPCR = 0x00;
}

void savePriceToMemory(unsigned char key,float price)
{
    union {
        float a;
        unsigned char bytes[4];
    } thing;
    unsigned char start_add, end_add;
    float readWrite;
    readWrite = price;
    switch (key){
    case 17:
        start_add = 0x76;
        end_add = 0x00;
        memoryOne = price;
        break;
    case 18:
        start_add = 0x76;
        end_add = 0x04;
        memoryTwo = price;
        break;
    case 19:
        start_add = 0x76;
        end_add = 0x08;
        memoryThree = price;
        break;
    case 20:
        start_add = 0x76;
        end_add = 0x0B;
        memoryFour = price;
        break;
    case 21:
        start_add = 0x76;
        end_add = 0x10;
        memoryFive = price;
        break;
    case 22:
        start_add = 0x76;
        end_add = 0x14;
        memorySix = price;
        break;
    case 23:
        start_add = 0x76;
        end_add = 0x18;
        memorySeven = price;
        break;
    }
    thing.a = price;
    CKCON1 = 0x0B;
    ISPCR = 0x80;
    IFMT = 0x02 ; 
    IFADRH = start_add;
    for (l = 0; l < 4; l++)
    {
        IFADRL = end_add + l;
        IFD = thing.bytes[l] ;
        SCMD = 0x46;
        SCMD = 0x0B9;
    }
    IFMT = 0x00;
    ISPCR = 0x00;
}