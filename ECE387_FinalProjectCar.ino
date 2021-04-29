/* Run Wheels, Servo, and remote
*/
#include <Wire.h>
#include <ELECHOUSE_CC1101.h>
#include <Servo.h>
                           // Wire colors to keep track of which wire is in which pin
const int LWhFwdPin = 4;  // blue
const int LWhBwdPin = 3;  // purple
const int LWhPWMPin = 5;  // green

const int RWhFwdPin = 8;  // grey
const int RWhBwdPin = 7;  // white
const int RWhPWMPin = 6;  // yellow

// Variables to determine catapult launch and wheel speeds
int button = 1;
int RSPD = 255;      // Right Wheel PWM - the recieved x value from the joystick
int LSPD = 255;      // Left Wheel PWM - the recieved y value from the joystick

// Servo object for the catapult
Servo catapult;
// Variable for servo position
int pos = 0;

void setup()
{
  // Set up data reception
  Serial.begin(9600);
  ELECHOUSE_cc1101.Init();
  ELECHOUSE_cc1101.SetReceive();
  
  catapult.attach(1);  // Servo is attached to pin 1 - 1 is sometimes finicky

  pinMode(LWhFwdPin, OUTPUT);
  pinMode(LWhBwdPin, OUTPUT);
  pinMode(LWhPWMPin, OUTPUT);
  pinMode(RWhFwdPin, OUTPUT);
  pinMode(RWhBwdPin, OUTPUT);
  pinMode(RWhPWMPin, OUTPUT);

  digitalWrite(LWhFwdPin, LOW);
  digitalWrite(LWhBwdPin, LOW);
  digitalWrite(LWhPWMPin, LOW);

  digitalWrite(RWhFwdPin, LOW);
  digitalWrite(RWhBwdPin, LOW);
  digitalWrite(RWhPWMPin, LOW);
}

byte RX_buffer[61]={0};
byte size,i,flag;

void loop()
{
  // Data received, call method to convert
  if (ELECHOUSE_cc1101.CheckReceiveFlag()) {
    // Convert the data to proper values
    recieve();
    // Check to see if catapult is launching
    if (button == 0) {
      // Stop car for launch
      digitalWrite(RWhFwdPin, LOW);
      digitalWrite(LWhFwdPin, LOW);
      digitalWrite(RWhBwdPin, LOW);
      digitalWrite(LWhBwdPin, LOW);
      analogWrite(RWhPWMPin, 0);
      analogWrite(LWhPWMPin, 0);
      // Short delay for processing purposes
      delay(10);
      // launch
      pos = 90;
      catapult.write(pos);
      // Return catapult to original position
      for (pos = 90; pos >= 0; pos -= 5) { // goes from 90 degrees to 0 degrees
        catapult.write(pos); 
      }
    } else if (RSPD < 0) {
      digitalWrite(RWhFwdPin, LOW);
      digitalWrite(LWhFwdPin, LOW);
      digitalWrite(RWhBwdPin, HIGH);
      digitalWrite(LWhBwdPin, HIGH);
      analogWrite(RWhPWMPin, -RSPD);
      analogWrite(LWhPWMPin, -LSPD);
    } else {
      digitalWrite(RWhBwdPin, LOW);
      digitalWrite(LWhBwdPin, LOW);
      digitalWrite(RWhFwdPin, HIGH);
      digitalWrite(LWhFwdPin, HIGH);
      analogWrite(RWhPWMPin, RSPD);
      analogWrite(LWhPWMPin, LSPD);
    }
    ELECHOUSE_cc1101.SetReceive();
  } else {
    digitalWrite(RWhFwdPin, LOW);
    digitalWrite(LWhFwdPin, LOW);
    digitalWrite(RWhBwdPin, LOW);
    digitalWrite(LWhBwdPin, LOW);
    analogWrite(RWhPWMPin, 0);
    analogWrite(LWhPWMPin, 0);
  }
}

// Take the received data and convert to proper PWM values
void recieve() {
  size=ELECHOUSE_cc1101.ReceiveData(RX_buffer);
    for(i=0;i<size;i++)
    {
      Serial.print(RX_buffer[i],DEC);
      Serial.print(' ');
    }
    Serial.println("");
}
