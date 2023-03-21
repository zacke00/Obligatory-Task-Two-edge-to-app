/**
 * wifi libraries
 * <WiFi.h> - WiFi library for Arduino
 * 
 * for API connection
 * <WiFiClient.h> - WiFi client library for Arduino
 * 
 * For insertion into API
 * <ArduinoJson.h> - ArduinoJson library for Arduino
 * 
 * Accelometer librarys
 * <SPI.h> - SPI library for Arduino
 * <Adafruit_LIS3DH.h> - Adafruit LIS3DH library for Arduino
 * <Adafruit_Sensor.h> - Adafruit Sensor library for Arduino
 * 
*/
#include <WiFi.h>
#include <WiFiClient.h>
#include <ArduinoJson.h>
#include <SPI.h>
#include <Adafruit_LIS3DH.h>
#include <Adafruit_Sensor.h>



//  **Accelometer**
//Define for the Accelometer
#define LIS3DH_CLK 13
#define LIS3DH_MISO 12
#define LIS3DH_MOSI 11
#define LIS3DH_MG2G_MULTIPLIER (0.001F)
// Used for hardware & software SPI
#define LIS3DH_CS 10

Adafruit_LIS3DH lis = Adafruit_LIS3DH();

/**       Ranges      **/
// Define variables for the highest and lowest x and y ranging
const float lowerRangeFrequency = -0.2;
const float upperRangeFrequency = 0.2;
const float lowerRangeAcceleration = 1;
const float upperRangeAcceleration = 2.4;

//Danger checking variables
const char* DangerResultPrint = "";
//-------------------------------


// **Connect to Wi-Fi**
// WiFi network name and password:
/*  *CHANGE IF NEEDED*  */
const char * ssid = "Student";
const char * password = "Kristiania1914";

// Internet address to send POST data to
const char * api_host = "172.26.80.126";
const int api_port = 3000;
const char* api_endpoint = "/";
//--------------------------------

// LED pin
const int LED_PIN = LED_BUILTIN;
//--------------------------------

// Elevator Name / address
const char * elevatorName = "Elevator";
const char * elevatorAddress = "Kongens gate 1";
// dagner variable for the API

DynamicJsonDocument doc(1024);
String payload;


/**
       **Connect to Wi-Fi**
 * @param ssid The SSID of the WiFi network to connect to
 * @param pwd The password of the WiFi network to connect to
 */
void connectToWiFi(const char * ssid, const char * pwd)
{
  Serial.println("Connecting to WiFi network: " + String(ssid));
  WiFi.begin(ssid, pwd); // start connecting to the wifi network

  while (WiFi.status() != WL_CONNECTED) 
  {
    // Blink LED while we're connecting:
    digitalWrite( LED_PIN, !digitalRead(LED_PIN) );
    delay(500);
    Serial.print(".");
  }

  Serial.println();
  Serial.println("Connected to WiFi");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
}


/**
 * Send a payload to an API server.
 * 
 * @param payload The payload to send to the API server
 */
void sendPayloadToAPI(String payload) {
  // Create a WiFiClient object to establish a TCP connection to the API server
  WiFiClient client;

  // Connect to the API server
  if (!client.connect(api_host, api_port)) {
    Serial.println("Error connecting to API server");
    return;
  }

  // TCP call to the API server
  client.print(String("POST ") + api_endpoint + " HTTP/1.1\r\n" +
               "Host: " + api_host + "\r\n" +
               "Content-Type: application/json\r\n" +
               "Content-Length: " + payload.length() + "\r\n" +
               "Connection: close\r\n\r\n" +
               payload + "\r\n");

  // Wait for the server to respond
  while (!client.available()) {
    delay(10);
  }

  // Read the response from the server and print it to the serial monitor
  String response = "";
  while (client.available()) {
    response += client.readString();
  }
  Serial.println("Payload:");
  Serial.println(payload);
  Serial.println("Response:");
  Serial.println(response);

  // Disconnect from the API server
  client.stop();
}


/* 
    **Setup**
*/   
void setup() {
  // **Connect to Wi-Fi**
  Serial.begin(115200);
  connectToWiFi(ssid, password);

  // **Accelometer**
  
  while (!Serial) yield();     // will pause Zero, Leonardo, etc until serial console opens
  lis.begin(0x18);
  
  lis.setRange(LIS3DH_RANGE_2_G);  // set the full scale range to ±2g
  
  Serial.print("Range = "); Serial.print(2 << lis.getRange());  
  Serial.println("G");
  //--------------------------------

}


