/* @file Receive.ino
|| @version 1.1
|| @author Heejoong
|| #2018-08-21
*/
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <Servo.h>

#define MAX_SIGNAL 2000
#define MIN_SIGNAL 1000
#define MOTOR_PIN 6
#define SERVO_PIN 5

RF24 radio(9, 10); //CE, CSN
Servo esc_motor;
Servo front_servo;

int joystick[3];
const byte address[6] = "00001"; 
//The address value can be changed to 5 strings, and the transmitter and the receiver must have the same address.
int motor;
int servo;

void setup() {
  Serial.begin(9600);
  Serial.println("Program begin...");
  Serial.println("This program will calibrate the ESC.");
  radio.begin();
  radio.openReadingPipe(0, address);
  radio.setPALevel(RF24_PA_MAX);
  //Sets the power level. If the distance between modules is close, set to minimum.
  //It can be set to RF24_PA_MIN, RF24_PA_LOW, RF24_PA_HIGH, RF24_PA_MAX etc. in order of close distance.
  //If you use RF24_PA_MAX, Recommended to use bypass capacitors at GND and 3.3V to have stable voltage during operation.
  
  esc_motor.attach(MOTOR_PIN);
  front_servo.attach(SERVO_PIN);
  radio.startListening();
  
  Serial.println("Now writing maximum output.");
  esc_motor.writeMicroseconds(MAX_SIGNAL);
  //delay(100);
  esc_motor.writeMicroseconds(MIN_SIGNAL);
  Serial.println("Sending minimum output");
  Serial.println("Please turn on the controller..."); //Set the idle value.
  //Do not operate the controller when turning it on. It cause calibration failure.
}
void loop() {
  if (radio.available()) {
    //The joystick recived from the nRF has a value betweent 1000 and 2000.
    radio.read(&joystick, sizeof(joystick));
    
    char buffer[20];
    sprintf(buffer , "X : %d, Y :  %d", joystick[1], joystick[2]);
    Serial.println(buffer);
    
    motor = joystick[1];
    servo = joystick[2];
    front_servo.write(servo);
    esc_motor.writeMicroseconds(motor);
  }
}
