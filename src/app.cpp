#include "app.h"

#include <Arduino.h>
#include <ctype.h>
#include <string.h>

#include "config.h"
#include "logger.h"
#include "network.h"
#include "power_manager.h"
#include "system_health.h"

namespace {
char g_commandBuffer[96] = {};
size_t g_commandLength = 0;

uint32_t g_lastTelemetryMs = 0;
uint32_t g_lastHealthReportMs = 0;

bool g_cloudAvailable = true;

void normalizeCommand(char* command)
{
    if (command == nullptr)
    {
        return;
    }

    char* first = command;
    while (*first != '\0' &&
           isspace(static_cast<unsigned char>(*first)))
    {
        first++;
    }

    if (first != command)
    {
        memmove(command, first, strlen(first) + 1);
    }

    size_t length = strlen(command);
    while (length > 0 &&
           isspace(static_cast<unsigned char>(command[length - 1])))
    {
        command[--length] = '\0';
    }

    for (size_t i = 0; command[i] != '\0'; i++)
    {
        command[i] = static_cast<char>(
            tolower(static_cast<unsigned char>(command[i]))
        );
    }
}

void printHelp()
{
    Serial.println();
    Serial.println("Available commands:");
    Serial.println("  help");
    Serial.println("  version");
    Serial.println("  status");
    Serial.println("  cloud on");
    Serial.println("  cloud off");
    Serial.println("  health");
    Serial.println();
}

void printStatus()
{
    Serial.println();
    Serial.println("[STATUS]");

    Serial.println("wifi_state=" +
                   String(networkIsConnected() ? "CONNECTED" : "DISCONNECTED"));

    if (networkIsConnected())
    {
        Serial.println("ip_address=" + networkIpAddress());
        Serial.println("rssi_dbm=" + String(networkRssi()));
    }
    else
    {
        Serial.println("ip_address=0.0.0.0");
        Serial.println("rssi_dbm=NA");
    }

    Serial.println("disconnect_duration_ms=" +
                   String(networkDisconnectedDurationMs()));
    Serial.println("reconnect_count=" +
                   String(networkReconnectCount()));
    Serial.println("cloud_available=" +
                   String(g_cloudAvailable ? "true" : "false"));
    Serial.println("power_mode=" +
                   String(powerModeToString(getPowerMode())));

    Serial.println();
}

void handleCommand(char* command)
{
    normalizeCommand(command);

    if (strlen(command) == 0)
    {
        return;
    }

    logInfo("COMMAND=" + String(command));

    if (strcmp(command, "help") == 0)
    {
        printHelp();
        return;
    }

    if (strcmp(command, "version") == 0)
    {
        Serial.println(String(FIRMWARE_NAME) + " v" +
                       String(FIRMWARE_VERSION));
        return;
    }

    if (strcmp(command, "status") == 0)
    {
        printStatus();
        return;
    }

    if (strcmp(command, "cloud on") == 0)
    {
        g_cloudAvailable = true;
        logInfo("CLOUD_SIMULATION=AVAILABLE");
        return;
    }

    if (strcmp(command, "cloud off") == 0)
    {
        g_cloudAvailable = false;
        logWarn("CLOUD_SIMULATION=UNAVAILABLE");
        return;
    }

   if (strcmp(command, "health") == 0)
    {
        Serial.println("HEALTH COMMAND RECEIVED");
        printSystemHealth();
        return;
    }

    logWarn("UNKNOWN_COMMAND");
}

void processSerialInput()
{
    while (Serial.available() > 0)
    {
        const char c = static_cast<char>(Serial.read());

        if (c == '\r' || c == '\n')
        {
            if (g_commandLength == 0)
            {
                continue;
            }

            g_commandBuffer[g_commandLength] = '\0';
            handleCommand(g_commandBuffer);
            g_commandLength = 0;
            g_commandBuffer[0] = '\0';
            continue;
        }

        if ((c == '\b' || c == 0x7F) && g_commandLength > 0)
        {
            g_commandLength--;
            continue;
        }

        if (c >= 32 && c <= 126 &&
            g_commandLength < sizeof(g_commandBuffer) - 1)
        {
            g_commandBuffer[g_commandLength++] = c;
        }
    }
}

bool publishTelemetry()
{
    if (!networkIsConnected())
    {
        logWarn("TELEMETRY_FAILED reason=wifi_disconnected");
        return false;
    }

    if (!g_cloudAvailable)
    {
        logWarn("TELEMETRY_FAILED reason=cloud_unavailable");
        return false;
    }

    const float simulatedTemperature =
        25.0f + static_cast<float>(random(0, 50)) / 10.0f;

    Serial.print("[TELEMETRY] device_id=");
    Serial.print(DEVICE_ID);
    Serial.print(" temperature_c=");
    Serial.print(simulatedTemperature, 1);
    Serial.print(" uptime_s=");
    Serial.println(millis() / 1000);

    return true;
}

void updateTelemetry()
{
    const uint32_t now = millis();

    if (now - g_lastTelemetryMs < TELEMETRY_INTERVAL_MS)
    {
        return;
    }

    g_lastTelemetryMs = now;

    const bool success = publishTelemetry();
    recordTelemetryResult(success);
}

void updateHealthReport()
{
    const uint32_t now = millis();

    if (now - g_lastHealthReportMs < HEALTH_REPORT_INTERVAL_MS)
    {
        return;
    }

    g_lastHealthReportMs = now;
    printSystemHealth();
}

void updatePowerPolicy()
{
    // =========================================================
    // PART B - TODO
    if (networkIsConnected())
    {
        setPowerMode(PowerMode::NORMAL);
        return;
    }

    if (networkDisconnectedDurationMs() >=
        LOW_POWER_AFTER_DISCONNECT_MS)
    {
        setPowerMode(PowerMode::LOW_POWER);
    }
}

}

void appBegin()
{
    systemHealthBegin();
    powerManagerBegin();

    logInfo("APPLICATION_READY");
    logInfo("Type 'help' for commands");

    g_lastTelemetryMs = millis();
    g_lastHealthReportMs = millis();
}

void appLoop()
{
    networkLoop();
    processSerialInput();
    updatePowerPolicy();
    updateTelemetry();
    updateHealthReport();
}
