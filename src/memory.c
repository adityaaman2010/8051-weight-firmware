//
// Created by aditya on 11-10-2022.
//
#pragma SRC
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
unsigned char start_add, end_add;
float readWrite;

union f  {
  float          f;          /* Floating-point value */
  unsigned long ul;          /* Unsigned long value */
};

void loadPricesFromMemory(void)
{
    memoryOne = loadPrice(17);
    memoryTwo = loadPrice(18);
    memoryThree = loadPrice(19);
    memoryFour = loadPrice(20);
    memoryFive = loadPrice(21);
    memorySix = loadPrice(22);
    memorySeven = loadPrice(23);

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

float loadPrice(unsigned char key)
{
    union f x;
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
    #pragma asm
        MOV CKCON1,#00001011b
        MOV ISPCR,#10000000b
        MOV IFMT,#01h
        MOV IFADRH,end_add
        MOV IFADRL,start_add
        MOV SCMD,#46h
        MOV SCMD,#0B9h
        MOV readWrite,IFD
        MOV IFMT,#00h
        MOV ISPCR,#00000000b
    #pragma endasm

    x.f = readWrite;
    if (x.ul == NaN){
        return 0.0;
    }
    return readWrite;
}

void savePriceToMemory(unsigned char key,float price)
{
    readWrite = price;
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

    #pragma asm
        MOV CKCON1,#00001011b
        MOV ISPCR,#10000000b 
        MOV IFMT,#02h ; 
        MOV IFADRH, end_add 
        MOV IFADRL, start_add
        MOV IFD, readWrite 
        MOV SCMD,#46h
        MOV SCMD,#0B9h

        MOV IFMT,#00h
        MOV ISPCR,#00000000b
    #pragma endasm

}