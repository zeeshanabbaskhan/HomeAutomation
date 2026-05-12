# 🏠 Smart Home Automation System using ESP32

<div align="center">

![ESP32](https://img.shields.io/badge/ESP32-IoT-blue)
![Arduino](https://img.shields.io/badge/Arduino-C%2B%2B-green)
![License](https://img.shields.io/badge/License-MIT-orange)
![Status](https://img.shields.io/badge/Project-Completed-success)

</div>

---

# 📌 Project Overview



This project is a complete IoT-based Smart Home Automation System developed using the ESP32 microcontroller.  
The system allows users to monitor and control home appliances and sensors through a modern web dashboard hosted directly on the ESP32.


The system allows users to:

- Control home appliances remotely
- Monitor temperature and humidity
- Automatically control fan speed
- Detect motion using PIR sensor
- Automatically control study table light
- Access a real-time dashboard through WiFi

The ESP32 acts as:

- Web Server
- WiFi Access Point
- Automation Controller
- Sensor Monitoring Unit

The entire web dashboard is hosted directly on the ESP32 itself.

---

# 🎯 Project Objectives

The main goals of this project are:

- Learn IoT system development
- Integrate sensors with ESP32
- Create a web-based control system
- Implement automation logic
- Understand asynchronous web servers
- Build real-time monitoring systems
- Develop smart home concepts

---

# 🚀 Features

# ✅ Real-Time Monitoring

- Live temperature updates
- Live humidity updates
- Motion detection status
- Distance detection system

---

# ✅ Smart Appliance Control

- Bedroom LED control
- Study Room LED control
- Living Room LED control
- Smart study table lighting
- Smart fan speed control

---

# ✅ Automation Features

- Automatic fan speed adjustment based on temperature
- PIR motion-based LED activation
- Ultrasonic distance-based table light control

---

# ✅ Web Dashboard

- Beautiful modern UI
- Real-time updates
- Mobile-friendly interface
- Fan control slider
- Dynamic sensor widgets

---

# 🧠 Concepts Used

This project includes concepts from:

| Domain | Concepts |
|---|---|
| IoT | Smart devices, automation |
| Embedded Systems | GPIO, sensors |
| Networking | WiFi, HTTP |
| Web Development | HTML, CSS, JavaScript |
| Electronics | LEDs, PWM, sensors |
| Programming | C++, async communication |

---

# 🛠 Technologies Used

| Technology | Purpose |
|---|---|
| C++ | ESP32 programming |
| HTML | Webpage structure |
| CSS | User interface design |
| JavaScript | Dynamic frontend |
| WiFi | Wireless communication |
| JSON | Data exchange |
| Async Web Server | Non-blocking communication |
| PWM | Fan speed control |

---

# 🔌 Hardware Components

| Component | Quantity |
|---|---|
| ESP32 | 1 |
| DHT11 Sensor | 1 |
| PIR Motion Sensor | 1 |
| Ultrasonic Sensor HC-SR04 | 1 |
| LEDs | Multiple |
| DC Fan / Motor | 1 |
| Motor Driver | 1 |
| Breadboard | 1 |
| Jumper Wires | Several |
| Power Supply | 1 |

---

# 📍 Pin Configuration

| Component | GPIO Pin |
|---|---|
| Bedroom LED | 13 |
| Study Room LED | 12 |
| Living Room LED | 14 |
| DHT11 Sensor | 27 |
| Ultrasonic TRIG | 21 |
| Ultrasonic ECHO | 19 |
| Study Table LED | 33 |
| PIR Motion Sensor | 26 |
| PIR Indicator LED | 32 |
| Fan PWM Pin | 18 |
| Fan Enable Pin | 5 |

---

# 🧱 Project Architecture

```text
                         ┌────────────────────┐
                         │    User Browser    │
                         └─────────┬──────────┘
                                   │
                              HTTP Requests
                                   │
                         ┌─────────▼──────────┐
                         │   ESP32 Web Server │
                         └─────────┬──────────┘
                                   │
      ┌────────────────────────────┼────────────────────────────┐
      │                            │                            │
      ▼                            ▼                            ▼
 Sensor Monitoring         Appliance Control             Automation Logic
      │                            │                            │
      ▼                            ▼                            ▼
 DHT11 / PIR /             LEDs / Fan Control           Smart Responses
 Ultrasonic Sensor
```

---

# ⚙️ Working Principle

# Step 1 — ESP32 Creates WiFi Network

The ESP32 creates its own wireless hotspot:

```cpp
WiFi.softAP("Smart Home", "12345678");
```

Users connect their phone or laptop directly to this WiFi network.

---

## WiFi Credentials

| Setting | Value |
|---|---|
| SSID | Smart Home |
| Password | 12345678 |

---

# Step 2 — ESP32 Starts Web Server

```cpp
AsyncWebServer server(80);
```

The ESP32 starts an HTTP server on port 80.

The dashboard becomes accessible through:

```text
http://192.168.4.1
```

---

# Step 3 — Web Dashboard Loads

The ESP32 sends a webpage containing:

- HTML
- CSS
- JavaScript
- Buttons
- Sensor widgets
- Fan slider

This webpage is stored directly inside ESP32 memory.

---

# 🌐 Web Dashboard

The dashboard includes:

## Temperature Widget
Displays real-time room temperature.

---

## Humidity Widget
Displays current humidity percentage.

---

## Bedroom Control
Allows user to:
- Toggle bedroom LED
- Monitor PIR motion LED

---

## Study Room Control
Allows user to:
- Toggle study room LED
- Monitor study table LED

---

## Living Room Control
Allows user to:
- Toggle living room LED
- Adjust fan speed

---

# 📡 Communication Flow

```text
Browser
   ↓
HTTP Request
   ↓
ESP32 Web Server
   ↓
GPIO Control / Sensor Reading
   ↓
JSON Response
   ↓
Dashboard Updates
```

---

# 🔥 Sensor Integration

# 🌡 DHT11 Temperature & Humidity Sensor

The DHT11 sensor measures:

- Temperature
- Humidity

## Code

```cpp
temperature = dht.readTemperature();
humidity = dht.readHumidity();
```

---

## Purpose

Used for:
- Environment monitoring
- Smart fan automation
- Real-time dashboard display

---

# 🚶 PIR Motion Sensor

The PIR sensor detects human movement.

## Working

If motion is detected:

```cpp
digitalWrite(BEDROOM_PIR_LED_PIN, HIGH);
```

Otherwise:

```cpp
digitalWrite(BEDROOM_PIR_LED_PIN, LOW);
```

---

## Applications

- Smart security systems
- Automatic room lighting
- Human presence detection

---

# 📏 Ultrasonic Sensor

The ultrasonic sensor measures distance.

---

# Working Process

1. Trigger pulse sent
2. Sound wave travels
3. Reflection received
4. Distance calculated

---

# Distance Formula

```math
d = \frac{v \times t}{2}
```

Where:

- \(d\) = distance
- \(v\) = speed of sound
- \(t\) = travel time

---

# Smart Study Table Logic

If distance is less than or equal to 6 cm:

```cpp
if(distance <= 6)
```

Then:
- Study table LED turns ON

Else:
- LED turns OFF

---

# 🌬 Smart Fan Control System

The fan supports:

- Manual control
- Automatic control

---

# 🎛 Manual Fan Control

The user controls the fan using a slider.

| Slider Value | Function |
|---|---|
| 0 | Fan OFF |
| 1 | Automatic mode |
| 2 | Low speed |
| 3 | Medium speed |
| 4 | High speed |

---

# ⚡ PWM Fan Speed Control

PWM = Pulse Width Modulation

Used for controlling motor speed.

## Code

```cpp
analogWrite(FAN_PWM_PIN, pwmValue);
```

---

# PWM Values

| PWM Value | Speed |
|---|---|
| 0 | OFF |
| 150 | Low |
| 200 | Medium |
| 255 | Full Speed |

---

# 🌡 Automatic Temperature-Based Fan System

The fan speed changes automatically according to temperature.

| Temperature | Fan Speed |
|---|---|
| Below 25°C | OFF |
| 25°C - 27°C | Low |
| 27°C - 30°C | Medium |
| Above 30°C | High |

---

# 🧠 Automation Logic

The project contains multiple automation systems.

---

# 1️⃣ Motion-Based Automation

If motion is detected:
- PIR LED turns ON

Otherwise:
- PIR LED turns OFF

---

# 2️⃣ Temperature-Based Automation

If temperature increases:
- Fan speed increases automatically

---

# 3️⃣ Distance-Based Automation

If object/person is near study table:
- Study light turns ON automatically

---

# 🌐 API Routes

| Route | Purpose |
|---|---|
| `/` | Homepage |
| `/data` | Sensor data |
| `/bedroomToggle` | Toggle bedroom LED |
| `/studyRoomToggle` | Toggle study room LED |
| `/livingRoomToggle` | Toggle living room LED |
| `/setFanSpeed` | Set fan speed |

---

# 🧩 JavaScript Functionality

# Real-Time Updates

```javascript
setInterval(fetchData, 1000);
```

The browser fetches updated sensor data every second.

---

# Button Actions

```javascript
fetch('/bedroomToggle');
```

Sends requests to ESP32.

---

# Fan Slider

```javascript
adjustFanSpeed(speedLevel);
```

Changes PWM fan speed.

---

# 🗂 Project Structure

```text
Smart-Home-Automation/
│
├── smart_home.ino
├── README.md
├── images/
│   ├── dashboard.png
│   ├── sensors.png
│   ├── fan-control.png
│   └── architecture.png
│
├── circuit/
│   └── circuit-diagram.png
│
└── libraries/
```

---

# 📥 Installation Guide

# Step 1 — Install Arduino IDE

Download:
https://www.arduino.cc/en/software

---

# Step 2 — Install ESP32 Board

Open Arduino IDE:

```text
File → Preferences
```

Add:

```text
https://dl.espressif.com/dl/package_esp32_index.json
```

Then:

```text
Tools → Board Manager
```

Install:
- ESP32 by Espressif Systems

---

# Step 3 — Install Required Libraries

Install:

- WiFi
- AsyncTCP
- ESPAsyncWebServer
- DHT Sensor Library
- Adafruit Sensor

---

# Step 4 — Upload Code

1. Connect ESP32
2. Select board
3. Select COM port
4. Click Upload

---

# Step 5 — Connect to ESP32 WiFi

| Setting | Value |
|---|---|
| SSID | Smart Home |
| Password | 12345678 |

---

# Step 6 — Open Dashboard

Open browser:

```text
http://192.168.4.1
```

---

# 📱 Future Improvements

Possible future upgrades:

- Mobile application
- Cloud IoT dashboard
- Firebase integration
- MQTT communication
- Voice assistant integration
- AI automation
- Energy monitoring
- Database storage
- User authentication
- Camera surveillance

---

# 🎓 Learning Outcomes

This project helps in learning:

- ESP32 programming
- Embedded systems
- Sensor integration
- IoT development
- Web servers
- Frontend-backend communication
- Automation systems
- PWM motor control
- Real-time monitoring

---

# 🌍 Real-World Applications

This system can be used in:

- Smart homes
- Smart offices
- Smart classrooms
- Automation labs
- IoT research
- Energy-saving systems
- Security systems

---

# 📸 Suggested Screenshots

Add screenshots for:

- Dashboard UI
- Sensor readings
- Fan control
- Circuit diagram
- Working hardware

Example:

```md
![Dashboard](images/dashboard.png)
```

---

# 🧪 Testing

## Tested Features

- LED switching
- WiFi connectivity
- Sensor reading
- Fan PWM control
- Motion detection
- Distance detection
- Real-time updates

---

# ⚠️ Known Limitations

- Works on local WiFi only
- No internet/cloud integration
- No authentication system
- DHT11 has low accuracy
- Basic security implementation

---

# 🔐 Security Improvements

Future security improvements:

- Login authentication
- HTTPS support
- Token-based API
- Secure WiFi credentials
- Cloud security integration

---

# 📚 Libraries Used

```cpp
#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>
```

---

# 🏁 Conclusion

This project demonstrates a complete Smart Home Automation System using ESP32 and IoT technologies.

The system combines:

- Real-time monitoring
- Automation
- Wireless communication
- Embedded systems
- Sensor integration
- Web technologies

to create a fully functional smart home environment.

This project is an excellent learning experience for students and developers interested in:

- IoT
- Embedded Systems
- Automation
- Smart Devices
- ESP32 Development
- Web-integrated hardware systems

---

# 👨‍💻 Author

## Zeeshan Abbas

### Smart Home Automation using ESP32

---
