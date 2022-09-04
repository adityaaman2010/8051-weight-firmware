#include "REG_MG82FG5Bxx.H"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "macro.h"
#include "utility.h"
#include "tm1640.h"
#include "keypad.h"
#include "weight.h"



void key_display(void);
void Keypad_GPIO_Config(void);
void key_sort(unsigned char);
unsigned char* get_number_display(unsigned char*, int, int);
unsigned char* get_number_display_float(double, int, int);
void initializeDisplay();

unsigned char hi_key_no, lo_key_no;
unsigned char xdata overflowHex[] = {0x10, 0x10, 0x10, 0x10, 0x10, 0x10};
unsigned char xdata no_digits[] = {0xed,0xa0,0xd9,0xf8,0xb4,0x7c,0x7d,0xe0,0xfd,0xfc};	//0,1,2,3,4,5,6,7,8,9
unsigned char xdata digi_chk[] = {0x40,0xc0,0xe0,0xe8,0xe9,0xed,0xfd};
unsigned char xdata prc[] = {0x00, 0x4d,0x45, 0xd5, 0x00};
unsigned char xdata ver[] = { 0xed, 0xa2,0x00,0x45,0x5d, 0xad};
unsigned char xdata compny_name[] = { 0xb5, 0x5d,0x0d, 0x0d, 0xed, 0x10, 0x7c, 0x4d, 0xf5,0x0d,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
unsigned char xdata blank_L[] = { 0x00,0x00,0x00,0x00,0x00,0x00};
unsigned char xdata bat_digi[] = { 0x00,0x1d, 0xf5, 0xfd,0x00};
unsigned char xdata bat_voltg[] = { 0xb4,0xa2, 0xa0, 0x00,0x00};
unsigned char final_display[6];


void main(void)
{ 	    double x;
		unsigned char key;
        double weight;
        double total;
        double currentPrice;
        unsigned char inputPrice[5];
        unsigned char* output;
        unsigned char temp[1];
        int precision = 2;
		initializeDisplay();
		while(1)
		{
            weight = getWeight();
            output = get_number_display_float(weight, 5, precision);
            TM1640_U_display(output);
			key = scan_keypad();
			Delay_Some_Time(10);
			if(key != 'A') {
                if (key < 11) {
                    key = 0x3f & key;
                    temp[0] = key;
                    strcat(inputPrice, temp);
                    x = atof(inputPrice);
                    total = x * weight;
                    output = get_number_display(inputPrice, 5, precision);
                    TM1640_M_display(output);
                    output = get_number_display_float(total, 6, precision);
                    TM1640_L_display(output);
                }
			}
			Delay_Some_Time(10);
//			key_display();
			Delay_Some_Time(100);
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


unsigned char* get_number_display(unsigned char* value, int displayLength, int precision){
    int value_len = 0;
    int set_flag = 0;
    int after_display = 0;
    int y, i, index;
    unsigned char t[sizeof(value)];
    unsigned char a;
    double x;
    x = atof(value);
    if (precision == 0){
        sprintf(value, "%.0lf", x);
    }else if (precision == 1) {
        sprintf(value, "%.1lf", x);
    }else if (precision == 2) {
        sprintf(value, "%.2lf", x);
    }else if (precision == 3) {
        sprintf(value, "%.3lf", x);
    }
    for(i=0; i<sizeof(value);i++){
        if(value[i] == '.'){
            break;
        }
        else{
            value_len++;
        }
    }
    if((value_len + precision) > (displayLength)){
        // Display out of bound values on display
        return overflowHex;
    }

    for (i= sizeof(value)-1; i > -1; i--){
        y = sizeof(value) - 1 - i;
        t[y] = value[i];
    }
    for(i=0;i < displayLength; i++) {
        final_display[i] = BLANK_HEX;
    }
    for(i=0;i < sizeof(t); i++) {
        if(t[i] == '.') {
            set_flag = 1;
            after_display = 1;
            continue;
        }
        index = t[i] & 0x0f;
        a = no_digits[index];
        if(after_display == 1) {
            if (set_flag == 1) {
                final_display[i-1] = a | 0x02;
                set_flag = 0;
            }else{
                final_display[i-1] = a;
            }
        }else{
            final_display[i] = a;
        }
    }
    return final_display;
}

unsigned char* get_number_display_float(double x, int displayLength, int precision){
    unsigned char value[displayLength + 1];
    unsigned char t[sizeof(value)];
    int value_len = 0;
    int set_flag = 0;
    int after_display = 0;
    int y, i, index;
    unsigned char a;
    if (precision == 0){
        sprintf(value, "%.0lf", x);
    }else if (precision == 1) {
        sprintf(value, "%.1lf", x);
    }else if (precision == 2) {
        sprintf(value, "%.2lf", x);
    }else if (precision == 3) {
        sprintf(value, "%.3lf", x);
    }
    for(i=0; i<strlen(value);i++){
        if(value[i] == '.'){
            break;
        }
        else{
            value_len++;
        }
    }
//    if((value_len + precision) > (displayLength)){
//        // Display out of bound values on display
//        return overflowHex;
//    }

    for (i= strlen(value) - 1; i > -1; i--){
        y = strlen(value) - 1 - i;
        t[y] = value[i];
    }
    t[strlen(value)] = '\0';
    for(i=0;i < displayLength; i++) {
        final_display[i] = BLANK_HEX;
    }
    for(i=0;i < strlen(t); i++) {
        
        if(t[i] == '.') {
            set_flag = 1;
            after_display = 1;
            continue;
        }
        index = t[i] & 0x0f;
        a = no_digits[index];
        if(after_display == 1) {
            if (set_flag == 1) {
                final_display[i-1] = a | 0x02;
                set_flag = 0;
            }else{
                final_display[i-1] = a;
            }
        }else{
            final_display[i] = a;
        }
        
    }
    return final_display;
}

void initializeDisplay() {
    TM1640_GPIO_Config();
    Keypad_GPIO_Config();
    TM1640_Init(DSPTM1640_DIS); 		//TM1640 initialization
    Delay_Some_Time(10 );

    TM1640_U_display(blank_L);
    TM1640_M_display(blank_L);
    TM1640_L_display(blank_L);

    TM1640_U_display(prc);
    ldelay();

    TM1640_L_display(ver);
    ldelay();

    TM1640_L_display(blank_L);
    TM1640_U_display(blank_L);
    TM1640_M_display(blank_L);
    ldelay();

    TM1640_UM_display(compny_name);
    ldelay();

    TM1640_digichk(digi_chk);
    ldelay();

    TM1640_L_display(blank_L);
    TM1640_U_display(blank_L);
    TM1640_M_display(blank_L);
    ldelay();

    TM1640_U_display(bat_digi);
    TM1640_M_display(bat_voltg);
    ldelay();
}
