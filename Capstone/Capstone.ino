

#include "ADXL335.h"
ADXL335 accelerometer;

#include <SoftwareSerial.h>                                  //add the soft serial libray
#define rxpin 11                                             //set the RX pin to pin 11
#define txpin 10                                             //set the TX pin to pin 10
SoftwareSerial BTSerial(txpin, rxpin);      

String inputString = "";         // a String to hold incoming data
bool stringComplete = false;     // whether the string is complete



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

 
 


  Serial.println(count);
  float ax, ay, az;
  get_accel(&ax, &ay, &az);
  
  if (count == DataAggregationInterval) {
    count = 0;
    
    BTSerial.print(ax);
    BTSerial.print(",");
    BTSerial.print(ay);
    BTSerial.print(",");
    BTSerial.print(az);
    BTSerial.print(",");
    BTSerial.println(",65\n");
    Serial.print("Send over bluetooth");
  }

  
  delay(1000);
  count++;
}

void get_accel(float * ax, float * ay, float * az)
{
  int x,y,z;
  double magnitude;

  // Get voltage Values
  accelerometer.getXYZ(&x,&y,&z);

  // Calculate acceleration
  accelerometer.getAcceleration(ax,ay,az);


  if (debug >= 3) {
    Serial.println("value of X/Y/Z: ");
    Serial.println(x);
    Serial.println(y);
    Serial.println(z);
  }

  if (debug >= 2) {
    Serial.println("accleration of X/Y/Z: ");
    Serial.print(*ax);
    Serial.println(" g");
    Serial.print(*ay);
    Serial.println(" g");
    Serial.print(*az);
    Serial.println(" g");

  }
}
