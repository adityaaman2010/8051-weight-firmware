//
// Created by aditya on 01-09-2022.
//
#include "REG_MG82FG5Bxx.h"
#include "macro.h"
#include "weight.h"
#include "utility.h"
#include "adc.h"
#include "memory.h"



unsigned int xdata offsetWeight = 0;
float xdata weightFactor = 0, autoZeroValue;
int xdata test = 0;
unsigned char xdata adc_digi[8];
unsigned long xdata codedZeroWeight = 55136;
unsigned int xdata adcCount = 0;
float* xdata capacityArray;



float getWeight()
{
	float xdata result = 0, finalResult, remainder = 0;
	adcCount = readCount() - offsetWeight;
	if (adcCount < 0)
	{
		return 0;
	}else
	{
		result = adcCount;
		if (weightFactor > 0)
		{
			result = weightFactor * result;
		}
		
	}
	if (result < autoZeroValue)
	{
		return 0;
	}
	finalResult = result;
	// if (capacityArray[0] > 0)
	// {
	// 	if (result <= (capacityArray[2]*1000))
	// 	{
	// 		finalResult = result - (finalResult % (int)capacityArray[1]);
	// 	}else if (capacityArray[0] > 1 && result <= (capacityArray[4]*1000))
	// 	{
	// 		finalResult = result - (finalResult % (int)capacityArray[3]);
	// 	}else if (capacityArray[0] > 2 && result <= (capacityArray[6]*1000))
	// 	{
	// 		finalResult = result - (finalResult % (int)capacityArray[5]);
	// 	}
	// }
	finalResult = finalResult / 1000;
	if (finalResult < 0)
	{
		return 0;
	}
	return round(finalResult, 3);
}

float getSettingWeight(void)
{
	float xdata result = 0;
	result = readCount() - offsetWeight;
	result = result / 1000;
	if (result < 0)
	{
		return 0;
	}
	return round(result, 3);
}



unsigned int getOffsetCount(void)
{
	return offsetWeight;
}

void setOffsetWeight(float w)
{
    offsetWeight = readCount();
	if (w == 0)
	{
		weightFactor = getWeightCalibration();
		capacityArray = loadCapacityAndResolution();
		autoZeroValue = loadAutoZeroTracking();
	}
	
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

void Adc_GPIO_Config(void)
{
		P4M0 = 0x10;
		P44 = 0;
		P45 = 1;
		ADC_CLK = 1;
}

