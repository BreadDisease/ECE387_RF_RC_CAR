#include <ELECHOUSE_CC1101.h>

#define size 3

byte TX_buffer[size] = {0};
byte i;

int VRx = A0;
int VRy = A1;
int SW = 3;

int xPosition = 0;
int yPosition = 0;
int SW_state = 0;
int mapX = 0;
int mapY = 0;

void setup() {
  Serial.begin(9600);
  pinMode(VRx, INPUT);
  pinMode(VRy, INPUT);
  pinMode(SW, INPUT_PULLUP);
  ELECHOUSE_cc1101.Init();
  //  for (i = 0; i < size; i++) {
  //    TX_buffer[i] = i;
  //  }
}

void loop() {
  xPosition = analogRead(VRx) / 4;
  yPosition = analogRead(VRy) / 4;
  SW_state = digitalRead(SW);
  TX_buffer[0] = xPosition;
  TX_buffer[1] = yPosition;
  TX_buffer[2] = SW_state;
  Serial.println(TX_buffer[0]);
  Serial.println(TX_buffer[1]);
  Serial.println(TX_buffer[2]);
  ELECHOUSE_cc1101.SendData(TX_buffer, size);
  delay(250);
}
