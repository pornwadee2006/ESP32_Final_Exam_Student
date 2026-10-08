#ifndef POWER_MANAGER_H
#define POWER_MANAGER_H

#include <Arduino.h>

enum class PowerMode
{
    NORMAL,
    LOW_POWER
};

void powerManagerBegin();
void setPowerMode(PowerMode mode);
PowerMode getPowerMode();
const char* powerModeToString(PowerMode mode);

#endif
