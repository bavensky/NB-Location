#include <Arduino.h>
#include "AIS_NB-IoT.h"
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include "DHT.h"

LiquidCrystal_I2C lcd(0x27, 16, 2);

#define DHTPIN 2
#define DHTTYPE DHT22
DHT dht(DHTPIN, DHTTYPE);

String serverIP = "159.89.205.216";
String serverPort = "22111";

AIS_NB_IoT AISnb;
AIS_NB_IoT_RES resp;

const long interval = 60000; //millisecond
unsigned long previousMillis = 0;

long cnt = 0;

int field1 = 0;
int field2 = 0;
int field3 = 0;
int field4 = 0;
int field5 = 0;
int field6 = 0;

void setup()
{
    Wire.begin();
    lcd.init();
    lcd.backlight();

    dht.begin();

    AISnb.debug = true;

    delay(2000);
    Serial.begin(9600);

    lcd.setCursor(0, 0);
    lcd.print("CMMC NB-IoT     ");

    AISnb.setupDevice(serverPort);

    String ip1 = AISnb.getDeviceIP();
    lcd.setCursor(0, 1);
    lcd.print(ip1);

    pingRESP pingR = AISnb.pingIP(serverIP);

    previousMillis = millis();
}

void loop()
{

    unsigned long currentMillis = millis();

    if (currentMillis - previousMillis >= 10 * interval)
    {
        lcd.setCursor(0, 0);
        lcd.print("CMMC NB-IoT     ");
        lcd.setCursor(0, 1);
        lcd.print(cnt);

        field1 = dht.readTemperature();
        field2 = dht.readHumidity();
        field3 = 0;
        field4 = 0;
        field5 = analogRead(A0) * (5.0 / 1023.0);
        field6 = cnt++;

        // Send data in String
        //UDPSend udp = AISnb.sendUDPmsgStr(serverIP, serverPort,"{\"myName\": \"SuperMAN\",\"field1\": 30,\"field2\":2,\"field3\":3}");
        UDPSend udp = AISnb.sendUDPmsgStr(serverIP, serverPort, "{\"myName\": \"supermanNB\",\"field1\": " + String(field1) + ",\"field2\":" + String(field2) + ",\"field3\":" + String(field3) + ",\"field4\":" + String(field4) + ",\"field5\":" + String(field5) + ",\"field6\":" + String(field6) + "}");

        previousMillis = currentMillis;
    }

    UDPReceive resp = AISnb.waitResponse();
}
