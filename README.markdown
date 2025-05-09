# Heater Control System 

## Overview
This repository contains the implementation of a **Basic Heater Control System**. The system simulates a temperature-based heater control using an ESP32 microcontroller, a DHT22 temperature sensor, an LCD display, and an LED to indicate heater status. The project is developed and simulated on the **Wokwi** platform using **C+** with the **Arduino framework** and **FreeRTOS** for task management.

The system monitors temperature and transitions between states (`Idle`, `Heating`, `Stabilizing`, `Target Reached`, `Stable`, `Overheat`) based on predefined thresholds, updating the heater status (simulated by an LED) and displaying information on a 16x2 LCD.

## Objectives

- Simulate a basic embedded heater control system using temperature-based thresholds.
- Manage system states through a deterministic state machine.
- Display real-time temperature and system state on an LCD.
- Provide serial and visual feedback of the system's behavior.
- Demonstrate the use of FreeRTOS with proper synchronization across cores.

## Hardware & Connections

| Component   | ESP32 Pin |
|-------------|-----------|
| DHT22       | GPIO 21   |
| LCD SDA     | GPIO 22   |
| LCD SCL     | GPIO 23   |
| LED (Heater)| GPIO 2    |

---

## System Features

| Feature                      | Description                                                                 |
|------------------------------|-----------------------------------------------------------------------------|
| Temperature Monitoring       | Reads temperature data using a DHT22 sensor every 1 second                  |
| State Machine Control        | Manages six operating states: `IDLE`, `HEATING`, `STABILIZING`, `TARGET_REACHED`, `STABLE`, `OVERHEAT` |
| Visual Feedback              | LED toggles ON during `HEATING` and `STABILIZING` states                   |
| LCD Display                  | Real-time temperature and state output on a 16x2 I2C LCD                   |
| FreeRTOS-Based Multithreading| Sensor logic and UI updates run as independent tasks on separate ESP32 cores |
| Thread-Safe Operation        | Uses a mutex to protect shared temperature and state data                  |
| Serial Logging               | Logs temperature and current system state for debugging and monitoring     |

---

## State Definitions

| State           | Description                                                                 |
|-----------------|-----------------------------------------------------------------------------|
| `IDLE`          | System is idle; awaiting heating trigger                                    |
| `HEATING`       | Heater is active; temperature is below defined threshold                   |
| `STABILIZING`   | Temperature is within a narrow range; system waits for it to stabilize     |
| `TARGET_REACHED`| Target temperature range has been achieved and maintained for a fixed time |
| `STABLE`        | Temperature remains in acceptable range without heating                    |
| `OVERHEAT`      | Temperature exceeds safety threshold; system triggers warning              |

---

## Threshold Configuration

| Parameter                 | Value     |
|---------------------------|-----------|
| Heating Threshold         | < 29.5 °C |
| Stabilizing Range (min)   | 29.5 °C   |
| Stabilizing Range (max)   | 30.5 °C   |
| Overheat Threshold        | ≥ 35.0 °C |
| Stabilization Duration    | 5 seconds |

---

## System Architecture

### Block Diagram
![alt text](image.png)

## Hardware Requirements (Simulated in Wokwi)
- **ESP32**: Microcontroller for running the system.
- **DHT22**: Temperature and humidity sensor for temperature readings.
- **16x2 I2C LCD**: Displays temperature and system state.
- **LED**: Simulates the heater (ON/OFF).
- **Resistor**: 220Ω for the LED circuit.
- **Connections**:
  - DHT22: Data pin to GPIO 21, VCC to 3.3V, GND to GND.
  - LCD: I2C SDA to GPIO 22, SCL to GPIO 23, VCC to 5V, GND to GND.
  - LED: Anode to GPIO 2 via 220Ω resistor, cathode to GND.

## Software Requirements
- **Platform**: Wokwi (online simulation platform).
- **Language**: C++ (Arduino framework).
- **Libraries**:
  - `LiquidCrystal_I2C`: For controlling the I2C LCD.
  - `DHTesp`: For interfacing with the DHT22 sensor.
  - `freertos`: For task management and mutex operations.
- **Arduino IDE**: For code compilation (or Wokwi's built-in editor).

## How to Run
1. **Access the Wokwi Simulation**:
   - Open the Wokwi simulation link: [Insert Wokwi Link Here].
   - Alternatively, create a new Wokwi project and configure the ESP32, DHT22, LCD, and LED as described in the hardware section.

2. **Upload the Code**:
   - Copy the contents of `src/main.cpp` into the Wokwi editor or Arduino IDE.
   - Ensure the required libraries (`LiquidCrystal_I2C`, `DHTesp`, `freertos`) are installed.

3. **Configure Wokwi**:
   - Add the ESP32, DHT22, 16x2 LCD, and LED to the Wokwi schematic.
   - Connect components as per the hardware requirements.
   - Update the `wokwi_simulation.toml` file (if provided) or manually set up the simulation.

4. **Run the Simulation**:
   - Start the Wokwi simulation to observe the system in action.
   - Monitor the LCD for temperature and state updates, the LED for heater status, and the Serial monitor for logs.

5. **Serial Monitor**:
   - Open the Serial monitor (115200 baud) to view temperature readings and state transitions.

## Design Document
The design document (`design_document.pdf`) includes:
- **Sensors**: Justification for using the DHT22 temperature sensor.
- **Communication Protocol**: I2C for LCD communication, GPIO for sensor and LED.
- **Block Diagram**: Illustrates the ESP32, DHT22, LCD, and LED connections.
- **Future Roadmap**:
  - Overheating protection with a buzzer alarm.
  - Multiple heating profiles via user input.
  - BLE integration for remote monitoring.

## Notes
- The **BLE advertising** bonus feature is not implemented due to time constraints but is outlined in the design document as a future enhancement.
- The system assumes a stable power supply and reliable sensor readings in the simulation environment.
- The `Overheat` state currently logs a message but can be extended with active cooling mechanisms.


## Acknowledgments
- **upliance.ai**: For providing this exciting opportunity to design and implement an embedded system.
- **Wokwi**: For the intuitive simulation platform.
- **Arduino & FreeRTOS Communities**: For robust libraries and documentation.