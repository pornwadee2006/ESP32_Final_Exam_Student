#ifndef NETWORK_H
#define NETWORK_H

#include <Arduino.h>

void networkBegin();
void networkLoop();

bool networkIsConnected();
int32_t networkRssi();
String networkIpAddress();

uint32_t networkReconnectCount();
uint32_t networkDisconnectedDurationMs();

#endif
