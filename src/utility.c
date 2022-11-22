//
// Created by aditya on 28-08-2022.
//

#include "utility.h"
#include "REG_MG82FG5Bxx.h"

void secondDelay(int second)
{
    int i;
    for (i = 0; i < second; i++)
    {
        int i;
        TMOD = 0x01;
        for(i=0; i<1000; i++){
            TH0 = 0xFC;
            TL0 = 0x66;
            TR0 = 1;
            while(TF0 != 1);
            TR0 = 0;
            TF0 = 0;
        }
    }
    
}

void ldelay(void) {
    unsigned long int i,j;
    for(i=0;i< 50; i++)
    {
        for(j=0;j<1000; j++);
    }
}

void Delay_Some_Time(unsigned int delaycount) {
    unsigned int i,j;
    for(i=0;i<delaycount; i++)
    {
        for(j=0;j<333; j++);
    }
}

void joinCharacter(unsigned char* a, unsigned char* b) {
    int i = 0;
    while(a[i] != '\0') {
        i++;
    }
    a[i] = b[0];
    a[i+1] = '\0';
}