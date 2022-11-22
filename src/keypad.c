//
// Created by aditya on 02-09-2022.
//

#include "REG_MG82FG5Bxx.h"
#include "macro.h"
#include "keypad.h"


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
                key_status = 1;
//                key_status = 21;
            }
            if(KCOL1 == 1)
            {
                while(KCOL1 == 1);
                key_status = 2;
//                key_status = 24;
            }
            if(KCOL2 == 1)
            {
                while(KCOL2 == 1);
                key_status = 3;
//                key_status = 19;
            }
            if(KCOL3 == 1)
            {
                while(KCOL3 == 1);
                key_status = 23;
//                key_status = 14;
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
                key_status = 4;
//                key_status = 18;
            }
            if(KCOL1 == 1)
            {
                while(KCOL1 == 1);
                key_status = 5;
//                key_status = 13;
            }
            if(KCOL2 == 1)
            {
                while(KCOL2 == 1);
                key_status = 6;
//                key_status = 22;
            }
            if(KCOL3 == 1)
            {
                while(KCOL3 == 1);
                key_status = 21;
//                key_status = 17;
            }
            if(KCOL4 == 1)
            {
                while(KCOL4 == 1);
                key_status = 22;
//                key_status = 12;
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
                key_status = 7;
//                key_status = 10;
            }
            if(KCOL1 == 1)
            {
                while(KCOL1 == 1);
                key_status = 8;
//                key_status = 11;
            }
            if(KCOL2 == 1)
            {
                while(KCOL2 == 1);
                key_status = 9;
//                key_status = 9;
            }
            if(KCOL3 == 1)
            {
                while(KCOL3 == 1);
                key_status = 19;
//                key_status = 6;
            }
            if(KCOL4 == 1)
            {
                while(KCOL4 == 1);
                key_status = 20;
//                key_status = 3;
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
                key_status = 0;
//                key_status = 15;
            }
            if(KCOL1 == 1)
            {
                while(KCOL1 == 1);
                key_status = 10;
//                key_status = 16;
            }
            if(KCOL2 == 1)
            {
                while(KCOL2 == 1);
                key_status = 11;
//                key_status = 8;
            }
            if(KCOL3 == 1)
            {
                while(KCOL3 == 1);
                key_status = 17;
//                key_status = 5;
            }
            if(KCOL4 == 1)
            {
                while(KCOL4 == 1);
                key_status = 18;
//                key_status = 2;
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
                key_status = 15;
//                key_status = 20;
            }
            if(KCOL1 == 1)
            {
                while(KCOL1 == 1);
                key_status = 14;
//                key_status = 0;
            }
            if(KCOL2 == 1)
            {
                while(KCOL2 == 1);
                key_status = 16;
//                key_status = 7;
            }
            if(KCOL3 == 1)
            {
                while(KCOL3 == 1);
                key_status = 12;
//                key_status = 4;
            }
            if(KCOL4 == 1)
            {
                while(KCOL4 == 1);
                key_status = 13;
//                key_status = 1;
            }
        }
    }
    return key_status;
}
