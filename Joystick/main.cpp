#include <Arduino.h>
#define ANALOG_X_PIN A3 
#define ANALOG_Y_PIN A2 
#define ANALOG_BUTTON_PIN A4 

// Source: https://arduinogetstarted.com/tutorials/arduino-joystick

// button digital 2
	 
//Default values when axis not actioned 
#define ANALOG_X_CORRECTION 128 
#define ANALOG_Y_CORRECTION 128 

int xValue = 0;
int yValue = 0;
	 
struct button { 
	 byte pressed = 0; 
}; 
	 
struct analog { 
	 short x, y; 
	 
	 button button; 
}; 
	 
void setup() 
{ 
     Serial.begin(9600); 
	//  pinMode(ANALOG_BUTTON_PIN, INPUT_PULLUP);  
} 
	 
void loop() 
{ 
    //  analog analog;
     xValue = analogRead(ANALOG_X_PIN);
     yValue = analogRead(ANALOG_Y_PIN);
	 
	//  analog.x = readAnalogAxisLevel(xValue) - ANALOG_X_CORRECTION; 
	//  analog.y = readAnalogAxisLevel(yValue) - ANALOG_Y_CORRECTION; 
	 
	//  analog.button.pressed = isAnalogButtonPressed(ANALOG_BUTTON_PIN); 
	 
	 Serial.print("X:"); 
	 Serial.println(xValue); 
	 
	 Serial.print("Y:"); 
	 Serial.println(yValue); 
	 
	 if (analog.button.pressed) { 
	   Serial.println("Button pressed"); 
	 } else { 
	   Serial.println("Button not pressed"); 
	 } 
	 
	 delay(200); 
} 
	 
byte readAnalogAxisLevel(int pin) 
{ 
	 return map(analogRead(pin), 0, 1023, 0, 255); 
} 
	 
bool isAnalogButtonPressed(int pin) 
{ 
	 return digitalRead(pin) == 0; 
} 
