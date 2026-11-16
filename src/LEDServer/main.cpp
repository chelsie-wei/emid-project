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

const char ssid[] = WIFI;  // change your network SSID (name)
const char pass[] = PASSWORD;   // change your network password (use for WPA, or use as key for WEP)

int masterReadings[16] = {};

const int LED_PIN = LED_BUILTIN;
const int serverPort = 4080;

WiFiServer TCPserver(serverPort);
int status = WL_IDLE_STATUS;

void setup() {
  Serial.begin(115200);
  //pinMode(LED_PIN, OUTPUT);
  pinMode(LED_BUILTIN, OUTPUT);

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
  //Serial.println("-> Please update the serverAddress in Arduino #1 code");

  // Listening for a TCP client (from Arduino #1)
  TCPserver.begin();
}

void loop() {
  // Wait for a TCP client from Arduino #1:
  WiFiClient client = TCPserver.available();
  
  if (!client || !client.available()) {
    return;
  }

  String message = client.readStringUntil('\n');

  int incoming[7];
  int count = sscanf(message.c_str(), "%d,%d,%d,%d,%d,%d,%d",
                     &incoming[0], &incoming[1], &incoming[2],
                     &incoming[3], &incoming[4], &incoming[5],
                     &incoming[6]);

  if (count != 7) {
    return;
  }

  masterReadings[3]  = incoming[6];  // Softpot 3
  masterReadings[5]  = incoming[3];  // Joystick X
  masterReadings[6] = incoming[4];  // Joystick Y
  masterReadings[11] = incoming[0];  // Button 6 state
  masterReadings[12] = incoming[1];  // Button 5 state
  masterReadings[13] = incoming[2];  // Button 4 state
  masterReadings[14] = incoming[5];  // Softpot 4


  for (int i = 0; i < 20; i++) {
    if (i > 0) {
      Serial.print(' ');
    }
    Serial.print(masterReadings[i]);
  }
  Serial.println();
}
