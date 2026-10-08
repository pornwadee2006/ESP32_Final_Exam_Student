#include "system_health.h"

#include <Arduino.h>
#include <WiFi.h>

#include "config.h"
#include "network.h"
#include "power_manager.h"

namespace {
uint32_t g_telemetrySuccessCount = 0;
uint32_t g_telemetryFailureCount = 0;
uint32_t g_lastSuccessfulTelemetryMs = 0;
}

void systemHealthBegin()
{
    g_telemetrySuccessCount = 0;
    g_telemetryFailureCount = 0;
    g_lastSuccessfulTelemetryMs = 0;
}

void recordTelemetryResult(bool success)
{
    if (success)
    {
        g_telemetrySuccessCount++;
        g_lastSuccessfulTelemetryMs = millis();
    }
    else
    {
        g_telemetryFailureCount++;
    }
}

uint32_t telemetrySuccessCount()
{
    return g_telemetrySuccessCount;
}

uint32_t telemetryFailureCount()
{
    return g_telemetryFailureCount;
}

uint32_t lastSuccessfulTelemetryMs()
{
    return g_lastSuccessfulTelemetryMs;
}

void printSystemHealth()
{
    Serial.println();
    Serial.println("[HEALTH]");

    Serial.println("telemetry_success_count=" +
                   String(telemetrySuccessCount()));
    Serial.println("telemetry_failure_count=" +
                   String(telemetryFailureCount()));
    Serial.println("last_successful_telemetry_ms=" +
                   String(lastSuccessfulTelemetryMs()));

    // =========================================================
    // PART B
    // =========================================================

    // device_id / firmware_version  (ASSUMED: macro ใน config.h)
    Serial.println("device_id=" + String(DEVICE_ID));
    Serial.println("firmware_version=" + String(FIRMWARE_VERSION));

    // wifi_state
    Serial.println("wifi_state=" +
                   String(WiFi.status() == WL_CONNECTED ? "connected"
                                                         : "disconnected"));

    // rssi_dbm (ถ้าไม่ได้เชื่อมต่อ รายงาน 0)
    int rssi = (WiFi.status() == WL_CONNECTED) ? WiFi.RSSI() : 0;
    Serial.println("rssi_dbm=" + String(rssi));

    // uptime_s
    Serial.println("uptime_s=" + String(millis() / 1000UL));

    // free_heap
    Serial.println("free_heap=" + String(ESP.getFreeHeap()));

    // reconnect_count  (ASSUMED: ฟังก์ชันใน network.h)
    Serial.println("reconnect_count=" + String(networkReconnectCount()));

    // power_mode  (ASSUMED: ฟังก์ชันใน power_manager.h)
    Serial.println("power_mode=" + String(powerModeToString(getPowerMode())));

    Serial.println();
}