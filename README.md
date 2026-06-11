# IoT Health Monitoring System

## Overview

Portable health monitoring system using ESP32.

Measures: 

- Heart Rate
- SpO2
- Temperature

Displays readings on OLED and sends them to Blynk for GUI based connectivity. 

## Hardware

- ESP32 ( Microcontroller )
- MAX30100 ( Heartrate Sensor )
- DHT11 ( Thermal Sensor )
- OLED SSD1306 ( Display Screen )

## Software

- Arduino IDE
- Blynk
- Adafruit SSD1306 Library

## Features

Real-time monitoring
Portable
Low cost
Offline operation
Optional IoT monitoring

## Results

| Parameter          | Reference Device | Proposed Device | Deviation |
|--------------------|----------------:|----------------:|----------:|
| Heart Rate (BPM)   | 72              | 71              | -1        |
| SpO₂ (%)           | 97              | 96              | -1        |
| Temperature (°C)   | 36.8            | 36.8            | 0         |

## Future Scope

- ECG
- Blood Pressure
- Cloud Storage
- AI Alerts
