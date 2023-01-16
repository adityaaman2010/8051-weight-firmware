#ifndef HX711_I2C_H
#define HX711_I2C_H

#include "REG_MG82FG5Bxx.H"



void i2c_start();
void i2c_stop();
void i2c_write(unsigned char data);
unsigned char i2c_read(unsigned char ack);
unsigned long read_hx711();

#endif