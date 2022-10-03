//
// Created by aditya on 11-10-2022.
//

#include "memory.h"
#include "REG_MG82FG5Bxx.h"
#include "macro.h"

float xdata memoryOne = -1;
float xdata memoryTwo = -1;
float xdata memoryThree = -1;
float xdata memoryFour = -1;
float xdata memoryFive = -1;
float xdata memorySix = -1;
float xdata memorySeven = -1;


void loadPricesFromMemory(void)
{

}

float getPriceFromMemory(unsigned char key)
{
    float xdata result;
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

void savePriceToMemory(unsigned char key, float price)
{

}