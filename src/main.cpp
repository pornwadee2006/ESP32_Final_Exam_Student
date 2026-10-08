#include <Arduino.h>

#include "app.h"
#include "config.h"
#include "logger.h"
#include "network.h"

void setup()
{
    loggerBegin(SERIAL_BAUD_RATE);

    logInfo("SYSTEM_BOOT");
    logInfo("DEVICE_ID=" + String(DEVICE_ID));
    logInfo("FIRMWARE=" + String(FIRMWARE_VERSION));

    networkBegin();
    appBegin();
}

void loop()
{
    appLoop();
    delay(1);
}
