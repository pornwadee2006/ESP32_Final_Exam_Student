#ifndef LOGGER_H
#define LOGGER_H

#include <Arduino.h>

void loggerBegin(uint32_t baudRate);
void logDebug(const String& message);
void logInfo(const String& message);
void logWarn(const String& message);
void logError(const String& message);

#endif
