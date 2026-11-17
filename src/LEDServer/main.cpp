/*
 * Created by ArduinoGetStarted.com
 *
 * This example code is in the public domain
 *
 * Tutorial page: https://arduinogetstarted.com/tutorials/communication-between-two-arduino
 */

// ARDUINO #2: TCP SERVER + AN LED
#include <WiFiS3.h>
#include <Arduino.h>
#include <ezButton.h>
#include <Adafruit_LSM6DSOX.h>
#include <Adafruit_LEDBackpack.h>
#include <math.h>

Adafruit_LSM6DSOX leftSox;
Adafruit_8x8matrix matrix;
bool leftImuReady = false;

const char ssid[] = USER;  // change your network SSID (name)
const char pass[] = PASSWORD;   // change your network password (use for WPA, or use as key for WEP)

int masterReadings[16] = {};

const int LED_PIN = LED_BUILTIN;
const int serverPort = 4080;

// pinkie button is 9 

#define BUTTON_3 9
#define BUTTON_2 10
#define BUTTON_1 11

#define THUMB_BUTTON_PIN 13

const float SOFT_POT_1_PIN = A0;
const float SOFT_POT_2_PIN = A1;

ezButton button1(BUTTON_1);
ezButton button2(BUTTON_2);
ezButton button3(BUTTON_3);
ezButton thumbButton(THUMB_BUTTON_PIN);

int thumbButtonReading = 0;

WiFiServer TCPserver(serverPort);
int status = WL_IDLE_STATUS;

bool matrixReady = false;

void showSampleLayer(int layer) {
  if (!matrixReady) {
    return;
  }

  const int x = (layer % 2) * 6;
  const int y = (layer / 2) * 6;

  matrix.clear();
  matrix.fillRect(x, y, 2, 2, LED_ON);
  matrix.writeDisplay();
}

void setup() {
  Serial.begin(57600);
  pinMode(THUMB_BUTTON_PIN, INPUT_PULLUP);

  matrixReady = matrix.begin(0x70);
  showSampleLayer(masterReadings[7]);

  leftImuReady = leftSox.begin_I2C();
  Serial.println(leftImuReady ? "Left IMU found" : "Left IMU not found");

  Serial.println("ARDUINO #2: TCP SERVER + AN LED");

  // check for the WiFi module:
  if (WiFi.status() == WL_NO_MODULE) {
    Serial.println("Communication with WiFi module failed!");
    // don't continue
    while (true)
      ;
  }

  String fv = WiFi.firmwareVersion();
  if (fv < WIFI_FIRMWARE_LATEST_VERSION) {
    Serial.println("Please upgrade the firmware");
  }

  // attempt to connect to WiFi network:
  while (status != WL_CONNECTED) {
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(ssid);
    // Connect to WPA/WPA2 network. Change this line if using open or WEP network:
    status = WiFi.begin(ssid, pass);

    // wait 10 seconds for connection:
    delay(10000);
  }

  // Print your local IP address:
  Serial.print("TCP Server IP address: ");
  Serial.println(WiFi.localIP());

  // Listening for a TCP client (from Arduino #1)
  TCPserver.begin();
}

void loop() {

  button1.loop();
  button2.loop();
  button3.loop();
  thumbButton.loop();

  // if (button1.isPressed()) {
  //   masterReadings[8] = 1 - masterReadings[8];
  // }

  // if (button2.isPressed()) {
  //   masterReadings[9] = 1 - masterReadings[9]; // button 2
  // }

  // if (button3.isPressed()) {
  //   masterReadings[14] = 1 - masterReadings[14]; // button 3
  // }

  // if (button1.isPressed()) {
  //   masterReadings[8] = 1;
  // }

  // if (button2.isPressed()) {
  //   masterReadings[9] = 1; // button 2
  // }

  // if (button3.isPressed()) {
  //   masterReadings[14] = 1; // button 3
  // }

  if (thumbButton.isPressed()) {
    masterReadings[7] = (thumbButtonReading + 1) % 4;
    showSampleLayer(masterReadings[7]);
    thumbButtonReading++;
  }

  // Wait for a TCP client from Arduino #1:
  WiFiClient client = TCPserver.available();
  
  if (!client || !client.available()) {
    return;
  }

  String message = client.readStringUntil('\n');

  int incoming[7];
  char tiltText[24];

  if (sscanf(message.c_str(), "%d,%d,%d,%d,%d,%d,%d,%23s",
            &incoming[0], &incoming[1], &incoming[2],
            &incoming[3], &incoming[4], &incoming[5],
            &incoming[6], tiltText) != 8) {
    return;
  }

  char *end;
  const float handTiltDegrees = strtof(tiltText, &end);
  if (end == tiltText || *end != '\0') {
    return;
  }

  masterReadings[0] = analogRead(SOFT_POT_1_PIN);  // Left-hand Softpot 1
  masterReadings[1] = analogRead(SOFT_POT_2_PIN);  // Left-hand Softpot 2

  masterReadings[3]  = incoming[6];  // Softpot 3
  masterReadings[5]  = incoming[3];  // Joystick X
  masterReadings[6] = incoming[4];  // Joystick Y
  masterReadings[10] = incoming[0];  // Button 6 state
  masterReadings[11] = incoming[1];  // Button 5 state
  masterReadings[12] = incoming[2];  // Button 4 state
  masterReadings[13] = incoming[5];  // Softpot 4

  masterReadings[8]  = digitalRead(BUTTON_1);
  masterReadings[9]  = digitalRead(BUTTON_2);
  masterReadings[14] = digitalRead(BUTTON_3); 

  float leftHandTiltDegrees = -1.0f;
  sensors_event_t accel, gyro, temp;

  if (leftImuReady && leftSox.getEvent(&accel, &gyro, &temp)) {
    const float ax = accel.acceleration.x;
    const float ay = accel.acceleration.y;
    const float az = accel.acceleration.z;

    leftHandTiltDegrees = atan2f(sqrtf(ax * ax + ay * ay), az) * 180.0f / PI;
  }


  for (int i = 0; i < 16; i++) {
    if (i > 0) {
      Serial.print(' ');
    }
    if (i == 4) {
      Serial.print(handTiltDegrees, 2);
    } else if (i == 2) {
      Serial.print(leftHandTiltDegrees, 2);
    } else {
      Serial.print(masterReadings[i]);
    }
  }
  Serial.println();
}
