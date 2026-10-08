#include "power_manager.h"
#include "logger.h"

namespace {
PowerMode g_powerMode = PowerMode::NORMAL;
}

void powerManagerBegin()
{
    g_powerMode = PowerMode::NORMAL;
    logInfo("POWER_MODE=NORMAL");
}

void setPowerMode(PowerMode mode)
{
    if (mode == g_powerMode)
    {
        return;
    }

    g_powerMode = mode;

    if (mode == PowerMode::LOW_POWER)
    {
        logWarn("POWER_MODE=LOW_POWER");
    }
    else
    {
        logInfo("POWER_MODE=NORMAL");
    }
}

PowerMode getPowerMode()
{
    return g_powerMode;
}

const char* powerModeToString(PowerMode mode)
{
    switch (mode)
    {
        case PowerMode::NORMAL:
            return "NORMAL";
        case PowerMode::LOW_POWER:
            return "LOW_POWER";
        default:
            return "UNKNOWN";
    }
}
