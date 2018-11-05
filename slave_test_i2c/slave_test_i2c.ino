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
#define dht_apin A1 // Analog Pin sensor is connected to
 
dht DHT;
String memory;
String data;
String readString; //main captured String 
String pirLED; //data String
String gasLED;
String humidityLED;
String lightLED;


//int LEDpir = 11;
//int LEDgas = 12;
//int LEDhumidity = 13;
//int LEDlight = 10;
int PIRPin = A0; // select the input pin for LDR
//int GasPin = A1;
int LightPin = A2;
int HumidityPin = dht_apin;
int ServoPin = 8;


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
  pinMode(ServoPin, OUTPUT);
}

void loop() {
  delay(100);
  
}

// function that executes whenever data is requested by master
// this function is registered as an event, see setup()
void requestEvent() {
  if(memory.length() == 0){
     PIRValue = analogRead(PIRPin);
     //GasValue = analogRead(GasPin);
     LightValue = analogRead(LightPin);
     DHT.read11(dht_apin);
     //HumidityValue = analogRead(HumidityPin);
     HumidityValue = DHT.humidity;
     TemperatureValue = DHT.temperature;
     digitalWrite(ServoPin, HIGH);
     delayMicroseconds(1500);
     digitalWrite(ServoPin, LOW);

     DynamicJsonBuffer jsonBuffer;
    JsonObject& root = jsonBuffer.createObject();
    
    JsonObject& room = root.createNestedObject("sensors");
    room["pir"] = PIRValue;
    room["gas"] = GasValue;
    room["humidity"] = HumidityValue;
    room["temperature"] = TemperatureValue;
    room["light"] = LightValue;

    
    root["room_name"] = "bedroom";
    root["home_id"] = 3; 
    
    //root.prettyPrintTo(Serial);
  
    
    root.printTo(memory);

    //StaticJsonBuffer<200> jsonBuffer1;
    
    
    memory += "*";
    //memory = String(PIRValue) + "," + String(GasValue) + "," + String(LightValue) + "," +re String(HumidityValue) + "," + String(TemperatureValue) + "Hi Serik how are you. This is test code. Do you see it all.*";
    //respond with message of 6 bytes  
    }

     char buff[30];

     memory.toCharArray(buff, 31);
     memory.remove(0, 30);
     Wire.write(buff);
}

void receiveEvent(int howMany) {
  bool x = true;
 
  while (Wire.available()) { // loop through all but the last
    char c = Wire.read(); // receive byte as a character
    if (c == '*') x = false;
     if (x) data += c;
  }

  if (!x){
    
    DynamicJsonBuffer jsonBuffer;
    //Serial.println("Data 1 is: ");
    //Serial.println(data);
    JsonObject& object = jsonBuffer.parseObject(data);
    //Serial.println(object.success());
    //Serial.println("Data 2 is: ");
    Serial.println(data);
    //Serial.println("Serial is: ");
    //object.printTo(Serial);
    Serial.println("");
    data = "";
  }
}
