//
// Created by aditya on 28-08-2022.
//

#ifndef WEIGHT_MACHINE_MACRO_H
#define WEIGHT_MACHINE_MACRO_H

#define ZERO_HEX     0xed
#define BLANK_HEX    0x00
#define GPIO_write_DIN_high()		P20 = 1
#define GPIO_write_DIN_low()		P20 = 0
#define GPIO_write_SCLK_high()	P21 = 1
#define GPIO_write_SCLK_low()		P21 = 0
#define     addr_TM1640   (0xc0+0x01)
#define     DSPTM1640_DIS     0
#define     DSPTM1640_ENB     1
//Mode setting
#define TM1640_ADD      0x40    //Macro definition automatic plus one mode
#define TM1640_FIEXD    0x44    //Macro definition fixed address mode
#define DSP1640_DIS     0x80    //Macro definition brightness off
#define TM1640_DISPLAY  0x8f    //Macro definition brightness 11/16 pulse width adjustable
#define     DSPTM1640_DIS     0

#define	ADC_DOUT	P45
#define	ADC_CLK		P44

#define KROW0  	P10					//Keypad Rows
#define KROW1  	P11
#define KROW2  	P12
#define KROW3 	P13
#define KROW4  	P14

#define KCOL0 	P15				// Keypad Coloums
#define KCOL1 	P16
#define KCOL2 	P22
#define KCOL3 	P24
#define KCOL4 	P26

#define UNIT_PRICE_MODE 1
#define MEMORY_SET_MODE 2
#define MEMORY_LOAD_MODE 3
#define ADDITION_MODE 4
#define CHNAGE_MODE 5
#define RECALL_MODE 6
#define SETTINGS_MODE 7

#endif //WEIGHT_MACHINE_MACRO_H
