/*
 * DHT: D4
 * MQ2: A0
 * flameSensor: D1
 */


#define BLYNK_TEMPLATE_ID "TMPL3fkPLVw8J"   //enter blynk template id
#define BLYNK_TEMPLATE_NAME "DrainXplorer"   //enter blynk template name
#define BLYNK_PRINT Serial
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <DHT.h>

char auth[] = "nEzwvUbzFqRLfOQCrQPbsGF9mWgCpQ60";  //enter auth code

char ssid[] = "Meliorate"; //enter ssid
char pass[] = "87654321"; //enter password

const int flameSensorPin = D1;

int mq2 = A0; // smoke sensor is connected with the analog pin A0
int data = 0;
DHT dht(D4, DHT11);
BlynkTimer timer;

double T, P;
char status;
float h,t;

void setup() 
{
  Serial.begin(9600);
  Blynk.begin(auth, ssid, pass, "blynk.cloud", 80);
  dht.begin();

  pinMode(flameSensorPin, INPUT);
  
  timer.setInterval(100L, DHT11sensor);
  timer.setInterval(100L, getSendData);
}

void DHT11sensor() 
{
  float h = dht.readHumidity();
  float t = dht.readTemperature();

  if (isnan(h) || isnan(t)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }
  Blynk.virtualWrite(V0, t);
  Blynk.virtualWrite(V1, h);
}

void getSendData()  //MQ2 Sensor
{
data = analogRead(mq2);
Blynk.virtualWrite(V2, data);
if (data > 350 )
    {
           WidgetLED LED(V3);
           LED.on();
           Blynk.logEvent("Gas","Warning! Harmful Gas Detected!");
    }
else
    {
      WidgetLED LED(V3);
      LED.off();
    }
}

void loop() {
  
  int flameValue = digitalRead(flameSensorPin);
  if (flameValue == LOW) 
  { 
    WidgetLED LED(V4);
    LED.on();
    Blynk.logEvent("Fire","Warning! Fire Detected!");
  } 
  else 
  {
    WidgetLED LED(V4);
      LED.off();
  }

  //hum: 85%
    if (h >= 85)
    {
       Blynk.logEvent("Humidity","Warning! High Humidity! ");
    }
   //temp: 45 c
      if (t >= 45)
    {
       Blynk.logEvent("Temperature","Warning! High Temperature ");
    }
 
  Blynk.run();
  timer.run();
}
