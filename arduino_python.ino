#include <ArduinoJson.h>
#include "dht.h"
#define dht_apin A4 // Analog Pin sensor is connected to
 
dht DHT;
String data;
String readString; //main captured String 
String pirLED; //data String
String gasLED;
String humidityLED;
String lightLED;

int ind1; // , locations
int ind2;
int ind3;
int ind4;
//StaticJsonBuffer<200> jsonBuffer1;
//JsonObject& data = jsonBuffer1.createObject();


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
  Serial.begin(9600); //initialize serial COM at 9600 baudrate
//  pinMode(LED, OUTPUT); //make the LED pin (13) as output
//  digitalWrite (LED, LOW);
  //Serial.println("Hi!, I am Arduino");
}
 
void loop() {
  delay(1000);
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
  
  root.printTo(Serial);
  Serial.println();
  delay(500);
  while (Serial.available()){
      char c = Serial.read();
      if (c == '*') {
      
 
      ind1 = readString.indexOf(',');  //finds location of first ,
      pirLED = readString.substring(0, ind1);   //captures first data String
      ind2 = readString.indexOf(',', ind1+1 );   //finds location of second ,
      gasLED = readString.substring(ind1+1, ind2+1);   //captures second data String
      ind3 = readString.indexOf(',', ind2+1 );
      humidityLED = readString.substring(ind2+1, ind3+1);
      ind4 = readString.indexOf(',', ind3+1 );
      lightLED = readString.substring(ind3+1); //captures remain part of data after last ,

//      Serial.print("pirLED = ");
//      Serial.println(pirLED); 
//      Serial.print("gasLED = ");
//      Serial.println(gasLED);
//      Serial.print("humidityLED = ");
//      Serial.println(humidityLED);
//      Serial.print("lightLED = ");
//      Serial.println(lightLED);
//      Serial.println();
//      Serial.println();

      if(pirLED[0] == '1') {
      digitalWrite(LEDpir, HIGH);  
      }else{
        digitalWrite(LEDpir, LOW); 
        }
      if(gasLED[0] == '1') {
      digitalWrite(LEDgas, HIGH);  
      }else{
        digitalWrite(LEDgas, LOW); 
        }
      if(humidityLED[0] == '1') {
      digitalWrite(LEDhumidity, HIGH);  
      }else{
        digitalWrite(LEDhumidity, LOW);  
        }
      if(lightLED[0] == '1') {
      digitalWrite(LEDlight, HIGH);  
      }else {
        digitalWrite(LEDlight, LOW);  
        }
      readString=""; //clears variable for new input
      pirLED="";
      gasLED="";
      humidityLED="";
      lightLED="";
    }  
    else {     
      readString += c; //makes the string readString
    }
  }
//  
//  if (data == '1'){
//  digitalWrite (LED, HIGH);
//  delay(500);
//  }
//  else if (data == '0'){
//  digitalWrite (LED, LOW);
//  delay(500);
//  }
  
}
