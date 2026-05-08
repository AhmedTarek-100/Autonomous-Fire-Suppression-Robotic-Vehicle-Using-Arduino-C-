# Autonomous & Bluetooth-Controlled Fire-Fighting Robot

This repository contains the Arduino source code for a smart, fire-fighting robot car. The robot operates in two distinct modes: it can be driven manually via a Bluetooth smartphone app, but it will automatically override manual controls to autonomously track, approach, and extinguish fires using an onboard water pump and a sweeping servo nozzle.

## 🚀 Features

*   **Dual-Mode Operation:** 100% manual control via Bluetooth, with an automatic override system when an emergency (fire) is detected.
*   **Autonomous Fire Tracking:** Utilizes three flame sensors (Left, Center, Right) to locate fire. The robot will autonomously pivot left or right to face the flames before stopping to extinguish them.
*   **Smart Extinguishing System:** Automatically triggers a relay to turn on a water pump when fire is detected.
*   **Sweeping Nozzle:** Uses a servo motor to sweep the water nozzle back and forth (between 60° and 120°) for a wider spray coverage area. Non-blocking logic ensures smooth movement.
*   **Variable Speed Control:** Supports PWM speed control from Bluetooth inputs mapping from speeds 100 to 255.

## 🛠️ Hardware Components

*   **Microcontroller:** Arduino Uno / Nano / Mega
*   **Motor Driver:** L298N (or similar) dual H-bridge motor driver
*   **Bluetooth Module:** HC-05 or HC-06
*   **Sensors:** 3x IR Flame Sensors (Digital/Analog)
*   **Actuators:**
    *   1x 5V Relay Module (Low-level trigger)
    *   1x Mini Water Pump
    *   1x Standard Servo Motor (for the nozzle)
    *   4x DC Gear Motors (for the chassis)
*   **Power Supply:** A high-current power source (e.g., 2S LiPo battery or 18650 cells) is highly recommended to handle the motors, servo, and pump simultaneously.

## 🔌 Pin Wiring & Connections

### Motor Driver (L298N)
| Arduino Pin | Motor Driver Pin | Function |
| :--- | :--- | :--- |
| `5` | `ENA` | Motor A Speed (PWM) |
| `6` | `ENB` | Motor B Speed (PWM) |
| `7` | `IN1` | Motor A Direction 1 |
| `8` | `IN2` | Motor A Direction 2 |
| `9` | `IN3` | Motor B Direction 1 |
| `4` | `IN4` | Motor B Direction 2 |

### Sensors & Actuators
| Arduino Pin | Component |
| :--- | :--- |
| `10 (RX)` | Bluetooth `TX` |
| `11 (TX)` | Bluetooth `RX` |
| `A3` | Left Flame Sensor (Analog Out) |
| `A4` | Center Flame Sensor (Analog Out) |
| `A5` | Right Flame Sensor (Analog Out) |
| `2` | Relay Module `IN` (Water Pump) |
| `3` | Servo Motor `Signal` |

## 📱 Bluetooth Controls

Connect to the HC-05/HC-06 module using a standard Arduino Bluetooth Controller app (e.g., "Arduino Bluetooth Car"). The code responds to the following character commands sent over serial at **9600 baud**:

**Directional Commands:**
*   `F`: Forward
*   `B`: Backward
*   `L`: Left
*   `R`: Right
*   `S`: Stop

**Speed Commands:**
*   `0` to `9`: Adjusts speed proportionally (100 to 242)
*   `q`: Maximum speed (255)

*Note: Bluetooth commands are temporarily ignored when the robot is actively fighting a fire to prevent the user from accidentally driving away from the emergency.*

## ⚙️ Installation & Usage

1.  Clone this repository or download the `.ino` file.
2.  Open the code in the [Arduino IDE](https://www.arduino.cc/en/software).
3.  Ensure you have the standard `Servo` and `SoftwareSerial` libraries installed (these are included by default in the Arduino IDE).
4.  **Important:** Disconnect the RX/TX pins or the Bluetooth module power before uploading the code to the Arduino to prevent serial interference.
5.  Select your Arduino board and COM port, then click **Upload**.
6.  Reconnect the Bluetooth module, pair it with your phone, and start driving!

## 🔧 Code Configuration (Tuning)

You can easily tweak the robot's sensitivity and behavior by modifying these global variables at the top of the code:

```cpp
// Lower value = less sensitive, Higher value = more sensitive. 
// Adjust based on your specific flame sensors and ambient lighting.
const int FIRE_THRESHOLD = 800; 

// Changes how fast the servo nozzle sweeps left and right.
int servoDirection = 2;
