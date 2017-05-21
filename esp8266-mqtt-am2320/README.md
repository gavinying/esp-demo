ESP8266 MQTT AM2320 Demo
============
A minimal viable package to demo how to use esp8266 core module and AM2320 temperature/humidity sensor.

## Hardware setup (NodeMCU ESP-12E)
![nodemcu+am2320](res/nodemcu-am2320.jpg)

## Software setup (Arduino)
Prerequisites:
1. Install [Arduino](https://www.arduino.cc/en/main/software) version 1.6.13
2. Install [esp8266 platform](https://github.com/esp8266/Arduino) through Boards Manager
3. Install [Adafruit DHT Humidity & Temperature Unified Sensor Library](https://github.com/adafruit/DHT-sensor-library)

Clone the demo code from [here](https://github.com/gavinying/esp-demo.git), change the WiFi settings and run in Arduino IDE.

Arduino console output
![Run the code](res/console-am2320.png)

Test with Thingsboard
![Test with Thingsboard](res/thingsboard-am2320.png)
