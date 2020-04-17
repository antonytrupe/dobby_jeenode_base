#include <DHT22.h>
// Only used for sprintf
#include <stdio.h>

// Data wire is plugged into port 7 on the Arduino
// Connect a 4.7K resistor between VCC and the data pin (strong pullup)
#define DHT22_PIN 4
unsigned long DELAY = 60*1000;

// Setup a DHT22 instance
DHT22 myDHT22(DHT22_PIN);
bool DEBUG = false;

void setup(void)
{
  // start serial port
  Serial.begin(9600);
  //Serial.println("DHT22 Library Demo");
}

void loop(void)
{
  DHT22_ERROR_t errorCode;
  int lastTempC=0,lastRH=0,tempC,rh=0;

  // The sensor can only be read from every 1-2s, and requires a minimum
  // 2s warm-up after power-on.
  delay(DELAY);
  if (DEBUG) {
    Serial.print("Requesting data...");
  }
  errorCode = myDHT22.readData();
  switch (errorCode)
  {
  case DHT_ERROR_NONE:
    if (DEBUG) {
      Serial.print("Got Data ");
    }
    //Serial.print(myDHT22.getTemperatureF());
    //Serial.print("F ");
    //Serial.print(myDHT22.getHumidity());
    //Serial.println("%");
    // Alternately, with integer formatting which is clumsier but more compact to store and
    // can be compared reliably for equality:
    //
    char buf[128];
    tempC=myDHT22.getTemperatureCInt();
    rh=myDHT22.getHumidityInt();
    if(tempC!=lastTempC){
      sprintf(buf, "[{\"location\":\"living room\",\"t\":%hi.%01hi}]",
        tempC / 10, abs(tempC % 10));
      Serial.println(buf);
    }
    if(rh!=lastRH){
      sprintf(buf, "[{\"location\":\"living room\",\"rh\": %i.%01i}]",
        rh / 10, rh % 10);
      Serial.println(buf);
    }

    lastTempC=tempC;
    lastRH=rh;
    break;
  case DHT_ERROR_CHECKSUM:
    if (DEBUG) {
      Serial.print("check sum error ");
    }
    break;
  case DHT_BUS_HUNG:
    if (DEBUG) {
      Serial.println("BUS Hung ");
    }
    break;
  case DHT_ERROR_NOT_PRESENT:
    if (DEBUG) {
      Serial.println("Not Present ");
    }
    break;
  case DHT_ERROR_ACK_TOO_LONG:
    if (DEBUG) {
      Serial.println("ACK time out ");
    }
    break;
  case DHT_ERROR_SYNC_TIMEOUT:
    if (DEBUG) {
      Serial.println("Sync Timeout ");
    }
    break;
  case DHT_ERROR_DATA_TIMEOUT:
    if (DEBUG) {
      Serial.println("Data Timeout ");
    }
    break;
  case DHT_ERROR_TOOQUICK:
    if (DEBUG) {
      Serial.println("Polled to quick ");
    }
    break;
  }
}
