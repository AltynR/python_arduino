#include <Wire.h>
#include <ArduinoJson.h>
String current = "";
String data = "";

void setup() {
    Wire.begin();               // join i2c bus (address optional for master)
    Serial.begin(9600);         // start serial for output
}

int ServoPin1 = 12;
int ServoPin2 = 13;

int PIRPin = A1;
int GasPin = A2;

int PIRValue = 0;
int GasValue = 0;

void loop() {
  
    
    bool x = true;
    
    Wire.requestFrom(8, 30);    // request 6 bytes from slave device #8
    
    while (Wire.available()) {  // slave may send less than requested
      char c = Wire.read();     // receive a byte as character
      if (c == '*') x = false;
      if (x) data += c;
    }
       
    if (!x) {
      DynamicJsonBuffer jsonBuffer;
      JsonObject& object = jsonBuffer.parseObject(data);
      if (object.success()){
        object.printTo(Serial);
        Serial.println();
      }
      data = "";
    }
    delay(200);
    
    if (current.length() == 0) {
      
      PIRValue = analogRead(PIRPin);
      GasValue = analogRead(GasPin);


      DynamicJsonBuffer jsonBuffer;
      JsonObject& root = jsonBuffer.createObject();

      JsonObject& room1 = root.createNestedObject("sensors");
      
      room1["pir"] = PIRValue;
      room1["gas"] = GasValue;
      root["room_name"] = "kitchen";
      root["home_id"] = 3;
      
      root.printTo(current);
      root.printTo(Serial);
      Serial.println();
      current += "*";
    }
    
    char buff[30];
    current.toCharArray(buff, 31);
    current.remove(0, 30);
    
    Wire.beginTransmission(8); // transmit to device #8
    Wire.write(buff);             // sends one byte
    Wire.endTransmission();    // stop transmitting

    delay(200);
}
