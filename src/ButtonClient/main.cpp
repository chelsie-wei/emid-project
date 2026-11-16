/*
 * Created by ArduinoGetStarted.com
 *
 * This example code is in the public domain
 *
 * Tutorial page: https://arduinogetstarted.com/tutorials/communication-between-two-arduino
 */

// ARDUINO #1: TCP CLIENT + A BUTTON/SWITCH

#include <ezButton.h> // need to import this 
#include <WiFiS3.h>
#include <Arduino.h>
// #include <Adafruit_LSM6DSOX.h>

#define BUTTON_6 9
#define BUTTON_5 10
#define BUTTON_4 11
#define ANALOG_X_PIN A3
#define ANALOG_Y_PIN A2
const float SOFT_POT_4_PIN = A1;
const float SOFT_POT_3_PIN = A0;

// Adafruit_LSM6DSOX sox;
// bool imuReady = false;

const char ssid[] = WIFI;  // change your network SSID (name)
const char pass[] = PASSWORD;   // change your network password (use for WPA, or use as key for WEP)
const int ANALOG_X_CORRECTION = 128;
const int ANALOG_Y_CORRECTION = 128;

ezButton button6(BUTTON_6);
ezButton button5(BUTTON_5);
ezButton button4(BUTTON_4);

int button6State = 0;
int button5State = 0;
int button4State = 0;

const int serverPort = 4080;

IPAddress serverAddress(172, 20, 10, 2);
WiFiClient TCPclient;
int status = WL_IDLE_STATUS;

int readAnalogAxisLevel(int pin) {
  return map(analogRead(pin), 0, 1023, 0, 255);
}

void setup() {
  Serial.begin(115200);

  button6.setDebounceTime(50); // set debounce time to 50 milliseconds
  button5.setDebounceTime(50);
  button4.setDebounceTime(50);

  pinMode(SOFT_POT_4_PIN, INPUT);
  pinMode(SOFT_POT_3_PIN, INPUT);

  Serial.println("ARDUINO #1: TCP CLIENT + A BUTTON/SWITCH");

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

  // print your board's IP address:
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());

  // connect to TCP server (Arduino #2)
  if (TCPclient.connect(serverAddress, serverPort))
    Serial.println("Connected to TCP server");
  else
    Serial.println("Failed to connect to TCP server");
}

void loop() {

  button6.loop(); // MUST call the loop() function first
  button5.loop();
  button4.loop();

  if (button6.isPressed()) {
    button6State = 1;
  }

  if (button5.isPressed()) {
    button5State = 1;
  }

  if (button4.isPressed()) {
    button4State = 1;
  }

  if (!TCPclient.connected()) {
    Serial.println("Connection is disconnected");
    TCPclient.stop();

    // reconnect to TCP server (Arduino #2)
    if (TCPclient.connect(serverAddress, serverPort))
      Serial.println("Reconnected to TCP server");
    else
      Serial.println("Failed to reconnect to TCP server");
  }

  static unsigned long lastSend = 0;
  const unsigned long now = millis();

  if (TCPclient.connected() && now - lastSend >= 20) {
    lastSend = now;

    const int readings[] = {
      button6State, // [0]
      button5State,
      button4State,
      readAnalogAxisLevel(ANALOG_X_PIN) - ANALOG_X_CORRECTION,  // [3] X
      readAnalogAxisLevel(ANALOG_Y_PIN) - ANALOG_Y_CORRECTION,   // [4] Y
      analogRead(SOFT_POT_4_PIN),       // [5] Softpot #4
      analogRead(SOFT_POT_3_PIN)
    };

    char message[64];
    snprintf(message, sizeof(message), "%d,%d,%d,%d,%d,%d,%d\n",
            readings[0], readings[1], readings[2],
            readings[3], readings[4], readings[5], readings[6]);

    TCPclient.print(message);
    button6State = 0; button5State = 0; button4State = 0;
  }
}
