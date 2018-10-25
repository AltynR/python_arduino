import serial #Serial imported for Serial communication
import time #Required to use delay functions
import json
import urllib3

ArduinoSerial = serial.Serial("COM3",9600) #Create Serial port object called arduinoSerialData
time.sleep(2) #wait for 2 secounds for the communication to get established
http = urllib3.PoolManager()
#print (ArduinoSerial.readline()) #read the serial data and print it as line
#print ("Enter 1 to turn ON LED and 0 to turn OFF LED")


while 1: #Do this forever
    time.sleep(0.5)
    #var = input() #get input from user
    #print ("you entered"), var #print the intput for confirmation
    sensorReading = ArduinoSerial.readline()
    #print(sensorValue)
    #print(sensorValue.decode('utf-8'))
    sensorValue = sensorReading.decode('utf-8')
    #print(type(sensorValue))
    print(sensorValue)

    j = json.loads(sensorValue)
    pir = j['pir']
    gas = j['gas']
    humidity = j['humidity']
    temperature = j['temperature']
    light = j['light']
    


    r = http.request('POST', '10.101.55.192:8081/query', fields={"pir":pir, "gas":gas, "humidity":humidity, "temperature":temperature, "light":light})
    #print("  ")
    #print(len(sensorValue))
    pirInt = int(pir)
    gasInt = int(gas)
    humidityInt = int(humidity)
    # temperatureInt = int(temperature)
    lightInt = int(light)

    pirLED = '0'
    gasLED = '0'
    humidityLED = '0'
    temperatureLED = '0'
    lightLED = '0'

    if(pirInt > 690):
        pirLED = '1';
    if(gasInt > 300):
        gasLED = '1';
    if(humidityInt > 30):
        humidityLED = '1';
    if (lightInt > 300): #if the value is 1
        lightLED = '1';

    # data = {}
    # data['pirLED'] = str(pirLED);
    # data['gasLED'] = str(gasLED);
    # data['humidityLED'] = str(humidityLED);
    # data['lightLED'] = str(lightLED);
    # json_data = json.dumps(data)
    data = pirLED + "," + gasLED + "," + humidityLED + "," + lightLED + "*";
    #print(sensorReading)
    ArduinoSerial.write(data.encode())
    #ArduinoSerial.write(json_data.encode()) #send 1
    #      print ("LED turned ON")
    #      #time.sleep(0.5)
    #
    # else: #if the value is 0
    #      #var = '0';
    #      ArduinoSerial.write('0'.encode()) #send 1
    #      print ("LED turned OFF")
         #time.sleep(0.5)
