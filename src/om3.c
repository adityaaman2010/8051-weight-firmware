#include "REG_MG82FG5Bxx.H"
#include "macro.h"
#include "utility.h"
#include "tm1640.h"


void key_display(void);
void Keypad_GPIO_Config(void);
unsigned char scan_keypad(void);
void key_sort(unsigned char);

unsigned char hi_key_no, lo_key_no;
unsigned char no_digits[] = {0xed,0xa0,0xd9,0xf8,0xb4,0x7c,0x7d,0xe0,0xfd,0xfc};	//0,1,2,3,4,5,6,7,8,9
unsigned char digi_chk[] = {0x40,0xc0,0xe0,0xe8,0xe9,0xed,0xfd};
unsigned char prc[] = {0x00, 0x4d,0x45, 0xd5, 0x00};
unsigned char ver[] = { 0xed, 0xa2,0x00,0x45,0x5d, 0xad};
unsigned char compny_name[] = { 0xb5, 0x5d,0x0d, 0x0d, 0xed, 0x10, 0x7c, 0x4d, 0xf5,0x0d,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
unsigned char blank_UM[] = {0x00,0x00,0x00,0x00,0x00};
unsigned char blank_L[] = { 0x00,0x00,0x00,0x00,0x00,0x00};
unsigned char bat_digi[] = { 0x00,0x1d, 0xf5, 0xfd,0x00};
unsigned char bat_voltg[] = { 0xb4,0xa2, 0xa0, 0x00,0x00};

unsigned char adc_digi[8];


void main(void)
{ 	
		
		unsigned char key ;
		TM1640_GPIO_Config();
		Keypad_GPIO_Config();
		TM1640_Init(DSPTM1640_DIS); 		//TM1640 initialization
		Delay_Some_Time(10 );
		
		TM1640_U_display(blank_UM);
		TM1640_M_display(blank_UM);
		TM1640_L_display(blank_L);
				
		TM1640_U_display(prc);
		ldelay();
			
		TM1640_L_display(ver);
		ldelay();
		
		TM1640_L_display(blank_L);
		TM1640_U_display(blank_UM);
		TM1640_M_display(blank_UM);
		ldelay();
		
		TM1640_UM_display(compny_name);
		ldelay();
		
		TM1640_digichk(digi_chk);
		ldelay();
		
		TM1640_L_display(blank_L);
		TM1640_U_display(blank_UM);
		TM1640_M_display(blank_UM);
		ldelay();
		
		TM1640_U_display(bat_digi);
		TM1640_M_display(bat_voltg);
		ldelay();
	
// 		TM1640_L_display(blank_L);
// 		TM1640_U_display(blank_UM);
// 		TM1640_M_display(blank_UM);
// 		ldelay();

		while(1)
		{
			key = scan_keypad();
			Delay_Some_Time(10);
			if(key != 'A')
			{
				key_sort(key);
			}
			Delay_Some_Time(10);
			key_display();
			Delay_Some_Time(5000);
		}
		
}

void Keypad_GPIO_Config(void)
{
		P1M0 = 0x1f;
		P22 = 1;
		P24 = 1;
		P26 = 1;
		P16 = 1;
		P15 = 1;
		P10 = 0;
		P11 = 0;
		P12 = 0;
		P13 = 0;
		P14 = 0;
		
}

void key_display(void)
{

    TM1640Start();
    TM1640WriteByte(0x44); //Set the starting address
    TM1640Stop();
    TM1640Start();
    TM1640WriteByte(0xc2);
    TM1640WriteByte(lo_key_no);
    TM1640Stop();
    TM1640Start();
    TM1640WriteByte(1);
    TM1640WriteByte(hi_key_no);
    TM1640Stop();

}

unsigned char scan_keypad(void)
{
	unsigned char i;
	unsigned char key_status = 'A';
	
	for(i = 0; i<5; i++)
	{
		if(	i == 0)
		{
			KROW0 = 1;
			KROW1 = 0;
			KROW2 = 0;
			KROW3 = 0;
			KROW4 = 0;
			if(KCOL0 == 1)
			{
				while(KCOL0 == 1);
				key_status = 21;
			}
			if(KCOL1 == 1)
			{
				while(KCOL1 == 1);
				key_status = 24;
			}
			if(KCOL2 == 1)
			{
				while(KCOL2 == 1);
				key_status = 19;
			}
			if(KCOL3 == 1)
			{
				while(KCOL3 == 1);
				key_status = 14;
			}
			if(KCOL4 == 1)
			{
				while(KCOL4 == 1);
				key_status = 4;
			}
		}
		if(	i == 1)
		{
			KROW0 = 0;
			KROW1 = 1;
			KROW2 = 0;
			KROW3 = 0;
			KROW4 = 0;
			if(KCOL0 == 1)
			{
				while(KCOL0 == 1);
				key_status = 18;
			}
			if(KCOL1 == 1)
			{
				while(KCOL1 == 1);
				key_status = 13;
			}
			if(KCOL2 == 1)
			{
				while(KCOL2 == 1);
				key_status = 22;
			}
			if(KCOL3 == 1)
			{
				while(KCOL3 == 1);
				key_status = 17;
			}
			if(KCOL4 == 1)
			{
				while(KCOL4 == 1);
				key_status = 12;
			}
		}
		if(	i == 2)
		{
			KROW0 = 0;
			KROW1 = 0;
			KROW2 = 1;
			KROW3 = 0;
			KROW4 = 0;
			if(KCOL0 == 1)
			{
				while(KCOL0 == 1);
				key_status = 10;
			}
			if(KCOL1 == 1)
			{
				while(KCOL1 == 1);
				key_status = 11;
			}
			if(KCOL2 == 1)
			{
				while(KCOL2 == 1);
				key_status = 9;
			}
			if(KCOL3 == 1)
			{
				while(KCOL3 == 1);
				key_status = 6;
			}
			if(KCOL4 == 1)
			{
				while(KCOL4 == 1);
				key_status = 3;
			}
		}
		if(	i == 3)
		{
			KROW0 = 0;
			KROW1 = 0;
			KROW2 = 0;
			KROW3 = 1;
			KROW4 = 0;
			if(KCOL0 == 1)
			{
				while(KCOL0 == 1);
				key_status = 15;
			}
			if(KCOL1 == 1)
			{
				while(KCOL1 == 1);
				key_status = 16;
			}
			if(KCOL2 == 1)
			{
				while(KCOL2 == 1);
				key_status = 8;
			}
			if(KCOL3 == 1)
			{
				while(KCOL3 == 1);
				key_status = 5;
			}
			if(KCOL4 == 1)
			{
				while(KCOL4 == 1);
				key_status = 2;
			}
		}
		if(	i == 4)
		{
			KROW0 = 0;
			KROW1 = 0;
			KROW2 = 0;
			KROW3 = 0;
			KROW4 = 1;
			if(KCOL0 == 1)
			{
				while(KCOL0 == 1);
				key_status = 20;
			}
			if(KCOL1 == 1)
			{
				while(KCOL1 == 1);
				key_status = 0;
			}
			if(KCOL2 == 1)
			{
				while(KCOL2 == 1);
				key_status = 7;
			}
			if(KCOL3 == 1)
			{
				while(KCOL3 == 1);
				key_status = 4;
			}
			if(KCOL4 == 1)
			{
				while(KCOL4 == 1);
				key_status = 1;
			}
		}
	}	
	return key_status;
}

void key_sort(unsigned char temp_key)
{
	if(temp_key <10)
	{
		hi_key_no = 0;
		lo_key_no = temp_key;
	}
	else
	{
		hi_key_no = temp_key/10;
		lo_key_no = temp_key%10;
	}
	hi_key_no = no_digits[hi_key_no];
	lo_key_no = no_digits[lo_key_no];
}
