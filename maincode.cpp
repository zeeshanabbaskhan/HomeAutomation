#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>

#define BEDROOM_LED_PIN 13
#define STUDY_ROOM_LED_PIN 12
#define LIVING_ROOM_LED_PIN 14
#define DHT_PIN 27
#define DHT_TYPE DHT11
#define TRIG_PIN 21
#define ECHO_PIN 19
#define STUDY_TABLE_LED_PIN 33

#define motionSensorPin 26   // Pin for motion sensor (PIR sensor)
#define BEDROOM_PIR_LED_PIN 32 // Pin for PIR LED

// Fan control pins
#define FAN_PWM_PIN 18
#define FAN_ENABLE_PIN 5


DHT dht(DHT_PIN, DHT_TYPE);
AsyncWebServer server(80);

float temperature = 0.0;
float humidity = 0.0;

String bedroomLedState = "OFF";
String studyRoomLedState = "OFF";
String livingRoomLedState = "OFF";
String bedroomPirLedState = "OFF";
String studyTableLedState = "OFF";  // New variable for study table LED state

bool bedroomLedManual = false;
bool fanManualControl = false;

long duration;
int distance;

void handleRoot(AsyncWebServerRequest *request) {
  String html = R"rawliteral(
  <!DOCTYPE html>
  <html>
  <head>
      <title>Smart Home Control</title>
      <style>
          body {
              font-family: 'Arial', sans-serif;
              margin: 0;
              background: linear-gradient(to right, #243B55, #141E30);
              color: white;
              text-align: center;
          }
          .header-box {
              background-color: rgba(0, 0, 0, 0.6);
              padding: 20px;
              border-radius: 10px;
              display: inline-block;
              margin-top: 20px;
              box-shadow: 0px 4px 10px rgba(0, 0, 0, 0.5);
          }
          h1 {
              font-size: 48px;
              color: #FFD700;
              text-shadow: 2px 2px 5px rgba(0, 0, 0, 0.4);
              margin: 0;
          }
          .widgets {
              display: flex;
              justify-content: center;
              align-items: flex-start;
              gap: 20px;
              padding: 20px;
              flex-wrap: wrap;
          }
          .widget {
              display: inline-block;
              width: 220px;
              height: 220px;
              background: linear-gradient(to bottom, #4caf50, #8bc34a);
              border-radius: 50%;
              margin: 10px;
              box-shadow: 0 4px 8px rgba(0, 0, 0, 0.2);
              text-align: center;
              color: black;
              font-family: 'Arial', sans-serif;
          }
          .widget p {
              margin: 0;
              line-height: 220px;
              font-size: 22px;
              font-weight: bold;
          }
          .temperature-widget {
              background: linear-gradient(to bottom, #00bcd4, #03a9f4);
          }
          .temperature-widget p {
              line-height: 200px;
              font-size: 20px;
          }
          .container {
              display: flex;
              justify-content: center;
              align-items: flex-start;
              gap: 20px;
              padding: 20px;
              flex-wrap: wrap;
          }
          .block {
              background: rgba(255, 255, 255, 0.1);
              padding: 10px;
              border-radius: 10px;
              box-shadow: 0px 4px 10px rgba(0, 0, 0, 0.2);
              width: 300px;
              height: 300px;
              display: flex;
              flex-direction: column;
              justify-content: space-between;
              text-align: center;
              margin-bottom: 5px;
          }
          .block h2 {
              font-size: 24px;
              margin-bottom: 5px;
          }
          .block .status {
              font-size: 16px;
              margin: 5px 0;
              font-weight: bold;
              color: #FFD700;
          }
          .block .button {
              background-color: #FF5733;
              color: white;
              padding: 5px 10px;
              border: none;
              border-radius: 30px;
              cursor: pointer;
              font-size: 16px;
              transition: all 0.3s ease-in-out;
              box-shadow: 0px 5px 15px rgba(0, 0, 0, 0.2);
          }
          .block .button:hover {
              background-color: #C70039;
              box-shadow: 0px 8px 20px rgba(0, 0, 0, 0.3);
          }
          #fanControl {
              width: 100%;
              height: 30px;
          }
      </style>
  </head>
  <body>
      <div class="header-box">
          <h1>SMART HOME CONTROL</h1>
      </div>
      <div class="widgets">
          <div class="widget temperature-widget">
              <p id="temp">Temperature: -- °C</p>
          </div>
          <div class="widget">
              <p id="hum">Humidity: -- %</p>
          </div>
      </div>
      <div class="container">
          <div class="block">
              <h2>Living Room</h2>
              <h3>Fan Control</h3>
              <input type="range" id="fanControl" min="0" max="4" step="1" onchange="adjustFanSpeed(this.value)" />
              <p id="fanStatus">Fan Speed: 0</p>
              <div class="status" id="livingRoomStatus">LED Status: --</div>
              <button class="button" onclick="toggleLivingRoom()">Toggle Living Room LED</button>
          </div>
          <div class="block">
              <h2>Bedroom</h2>
              <div class="status" id="bedroomStatus">LED Status: --</div>
              <div class="status" id="pirStatus">PIR LED Status: --</div>
              <button class="button" onclick="toggleBedroom()">Toggle Bedroom LED</button>
          </div>
          <div class="block">
              <h2>Study Room</h2>
              <div class="status" id="studyRoomStatus">LED Status: --</div>
              <div class="status" id="studyTableStatus">Study Table LED Status: --</div>
              <button class="button" onclick="toggleStudyRoom()">Toggle Study Room LED</button>
          </div>
      </div>
      <script>
          async function fetchData() {
              const res = await fetch('/data');
              const data = await res.json();
              document.getElementById('temp').innerText = "Temperature: " + data.temperature + " °C";
              document.getElementById('hum').innerText = "Humidity: " + data.humidity + " %";
              document.getElementById('bedroomStatus').innerText = "LED Status: " + data.bedroomLedState;
              document.getElementById('studyRoomStatus').innerText = "LED Status: " + data.studyRoomLedState;
              document.getElementById('livingRoomStatus').innerText = "LED Status: " + data.livingRoomLedState;
              document.getElementById('pirStatus').innerText = "PIR LED Status: " + data.bedroomPirLedState;
              document.getElementById('studyTableStatus').innerText = "Study Table LED Status: " + data.studyTableLedState;
          }
          async function toggleBedroom() {
              await fetch('/bedroomToggle');
              fetchData();
          }
          async function toggleStudyRoom() {
              await fetch('/studyRoomToggle');
              fetchData();
          }
          async function toggleLivingRoom() {
              await fetch('/livingRoomToggle');
              fetchData();
          }
          async function adjustFanSpeed(speedLevel) {
              document.getElementById('fanStatus').innerText = "Fan Speed: " + speedLevel;
              await fetch(`/setFanSpeed?speed=${speedLevel}`);
          }
          setInterval(fetchData, 1000);
      </script>
  </body>
  </html>
  )rawliteral";
  request->send(200, "text/html", html);
}

void handleBedroomToggle(AsyncWebServerRequest *request) {
  bedroomLedManual = !bedroomLedManual;
  digitalWrite(BEDROOM_LED_PIN, bedroomLedManual ? HIGH : LOW);
  bedroomLedState = bedroomLedManual ? "ON (Manual)" : "OFF";
  request->send(200, "text/plain", "Bedroom LED toggled");
}

void handleStudyRoomToggle(AsyncWebServerRequest *request) {
  if (digitalRead(STUDY_ROOM_LED_PIN) == LOW) {
    digitalWrite(STUDY_ROOM_LED_PIN, HIGH);
    studyRoomLedState = "ON";
  } else {
    digitalWrite(STUDY_ROOM_LED_PIN, LOW);
    studyRoomLedState = "OFF";
  }
  request->send(200, "text/plain", "Study Room LED toggled");
}

void handleLivingRoomToggle(AsyncWebServerRequest *request) {
  if (digitalRead(LIVING_ROOM_LED_PIN) == LOW) {
    digitalWrite(LIVING_ROOM_LED_PIN, HIGH);
    livingRoomLedState = "ON";
  } else {
    digitalWrite(LIVING_ROOM_LED_PIN, LOW);
    livingRoomLedState = "OFF";
  }
  request->send(200, "text/plain", "Living Room LED toggled");
}

void handleData(AsyncWebServerRequest *request) {
  temperature = dht.readTemperature();
  humidity = dht.readHumidity();
  String json = "{\"temperature\":" + String(temperature) +
                ",\"humidity\":" + String(humidity) +
                ",\"bedroomLedState\":\"" + bedroomLedState +
                "\",\"studyRoomLedState\":\"" + studyRoomLedState +
                "\",\"livingRoomLedState\":\"" + livingRoomLedState +
                "\",\"bedroomPirLedState\":\"" + bedroomPirLedState +
                "\",\"studyTableLedState\":\"" + studyTableLedState + "\"}";
  request->send(200, "application/json", json);
}


void handleFanSpeed(AsyncWebServerRequest *request) {
  String speed = request->getParam("speed")->value();
  int speedLevel = speed.toInt();

  if (speedLevel == 0) {
    // Slider at 0: Turn off the fan (both manual and automatic control disabled)
   
    digitalWrite(FAN_ENABLE_PIN, LOW);  // Turn off the fan
    analogWrite(FAN_PWM_PIN, 0);
     fanManualControl = true;
        // Set PWM to 0
  } else if (speedLevel == 1) {
    // Slider at 1: Automatic control based on temperature
    fanManualControl = false;  // Disable manual control
  } else {
    // Slider at 2, 3, or 4: Manual control
    fanManualControl = true;  // Enable manual control
    int pwmValue = 0;
    switch (speedLevel) {
      case 2:
        pwmValue = 150;  // Low speed
        break;
      case 3:
        pwmValue = 200;  // Medium speed
        break;
      case 4:
        pwmValue = 255;  // High speed
        break;
      default:
        pwmValue = 0;
        break;
    }
    digitalWrite(FAN_ENABLE_PIN, LOW);  // Enable the fan
    analogWrite(FAN_PWM_PIN, pwmValue);  // Set fan speed using PWM
  }

  request->send(200, "text/plain", "Fan speed set to: " + String(speedLevel));
}

// Temperature-based fan control logic, will only work if the fan is not manually controlled
void adjustFanSpeedBasedOnTemperature() {
  if (fanManualControl) {
    return;  // Skip if fan is manually controlled (by slider)
  }

  temperature = dht.readTemperature();

  if (temperature >= 30) {
    // Full speed
    analogWrite(FAN_PWM_PIN, 255);
    digitalWrite(FAN_ENABLE_PIN, LOW);
  } else if (temperature >= 27) {
    // High speed
    analogWrite(FAN_PWM_PIN, 200);
    digitalWrite(FAN_ENABLE_PIN, LOW);
  } else if (temperature >= 25) {
    // Low speed
    analogWrite(FAN_PWM_PIN, 150);
    digitalWrite(FAN_ENABLE_PIN, LOW);
  } else {
    // Off
    analogWrite(FAN_PWM_PIN, 0);
    digitalWrite(FAN_ENABLE_PIN, LOW);
  }
}


void setup() {
    Serial.begin(115200);  // Use a higher baud rate for serial communication
  pinMode(BEDROOM_LED_PIN, OUTPUT);
  pinMode(STUDY_ROOM_LED_PIN, OUTPUT);
  pinMode(LIVING_ROOM_LED_PIN, OUTPUT);
  pinMode(FAN_PWM_PIN, OUTPUT);
  pinMode(FAN_ENABLE_PIN, OUTPUT);
  pinMode(motionSensorPin, INPUT);  // Set motion sensor pin as input
  pinMode(BEDROOM_PIR_LED_PIN, OUTPUT); // PIR LED output pin
   pinMode(STUDY_TABLE_LED_PIN, OUTPUT);

 pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);

  dht.begin();
 
   WiFi.softAP("Smart Home", "12345678");  // Set SSID, password, and channel
 

  server.on("/", HTTP_GET, handleRoot);
  server.on("/bedroomToggle", HTTP_GET, handleBedroomToggle);
  server.on("/studyRoomToggle", HTTP_GET, handleStudyRoomToggle);
  server.on("/livingRoomToggle", HTTP_GET, handleLivingRoomToggle);
  server.on("/data", HTTP_GET, handleData);
  server.on("/setFanSpeed", HTTP_GET, handleFanSpeed);
  server.begin();
}





void loop() {
  // PIR sensor logic for bedroom PIR LED
  int motionState = digitalRead(motionSensorPin);
  digitalWrite(BEDROOM_PIR_LED_PIN, motionState ? HIGH : LOW);
  bedroomPirLedState = motionState ? "ON" : "OFF";

  if (fanManualControl == false) {
    adjustFanSpeedBasedOnTemperature();
  }
  
  // Ultrasonic sensor logic for study table LED
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(20);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(20);
  digitalWrite(TRIG_PIN, LOW);
  duration = pulseIn(ECHO_PIN, HIGH);
  distance = duration * 0.0344 / 2;
  
  // Control the study table LED based on ultrasonic sensor
  if (distance <= 6) {
    digitalWrite(STUDY_TABLE_LED_PIN, HIGH);
    studyTableLedState = "ON";
  } else {
    digitalWrite(STUDY_TABLE_LED_PIN, LOW);
    studyTableLedState = "OFF";
  }
}
