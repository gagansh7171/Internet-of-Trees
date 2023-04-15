# Internet-of-Trees
[![Arduino](https://img.shields.io/badge/Arduino-1.8.19-yellow.svg)](https://www.arduino.cc/)
[![PainlessMesh](https://img.shields.io/badge/PainlessMesh-v1.5.0-blue)](https://github.com/gmag11/PainlessMesh)
[![Adafruit_Sensor](https://img.shields.io/badge/Adafruit__Sensor-v1.1.3-green)](https://github.com/adafruit/Adafruit_Sensor)
[![Adafruit BMP280 Library](https://img.shields.io/badge/Adafruit%20BMP280%20Library-v2.6.6-orange)](https://github.com/adafruit/Adafruit_BMP280_Library)
[![Django Version](https://img.shields.io/badge/django-v3.2.4-brightgreen.svg)](https://www.djangoproject.com/)
[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)

[Gagan Sharma](https://gagansh7171.github.io/#/)<sup>&dagger;</sup>, [Md Junaid Mahmood](https://github.com/MdJunaidMahmood)<sup>&dagger;</sup>


## Setup
Setup of the project requires -
* 3 NodeMCU ESP8266 chip
* 1 BMP280 Altimeter
* 1 ADXL345 Accelerometer
* A PC to setup the server and another to run [main.py](https://github.com/gagansh7171/Internet-of-Trees/blob/master/arduino_code/main.py) (this purpose can be filled by only one device also).
* An End-User device to operate the front-end
### Server
* Make sure the PC is connected to a public hotspot.
* Clone the repository and enter iot_backend.
* Run entry.sh in the directory.
```
cd iot_backend
./entry.sh
```
* The server has started on port 8000.
* Note the ip_address of the server and set it in [main.py](https://github.com/gagansh7171/Internet-of-Trees/blob/master/arduino_code/main.py).

### Setup Master Node
* Upload the code for [master node](https://github.com/gagansh7171/Internet-of-Trees/blob/master/arduino_code/treeDedicatedNode/treeDedicatedNode.ino) in one of the chips using Arduino IDE.
* Copy [main.py](https://github.com/gagansh7171/Internet-of-Trees/blob/master/arduino_code/main.py) in a PC and connect the chip with a serial wire with the system.
* The chip and the system are supposed to communicate via serial communication.

### Setup Accelerometer Node
* Upload the code for [slave 1](https://github.com/gagansh7171/Internet-of-Trees/blob/master/arduino_code/treeSlaveNode1/treeSlaveNode1.ino) in the second chip.
* Connect Accelerometer with port D2, D1.

### Setup BMP Node
* Upload the code for [slave 2](https://github.com/gagansh7171/Internet-of-Trees/blob/master/arduino_code/treeSlaveNode2/treeSlaveNode2.ino) in the third chip.
* Connect BMP280 with port D3, D4.

### Network Setup
* Setup a public Hotspot (preferably using Mobile Data) so that the master node PC is in reach and is connected in the network. 
* The server PC should also be connected in the same network. 
* Arduino chips should reasonably separated so that they are within the reach of the mesh network setup.

### Frontend
* Clone iot_backend and run `npm install` to install all requirements.
* Run `npm start` to start the frontend server.
* The frontend machine must be on the same machine/network as the server.

## Demonstration
* The demo and working of project are explained in [this presentation](https://docs.google.com/presentation/d/17LH_eSXLSI2FAAQ9XPW_T9FXX8uTQRpLUsLXDStAXqw/edit#slide=id.p).
* The working pics of frontend are available [here](https://github.com/gagansh7171/Internet-of-Trees/tree/master/demonstration/web_server/frontend).
