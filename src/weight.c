//
// Created by aditya on 01-09-2022.
//

#include "REG_MG82FG5Bxx.h"
#include "macro.h"
#include "weight.h"
#include "utility.h"

float xdata offsetWeight = 0.00;
float xdata weightFactor = 1;
int xdata test = 0;

double getWeight() {
	if (2.5 - offsetWeight < 0)
	{
		return 0;
	}
	return 2.5 - offsetWeight;
}

void setOffsetWeight(float w)
{
    offsetWeight = w;
}

int getAdcRead(void)
{
	secondDelay(2);
	if (test == 0)
	{
		test += 1;
		return 25006;
	}
	else if (test == 1)
	{
		test = 2;
		return 0;
	}
	else
	{
		test = 0;
		return 35000;
	}
	
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
		Delay_Some_Time(100);
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
		Delay_Some_Time(100);
	}
	return a;
	
}	
