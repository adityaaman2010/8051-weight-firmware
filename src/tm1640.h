//
// Created by aditya on 28-08-2022.
//

#ifndef WEIGHT_MACHINE_TM1640_H
#define WEIGHT_MACHI

void TM1640_GPIO_Config(void);
void TM1640_Init(unsigned char);
void TM1640Start(void);
void TM1640WriteByte(unsigned char );
void TM1640Stop(void);
void TM1640_display(unsigned char *);
void TM1640_U_display(unsigned char *);
void TM1640_M_display(unsigned char *);
void TM1640_L_display(unsigned char *);
void TM1640_UM_display(unsigned char *);
void TM1640_digichk(unsigned char *);
unsigned char getHexFromAlphabet(unsigned char);
unsigned char* getNumberDisplayFloat(float, int, int);
unsigned char* getAlphaHex();
unsigned char* getNumberHex();
void displayCompanyName(unsigned char *);

#endif //WEIGHT_MACHINE_TM1640_H
