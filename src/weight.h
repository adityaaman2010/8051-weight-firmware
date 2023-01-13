//
// Created by aditya on 01-09-2022.
//

#ifndef WEIGHT_MACHINE_WEIGHT_H
#define WEIGHT_MACHINE_WEIGHT_H

double getWeight();
void setOffsetWeight(float);
unsigned long int Adc_Read(void);
int getAdcRead(void);
void Adc_GPIO_Config(void);
void Adc_Process(unsigned long int adc_temp);
unsigned char* getAdcWeight(void);

#endif //WEIGHT_MACHINE_WEIGHT_H
