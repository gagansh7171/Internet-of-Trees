/*
 * Code for Slave Node
 */

/*
 * Libraries included for ADXL345 accelerometer
 * Connect SCL to D1 of Node MCU
 * Connect SDA to D2 of Node MCU
 */
#include <Wire.h>
#include <Adafruit_Sensor.h> 
#include <Adafruit_ADXL345_U.h>

Adafruit_ADXL345_Unified accel = Adafruit_ADXL345_Unified();

 
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

Scheduler userScheduler; // to control your personal task
painlessMesh  mesh;
bool registered = false;
String dbId = "-1";
//bool responseContd = false;
//String responseBuffer = "";

uint32_t dedicatedMACaddr = 0;
bool dedicatedMACfound = false;
String meshBuffer = "";
bool meshContd = false;
double accelerationThreshold = 250.0;

double vibrationLimit = 145.0;
double vibrationRange = 35.0;
int vibrationThreshold = 3;
int streak = 0;

// User stub
void sendMessage() ; // Prototype so PlatformIO doesn't complain
Task taskSendMessage( TASK_SECOND * INTERVAL_SECONDS, TASK_FOREVER, &sendMessage );

/*
 * Code to check whether there is some error or not 
 */

void updateStatus() {
    sensors_event_t event; 
    accel.getEvent(&event);

    // Code for Accelerometer
    double acclX = event.acceleration.x;
    double acclY = event.acceleration.y;
    double acclZ = event.acceleration.z;

    Serial.println(acclX);
    Serial.println(acclY);
    Serial.println(acclZ);

    double netAccln = acclX*acclX + acclY*acclY + acclZ*acclZ;
    Serial.println(netAccln);
    if(netAccln >= accelerationThreshold) {
        String alertServer = "[" + server + dbId + "/|{\"id\":\"" + dbId + "\",\"state\":\"3\"" + "}]";
        postRequest(alertServer);
    }

    if((netAccln >= vibrationLimit - vibrationRange) && (netAccln <= vibrationLimit + vibrationRange)) {
      streak = streak + 1;
    }else {
      streak = streak - 1;
      if(streak < 0) {
        streak = 0;
      }
    }

    Serial.println("+++++++++++");
    Serial.println(streak);
    Serial.println("+++++++++++");

    if(streak >= vibrationThreshold) {
        String alertServer = "[" + server + dbId + "/|{\"id\":\"" + dbId + "\",\"state\":\"2\"" + "}]";
        postRequest(alertServer);
    }
}



/*
 * Checks if message is intended to the Dedicated Node
 * Otherwise broadcast the message so that it can reach the other Nodes
 */
void checkIfMine(){
    int index = meshBuffer.indexOf('|');
    String macid = meshBuffer.substring(index + 1, meshBuffer.length() - 1);
    if(macid == WiFi.macAddress()) {
      dbId = meshBuffer.substring(1, index);
      // Serial.println(dbId);
      registered = true;
    }
}


/*
 * Send Message to the Python App
 */
void postRequest(String &msg) {
    mesh.sendBroadcast(msg);
}


/*
 * If the node is registered then message should contain a specific url having unique id for the Node MCU
 * If the node is not registered then request for a id by broadcasting your MAC Address
 */
void sendMessage() {
    //String msg = "[" + server + String(dbId) + "/" + "|" + "{\"name\":\"test\",\"salary\":\"123\",\"age\":\"23\"}]";      
    //postRequest(msg); 
    // Serial.println(registered);

    updateStatus();
    
    if(!registered) {
      String mac = WiFi.macAddress();
      String msg = "[" + server + "|" + "{\"macid\":\""+mac+"\"}]";
      // Serial.print("Send: ");
      // Serial.println(msg);
      postRequest(msg); 
    }
}


/*
 * Receiving Message assuming packets comes in different packets
 */
void receivedCallback(uint32_t from, String &msg) {
    if(dedicatedMACfound) {
      if(from != dedicatedMACaddr) return;
    }else {
      if(msg.charAt(0) != '(') return;
      else {
        dedicatedMACfound = true;
        dedicatedMACaddr = from;
      }
    }

    // Serial.print("Receive: ");
    // Serial.println(msg);
    for(int i = 0; i < msg.length() ; i++) {
        if(meshContd){
          if(msg.charAt(i) == ')'){
            meshContd = false;
            meshBuffer = meshBuffer + ")";
          }else{
              meshBuffer = meshBuffer + msg.charAt(i);
          }
        }else if(msg.charAt(i) == '('){
          meshContd = true;
          meshBuffer = meshBuffer + ")";
        }
        
        if(!meshContd && meshBuffer.length() != 0){
          checkIfMine();
          meshBuffer = "";
        }
    }
}

void setup() {
    Serial.begin(115200);

    // Wire.begin(D2, D1);
    if(!accel.begin(0x53)) {
        //Serial.println("No valid accelerometer found");
        while(1);
    }
    //Serial.println("Accelerometer Connected");

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
