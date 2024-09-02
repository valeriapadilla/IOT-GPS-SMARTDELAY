#include <Arduino.h>
#include <WiFi.h>
#include <Wire.h>
#include <TinyGPS++.h>
#include "ClosedCube_HDC1080.h"

const char* ssid ="UPBWiFi";
const char* host = "10.38.32.137";
const uint16_t port = 80;
static const uint32_t GPSBaud = 9600;
WiFiClient client;
TinyGPSPlus gps;
ClosedCube_HDC1080 sensor;
float latitud;
float longitud;


// put function declarations here:
float prunning(float param[]);
float readingHum(int loop);
float readingTem(int loop);
void senddata ();

void setup() {
  Wire.begin(4,0);
  sensor.begin(0x40);
  delay(20);
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid);
  Serial1.begin(9600, SERIAL_8N1, 34, 12);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
}


float Prunning(float param[])
{
  float mean;
  float total = 0;
  for(int i=0; i<3; i++){
    total = total + param[i];
  }
  mean = total/3;
  delay(100);
  return mean;
}

float readingTem(int loop)
{
  float chrip[loop];

  for (int i = 0; i < loop; i++)
  {
    float var = sensor.readTemperature();
    delay(100);
    chrip[i] = var;
    delay(100);
  }
    return Prunning(chrip);
}

float readingHum(int loop)
{
  float chrip[loop];
  for (int i = 0; i < loop; i++)
  {
    float var = sensor.readHumidity();
    delay(100);
    chrip[i] = var;
    delay(100);
  }
    return Prunning(chrip);
}

// Function definition
static void smartDelay(long ms)                
{
  unsigned long start = millis();
  do
  {
    while (Serial1.available())
      gps.encode(Serial1.read());
  } while (millis() - start < ms);
}

void sendData() {
  //Bundling
   String message = "{\"id\": \"point17\" , \"lat\":" + String(gps.location.lat(), 6) + ", \"lon\":" + String(gps.location.lng(), 6) + ", \"temperatura\":" + String(readingTem(3)) +", \"humedad\":" + String(readingHum(3)) + "}";

  if (client.connect(host, port)) { 
    
    client.println("POST /update_data HTTP/1.1");
    client.println("Host: 10.38.32.137");
    client.println("Content-Type: application/json");
    client.print("Content-Length: ");
    client.println(message.length());
    client.println("");  
    client.println(message);
  }
 
  delay(500);
  while (client.available()) {
    char c = static_cast<char>(client.read());
    Serial.print(c);
  }
  
  Serial.println();
}

void loop() {
  delay(10000);
  smartDelay(100);
  Serial.println("Midiendo temp y humedad");
  Serial.println(readingTem(3));
  Serial.println(readingHum(3));

  longitud = (gps.location.lng());
  latitud = gps.location.lat();
  Serial.println("Latitud" + String(latitud,6));
  Serial.println("LOngutd: " + String(longitud,6));
  sendData();
}
