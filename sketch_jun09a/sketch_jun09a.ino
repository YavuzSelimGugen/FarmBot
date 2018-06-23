#include <ESP8266WiFi.h>
#define BLYNK_PRINT Serial
#include <BlynkSimpleEsp8266.h>
#include "DHT.h"
#define DHTPIN D3
#define DHTTYPE DHT11
#define Red D0

char auth[] = "9c6d0f48e8314e88bb5b1ba7e22f22d3";
char ssid[] = "1637";
char pass[] = "lalezar71";

DHT dht(DHTPIN, DHTTYPE);
 
#define soilMoisterPin A0
#define soilMoisterVcc D8
int soilMoister = 0;

float hum = 0;
float temp = 0;
float ylVal = 0;

void setup() {
  Serial.begin(115200);
  Blynk.begin(auth, ssid, pass);
  dht.begin();
  digitalWrite (soilMoisterVcc, LOW);
  pinMode(Red, OUTPUT);
}

void loop() {
  delay(1000);
  Blynk.run(); 
  digitalWrite(Red, LOW);
  delay(2000);
  digitalWrite(Red, HIGH);
  getDht();
  getSoilMoisterData();
}

void getDht(void)
{
  float tempIni = temp;
  float humIni = hum;
  temp = dht.readTemperature();
  hum = dht.readHumidity();
  if (isnan(hum) || isnan(temp))   // Check if any reads failed and exit early (to try again).
  {
    Serial.println("Failed to read from DHT sensor!");
    temp = tempIni;
    hum = humIni;
    return;
  }
  Blynk.virtualWrite(V1,temp);    
  Blynk.virtualWrite(V0,hum);  
  }

void getSoilMoisterData(void)
{
  
  soilMoister = 0;
  digitalWrite (soilMoisterVcc, HIGH);
  delay (500);
  int N = 3;
  for(int i = 0; i < N; i++) // read sensor "N" times and get the average
  {
    soilMoister += analogRead(soilMoisterPin);   
    delay(150);
  }
  digitalWrite (soilMoisterVcc, LOW);
  soilMoister = soilMoister/N; 
  Blynk.virtualWrite(V2,soilMoister);
  soilMoister = map(soilMoister, 380, 0, 0, 100); 
}
