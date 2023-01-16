#include <stdio.h>

#include "adc.h"

sbit SDA = P3^2;
sbit SCL = P3^3;


void i2c_start();
void i2c_stop();
void i2c_write(unsigned char data);
unsigned char i2c_read(unsigned char ack);
unsigned long read_hx711();

void main() {
    unsigned long weight;
    while(1) {
        weight = read_hx711();
        printf("Weight: %lu\n", weight);
        delay(1000); //todo change delay function
    }
}

unsigned long read_hx711() {
    unsigned long count;
    unsigned char data[3];
    i2c_start();
    i2c_write(0x90);
    i2c_write(0xE0);
    i2c_stop();
    i2c_start();
    i2c_write(0x91);
    data[2] = i2c_read(1);
    data[1] = i2c_read(1);
    data[0] = i2c_read(0);
    i2c_stop();
    count = ((unsigned long)data[2] << 16) | ((unsigned long)data[1] << 8) | (unsigned long)data[0];
    return count;
}

void i2c_start() {
    SDA = 1;
    SCL = 1;
    delay(1);
    SDA = 0;
    delay(1);
    SCL = 0;
}

void i2c_stop() {
    SCL = 0;
    SDA = 0;
    delay(1);
    SCL = 1;
    delay(1);
    SDA = 1;
}

void i2c_write(unsigned char data) {
    unsigned char i;
    for(i = 0; i < 8; i++) {
        SCL = 0;
        if((data & 0x80) != 0) {
            SDA = 1;
        } else {
            SDA = 0;
        }
        delay(1);
        SCL = 1;
        delay(1);
        data <<= 1;
    }
    SCL = 0;
    SDA = 1;
    delay(1);
    SCL = 1;
    delay(1);
    SCL = 0;
}

unsigned char i2c_read(unsigned char ack) {
    unsigned char i, data = 0;
    SDA = 1;
    for(i = 0; i < 8; i++) {
        SCL = 0;
        delay(1);
        SCL = 1;
        data <<= 1;
        if(SDA != 0) {
            data |= 0x01;
        }
        delay(1);
    }
    SCL = 0;
    if(ack == 1) {
        SDA = 0;
    } else {
        SDA = 1;
    }
    delay(1);
    SCL = 1;
    delay(1);
    SCL = 0;
    return data;
}
