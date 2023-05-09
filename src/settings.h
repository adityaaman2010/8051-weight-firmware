#ifndef SETTINGS
#define SETTINGS

void handlePassword(void);
void handleFrameConfirmation(int);
void setBlankHex(int);
void resetCurrentPassword(void);
void handleCompanyNameLength(int);
void handleBuzzerOnOff(int);
void handleCompanyNameInput(int);
void handlePowerOffOn(int);
void handleDisplaySingleZero(int);
void handleChangePassword(int);
void handlePasswordConfirmation(int);
int handleSettings(void);
void handleAutoZeroTracking(int);
void handlePrecisionSetting(int);
void handleResolutionTimes(int);
void handleResolutionAndCapacityInput(int);
void handleCalibration(void);
void setCurrentCapacity(int);
unsigned int debugMehtod(void);

#endif // SETTINGS
