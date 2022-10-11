//
// Created by aditya on 11-10-2022.
//
// #pragma SRC
#include "memory.h"
#include "REG_MG82FG5Bxx.h"
#include "macro.h"



float xdata memoryOne = 0.0;
float xdata memoryTwo = 0.0;
float xdata memoryThree = 0.0;
float xdata memoryFour = 0.0;
float xdata memoryFive = 0.0;
float xdata memorySix = 0.0;
float xdata memorySeven = 0.0;


union f  {
  float          f;          /* Floating-point value */
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

    IFADRL = end_add;
    SCMD = 0x46;
    SCMD = 0x0B9;
    thing.bytes[0] = IFD;

    IFADRL = end_add + 1;
    SCMD = 0x46;
    SCMD = 0x0B9;
    thing.bytes[1] = IFD;

    IFADRL = end_add + 2;
    SCMD = 0x46;
    SCMD = 0x0B9;
    thing.bytes[2] = IFD;

    IFADRL = end_add + 3;
    SCMD = 0x46;
    SCMD = 0x0B9;
    thing.bytes[3] = IFD;

    IFMT = 0x00;
    ISPCR = 0x00;
    // #pragma asm
    //     MOV CKCON1,#00001011b
    //     MOV ISPCR,#10000000b
    //     MOV IFMT,#01h
    //     MOV IFADRH,end_add
    //     MOV IFADRL,start_add
    //     MOV SCMD,#46h
    //     MOV SCMD,#0B9h
    //     MOV readWrite,IFD
    //     MOV IFMT,#00h
    //     MOV ISPCR,#00000000b
    // #pragma endasm

    x.f = thing.a;
    if (x.ul == NaN){
        return 0.0;
    }
    return thing.a;
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
        IFADRL = end_add;
        IFD = thing.bytes[0] ;
        SCMD = 0x46;
        SCMD = 0x0B9;

        IFADRL = end_add + 1;
        IFD = thing.bytes[1] ;
        SCMD = 0x46;
        SCMD = 0x0B9;

        IFADRL = end_add + 2;
        IFD = thing.bytes[2] ;
        SCMD = 0x46;
        SCMD = 0x0B9;

        IFADRL = end_add + 3;
        IFD = thing.bytes[3] ;
        SCMD = 0x46;
        SCMD = 0x0B9;


        IFMT = 0x00;
        ISPCR = 0x00;

    // #pragma asm
    //     MOV CKCON1,#00001011b
    //     MOV ISPCR,#10000000b 
    //     MOV IFMT,#02h ; 
    //     MOV IFADRH, end_add 
    //     MOV IFADRL, start_add
    //     MOV IFD, readWrite 
    //     MOV SCMD,#46h
    //     MOV SCMD,#0B9h

    //     MOV IFMT,#00h
    //     MOV ISPCR,#00000000b
    // #pragma endasm

}