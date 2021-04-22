/* Run Wheels, Servo, and remote
*/
#include <Wire.h>
#include <RH_ASK.h>
#include <SPI.h>
#include <Servo.h>

const int RSPD = 255;       // Right Wheel PWM - currently a stand-in
const int LSPD = 255;       // Left Wheel PWM - currently a stand-in
const int LWhFwdPin = 12; // blue
const int LWhBwdPin = 10; // purple
const int LWhPWMPin = 13; // green

const int RWhFwdPin = 8; // grey
const int RWhBwdPin = 7; // white
const int RWhPWMPin = 6; // yellow

int stopCount = 300;  // Just a value for testing, to make car stop after a time if ya want
// Instance of RadioHead amplitude shift-keying
RH_ASK driver;

// Servo object for the catapult
Servo catapult;
// Variable for servo position
int pos = 0;

// String to compare message to, not currently implemented
String sentMsg;

void setup()
{
  Serial.begin(9600); // Just for testing, delete later
  if (!driver.init()) Serial.println("init failed");

  catapult.attach(4);  // Servo is attached to pin 4

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

void loop()
{
  // Data storage of message
  uint8_t buf[RH_ASK_MAX_MESSAGE_LEN];
  uint8_t buflen = sizeof(buf);

  // Data received, call method to print
  if (driver.recv(buf, &buflen)) {
    printASCII(buf, buflen);
    // Check to see if catapult is launching
    if (launch (dummy value)) {
      // Should car stop???
      // launch
      pos = 90;
      catapult.write(pos);
      // Return catapult to original position
      for (pos = 90; pos >= 0; pos -= 1) { // goes from 90 degrees to 0 degrees
        catapult.write(pos); 
      }
    }
    // Decode the recieved data and formulate it into a power value for each wheel
    // Consider adding extra value to whichever wheel tends to lag, to balance the motors
    // If backwards (this if is a stand-in for whichever value will determine forward or backwards) 
    if (RSPD < 0) {
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
  } else {
    digitalWrite(RWhFwdPin, LOW);
    digitalWrite(LWhFwdPin, LOW);
    digitalWrite(RWhBwdPin, LOW);
    digitalWrite(LWhBwdPin, LOW);
    analogWrite(RWhPWMPin, 0);
    analogWrite(LWhPWMPin, 0);
  }
}

// Method to print ASCII text of message to Serial monitor
void printASCII(const uint8_t* buf, uint8_t buflen) {
  uint8_t i;
  sentMsg = "";
  for (i = 0; i < buflen; i++) {
    Serial.write(buf[i]);
    // Forms string from decimal values received and stores in sentMsg
    sentMsg = sentMsg + char(buf[i]);
  }
  Serial.println("");
}
