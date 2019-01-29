/* @file Send.ino
|| @version 1.2
|| @author Heejoong
|| #
*/
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

RF24 radio(9, 10); //CE, CSN

int joystick[3];
const byte address[6] = "00001"; 
//The address value can be changed to 5 strings, and the transmitter and the receiver must have the same address.
int inputPinX = A1;
int inputPinY = A2;

void setup() {
  Serial.begin(9600);
  radio.begin();
  radio.openWritingPipe(address); //Set address
  radio.setPALevel(RF24_PA_MAX); 
  //Sets the power level. If the distance between modules is close, set to minimum.
  //It can be set to RF24_PA_MIN, RF24_PA_LOW, RF24_PA_HIGH, RF24_PA_MAX etc. in order of close distance.
  //If you use RF24_PA_MAX, Recommended to use bypass capacitors at GND and 3.3V to have stable voltage during operation.
  
  radio.stopListening();  //transmitter..
  pinMode(inputPinX, INPUT);
  pinMode(inputPinY, INPUT);
}
void loop() {
  joystick[0] = analogRead(inputPinSW);
  joystick[1] = analogRead(inputPinX)*0.9+1000; //value of ESC
  joystick[2] = analogRead(inputPinY)/17+55;    //value of servo
  radio.write(&joystick, sizeof(joystick) ); 
  //Send to receiver
}

