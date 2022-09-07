//
// Created by aditya on 28-08-2022.
//

#include "utility.h"

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