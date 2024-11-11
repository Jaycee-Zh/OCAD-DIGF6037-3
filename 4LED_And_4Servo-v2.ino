/*
Combination of LED and Multi-Servo Control via Serial
-----------------------------
This sketch reads colon-separated values over Serial and applies them to certain pins to control LED or Servo.
Based on:
- https://github.com/DigitalFuturesOCADU/CC2024/blob/main/experiment3/examples/example1_controlLEDs/arduino/serial_read2Vals_LEDbrightness/serial_read2Vals_LEDbrightness.ino
- https://github.com/DigitalFuturesOCADU/CC2024/blob/main/experiment3/examples/example2_controlServos/webSerial_4Servo_CSV/webSerial_4Servo_CSV.ino

Hardware Setup:
--------------
1. Connect servos to the following Arduino pins:
   - S0: Servo 1: Pin 2
   - S1: Servo 2: Pin 3
   - S2: Servo 3: Pin 4
   - S3: Servo 4: Pin 5
   - L0: LED 1: Pin 9
   - L1: LED 2: Pin 10
   - L2: LED 3: Pin 11
   - L3: LED 4: Pin 12
2. Power your servos appropriately (most servos need external power)
   - DO NOT power multiple servos directly from Arduino's 5V!
   - Use an external power supply or battery pack

Usage Examples:
--------------
Send these strings over Serial (end with newline):
  "L1: 90"     -> Sets first LED to 90 light level
  "S2:90"      -> Sets second servo to 90


Serial Configuration:
-------------------
- Baud Rate: 57600
- Line Ending: Newline (\n)

Note: You can modify maxServos constant to support fewer/more servos
(remember to update servoPins array accordingly)
*/
#include <Servo.h>

// Define number of servos
const int ServosNum = 4;
const char servoLetter = 'S';

// Create servo objects
Servo servos[ServosNum];  // Array to hold servo objects

// Define constants for servo pin numbers
const int servoPins[ServosNum] = { 2, 3, 4, 5 };  // Pins for servos 1-4

// Define number of led
const int ledNum = 4;
const char ledLetter = 'L';

// Define constants for LED pin numbers
const int ledPins[] = { 9, 10, 11, 12 };  // Pins for leds

// Define inintial value
const int ledInitial[] = {0,0,0,0};
const int servoInitial[] = {90,90,90,90};

void setup() {

  // Initialize serial communication at 57600 baud rate
  Serial.begin(57600);

  // Set all LED pins as OUTPUT and initial value
  for (int i = 0; i < ledNum; i++) {
    pinMode(ledPins[i], OUTPUT);
    // set LED initial value
    analogWrite(ledPins[i], ledInitial[i]);
  }


  // Attach all servos to their pins and set initial value
  for (int i = 0; i < ServosNum; i++) {
    servos[i].attach(servoPins[i]);
    // set servos initial value
      servos[i].write(servoInitial[i]);
  }
}

void loop() {
  // Check if data is available on the serial port
  if (Serial.available() > 0) {
    // Read the incoming data until a newline character
    String input = Serial.readStringUntil('\n');
    // If the input is not empty, process it
    if (input.length() > 0) {
      processInput(input);
    }
  }
  
}

// process LED input
void processInput(String input) {
    String type;
    int index;
    int value;

  // Find the position of the colon in the input string
  int colonIndex = input.indexOf(':');

  // If a colon is found in the input
  if (colonIndex != -1) {
    // Serial.println("string!");
    // Split the input string into three parts
    type = input.substring(0, 1);
    index = input.substring(1, 2).toInt();
    value = input.substring(colonIndex + 1).toInt();

    if (type == "L") {
      value = constrain(value, 0, 255);
      analogWrite(ledPins[index], value);
    } else if (type == "S") {
      value = constrain(value, 0, 180);
      servos[index].write(value);
    }
  }
}

  // // process LED input
  // void processInput(String input) {
  //   // Find the position of the comma in the input string
  //   int commaIndex = input.indexOf(',');

  //   // If a comma is found in the input
  //   if (commaIndex != -1) {
  //     // Split the input string into two parts
  //     String value1String = input.substring(0, commaIndex);
  //     String value2String = input.substring(commaIndex + 1);

  //     // Convert the string values to integers and constrain them between 0 and 255
  //     int value1 = constrain(value1String.toInt(), 0, 255);
  //     int value2 = constrain(value2String.toInt(), 0, 255);

  //     // Set the brightness of LED 1 (both pins) using value1
  //     analogWrite(led1Pin1, value1);
  //     analogWrite(led1Pin2, value1);

  //     // Set the brightness of LED 2 (both pins) using value2
  //     analogWrite(led2Pin1, value2);
  //     analogWrite(led2Pin2, value2);
  //   }
  // }

  // process servo input
  // void processInput(String input) {
  //   int lastIndex = 0;
  //   int servoIndex = 0;

  //   // Keep finding commas until we run out or hit our servo limit
  //   while(servoIndex < maxServos) {
  //     int nextComma = input.indexOf(',', lastIndex);
  //     String valueString;

  //     // Extract the value string
  //     if(nextComma == -1) {
  //       // Last value
  //       valueString = input.substring(lastIndex);
  //       // Convert to int, constrain, and apply to final servo
  //       servos[servoIndex].write(constrain(valueString.toInt(), 0, 180));
  //       break;  // No more values to process
  //     } else {
  //       // Not the last value
  //       valueString = input.substring(lastIndex, nextComma);
  //       // Convert to int, constrain, and apply to current servo
  //       servos[servoIndex].write(constrain(valueString.toInt(), 0, 180));
  //       // Move to next value
  //       lastIndex = nextComma + 1;
  //       servoIndex++;
  //     }
  //   }
  // }