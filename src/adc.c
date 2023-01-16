#include <stdio.h>
#include "REG_MG82FG5Bxx.h"
#include "adc.h"
#include "utility.h"

#define SDA  P44
#define SCL  P45


void main1() {
    unsigned long weight;
    while(1) {
        weight = read_hx711();
        printf("Weight: %lu\n", weight);
        Delay_Some_Time(1000); //todo change Delay_Some_Time function
    }
}

float read_hx711() {
    unsigned long count;
    unsigned char dataa[] = {0x00,0x00,0x00};
    i2c_start();
    i2c_write(0x90);
    i2c_write(0xE0);
    i2c_stop();
    i2c_start();
    i2c_write(0x91);
    dataa[2] = i2c_read(1);
    dataa[1] = i2c_read(1);
    dataa[0] = i2c_read(0);
    i2c_stop();
    count = ((unsigned long)dataa[2] << 16) | ((unsigned long)dataa[1] << 8) | (unsigned long)dataa[0];
    return count/66.9;
}

void i2c_start() {
    SDA = 1;
    SCL = 1;
    Delay_Some_Time(1);
    SDA = 0;
    Delay_Some_Time(1);
    SCL = 0;
}

void i2c_stop() {
    SCL = 0;
    SDA = 0;
    Delay_Some_Time(1);
    SCL = 1;
    Delay_Some_Time(1);
    SDA = 1;
}

void i2c_write(unsigned char dataa) {
    unsigned char i;
    for(i = 0; i < 8; i++) {
        SCL = 0;
        if((dataa & 0x80) != 0) {
            SDA = 1;
        } else {
            SDA = 0;
        }
        Delay_Some_Time(1);
        SCL = 1;
        Delay_Some_Time(1);
        dataa <<= 1;
    }
    SCL = 0;
    SDA = 1;
    Delay_Some_Time(1);
    SCL = 1;
    Delay_Some_Time(1);
    SCL = 0;
}

unsigned char i2c_read(unsigned char ack) {
    unsigned char i, dataa = 0;
    SDA = 1;
    for(i = 0; i < 8; i++) {
        SCL = 0;
        Delay_Some_Time(1);
        SCL = 1;
        dataa <<= 1;
        if(SDA != 0) {
            dataa |= 0x01;
        }
        Delay_Some_Time(1);
    }
    SCL = 0;
    if(ack == 1) {
        SDA = 0;
    } else {
        SDA = 1;
    }
    Delay_Some_Time(1);
    SCL = 1;
    Delay_Some_Time(1);
    SCL = 0;
    return dataa;
}
