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
unsigned char xdata adc_digi[8];

unsigned char* getAdcWeight()
{
	Adc_Process(Adc_Read());
	return adc_digi;
}

double getWeight()
{
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

void Adc_GPIO_Config(void)
{
		P4M0 = 0x10;
		P44 = 0;
		P45 = 1;
		ADC_CLK = 1;
}

void Adc_Process(unsigned long int adc_temp)
{
	unsigned long int adc_sample;
	adc_sample = adc_temp;

	while(adc_sample > 0)
	{
		if(adc_sample >= 10000000)
		{
			adc_digi[7] = adc_sample /  10000000;
			adc_sample = adc_sample  %  10000000; 
			adc_digi[6] = adc_sample /  1000000;
			adc_sample = adc_sample  %  1000000;
			adc_digi[5] = adc_sample /  100000;
			adc_sample = adc_sample  %  100000;
			adc_digi[4] = adc_sample /  10000;
			adc_sample = adc_sample  %  10000;
			adc_digi[3] = adc_sample /  1000;
			adc_sample = adc_sample  %  1000;
			adc_digi[2] = adc_sample /  100;
			adc_sample = adc_sample  %  100;
			adc_digi[1] = adc_sample /  10;
			adc_sample = adc_sample  %  10;
			adc_digi[0] = adc_sample;
			break;
		}
		
	
		if(adc_sample >= 1000000 && adc_sample <10000000)
		{
			adc_digi[7] = 0 ;
			adc_digi[6] = adc_sample /  1000000;
			adc_sample = adc_sample  %  1000000;
			adc_digi[5] = adc_sample /  100000;
			adc_sample = adc_sample  %  100000;
			adc_digi[4] = adc_sample /  10000;
			adc_sample = adc_sample  %  10000;
			adc_digi[3] = adc_sample /  1000;
			adc_sample = adc_sample  %  1000;
			adc_digi[2] = adc_sample /  100;
			adc_sample = adc_sample  %  100;
			adc_digi[1] = adc_sample /  10;
			adc_sample = adc_sample  %  10;
			adc_digi[0] = adc_sample;
			break;
		}
		if(adc_sample >= 100000 && adc_sample < 1000000)
		{
			adc_digi[7] = 0 ;
			adc_digi[6] = 0 ;
			adc_digi[5] = adc_sample /  100000;
			adc_sample = adc_sample  %  100000;
			adc_digi[4] = adc_sample /  10000;
			adc_sample = adc_sample  %  10000;
			adc_digi[3] = adc_sample /  1000;
			adc_sample = adc_sample  %  1000;
			adc_digi[2] = adc_sample /  100;
			adc_sample = adc_sample  %  100;
			adc_digi[1] = adc_sample /  10;
			adc_sample = adc_sample  %  10;
			adc_digi[0] = adc_sample;
			break;	
		}
	
		if(adc_sample >=10000 && adc_sample <100000)
		{
			adc_digi[7] = 0 ;
			adc_digi[6] = 0 ;
			adc_digi[5] = 0 ;
			adc_digi[4] = adc_sample /  10000;
			adc_sample = adc_sample  %  10000;
			adc_digi[3] = adc_sample /  1000;
			adc_sample = adc_sample  %  1000;
			adc_digi[2] = adc_sample /  100;
			adc_sample = adc_sample  %  100;
			adc_digi[1] = adc_sample /  10;
			adc_sample = adc_sample  %  10;
			adc_digi[0] = adc_sample;
			break;
		}
		if(adc_sample >=1000 && adc_sample <10000)
		{
			adc_digi[7] = 0 ;
			adc_digi[6] = 0 ;
			adc_digi[5] = 0 ;
			adc_digi[4] = 0 ;
			adc_digi[3] = adc_sample /  1000;
			adc_sample = adc_sample  %  1000;
			adc_digi[2] = adc_sample /  100;
			adc_sample = adc_sample  %  100;
			adc_digi[1] = adc_sample /  10;
			adc_sample = adc_sample  %  10;
			adc_digi[0] = adc_sample;
			break;
		}
		if(adc_sample >=100 && adc_sample <1000)
		{
			adc_digi[7] = 0 ;
			adc_digi[6] = 0 ;
			adc_digi[5] = 0 ;
			adc_digi[4] = 0 ;
			adc_digi[3] = 0 ;
			adc_digi[2] = adc_sample /  100;
			adc_sample = adc_sample  %  100;
			adc_digi[1] = adc_sample /  10;
			adc_sample = adc_sample  %  10;
			adc_digi[0] = adc_sample;
			break;
		}
		if(adc_sample >=10 && adc_sample <100)
		{
			adc_digi[7] = 0 ;
			adc_digi[6] = 0 ;
			adc_digi[5] = 0 ;
			adc_digi[4] = 0 ;
			adc_digi[3] = 0 ;
			adc_digi[2] = 0 ;
			adc_digi[1] = adc_sample /  10;
			adc_sample = adc_sample  %  10;
			adc_digi[0] = adc_sample;
			break;
		}
		if(adc_sample < 10 )
		{
			adc_digi[7] = 0 ;
			adc_digi[6] = 0 ;
			adc_digi[5] = 0 ;
			adc_digi[4] = 0 ;
			adc_digi[3] = 0 ;
			adc_digi[2] = 0 ;
			adc_digi[1] = 0 ;
			adc_digi[0] = adc_sample;
			break;
		}
	}
}

