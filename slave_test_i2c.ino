// Wire Slave Sender
// by Nicholas Zambetti <http://www.zambetti.com>

// Demonstrates use of the Wire library
// Sends data as an I2C/TWI slave device
// Refer to the "Wire Master Reader" example for use with this

// Created 29 March 2006

// This example code is in the public domain.


#include <Wire.h>

#include <ArduinoJson.h>
#include "dht.h"
#define dht_apin A4 // Analog Pin sensor is connected to
 
dht DHT;
String memory;
String readString; //main captured String 
String pirLED; //data String
String gasLED;
String humidityLED;
String lightLED;


int LEDpir = 11;
int LEDgas = 12;
int LEDhumidity = 13;
int LEDlight = 10;
int PIRPin = A0; // select the input pin for LDR
int GasPin = A1;
int LightPin = A2;
int HumidityPin = dht_apin;



int PIRValue = 0; // variable to store the value coming from the sensor
int GasValue = 0;
int LightValue = 0;
int HumidityValue = 0;
int TemperatureValue = 0;

void setup() {
  Wire.begin(8);                // join i2c bus with address #8
  Wire.onReceive(receiveEvent); // register event
  Wire.onRequest(requestEvent); // register event
  Serial.begin(9600);
}

void loop() {
  delay(100);
  
}

// function that executes whenever data is requested by master
// this function is registered as an event, see setup()
void requestEvent() {
  if(memory.length() == 0){
     PIRValue = analogRead(PIRPin);
     GasValue = analogRead(GasPin);
     LightValue = analogRead(LightPin);
     DHT.read11(dht_apin);
     //HumidityValue = analogRead(HumidityPin);
     HumidityValue = DHT.humidity;
     TemperatureValue = DHT.temperature;

    StaticJsonBuffer<200> jsonBuffer;
    JsonObject& root = jsonBuffer.createObject();
    root["pir"] = PIRValue;
    root["gas"] = GasValue;
    root["humidity"] = HumidityValue;
    root["temperature"] = TemperatureValue;
    root["light"] = LightValue;
    
    root.printTo(memory);
    memory += "*";
    //memory = String(PIRValue) + "," + String(GasValue) + "," + String(LightValue) + "," + String(HumidityValue) + "," + String(TemperatureValue) + "Hi Serik how are you. This is test code. Do you see it all.*";
    //respond with message of 6 bytes  
    }

     char buff[30];

     memory.toCharArray(buff, 31);
     memory.remove(0, 30);
     Wire.write(buff);
}

void receiveEvent(int howMany) {
  bool x = true;
  String data = "";
  while (Wire.available()) { // loop through all but the last
    char c = Wire.read(); // receive byte as a character
    if (c == '*') x = false;
     if (x) data += c;
  }
  Serial.print(data);
  if (!x) Serial.println("");
}
