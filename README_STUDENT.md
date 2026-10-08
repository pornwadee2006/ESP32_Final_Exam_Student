# ESP32 Final Examination Starter Project

## Topics
- Power Management
- Software Development and Deployment on Wireless Nodes

## Rules

You must modify this existing project.

### Do NOT
- delete this project and create a new project,
- replace an entire supplied module with newly written code,
- rename required files or public functions,
- merge all modules into `main.cpp`.

### You MAY
- modify existing logic,
- complete the marked `PART B - TODO` sections,
- add small helper functions,
- add variables/constants when necessary,
- make minimal fixes to defective existing code.

A program that works but does not preserve the supplied architecture cannot receive full marks.

## Before Build
Edit `include/config.h`:
- replace `TEAMXX` with your assigned team ID,
- enter the examination Wi-Fi SSID and password.

## Existing Commands
- `help`
- `version`
- `status`
- `cloud on`
- `cloud off`

Part B requires you to add:
- `health`

## Part B
Search for `PART B - TODO`.

There are three required completion areas:
1. Complete `printSystemHealth()`.
2. Add the `health` command to the existing parser.
3. Implement the power-aware policy in `updatePowerPolicy()`.

## Part C
The same starter project contains **two unmarked software defects**:
- Fault A relates to **Power Management**.
- Fault B relates to **Software Development / Deployment Health**.

The defects are not marked with TODO or FAULT comments.

For each fault, record:
- Symptom
- Evidence
- File / Function
- Root Cause
- Minimal Fix
- Verification Test

## Useful Test Actions
- Turn the examination Wi-Fi access point OFF and ON.
- Use `status`.
- Use `health` after you implement it.
- Use `cloud off` and `cloud on` to simulate backend availability.

Do not assume a reported health value is correct merely because the program prints it.
