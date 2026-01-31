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

void setup() {
  //serial setup
  Serial.begin(115200);
  //dht setup
  dht.begin();
  //ultra sonic sensor setup
  pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output
  pinMode(echoPin, INPUT); // Sets the echoPin as an Input

  uart_link_begin(1, 15200, RX_PIN, TX_PIN);
 
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

  SensorData status {
    .type = 1,
    .door_status = 1,
    .temp = 23,
    .humidity = 55,
    .food = 0,
    .water = 1
  };
  uart_send_sensor(status);


  CommandData cmd;
  if (uart_read_command(cmd)) {
    Serial.printf(
      "CMD=%u door=%u temp=%d\n",
      cmd.command,
      cmd.door_status,
      cmd.temperature
    );
  }

  
  delay(2000);
}

