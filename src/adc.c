#include "REG_MG82FG5Bxx.h"
#include "adc.h"
#define DT P41
#define SCK P40
#define P3_0 P30
void delay(unsigned char);
void uart_init(void);
void Adc_Digi_Conv(void);
void Adc_Process(unsigned long count);
unsigned long w;
unsigned char local_adc_digi[8];
//unsigned char adc_sub[5];

unsigned long readCount()
{
    unsigned int val=101.187;
    unsigned long sample=555053;
    unsigned long w;
    long count = 0;
    unsigned char i;
	  P4M0=0x03;
	  DT=0;
	  SCK=0;
	 // DT=0;
    DT = 1;
	  P4M0=0x01;
	  DT=1;
    SCK = 0;
	 // DT=1;
    while(DT == 1);

    for(i = 0; i < 24; i++)
    {
			  
        SCK = 1;
				delay(17);
        count = count << 1;
        SCK = 0;
		 	  delay(1);
        if(DT == 1)
        {
					count++;
				}
    }
		SCK = 1;
    delay(17);
    SCK = 0;
		delay(1);
    count = count ^ 0x800000;
    w = (((count - sample) / val) - 2 * ((count - sample) / val));
    return w;
}


void delay(unsigned char d_count)
{
    unsigned char i;
    for(i = 0; i < d_count; i++);
}
void uart_init(void)
{
	CKCON0=0x08;
	MCDS0 ;
	IHRCOE ;
	P3M0=0x01;
	S0CON=0x40;
	PCON0=0x80;
	S0CFG=0x00;
	AUXR2=0x08;
	TCON=0x40;
	TMOD=0x20;
	TL1=178;
	TH1=178;
	TCON=0x40;
	TMOD=0x20;
	P4M0=0x01;
}

void Adc_Process(unsigned long w)
{
	//unsigned char i;
	unsigned long adc_sample;
	adc_sample = w;

	while(adc_sample > 0)
	{
		if(adc_sample >= 10000000)
		{
			local_adc_digi[0] = adc_sample /  10000000;
			adc_sample = adc_sample  %  10000000; 
			local_adc_digi[1] = adc_sample /  1000000;
			adc_sample = adc_sample  %  1000000;
			local_adc_digi[2] = adc_sample /  100000;
			adc_sample = adc_sample  %  100000;
			local_adc_digi[3] = adc_sample /  10000;
			adc_sample = adc_sample  %  10000;
			local_adc_digi[4] = adc_sample /  1000;
			adc_sample = adc_sample  %  1000;
			local_adc_digi[5] = adc_sample /  100;
			adc_sample = adc_sample  %  100;
			local_adc_digi[6] = adc_sample /  10;
			adc_sample = adc_sample  %  10;
			local_adc_digi[7] = adc_sample;
			break;
		}
		
	
		if(adc_sample >= 1000000 && adc_sample <10000000)
		{
			local_adc_digi[0] = 0 ;
			local_adc_digi[1] = adc_sample /  1000000;
			adc_sample = adc_sample  %  1000000;
			local_adc_digi[2] = adc_sample /  100000;
			adc_sample = adc_sample  %  100000;
			local_adc_digi[3] = adc_sample /  10000;
			adc_sample = adc_sample  %  10000;
			local_adc_digi[4] = adc_sample /  1000;
			adc_sample = adc_sample  %  1000;
			local_adc_digi[5] = adc_sample /  100;
			adc_sample = adc_sample  %  100;
			local_adc_digi[6] = adc_sample /  10;
			adc_sample = adc_sample  %  10;
			local_adc_digi[7] = adc_sample;
			break;
		}
		if(adc_sample >= 100000 && adc_sample < 1000000)
		{
			local_adc_digi[0] = 0 ;
			local_adc_digi[1] = 0 ;
			local_adc_digi[2] = adc_sample /  100000;
			adc_sample = adc_sample  %  100000;
			local_adc_digi[3] = adc_sample /  10000;
			adc_sample = adc_sample  %  10000;
			local_adc_digi[4] = adc_sample /  1000;
			adc_sample = adc_sample  %  1000;
			local_adc_digi[5] = adc_sample /  100;
			adc_sample = adc_sample  %  100;
			local_adc_digi[6] = adc_sample /  10;
			adc_sample = adc_sample  %  10;
			local_adc_digi[7] = adc_sample;
			break;	
		}
	
		if(adc_sample >=10000 && adc_sample <100000)
		{
			local_adc_digi[0] = 0 ;
			local_adc_digi[1] = 0 ;
			local_adc_digi[2] = 0 ;
			local_adc_digi[3] = adc_sample /  10000;
			adc_sample = adc_sample  %  10000;
			local_adc_digi[4] = adc_sample /  1000;
			adc_sample = adc_sample  %  1000;
			local_adc_digi[5] = adc_sample /  100;
			adc_sample = adc_sample  %  100;
			local_adc_digi[6] = adc_sample /  10;
			adc_sample = adc_sample  %  10;
			local_adc_digi[7] = adc_sample;
			break;
		}
		if(adc_sample >=1000 && adc_sample <10000)
		{
			local_adc_digi[0] = 0 ;
			local_adc_digi[1] = 0 ;
			local_adc_digi[2] = 0 ;
			local_adc_digi[3] = 0 ;
			local_adc_digi[4] = adc_sample /  1000;
			adc_sample = adc_sample  %  1000;
			local_adc_digi[5] = adc_sample /  100;
			adc_sample = adc_sample  %  100;
			local_adc_digi[6] = adc_sample /  10;
			adc_sample = adc_sample  %  10;
			local_adc_digi[7] = adc_sample;
			break;
		}
		if(adc_sample >=100 && adc_sample <1000)
		{
			local_adc_digi[0] = 0 ;
			local_adc_digi[1] = 0 ;
			local_adc_digi[2] = 0 ;
			local_adc_digi[3] = 0 ;
			local_adc_digi[4] = 0 ;
			local_adc_digi[5] = adc_sample /  100;
			adc_sample = adc_sample  %  100;
			local_adc_digi[6] = adc_sample /  10;
			adc_sample = adc_sample  %  10;
			local_adc_digi[7] = adc_sample;
			break;
		}
		if(adc_sample >=10 && adc_sample <100)
		{
			local_adc_digi[0] = 0 ;
			local_adc_digi[1] = 0 ;
			local_adc_digi[2] = 0 ;
			local_adc_digi[3] = 0 ;
			local_adc_digi[4] = 0 ;
			local_adc_digi[5] = 0 ;
			local_adc_digi[6] = adc_sample /  10;
			adc_sample = adc_sample  %  10;
			local_adc_digi[7] = adc_sample;
			break;
		}
		if(adc_sample < 10 )
		{
			local_adc_digi[0] = 0 ;
			local_adc_digi[1] = 0 ;
			local_adc_digi[2] = 0 ;
			local_adc_digi[3] = 0 ;
			local_adc_digi[4] = 0 ;
			local_adc_digi[5] = 0 ;
			local_adc_digi[6] = 0 ;
			local_adc_digi[7] = adc_sample;
			break;
		}
	}
		
}

void Adc_Digi_Conv(void)
{
	unsigned char i;
	//unsigned char a[10];
  for(i=0;i<8;i++)
	{
		local_adc_digi[i] += 0x30;
	}
	//S0BUF='M';
}


