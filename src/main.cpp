/*********
  Rui Santos
  Complete project details at https://RandomNerdTutorials.com/esp32-hc-sr04-ultrasonic-arduino/
  
  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files.
  
  The above copyright notice and this permission notice shall be included in all
  copies or substantial portions of the Software.
*********/
#include <Arduino.h>
#include <DHT.h>
#include <Stepper.h>
#include "../include/uartcom.h"

//DHT
#define DHTPIN 4
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

//UART definitions
#define RX_PIN 16
#define TX_PIN 17

//ultra sonic sensor
const int trigPin = 5;
const int echoPin = 18;
long duration;
float distanceCm;
float distanceInch;

//define sound speed in cm/uS
#define SOUND_SPEED 0.034
#define CM_TO_INCH 0.393701


//steps per revolution for stepper motor
const int stepsPerRevolution = 2048;
const int stepperSpeed = 200;

//pins for Stepper motor
#define IN1 32
#define IN2 33
#define IN3 25
#define IN4 26

//initialized stepper
Stepper myStepper(stepsPerRevolution, IN1, IN3, IN2, IN4);


void setup() {

  myStepper.setSpeed(5);
  //serial setup
  Serial.begin(115200);
  //dht setup
  dht.begin();
  //ultra sonic sensor setup
  pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output
  pinMode(echoPin, INPUT); // Sets the echoPin as an Input

  //begin serial for communication with central board
  Serial2.begin(115200, SERIAL_8N1, RX_PIN, TX_PIN);
}

void loop() {
  int temp = dht.readTemperature(true);
  int humidity = dht.readHumidity();
  Serial.print("Humidity: ");
  Serial.print(humidity);
  Serial.print("%  |  Temp: ");
  Serial.print(temp);
  Serial.print("°F");

  //sonic 
  // Clears the trigPin
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  // Sets the trigPin on HIGH state for 10 micro seconds
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  
  // Reads the echoPin, returns the sound wave travel time in microseconds
  duration = pulseIn(echoPin, HIGH);
  
  // Calculate the distance
  distanceCm = duration * SOUND_SPEED/2;
  
  // Convert to inches
  distanceInch = distanceCm * CM_TO_INCH;
  
  // Prints the distance in the Serial Monitor
  Serial.print("Distance (cm): ");
  Serial.println(distanceCm);
  Serial.print("Distance (inch): ");
  Serial.println(distanceInch);

  bool food = 0;
  bool water = 0;
  //Serial2.printf("%d,%d,%d,%d\n", temp, humidity,food, water);
  //Serial2.print("Hello World!");
  Serial2.printf("{\"temp\":%d,\"humidity\":%d,\"food\":%d,\"water\":%d}\n",
               temp, humidity, food, water);

  
  if(Serial2.available())
  {
    String msg = Serial2.readStringUntil(',');
    Serial.println(msg);
    uint8_t command = msg.toInt();

    //read in desired door state
    msg = Serial2.readStringUntil(',');
    uint8_t state = msg.toInt();

    //value to change temp to if correct state
    msg = Serial2.readStringUntil(',');
    int set_temp = msg.toInt();

    switch(command)
    {
      case 1:
        myStepper.step(stepsPerRevolution);  
        break; 
      case 2:
        myStepper.step(-stepsPerRevolution);
      case 3:
        break;
    }

  }
  delay(2000);
}

