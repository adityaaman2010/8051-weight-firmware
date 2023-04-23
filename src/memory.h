//
// Created by aditya on 11-10-2022.
//

#ifndef WEIGHT_MACHINE_MEMORY_H
#define WEIGHT_MACHINE_MEMORY_H

#define NaN       0xFFFFFFFF
int loadInteger(unsigned char, unsigned char);
unsigned char loadCharacter(unsigned char, unsigned char);
void saveInteger(unsigned char, unsigned char, int);
float getPriceFromMemory(unsigned char);
void savePriceToMemory(unsigned char, float);
void loadPricesFromMemory(void);
float loadOnePrice(unsigned char);
unsigned char* getMemoryNumber(unsigned char);
int loadPrecision(void);
int loadDisplayZeroFlag(void);
int loadPowerOffFlag(void);
int loadBuzzerFlag(void);
unsigned char* loadCompanyName(void);
void savePrecision(int);
void saveDisplayZeroFlag(int);
void savePowerOffFlag(int);
void saveBuzzerFlag(int);
void saveCharacter(unsigned char, unsigned char, unsigned char);
void saveCompanyName(unsigned char*);
unsigned char* loadPassword(void);
void savePassword(unsigned char*);
float loadFloat(unsigned char, unsigned char);
void saveFloat(unsigned char, unsigned char, float);
float loadAutoZeroTracking(void);
void saveAutoZeroTracking(float);
void saveCapacityAndResolution(float*);
float loadCapacityAndResolution(int i);
void saveWeightCalibration(float value);
float getWeightCalibration(void);


#endif // !WEIGHT_MACHINE_MEMORY_H