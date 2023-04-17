/*
 * Code for Dedicated Node
 */

/*
 * Libraries included for ADXL345 accelerometer
 * Connect SCL to D1 of Node MCU
 * Connect SDA to D2 of Node MCU
 */

/*
 * Libraries included for BMP280 
 * Connect SCL to D4 of Node MCU
 * Connect SDA to D3 of Node MCU
 */
#include <Wire.h>
#include <SPI.h>

 
/*
 * Libraries included for Mesh Network
 * It also include library for finding MAC Address of ESP8266
 */
#include "painlessMesh.h"
#include <ESP8266WiFi.h>
#define   MESH_PREFIX     "ESP_MESH_WiFi"
#define   MESH_PASSWORD   "password"
#define   MESH_PORT       5555
#define   INTERVAL_SECONDS 7

/*
 * Address of the Server 
 * Registered - Is the MAC Address of the Node MCU registered by the server or not
 * Dedicated - Is the Node MCU dedicated for communicating with the server
 * dbId - Identity of the Node MCU as assigned by the server
 */
String server = "trees/";
#define totalTrees 5

Scheduler userScheduler; // to control your personal task
painlessMesh  mesh;
bool registered = false;
String dbId = "-1";
bool responseContd = false;
String responseBuffer = "";


uint32_t meshMACaddr[totalTrees];
String meshBuffer[totalTrees];
bool meshContd[totalTrees];
int entries = 0;

// User stub
void sendMessage() ; // Prototype so PlatformIO doesn't complain
Task taskSendMessage( TASK_SECOND * INTERVAL_SECONDS, TASK_FOREVER, &sendMessage );


/*
 * Checks if message is intended to the Dedicated Node
 * Otherwise broadcast the message so that it can reach the other Nodes
 */
void checkIfMine(){
    int index = responseBuffer.indexOf('|');
    String macid = responseBuffer.substring(index + 1, responseBuffer.length() - 1);
    if(macid == WiFi.macAddress()) {
      dbId = responseBuffer.substring(1, index);
      // Serial.println(dbId);
      registered = true;
    }else {
      mesh.sendBroadcast(responseBuffer);
    }
}


/*
 * Periodically check the buffer and update the response from Python App to Dedicated Node accordingly
 * Using ( and ) to demarcate replies from the Python app
 */
void cleanBuffer() {
    if(Serial.available()) {
      String temp = Serial.readString();
        for(int i = 0; i < temp.length() ; i++) {
            if(temp.charAt(i) == '$') continue;
            
            if(responseContd){
              if(temp.charAt(i) == ')'){
                responseContd = false;
                responseBuffer = responseBuffer + ")";
              }else{
                  responseBuffer = responseBuffer + temp.charAt(i);
              }
            }else if(temp.charAt(i) == '('){
              responseContd = true;
              responseBuffer = responseBuffer + "(";
            }
            
            if(!responseContd && responseBuffer.length() != 0){
              checkIfMine();
              responseBuffer = "";
            }
        }
    }
}


/*
 * Send Message to the Python App
 */
void postRequest(String &msg) {
    Serial.println(msg);
    cleanBuffer();
}


/*
 * If the node is registered then message should contain a specific url having unique id for the Node MCU
 * If the node is not registered then request for a id by broadcasting your MAC Address
 */
void sendMessage() {
    cleanBuffer();
  
    //String msg = "[" + server + String(dbId) + "/" + "|" + "{\"name\":\"test\",\"salary\":\"123\",\"age\":\"23\"}]";      
    //postRequest(msg); 
    //Serial.println(registered);
    if(!registered) {
      String mac = WiFi.macAddress();
      String msg = "[" + server + "|" + "{\"macid\":\""+mac+"\"}]";
      postRequest(msg); 
    }
}


/*
 * Receiving Message assuming packets comes in different packets
 */
void receivedCallback(uint32_t from, String &msg) {
    int index = 0;
    while(index < entries && meshMACaddr[index] != from) index++;
    
    if(index == entries) {
      meshMACaddr[index] = from;
      meshBuffer[index] = "";
      meshContd[index] = false;
      entries += 1;
    }

    for(int i = 0; i < msg.length() ; i++) {
        if(meshContd[index]){
          if(msg.charAt(i) == ']'){
            meshContd[index] = false;
            meshBuffer[index] = meshBuffer[index] + "]";
          }else{
              meshBuffer[index] = meshBuffer[index] + msg.charAt(i);
          }
        }else if(msg.charAt(i) == '['){
          meshContd[index] = true;
          meshBuffer[index] = meshBuffer[index] + "[";
        }
        
        if(!meshContd[index] && meshBuffer[index].length() != 0){
          postRequest(meshBuffer[index]);
          meshBuffer[index] = "";
        }
    }
}


void setup() {
  Serial.begin(115200);
  
  mesh.setDebugMsgTypes( ERROR | STARTUP );  // set before init() so that you can see startup messages
  mesh.init(MESH_PREFIX, MESH_PASSWORD, &userScheduler, MESH_PORT);
  mesh.onReceive(&receivedCallback);

  userScheduler.addTask(taskSendMessage);
  taskSendMessage.enable(); 
}


void loop() {
  // it will run the user scheduler as well
  mesh.update();
}
