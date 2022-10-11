//
// Created by aditya on 11-10-2022.
//

#ifndef WEIGHT_MACHINE_MEMORY_H
#define WEIGHT_MACHINE_MEMORY_H

#define NaN       0xFFFFFFFF
float getPriceFromMemory(unsigned char);
void savePriceToMemory(unsigned char, float);
void loadPricesFromMemory(void);
float loadOnePrice(unsigned char);
unsigned char* getMemoryNumber(unsigned char);

#endif // !WEIGHT_MACHINE_MEMORY_H