void loop() {
  // **Accelometer**

  //  Danger levels
  float DangerFrequency_X_higher = 0;
  float DangerFrequency_X_lower = 0;

  float DangerFrequency_Y_higher = 0;
  float DangerFrequency_Y_lower = 0;

  float DangerFrequency_Z_Higher = 0;
  float DangerFrequency_Z_Lower = 1.1;

  float DangerResult = 0;
  //  Danger_calm = 3
  //  Danger_Heavy = 5
  //  Danger_Extreme = 6
  //  Worst_Case = 8

  //  Define variables for the highest and lowest x,y and z readings
  // X
  static float highest_x_Reading = 0;
  static float lowest_x_Reading = 0;
  // Y
  static float highest_y_Reading = 0;
  static float lowest_y_Reading = 0;
  // Z
  static float highest_z_Reading = 0;
  static float lowest_z_Reading = 0;
  // Define variable for the time the reading can occur / 2 secounds
  static unsigned long triggerStart= 0;
  // Flags for printing the highest and lowest readings
  static bool printedHighestReading = false;
  static bool printedLowestReading = false;

  // lis.read() for reading the accelometer
  lis.read();
  float xAccelometer = lis.x / 1000.0 * LIS3DH_MG2G_MULTIPLIER * 100;  // convert raw x acceleration value to percentage
  float yAccelometer = lis.y / 1000.0 * LIS3DH_MG2G_MULTIPLIER * 100;  // convert raw y acceleration value to percentage
  float zAccelometer = lis.z / 1000.0 * LIS3DH_MG2G_MULTIPLIER * 100;  // convert raw z acceleration value to percentage
  // ** checking values **
  // Print the x and y acceleration values only if its above 0.2 or below -0.2
  if(xAccelometer < lowerRangeFrequency || xAccelometer > upperRangeFrequency){
  Serial.print("X acceleration: ");
  Serial.print(xAccelometer);
  Serial.println("%");

  // Now we will mark the highest reading and print it out after 2 secounds
    if (xAccelometer > highest_x_Reading )
      {
        highest_x_Reading = xAccelometer;
        printedHighestReading = false;
        if(highest_x_Reading > 1.1)
        {
        DangerFrequency_X_higher = 8;
        }else{
        DangerFrequency_X_higher = 1;
        }
      }
      else if(xAccelometer < lowest_x_Reading)
      {
        lowest_x_Reading = xAccelometer;
        printedLowestReading = false;
        if(lowest_x_Reading < -1.1)
        {
        DangerFrequency_X_lower = 8;
        }else{
        DangerFrequency_X_lower = 1;
        }
      }
        triggerStart = millis();
    }

  //We will now do the exact same thing for the y axis
  if (yAccelometer < lowerRangeFrequency || yAccelometer > upperRangeFrequency)
  {
  Serial.print("y acceleration: ");
  Serial.print(yAccelometer);
  Serial.println("%");
        if (yAccelometer > highest_y_Reading)
    {
      highest_y_Reading = yAccelometer;
      printedHighestReading = false;
      if(highest_y_Reading > 1.1)
      {
      DangerFrequency_Y_higher = 8;
      }else{
      DangerFrequency_Y_higher = 1;
      }
    } 
    else if ( yAccelometer < lowest_y_Reading)
    {
      lowest_y_Reading = yAccelometer;
      printedLowestReading = false;
      if (lowest_y_Reading < -1.1)
      {
      DangerFrequency_Y_lower = 8;
      }else{
      DangerFrequency_Y_lower = 1;
      }
    }
      triggerStart = millis();
  }


  // We will now do the exact same thing for the z axis
  if (zAccelometer < lowerRangeAcceleration || zAccelometer > upperRangeAcceleration)
  {
  Serial.print("z acceleration: ");
  Serial.print(zAccelometer);
  Serial.println("%");
        if (zAccelometer > highest_z_Reading)
    {
      highest_z_Reading = zAccelometer;
      printedHighestReading = false;
      if (highest_z_Reading > 3)
      {
        DangerFrequency_Z_Higher = 8;
      }
      else{
        DangerFrequency_Z_Higher = 2;
      }
    } 
    else if ( zAccelometer < lowest_z_Reading)
    {
      lowest_z_Reading = zAccelometer;
      printedLowestReading = false;
      if (lowest_z_Reading < -1.5)
      {
        DangerFrequency_Z_Lower = 8;
      }else{
        DangerFrequency_Z_Lower = 2;
      }
    }
      triggerStart = millis();
  }

/*
  * Highest readings 
  * Flag checking if we will print or not. 
*/
if ((!printedHighestReading || !printedLowestReading) && (
  highest_x_Reading > upperRangeFrequency     // x Frequency
|| highest_y_Reading > upperRangeFrequency    // y Frequency
|| highest_z_Reading > upperRangeAcceleration // z Acceleration
|| lowest_x_Reading < lowerRangeFrequency     // x Frequency
|| lowest_y_Reading < lowerRangeFrequency     // y Frequency
|| lowest_z_Reading < lowerRangeAcceleration  // z Acceleration
) && millis() - triggerStart > 1000) {
  DangerResult = DangerFrequency_X_higher + DangerFrequency_X_lower + DangerFrequency_Y_higher + DangerFrequency_Y_lower + DangerFrequency_Z_Higher + DangerFrequency_Z_Lower;
  //Payload
  doc["name"] = elevatorName;
  doc["address"] = elevatorAddress;
  doc["Axis_X_high"] = highest_x_Reading;
  doc["Axis_Y_high"] = highest_y_Reading;
  doc["Axis_Z_high"] = highest_z_Reading;
  doc["Axis_X_low"] = lowest_x_Reading;
  doc["Axis_Y_low"] = lowest_y_Reading;
  if (lowest_z_Reading = 1.10)
  {
   doc["Axis_Z_low"] = 0; 
  }else {
  doc["Axis_Z_low"] = lowest_z_Reading;
  }
  if(DangerResult >= 8){
    DangerResultPrint = "Worst Case";
  }else if (DangerResult >= 6) {
    DangerResultPrint = "Extreme";
  }else if(DangerResult >= 4){
    DangerResultPrint = "Heavy";
  }else if(DangerResult >= 3){
    DangerResultPrint = "Calm";
  }else if(DangerResult < 3){
    DangerResultPrint = "No Danger";
  }
  doc["Danger"] = DangerResultPrint;
  //--------------------------------

  //Serializing the payload
  serializeJson(doc, payload);
  Serial.print("Danger =>  ");
  Serial.println(DangerResult);
  //Printing the payload

    if (highest_x_Reading > upperRangeFrequency || highest_y_Reading > upperRangeFrequency || highest_z_Reading > upperRangeFrequency) {
      Serial.println("Highest X reading: ");
      Serial.println(highest_x_Reading);
      Serial.println("Highest Y reading: ");
      Serial.println(highest_y_Reading);
      Serial.println("Highest Z reading: ");
      Serial.println(highest_z_Reading);
      printedHighestReading = true;   //flag for printing
    }
    if (lowest_x_Reading < lowerRangeFrequency || lowest_y_Reading < lowerRangeFrequency || lowest_z_Reading < lowerRangeFrequency) {
      Serial.println("Lowest X reading: ");
      Serial.println(lowest_x_Reading);
      Serial.println("Lowest Y reading: ");
      Serial.println(lowest_y_Reading);
      Serial.println("Lowest Z reading: ");
      if (lowest_z_Reading = 1.10){
        Serial.println(0.00);
      }
      else{
      Serial.println(lowest_z_Reading);
      }
      printedLowestReading = true;    //flag for printing
    }
    
    sendPayloadToAPI(payload); // send the payload to the API
    payload = ""; // reset the payload
    printedHighestReading = false; // reset flag
    printedLowestReading = false; // reset flag
}
  /**
   * Resetting the readings if they have been printed by checking if there has been a delay of 2 secounds
  */
  if(millis() - triggerStart > 1000){
    DangerFrequency_X_higher = 0;
    DangerFrequency_X_lower = 0;
    DangerFrequency_Y_higher = 0;
    DangerFrequency_Y_lower = 0;
    DangerFrequency_Z_Higher = 0;
    DangerFrequency_Z_Lower = 0;

    highest_x_Reading = 0;
    highest_y_Reading = 0;
    highest_z_Reading = 0;

    lowest_x_Reading = 0;
    lowest_y_Reading = 0;
    //1.1 because it needs to be higher than 1(lowerRangeAcceleration) to not state true for the if statement printing the readings
    lowest_z_Reading = 1.1;
    
    DangerResult = 0;

    printedHighestReading = false;
    printedLowestReading = false;
  }
delay(10);
}