//
// Created by aditya on 01-09-2022.
//

#include "REG_MG82FG5Bxx.h"
#include "macro.h"
#include "weight.h"

float xdata offsetWeight = 0.00;
float xdata weightFactor = 1;

double getWeight() {
    return 2.5 - offsetWeight;
}

void setOffsetWeight(float w)
{
    offsetWeight = w;
}

unsigned long int Adc_Read(void)
{
	unsigned char i;
	unsigned long int a;
	a = 0;
	ADC_CLK = 0;
	while(ADC_DOUT == 1);
	for(i = 0; i<24; i++)
	{
		ADC_CLK = 1;
		MicroSecondDelay();
		if(ADC_DOUT == 1)
		{
			a << i;
			a = a|1;
		}
		else
		{
			a << i;
			a = a|0;
		}
		ADC_CLK = 1;
		MicroSecondDelay();
	}
	return a;
	
}	
