#ifndef HX711_I2C_H
#define HX711_I2C_H



void i2c_start(void);
void i2c_stop(void);
void i2c_write(unsigned char);
unsigned char i2c_read(unsigned char);
float read_hx711();

#endif