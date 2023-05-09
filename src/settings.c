#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "REG_MG82FG5Bxx.H"
#include "macro.h"
#include "utility.h"
#include "tm1640.h"
#include "keypad.h"
#include "weight.h"
#include "adc.h"
#include "memory.h"
#include "settings.h"

unsigned char xdata numbers[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
int xdata resolutionArray[] = {1, 2, 5, 10, 20, 50, 100, 200, 500};
unsigned char xdata scannedKey, displayArray[7], step, breakSettings, currentPassword[4], frameName[21], newPass[4], capacityString[6];
unsigned char* xdata numbersHex;
unsigned char* xdata displayOutput;
unsigned char* xdata alphabetHex;
int xdata cursor = -1, k, setCompanyName = 1, frameNameAdded, stableBuzzer = 1, powerOff = 1;
int xdata displayZero = 1, setPassword = 1, decimalPrecision = 3, resolutionSteps = 1, isResolution = 1, weightPrecision = 3;
int xdata capacityLength = 0;
unsigned int xdata adcRead = 0, randomFloat;
float xdata autoZero = 1.0, currentCapacity, weightMultiplier;
float xdata resolutionCapacityArray[7];


int handleSettings(void)
{
    step = 0;
    breakSettings = -1;
    frameName[0] = 0;
    resetCurrentPassword();
    numbersHex = getNumberHex();
    alphabetHex = getAlphaHex();
    while (1)
    {
        scannedKey = scan_keypad();
        Delay_Some_Time(5);
        if (scannedKey != 'A')
        {
            switch (step)
            {
                case 0:
                    handlePassword();
                    break;
                case 1:
                    handlePasswordConfirmation(0);
                    break;
                case 2:
                    handleChangePassword(0);
                    break;
                case 3:
                    handleFrameConfirmation(0);
                    break;
                case 4:
                    handleCompanyNameLength(0);
                    break;
                case 5:
                    handleCompanyNameInput(0);
                    break;
                case 6:
                    handleBuzzerOnOff(0);
                    break;
                case 7:
                    handlePowerOffOn(0);
                    break;
                case 8:
                    handleDisplaySingleZero(0);
                    break;
                case 9:
                    handleAutoZeroTracking(0);
                    break;
                case 10:
                    handlePrecisionSetting(0);
                    break;
                case 11:
                    handleResolutionTimes(0);
                    break;
                case 12:
                    handleResolutionAndCapacityInput(0);
                    break;
                default:
                    breakSettings = 1;
                    break;
            }
            if (breakSettings == 1)
            {
                break;
            }
        }
        
    }
    return 1;
    
}

void handleCalibration(void)
{
    int xdata start = 0, loadAdcRead, breakOut = 0;
    float xdata weight, temp;
    while (1)
    {
        setBlankHex(5);
        displayArray[0] = getHexFromAlphabet('I');
        displayArray[1] = getHexFromAlphabet('L');
        displayArray[2] = getHexFromAlphabet('A');
        displayArray[3] = getHexFromAlphabet('C');
        TM1640_U_display(displayArray);
        if (start == 0)
        {
            setBlankHex(5);
            TM1640_M_display(displayArray);
            while (1)
            {
                scannedKey = scan_keypad();
                Delay_Some_Time(5);
                if (scannedKey != 'A' && scannedKey == 16)
                {
                    start = 1;
                    setBlankHex(5);
                    displayArray[0] = getHexFromAlphabet('o');
                    displayArray[0] = getHexFromAlphabet('-');
                    displayArray[2] = getHexFromAlphabet('L');
                    displayArray[3] = getHexFromAlphabet('A');
                    displayArray[4] = getHexFromAlphabet('C');
                    TM1640_M_display(displayArray);
                    break;
                }
            }
            
        }else if (start == 1)
        {
            while (1)
            {
                adcRead = (unsigned int) readCount();
                displayOutput = getNumberDisplayFloat(adcRead, 5, 0);
                TM1640_M_display(displayOutput);
                scannedKey = scan_keypad();
                Delay_Some_Time(5);
                if (scannedKey != 'A' && scannedKey == 16)
                {
                    start = 2;
                    setBlankHex(5);
                    displayArray[0] = getHexFromAlphabet('d');
                    displayArray[1] = getHexFromAlphabet('A');
                    displayArray[2] = getHexFromAlphabet('O');
                    displayArray[3] = getHexFromAlphabet('L');
                    TM1640_M_display(displayArray);
                    setOffsetWeight(1);
                    break;
                }
            }
        }else if (start == 2)
        {
            
            while (1)
            {
                scannedKey = scan_keypad();
                Delay_Some_Time(5);
                if (scannedKey != 'A' && scannedKey == 16)
                {
                    weight = getSettingWeight();
                    frameNameAdded = 0;
                    start = 3;
                    break;
                }
            }
        }else if (start == 3)
        {
            currentCapacity = weight;
            while (1)
            {
                scannedKey = scan_keypad();
                if (scannedKey != 'A')
                {
                    if (scannedKey == 14)
                    {
                        switch (frameNameAdded - weightPrecision)
                        {
                        case -3:
                            temp = 0.001;
                            break;
                        case -2:
                            temp = 0.01;
                            break;
                        case -1:
                            temp = 0.1;
                            break;
                        case 0:
                            temp = 1;
                            break;
                        case 1:
                            temp = 1;
                            break;
                        case 2:
                            temp = 10;
                            break;
                        default:
                            break;
                        }
                        if (currentCapacity + temp < 99)
                        {
                            currentCapacity += temp;
                        }
                        
                    }else if (scannedKey == 15)
                    {
                        switch (frameNameAdded - weightPrecision)
                        {
                        case -3:
                            temp = 0.001;
                            break;
                        case -2:
                            temp = 0.01;
                            break;
                        case -1:
                            temp = 0.1;
                            break;
                        case 0:
                            temp = 1;
                            break;
                        case 1:
                            temp = 1;
                            break;
                        case 2:
                            temp = 10;
                            break;
                        default:
                            break;
                        }
                        if (currentCapacity - temp >= 0)
                        {
                            currentCapacity -= temp;
                        }
                    }else if (scannedKey == 11)
                    {
                        setCurrentCapacity(0);
                        frameNameAdded = 0;
                    }else if (scannedKey == 16)
                    {
                        start = 4;
                        setBlankHex(5);
                        displayArray[0] = getHexFromAlphabet('F');
                        displayArray[1] = getHexFromAlphabet('-');
                        displayArray[2] = getHexFromAlphabet('L');
                        displayArray[3] = getHexFromAlphabet('A');
                        displayArray[4] = getHexFromAlphabet('C');
                        TM1640_M_display(displayArray);
                        break;
                    }       
                }
                displayOutput = getNumberDisplayFloat(currentCapacity, 5, weightPrecision);
                TM1640_M_display(displayOutput);
            }
            
            
        }else if (start == 4)
        {
            while(1)
            {
                adcRead = (unsigned int) readCount();
                displayOutput = getNumberDisplayFloat(adcRead, 5, 0);
                TM1640_M_display(displayOutput);
                scannedKey = scan_keypad();
                if (scannedKey != 'A' && scannedKey == 16)
                {
                    start = 5;
                    randomFloat = adcRead - getOffsetCount();
                    weightMultiplier = ((int)(currentCapacity * 1000)) / randomFloat;
                    weightMultiplier = round(weightMultiplier, 3);
                    saveWeightCalibration(weightMultiplier);
                    setBlankHex(5);
                    displayArray[0] = getHexFromAlphabet('E');
                    displayArray[1] = getHexFromAlphabet('n');
                    displayArray[2] = getHexFromAlphabet('O');
                    displayArray[3] = getHexFromAlphabet('d');
                    TM1640_M_display(displayArray);
                    break;
                }
            }
        }
        if (start == 5)
        {
            while(1)
            {
                scannedKey = scan_keypad();
                if (scannedKey != 'A' && scannedKey == 16)
                {
                    start = 6;
                    breakOut = 1;
                    break;
                }
            }
        }
        if (breakOut == 1)
        {
            break;
        }
        
    }
}

void handleResolutionAndCapacityInput(int isSetting)
{
    float temp;
    if (isSetting == 0)
    {
        if (isResolution == 1)
        {
            if (scannedKey == 14 && frameNameAdded < 8){
                frameNameAdded += 1;
            }else if (scannedKey == 14)
            {
                frameNameAdded = 0;
            }else if (scannedKey == 16)
            {
                // here we are using cursor as step ex: ld 1, ld 2 and ld 3
                resolutionCapacityArray[2*cursor + 1] = (float)resolutionArray[frameNameAdded];
                isResolution = 0;
                frameNameAdded = 0;
                setCurrentCapacity(cursor+1);
            }
        }
        else
        {
            // here we are using frameNameAdded for position of shift
            /* code */
            if (scannedKey == 14)
            {
                switch (frameNameAdded - weightPrecision)
                {
                case -3:
                    temp = 0.001;
                    break;
                case -2:
                    temp = 0.01;
                    break;
                case -1:
                    temp = 0.1;
                    break;
                case 0:
                    temp = 1;
                    break;
                case 1:
                    temp = 1;
                    break;
                case 2:
                    temp = 10;
                    break;
                default:
                    break;
                }
                if (currentCapacity + temp < 99)
                {
                    currentCapacity += temp;
                }
                
            }else if (scannedKey == 15)
            {
                if (frameNameAdded == capacityLength)
                {
                    frameNameAdded = 0;
                }else if (frameNameAdded + 1 == weightPrecision)
                {
                    frameNameAdded += 2;
                }else{
                    frameNameAdded += 1;
                }
            }else if (scannedKey == 11)
            {
                setCurrentCapacity(0);
                frameNameAdded = 0;
            }else if (scannedKey == 16)
            {
                resolutionCapacityArray[2*cursor + 2] = currentCapacity;
                if (cursor + 1 == resolutionCapacityArray[0])
                {
                    saveCapacityAndResolution(resolutionCapacityArray);
                    step = 13;
                    handleCalibration();
                    return;
                }else
                {
                    cursor += 1;
                    if (cursor == 1)
                    {
                        frameNameAdded = 1;
                    }else
                    {
                        frameNameAdded = 2;
                    }
                    isResolution = 1;
                }
            }
        }
    }else
    {
        frameNameAdded = 0;
        cursor = 0;
        capacityLength = 2 + weightPrecision;
    }
    setBlankHex(5);
    if (isResolution == 1)
    {
        displayArray[0] = numbersHex[cursor+1];
        displayArray[1] = getHexFromAlphabet('-');
        displayArray[2] = getHexFromAlphabet('d');
        displayArray[3] = getHexFromAlphabet('L');
    }
    else
    {
        displayArray[0] = numbersHex[cursor+1];
        displayArray[1] = getHexFromAlphabet('-');
        displayArray[2] = getHexFromAlphabet('P');
        displayArray[3] = getHexFromAlphabet('A');
        displayArray[4] = getHexFromAlphabet('C');
    }
    TM1640_U_display(displayArray);
    
    if (isResolution == 1)
    {
        displayOutput = getNumberDisplayFloat(resolutionArray[frameNameAdded], 5, 0);
    }else
    {
        displayOutput = getNumberDisplayFloat(currentCapacity, 5, weightPrecision);
    }
    TM1640_M_display(displayOutput);
}

void setCurrentCapacity(int m)
{
    currentCapacity = m * 10;
}

void handleResolutionTimes(int isSetting)
{
    if (isSetting == 0)
    {
        if (scannedKey == 14)
        {
            if (resolutionSteps > 2)
            {
                resolutionSteps = 1;
            }else
            {
                resolutionSteps += 1;
            }
        }else if (scannedKey == 16)
        {
            step = 12;
            resolutionCapacityArray[0] = resolutionSteps;
            handleResolutionAndCapacityInput(1);
            return;
        }
    }
    setBlankHex(5);
    displayArray[0] = getHexFromAlphabet('P');
    displayArray[1] = getHexFromAlphabet('E');
    displayArray[2] = getHexFromAlphabet('t');
    displayArray[3] = getHexFromAlphabet('S');
    TM1640_U_display(displayArray);
    setBlankHex(5);
    displayArray[0] = numbersHex[resolutionSteps];
    displayArray[1] = getHexFromAlphabet('-');
    displayArray[2] = getHexFromAlphabet('S');
    TM1640_M_display(displayArray);
}

void handlePrecisionSetting(int isSetting)
{
    if (isSetting == 0)
    {
        if (scannedKey == 15)
        {
            if (decimalPrecision == 3)
            {
                decimalPrecision = 0;
            }else
            {
                decimalPrecision += 1;
            }
        }else if (scannedKey == 16)
        {
            savePrecision(decimalPrecision);
            step = 11;
            handleResolutionTimes(1);
            return;
        }
    }
    setBlankHex(5);
    displayArray[0] = getHexFromAlphabet('P');
    displayArray[1] = getHexFromAlphabet('d');
    TM1640_U_display(displayArray);
    displayOutput = getNumberDisplayFloat(0.0, 5, decimalPrecision);
    if (decimalPrecision == 0)
    {
        displayOutput[0] = displayOutput[0] | 0x02;
    }
    TM1640_M_display(displayOutput);
}

void handleAutoZeroTracking(int isSetting)
{
    if (isSetting == 0)
    {
        if (scannedKey == 14)
        {
            if (autoZero > 9)
            {
                autoZero = 1.0;
            }else
            {
                autoZero += 0.5;
            }
        }else if (scannedKey == 16)
        {
            saveAutoZeroTracking(autoZero);
            step = 10;
            handlePrecisionSetting(1);
            return;
        }
    }
    setBlankHex(5);
    displayArray[0] = getHexFromAlphabet('O');
    displayArray[1] = getHexFromAlphabet('o');
    displayArray[2] = getHexFromAlphabet('t');
    displayArray[3] = getHexFromAlphabet('U');
    displayArray[4] = getHexFromAlphabet('A');
    TM1640_U_display(displayArray);
    displayOutput = getNumberDisplayFloat(autoZero, 5, 1);
    displayOutput[2] = displayOutput[1];
    displayOutput[1] = displayOutput[0];
    displayOutput[0] = getHexFromAlphabet('d') | 0x02;
    TM1640_M_display(displayOutput);
}

void handleDisplaySingleZero(int isSetting)
{
    if (isSetting == 0)
    {
        if (scannedKey == 14)
        {
            displayZero = displayZero == 1 ? 0 : 1;
        }
        else if (scannedKey == 16)
        {
            saveDisplayZeroFlag(displayZero);
            handleAutoZeroTracking(1);
            step = 9;
            return;
        }
    }
    setBlankHex(5);
    displayArray[0] = getHexFromAlphabet('O');
    displayArray[1] = getHexFromAlphabet('-');
    displayArray[2] = getHexFromAlphabet('P');
    displayArray[3] = getHexFromAlphabet('S');
    displayArray[4] = getHexFromAlphabet('d');
    TM1640_U_display(displayArray);
    setBlankHex(5);
    if (displayZero == 1)
    {
        displayArray[0] = getHexFromAlphabet('S');
        displayArray[1] = getHexFromAlphabet('E');
        displayArray[2] = getHexFromAlphabet('y');
    }else
    {
        displayArray[0] = getHexFromAlphabet('O');
        displayArray[1] = getHexFromAlphabet('n');
    }
    
    TM1640_M_display(displayArray);
}

void handlePowerOffOn(int isSetting)
{
    if (isSetting == 0)
    {
        if (scannedKey == 14)
        {
            powerOff = powerOff == 1 ? 0 : 1;
        }
        else if (scannedKey == 16)
        {
            savePowerOffFlag(powerOff);
            step = 8;
            handleDisplaySingleZero(1);
            return;
        }
    }
    setBlankHex(5);
    displayArray[0] = getHexFromAlphabet('F');
    displayArray[1] = getHexFromAlphabet('F');
    displayArray[2] = getHexFromAlphabet('O');
    displayArray[3] = getHexFromAlphabet('-');
    displayArray[4] = getHexFromAlphabet('P');
    TM1640_U_display(displayArray);
    setBlankHex(5);
    if (powerOff == 1)
    {
        displayArray[0] = getHexFromAlphabet('S');
        displayArray[1] = getHexFromAlphabet('E');
        displayArray[2] = getHexFromAlphabet('y');
    }else
    {
        displayArray[0] = getHexFromAlphabet('O');
        displayArray[1] = getHexFromAlphabet('n');
    }
    
    TM1640_M_display(displayArray);
}

void handleBuzzerOnOff(int isSetting)
{
    if (isSetting == 0)
    {
        if (scannedKey == 14)
        {
            stableBuzzer = stableBuzzer == 1 ? 0 : 1;
        }
        else if (scannedKey == 16)
        {
            saveBuzzerFlag(stableBuzzer);
            step = 7;
            handlePowerOffOn(1);
            return;
        }
    }
    setBlankHex(6);
    TM1640_L_display(displayArray);
    setBlankHex(5);
    displayArray[0] = getHexFromAlphabet('Z');
    displayArray[1] = getHexFromAlphabet('U');
    displayArray[2] = getHexFromAlphabet('b');
    displayArray[3] = getHexFromAlphabet('t');
    displayArray[4] = getHexFromAlphabet('S');
    TM1640_U_display(displayArray);
    setBlankHex(5);
    if (stableBuzzer == 1)
    {
        displayArray[0] = getHexFromAlphabet('S');
        displayArray[1] = getHexFromAlphabet('E');
        displayArray[2] = getHexFromAlphabet('y');
    }else
    {
        displayArray[0] = getHexFromAlphabet('O');
        displayArray[1] = getHexFromAlphabet('n');
    }
    
    TM1640_M_display(displayArray);
}

void handleCompanyNameInput(int isSetting)
{
    int i;
    if (isSetting == 0)
    {
        if (scannedKey == 14)
        {
            if (cursor == strlen(alphabetHex) - 1)
            {
                cursor = 0;
            }else
            {
                cursor += 1;
            }
            
        }
        else if (scannedKey == 15)
        {
            if (frameNameAdded < frameName[0])
            {
                frameNameAdded += 1;
                frameName[frameNameAdded] = alphabetHex[cursor];
                cursor = 0;
            }
            if (frameName[0] == frameNameAdded)
            {
                saveCompanyName(frameName);
                step = 6;
                handleBuzzerOnOff(1);
                return;
            }
            
        }
        else if (scannedKey == 16 && (frameName[0] == frameNameAdded + 1 || frameName[0] == frameNameAdded))
        {
            if (frameName[0] == frameNameAdded + 1)
            {
                frameNameAdded += 1;
            }
            
            frameName[frameNameAdded] = alphabetHex[cursor];
            saveCompanyName(frameName);
            step = 6;
            handleBuzzerOnOff(1);
            return;
        }else if (scannedKey == 11)
        {
            frameNameAdded = 0;
            cursor = 0;
            setBlankHex(5);
            TM1640_U_display(displayArray);
            setBlankHex(6);
            TM1640_L_display(displayArray);
        }
    }else
    {
        frameNameAdded = 0;
        cursor = 0;
        setBlankHex(5);
        TM1640_U_display(displayArray);
        setBlankHex(6);
        TM1640_L_display(displayArray);
    }
    i = 1;
    setBlankHex(5);
    displayArray[0] = alphabetHex[cursor];
    for(k=frameNameAdded; k > 0 && k > frameNameAdded - 4; k--)
    {
        displayArray[i] = frameName[k];
        i++;
    }
    TM1640_M_display(displayArray);
    if (frameNameAdded > 4)
    {
        setBlankHex(5);
        i = 0;
        for(k = frameNameAdded - 4; k > 0 && k >= frameNameAdded - 8; k--)
        {
            displayArray[i] = frameName[k];
            i++;
        }
        TM1640_U_display(displayArray);
    }
    
    
}

void handleCompanyNameLength(int isSetting)
{
    if (isSetting == 0)
    {
        if (scannedKey == 14 && frameName[0] < 20)
        {
            frameName[0] += 1;
        }
        else if (scannedKey == 15 && frameName[0] > 0)
        {
            frameName[0] -= 1;
        }
        else if (scannedKey == 11)
        {
            frameName[0] = 0;
        }
        else if (scannedKey == 16)
        {
            if (frameName[0] > 0)
            {
                step = 5;
                handleCompanyNameInput(1);
                return;
            }else
            {
                step = 6;
                handleBuzzerOnOff(1);
                return;
            }
        }
        
    }
    setBlankHex(6);
    displayArray[0] = getHexFromAlphabet('H');
    displayArray[1] = getHexFromAlphabet('t');
    displayArray[2] = getHexFromAlphabet('n');
    displayArray[3] = getHexFromAlphabet('E');
    displayArray[4] = getHexFromAlphabet('L');
    TM1640_L_display(displayArray);
    displayOutput = getNumberDisplayFloat(frameName[0], 5, 0);
    TM1640_M_display(displayOutput);
}

void handleFrameConfirmation(int isSetting)
{
    if (isSetting == 0)
    {
        if (scannedKey == 14)
        {
            setCompanyName = setCompanyName == 1 ? 0 : 1;
        }
        else if (scannedKey == 16)
        {
            if (setCompanyName == 1)
            {
                step = 4;
                handleCompanyNameLength(1);
                return;
            }else
            {
                step = 6;
                handleBuzzerOnOff(1);
                return;
            }
            
        }
    }
    if (isSetting == 1)
    {
        setBlankHex(6);
        TM1640_L_display(displayArray);
    }
    setBlankHex(5);
    displayArray[0] = getHexFromAlphabet('E');
    displayArray[1] = getHexFromAlphabet('m');
    displayArray[2] = getHexFromAlphabet('A');
    displayArray[3] = getHexFromAlphabet('r');
    displayArray[4] = getHexFromAlphabet('F');
    TM1640_U_display(displayArray);
    setBlankHex(5);
    if (setCompanyName == 1)
    {
        displayArray[0] = getHexFromAlphabet('S');
        displayArray[1] = getHexFromAlphabet('E');
        displayArray[2] = getHexFromAlphabet('y');
    }else
    {
        displayArray[0] = getHexFromAlphabet('O');
        displayArray[1] = getHexFromAlphabet('n');
    }
    TM1640_M_display(displayArray);
}

void handleChangePassword(int isSetting)
{
    if (isSetting == 0)
    {
        if (scannedKey == 14)
        {
            if (frameNameAdded == 9)
            {
                frameNameAdded = 0;
            }else
            {
                frameNameAdded += 1;
            }
            newPass[cursor] = numbersHex[frameNameAdded];
        }
        else if (scannedKey == 15)
        {
            if (cursor < 3)
            {
                cursor += 1;
                frameNameAdded = 0;
            }
        }
        else if (scannedKey == 11)
        {
            cursor = 0;
            frameNameAdded = 0;
            newPass[0] = 0xed;
            newPass[1] = 0xed;
            newPass[2] = 0xed;
            newPass[3] = 0xed;
        }else if (scannedKey < 10)
        {
            if (cursor < 4)
            {
                newPass[cursor] = numbersHex[scannedKey];
                cursor += 1;
                frameNameAdded = 0;
            }
        }else if (scannedKey == 16)
        {
            savePassword(newPass);
            step = 3;
            handleFrameConfirmation(1);
            return;
        }
        
    }else
    {
        cursor = 0;
        frameNameAdded = 0;
        newPass[0] = 0xed;
        newPass[1] = 0xed;
        newPass[2] = 0xed;
        newPass[3] = 0xed;
    }
    setBlankHex(5);
    displayArray[0] = getHexFromAlphabet('g');
    displayArray[1] = getHexFromAlphabet('n');
    displayArray[2] = getHexFromAlphabet('A');
    displayArray[3] = getHexFromAlphabet('H');
    displayArray[4] = getHexFromAlphabet('C');
    TM1640_U_display(displayArray);

    setBlankHex(5);
    displayArray[2] = getHexFromAlphabet('S');
    displayArray[3] = getHexFromAlphabet('A');
    displayArray[4] = getHexFromAlphabet('P');
    TM1640_M_display(displayArray);
    setBlankHex(6);
    displayArray[3] = newPass[0];
    displayArray[2] = newPass[1];
    displayArray[1] = newPass[2];
    displayArray[0] = newPass[3];
    TM1640_L_display(displayArray);
}

void handlePasswordConfirmation(int isSetting)
{
    if (isSetting == 0)
    {
        if (scannedKey == 14)
        {
            setPassword = setPassword == 1 ? 0 : 1;
        }
        else if (scannedKey == 16)
        {
            if (setPassword == 1)
            {
                step = 2;
                handleChangePassword(1);
                return;
            }else
            {
                step = 3;
                handleFrameConfirmation(1);
                return;
            }
            
        }
        
    }
    setBlankHex(5);
    displayArray[0] = getHexFromAlphabet('g');
    displayArray[1] = getHexFromAlphabet('n');
    displayArray[2] = getHexFromAlphabet('A');
    displayArray[3] = getHexFromAlphabet('H');
    displayArray[4] = getHexFromAlphabet('C');
    TM1640_U_display(displayArray);

    setBlankHex(5);
    displayArray[2] = getHexFromAlphabet('S');
    displayArray[3] = getHexFromAlphabet('A');
    displayArray[4] = getHexFromAlphabet('P');
    TM1640_M_display(displayArray);
    setBlankHex(6);
    if (setPassword == 1)
    {
        displayArray[0] = getHexFromAlphabet('S');
        displayArray[1] = getHexFromAlphabet('E');
        displayArray[2] = getHexFromAlphabet('y');
    }else
    {
        displayArray[0] = getHexFromAlphabet('O');
        displayArray[1] = getHexFromAlphabet('n');
    }
    
    TM1640_L_display(displayArray);
}

void handlePassword()
{
    int i;
    unsigned char* password = loadPassword();
    if (scannedKey == 14 || scannedKey == 15 || scannedKey == 16)
    {
        switch (scannedKey)
        {
        case 14:
            if (cursor == -1)
            {
                cursor = 0;
            }
            if (currentPassword[cursor] > 8)
            {
                currentPassword[cursor] = 0;
            }else
            {
                currentPassword[cursor] += 1;
            }
            break;
        case 15:
            if (cursor == -1)
            {
                cursor = 1;
            }else if (cursor == 3)
            {
                cursor = 0;
            }else
            {
                cursor += 1;
            }
            break;
        case 16:
            if (
                numbersHex[currentPassword[0]] == password[0] &&
                numbersHex[currentPassword[1]] == password[1] &&
                numbersHex[currentPassword[2]] == password[2] &&
                numbersHex[currentPassword[3]] == password[3]
                )
            {
                step = 1;
                handlePasswordConfirmation(1);
                return;
            }else
            {
                resetCurrentPassword();
            }
            break;
        }
    }
    setBlankHex(6);
    // displayArray[0] = password[3];
    // displayArray[1] = password[2];
    // displayArray[2] = password[1];
    // displayArray[3] = password[0];
    // TM1640_L_display(displayArray);
    setBlankHex(5);
    displayArray[1] = getHexFromAlphabet('S');
    displayArray[2] = getHexFromAlphabet('S');
    displayArray[3] = getHexFromAlphabet('A');
    displayArray[4] = getHexFromAlphabet('P');
    TM1640_U_display(displayArray);
    setBlankHex(5);
    for (k = 0; k < 4; k++)
    {
        i = 3 - k;
        displayArray[k] = numbersHex[currentPassword[i]];
    }
    TM1640_M_display(displayArray);
    setBlankHex(6);
    TM1640_L_display(displayArray);
    
}

void resetCurrentPassword()
{
    for (k = 0; k < 4; k++)
    {
        currentPassword[k] = 0;
    }
    cursor = 0;
}

void setBlankHex(int i)
{
    for(i=0;i < 5; i++)
    {
        displayArray[i] = BLANK_HEX;
    }
    displayArray[i] = '\0';
}