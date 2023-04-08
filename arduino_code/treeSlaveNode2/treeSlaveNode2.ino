/*
 * Code for Slave Node
 */

/*
 * Libraries included for BMP280 
 * Connect SCL to D4 of Node MCU
 * Connect SDA to D3 of Node MCU
 */
#include <Wire.h>
#include <SPI.h>
#include <Adafruit_BMP280.h>

Adafruit_BMP280 bmp;

 
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
int normalHeight = -1;
int thresholdTemperature = 30;

// User stub
void sendMessage() ; // Prototype so PlatformIO doesn't complain
Task taskSendMessage( TASK_SECOND * INTERVAL_SECONDS, TASK_FOREVER, &sendMessage );


/*
 * Code to check whether there is some error or not 
 */

void updateStatus() {

    // Code for BMP 280
    double temperature = bmp.readTemperature();
    double altitude = bmp.readAltitude(1013.25);

    // Serial.println(temperature);
    // double pressure = bmp.readPressure();

    Serial.println(altitude);
    Serial.println(normalHeight);
    if(temperature >= thresholdTemperature) {
        String alertServer = "[" + server + dbId + "/|{\"id\":\"" + dbId + "\",\"state\":\"1\"" + "}]";
        postRequest(alertServer);
    }else if(altitude < normalHeight) {
        String alertServer = "[" + server + dbId + "/|{\"id\":\"" + dbId + "\",\"state\":\"4\"" + "}]";
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
    Wire.begin(D3, D4);
    if (!bmp.begin(0x76)) {
      //Serial.println("No valid BMP 280 sensor found");
      while (1);
    }
    Serial.println("BMP 280 sensor Connected");
  
    /* Default settings from datasheet. */
    bmp.setSampling(Adafruit_BMP280::MODE_NORMAL,     /* Operating Mode. */
                  Adafruit_BMP280::SAMPLING_X2,     /* Temp. oversampling */
                  Adafruit_BMP280::SAMPLING_X16,    /* Pressure oversampling */
                  Adafruit_BMP280::FILTER_X16,      /* Filtering. */
                  Adafruit_BMP280::STANDBY_MS_500); /* Standby time. */

    double altitude = bmp.readAltitude(1013.25);
    normalHeight = altitude;
    
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
