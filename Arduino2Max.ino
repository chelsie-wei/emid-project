#include <Wire.h>

// Filter cutoff, Reverb amt, LFO rate, LFO amt, Volume, Bpm Delta, 
// Sample layer, 

// how to link electronics to sample code 3

int diLedPin = 23;      // Digital Led implementation
int anLedPin = 2;       // PWM Led implementation
bool ledOn = 0;
int pushButton = 22; // Yellow button (Start)
int Analog1 = A1;   // Multiswitch position 0
int Analog2 = A2;
int softpot = A3;
int FSR = A4;
int bend = A5;
int Analog6 = A6;
int Analog7 = A7;
int infrared = A8;
int potentiometer = A9;
int val = 0;
int maxIn = 0;


// Set up for Serial baudrate (increase for faster printing speed)
void setup(void) {
    Serial.begin(9600);
    Wire.begin();
    // Declare operation mode for the digital pins
    pinMode(pushButton, INPUT);
    pinMode(diLedPin, OUTPUT);
}

void loop(void) {
    // Checks to see if there is or isn't information coming from Max on the serial port
    if(Serial.available()) {
    maxIn = Serial.read();
    // format between 0-255 (analog range) to avoid damaging the analog led
    maxIn = maxIn % 255;
    // activate blue LED if maxIN is greater than 100
    if(maxIn > 50) {
        ledOn = 1;
    } else {
        ledOn = 0;
    }
    
    analogWrite(anLedPin, maxIn); // SerialRead values go from 0 to 250, analogWrite values from 0 to 255
    digitalWrite(diLedPin, ledOn);
    // Serial.peek access next packet without deleting
 }

 // Button, Infrared, Bend, FSR, softpot, potentiometer
 // formatted as:  [1, 230, 684, 300, 540, 765]
 val = digitalRead(pushButton);
 Serial.print(val);
 Serial.print(" ");


 val = analogRead(infrared);
 Serial.print(val);
 Serial.print(" ");


 val = analogRead(bend);
 Serial.print(val);
 Serial.print(" ");


 val = analogRead(FSR);
 Serial.print(val);
 Serial.print(" ");


 val = analogRead(softpot);
 Serial.print(val);
 Serial.print(" ");


 val = analogRead(potentiometer);
 Serial.print(val);
 Serial.print(" ");


 val = analogRead(Analog6);
 Serial.print(val);
 Serial.print(" ");


 val = analogRead(Analog7);
 Serial.print(val);
 Serial.print(" ");

 // new line after all readings are completed to format
 Serial.println();
}