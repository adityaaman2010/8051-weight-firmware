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

double stof(const char* s){
    double rez = 0, fact = 1;
    int point_seen, d;
    if (*s == '-'){
        s++;
        fact = -1;
    };
    for (point_seen = 0; *s; s++){
        if (*s == '.'){
            point_seen = 1;
            continue;
        };
        d = *s - '0';
        if (d >= 0 && d <= 9){
            if (point_seen) fact /= 10.0f;
            rez = rez * 10.0f + (float)d;
        };
    };
    return rez * fact;
}