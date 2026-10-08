#include "logger.h"

namespace {
void logMessage(const char* level, const String& message)
{
    Serial.print("[");
    Serial.print(level);
    Serial.print("] ");
    Serial.println(message);
}
}

void loggerBegin(uint32_t baudRate)
{
    Serial.begin(baudRate);
    delay(250);
}

void logDebug(const String& message) { logMessage("DEBUG", message); }
void logInfo(const String& message)  { logMessage("INFO", message); }
void logWarn(const String& message)  { logMessage("WARN", message); }
void logError(const String& message) { logMessage("ERROR", message); }
