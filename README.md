# Internet-of-Trees
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
* The working pics of frontend are available [here](https://github.com/gagansh7171/Internet-of-Trees/tree/master/pics).
