#ifndef CONFIG_H
#define CONFIG_H

#include <Arduino.h>

constexpr const char* DEVICE_ID = "TEAMPONFAH";
constexpr const char* FIRMWARE_NAME = "wireless-node-final-exam";
constexpr const char* FIRMWARE_VERSION = "2.0.0";

constexpr const char* WIFI_SSID = "OPPO A92JR";
constexpr const char* WIFI_PASSWORD = "0000000000";

constexpr uint32_t SERIAL_BAUD_RATE = 115200;
constexpr uint32_t WIFI_CONNECT_TIMEOUT_MS = 10000;
constexpr uint32_t WIFI_RETRY_INTERVAL_MS = 5000;
constexpr uint32_t HEALTH_REPORT_INTERVAL_MS = 15000;
constexpr uint32_t TELEMETRY_INTERVAL_MS = 10000;
constexpr uint32_t LOW_POWER_AFTER_DISCONNECT_MS = 30000;

#endif
