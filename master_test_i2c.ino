#include <Wire.h>

String memo = "{\"pir\":236,\"gas\":227,\"humidity\":0,\"temperature\":0,\"light\":222}*";
String current = "";
void setup() {
    Wire.begin();               // join i2c bus (address optional for master)
    Serial.begin(9600);         // start serial for output
}

void loop() {
    
    String data = "";
    bool x = true;
    
    Wire.requestFrom(8, 30);    // request 6 bytes from slave device #8
    
    while (Wire.available()) {  // slave may send less than requested
      char c = Wire.read();     // receive a byte as character
      if (c == '*') x = false;
      if (x) data += c;
    }
        
    Serial.print(data);
    if (!x) Serial.println("");
    delay(500);
   
    if (current.length() == 0) {
      current += memo;
    }
    char buff[32];
    current.toCharArray(buff, 32);
    current.remove(0, 32);
    
    Wire.beginTransmission(8); // transmit to device #8
    Wire.write(buff);             // sends one byte
    Wire.endTransmission();    // stop transmitting
}
