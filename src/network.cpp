#include "network.h"

#include <WiFi.h>

#include "config.h"
#include "logger.h"

namespace {
bool g_previousConnected = false;
uint32_t g_reconnectCount = 0;
uint32_t g_lastReconnectAttemptMs = 0;
uint32_t g_disconnectStartMs = 0;
}

void networkBegin()
{
    WiFi.mode(WIFI_STA);
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

    logInfo("Wi-Fi connecting to SSID: " + String(WIFI_SSID));

    const uint32_t startMs = millis();

    while (WiFi.status() != WL_CONNECTED &&
           millis() - startMs < WIFI_CONNECT_TIMEOUT_MS)
    {
        delay(100);
    }

    g_previousConnected = (WiFi.status() == WL_CONNECTED);

    if (g_previousConnected)
    {
        logInfo("WIFI_CONNECTED");
        logInfo("IP=" + WiFi.localIP().toString());
        g_disconnectStartMs = 0;
    }
    else
    {
        logWarn("WIFI_INITIAL_CONNECT_TIMEOUT");
        g_disconnectStartMs = millis();
    }

    g_lastReconnectAttemptMs = millis();
}

void networkLoop()
{
    const uint32_t now = millis();
    const bool connected = (WiFi.status() == WL_CONNECTED);

    if (connected != g_previousConnected)
    {
        g_previousConnected = connected;

        if (connected)
        {
            logInfo("WIFI_RECOVERED");
            logInfo("IP=" + WiFi.localIP().toString());
            g_disconnectStartMs = 0;
        }
        else
        {
            logWarn("WIFI_DISCONNECTED");
            g_disconnectStartMs = now;
        }
    }

    if (!connected)
    {
        // g_disconnectStartMs = now;  ❌ BUG

        if (now - g_lastReconnectAttemptMs >= WIFI_RETRY_INTERVAL_MS)
        {
            g_lastReconnectAttemptMs = now;
            g_reconnectCount++;

            logWarn("WIFI_RECONNECT_ATTEMPT count=" +
                    String(g_reconnectCount));

            WiFi.disconnect();
            WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
        }
    }
}

bool networkIsConnected()
{
    return WiFi.status() == WL_CONNECTED;
}

int32_t networkRssi()
{
    return networkIsConnected() ? WiFi.RSSI() : -127;
}

String networkIpAddress()
{
    return networkIsConnected() ? WiFi.localIP().toString() : "0.0.0.0";
}

uint32_t networkReconnectCount()
{
    return g_reconnectCount;
}

uint32_t networkDisconnectedDurationMs()
{
    if (networkIsConnected() || g_disconnectStartMs == 0)
    {
        return 0;
    }

    return millis() - g_disconnectStartMs;
}
