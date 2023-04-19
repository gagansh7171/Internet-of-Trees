# Internet-of-Trees
[![Arduino](https://img.shields.io/badge/Arduino-1.8.19-yellow.svg)](https://www.arduino.cc/)
[![PainlessMesh](https://img.shields.io/badge/PainlessMesh-v1.5.0-blue)](https://github.com/gmag11/PainlessMesh)
[![Adafruit_Sensor](https://img.shields.io/badge/Adafruit__Sensor-v1.1.3-green)](https://github.com/adafruit/Adafruit_Sensor)
[![Adafruit ADXL345 Library](https://img.shields.io/badge/Adafruit%20ADXL345%20Library-v1.3.2-red)](https://github.com/adafruit/Adafruit_ADXL345)
[![Adafruit BMP280 Library](https://img.shields.io/badge/Adafruit%20BMP280%20Library-v2.6.6-orange)](https://github.com/adafruit/Adafruit_BMP280_Library)
[![Django Version](https://img.shields.io/badge/django-v3.2.4-brightgreen.svg)](https://www.djangoproject.com/)
[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)

[Gagan Sharma](https://gagansh7171.github.io/#/)<sup>&dagger;</sup>, [Md Junaid Mahmood](https://github.com/MdJunaidMahmood)<sup>&dagger;</sup>

## Contents

1. [Overview](#1-overview)
2. [Setup](#2-setup)
3. [Demonstration](#3-demonstration)

## 1. Overview
This repository contains the code for our work done as a part of *CSN-527: Internet of Things* course project. Our objective is to build a protoype to monitor every tree of a garden. Currently we support detection of following threats:
* Fall Detection - It triggers whenever a tree falls 
* Fall Threat - It triggers whenever a tree is shaking violently, possible due to strong winds or other natural/man-made reasons
* Chop Threat - It triggers whenever someone is deliberately trying to cut down a tree
* Fire Threat - It triggers whenever a tree catches fire

We maintain a dedicated NodeMCU termed as *Master Node* that can communicate with the web server. All other NodeMCU are termed as *Slave Node*. The web server keeps the information about the current status of every tree that can be viewed by the client over a website. Whenever any threat is being detected by a *Slave Node*, it broadcasts the information. Consequently the information reaches *Master Node* that sends it to the web server.

For establishing Mesh Topology, we have used [PainlessMesh](https://github.com/gmag11/PainlessMesh) library. We have used [Adafruit ADXL345](https://github.com/adafruit/Adafruit_ADXL345) and [Adafruit BMP280](https://github.com/adafruit/Adafruit_BMP280_Library) libraries for controlling ADXL345 Accelerometer and BMP280 Altimeter respectively. In our work we have paid special emphasis on network reliability and handled [Partial Packet Problem](https://stackoverflow.com/questions/10758547/partial-receipt-of-packets-from-socket-c) on our own. This reduces the dependency on third party libraries for necessary network infrastructure.

## 2. Setup
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
* Connect **SCL** and **SDA** of Accelerometer with pin D1 and D2 of the second chip respectively.

### Setup BMP Node
* Upload the code for [slave 2](https://github.com/gagansh7171/Internet-of-Trees/blob/master/arduino_code/treeSlaveNode2/treeSlaveNode2.ino) in the third chip.
* Connect **SCL** and **SDA** of BMP280 with pin D4 and D3 of the third chip respectively.

### Network Setup
* Setup a public Hotspot (preferably using Mobile Data) so that the master node PC is in reach and is connected in the network. 
* The server PC should also be connected in the same network. 
* Arduino chips should be reasonably separated so that they are within the reach of the mesh network setup.

### Frontend
* Clone iot_backend and run `npm install` to install all requirements.
* Run `npm start` to start the frontend server.
* The frontend machine must be on the same machine/network as the server.

## 3. Demonstration
* Working of project are explained in detail in [this presentation](https://docs.google.com/presentation/d/17LH_eSXLSI2FAAQ9XPW_T9FXX8uTQRpLUsLXDStAXqw/edit#slide=id.p).
* Demonstration of various features of our work is available [here](https://github.com/gagansh7171/Internet-of-Trees/tree/master/demonstration)
* The working pics of frontend visible to the client are available [here](https://github.com/gagansh7171/Internet-of-Trees/tree/master/demonstration/web_server/frontend).

## Acknowledgement

This repo is a part of our course project for CSN-527: Internet of Things under [Professor Rahul Thakur](https://cse.iitr.ac.in/~CSE/Rahul_Thakur) at CSE Department, IIT Roorkee. The code is open-sourced under the MIT License.

### Team Members (Group - 13)

- #### Gagan Sharma: 19114032

- #### Md Junaid Mahmood: 19116040
