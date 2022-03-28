/*****************************************************************************/
//  Function:  Get the accelemeter of the x/y/z axis. 
//  Hardware:  Grove - 3-Axis Analog Accelerometer
//  Arduino IDE: Arduino-1.0
//  Author:  Frankie.Chu    
//  Date:    Jan 11,2013
//  Version: v1.0
//  by www.seeedstudio.com
//
//  This library is free software; you can redistribute it and/or
//  modify it under the terms of the GNU Lesser General Public
//  License as published by the Free Software Foundation; either
//  version 2.1 of the License, or (at your option) any later version.
//
//  This library is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
//  Lesser General Public License for more details.
//
//  You should have received a copy of the GNU Lesser General Public
//  License along with this library; if not, write to the Free Software
//  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
//
/*******************************************************************************/

#include "ADXL335.h"
ADXL335 accelerometer;

#include <SoftwareSerial.h>                                                    //add the soft serial libray
#define rxpin 11                                                                //set the RX pin to pin 11
#define txpin 10                                                               //set the TX pin to pin 10
SoftwareSerial BTSerial(rxpin, txpin);      

String inputString = "";         // a String to hold incoming data
bool stringComplete = false;  // whether the string is complete



//#define DEBUG
//
//#ifndef DEBUG
//#endif


const int debug = 1;
const int DataAggregationInterval = 5;

int count;
bool started = 0;


void setup()
{
  Serial.begin(9600);
  BTSerial.begin(9600);
  
  accelerometer.begin();

  // reserve 200 bytes for the Bluetooth inputString:
  inputString.reserve(200);
  count = 0;
}

void loop()
{
  
  while (!started) {
    if (stringComplete) {
      Serial.println("rcvd something");

      if (inputString == "start\n") {
        Serial.println("rcvd start");
        started = 1;
      }
      
      // clear the string:
      inputString = "";
      stringComplete = false;
    } else {
      Serial.println("Waiting for start signal");
      delay(1000);
      serialEvent();
    }
  }

  // print the string when a newline arrives:
  if (stringComplete) {
    Serial.println(inputString);

    if (inputString == "end\n") {
      Serial.println("ended");
      started = 0;
      return;
    }
    
    // clear the string:
    inputString = "";
    stringComplete = false;
  }

  
    

 
  if (count == DataAggregationInterval) {
    count = 0;
    BTSerial.println("Acceleration: 15, HR: 65");
  }


  
  double magnitude;
  magnitude = get_mag_accel();
  
  delay(1000);
  count++;
}

double get_mag_accel()
{
  int x,y,z;
  float ax,ay,az;
  double magnitude;

  // Get voltage Values
  accelerometer.getXYZ(&x,&y,&z);

  // Calculate acceleration
  accelerometer.getAcceleration(&ax,&ay,&az);

  // Calculate magnitude of acceleration
  magnitude = sqrt(sq(ax) + sq(ay) + sq(az));


  if (debug >= 3) {
    Serial.println("value of X/Y/Z: ");
    Serial.println(x);
    Serial.println(y);
    Serial.println(z);
  }

  if (debug >= 2) {
    Serial.println("accleration of X/Y/Z: ");
    Serial.print(ax);
    Serial.println(" g");
    Serial.print(ay);
    Serial.println(" g");
    Serial.print(az);
    Serial.println(" g");

    Serial.println("Magnitude: ");
    Serial.print(magnitude);
    Serial.println(" g");
    Serial.println("");

  }
  return magnitude;
}

/*
  SerialEvent occurs whenever a new data comes in the hardware serial RX. This
  routine is run between each time loop() runs, so using delay inside loop can
  delay response. Multiple bytes of data may be available.
*/
void serialEvent() {
  while (Serial.available()) {
    // get the new byte:
    char inChar = (char)Serial.read();
    // add it to the inputString:
    inputString += inChar;
    // if the incoming character is a newline, set a flag so the main loop can
    // do something about it:
    if (inChar == '\n') {
      stringComplete = true;
    }
  }
}
