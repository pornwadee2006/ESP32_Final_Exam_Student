#ifndef SYSTEM_HEALTH_H
#define SYSTEM_HEALTH_H

#include <Arduino.h>

void systemHealthBegin();
void recordTelemetryResult(bool success);

uint32_t telemetrySuccessCount();
uint32_t telemetryFailureCount();
uint32_t lastSuccessfulTelemetryMs();

void printSystemHealth();

#endif
