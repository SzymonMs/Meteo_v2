// Clasic Libraries
#include <Arduino.h>
#include <stdio.h>
#include <SPI.h>
#include <math.h>
// Extra Libraries
#include "Adafruit_BMP085.h"
#include "epd1in54_V2.h"
#include "imagedata.h"
#include "epdpaint.h"
#include <OneWire.h>
#include <DallasTemperature.h>
#include <dhtnew.h>
// Definitions
#define COLORED 0
#define UNCOLORED 1
#define OUTTEMPSENSOR 2
#define HUMIDITYSENSOR 5

// Temperature and pressure sensor object
Adafruit_BMP085 bmp;
// E-Paper object
Epd epd;
unsigned char image[1024];
Paint paint(image, 0, 0);
// Outdoor temperature sensor object
OneWire oneWire(OUTTEMPSENSOR);
DallasTemperature outTermo(&oneWire);
// Humidity Sensor
DHTNEW humiditySensor(HUMIDITYSENSOR);

// Variables for storing sensor data
float temperature = 0.0f;
float pressure = 0.0f;
float oldTemperature = 0.0f;
float oldPressure = 0.0f;
float temperatureDiff;
char tempStr[10];
char pressStr[10];

float outdoortemperature = 0.0f;
float oldOutTemperature = 0.0f;
float temperatureOutDiff;
char outtempStr[10];

float humidty = 0.0f;
char humidityStr[10];
float oldHumidity = 0.0f;
float humidityDiff;

int loopCounter = 0;

void setup()
{
  Serial.begin(9600);

  // Initialize BMP180, DS18B20 sensors and E-Paper display
  bmp.begin();
  epd.LDirInit();
  epd.Clear();
  paint.SetWidth(200);
  paint.SetHeight(24);
  outTermo.begin();

  // Create static labels on the display
  paint.Clear(COLORED);
  paint.DrawStringAt(0, 4, "Temperature [*C]", &Font16, UNCOLORED);
  epd.SetFrameMemory(paint.GetImage(), 0, 10, paint.GetWidth(), paint.GetHeight());
  paint.Clear(COLORED);
  paint.DrawStringAt(0, 4, "Pressure [hPa]", &Font16, UNCOLORED);
  epd.SetFrameMemory(paint.GetImage(), 0, 50, paint.GetWidth(), paint.GetHeight());
  paint.Clear(COLORED);
  paint.DrawStringAt(0, 4, "Out temp [*C]", &Font16, UNCOLORED);
  epd.SetFrameMemory(paint.GetImage(), 0, 90, paint.GetWidth(), paint.GetHeight());
  paint.Clear(COLORED);
  paint.DrawStringAt(0, 4, "Humidity [%]", &Font16, UNCOLORED);
  epd.SetFrameMemory(paint.GetImage(), 0, 130, paint.GetWidth(), paint.GetHeight());
 // epd.DisplayFrame();
}

void loop()
{
  // Read Humidity
  humiditySensor.read();
  humidty = humiditySensor.getHumidity();
  dtostrf(humidty,4,1,humidityStr);
  humidityDiff = fabs(humidty-oldHumidity);
  // Read temperature and pressure from BMP180
  pressure = bmp.readPressure() / 100;
  temperature = bmp.readTemperature();
  dtostrf(temperature, 4, 1, tempStr);
  dtostrf(pressure, 4, 0, pressStr);
  temperatureDiff = fabs(temperature - oldTemperature);
  // Read temperature from DS18B20
  outTermo.requestTemperatures(); // Send the command to get temperatures
  outdoortemperature = outTermo.getTempCByIndex(0);
  dtostrf(outdoortemperature,4,1,outtempStr);
  temperatureOutDiff = fabs(outdoortemperature-oldOutTemperature);

  // Update display only if significant change detected
  if (temperatureDiff > 0.5f ||temperatureOutDiff > 0.5f || oldPressure != pressure || humidityDiff>1.0f)
  {
    paint.Clear(UNCOLORED);
    paint.DrawStringAt(0, 4, tempStr, &Font20, COLORED);
    epd.SetFrameMemory(paint.GetImage(), 0, 30, paint.GetWidth(), paint.GetHeight());
    paint.Clear(UNCOLORED);
    paint.DrawStringAt(0, 4, pressStr, &Font20, COLORED);
    epd.SetFrameMemory(paint.GetImage(), 0, 70, paint.GetWidth(), paint.GetHeight());
    paint.Clear(UNCOLORED);
    paint.DrawStringAt(0, 4, outtempStr, &Font20, COLORED);
    epd.SetFrameMemory(paint.GetImage(), 0, 110, paint.GetWidth(), paint.GetHeight());
    paint.Clear(UNCOLORED);
    paint.DrawStringAt(0, 4, humidityStr, &Font20, COLORED);
    epd.SetFrameMemory(paint.GetImage(), 0, 150, paint.GetWidth(), paint.GetHeight());
    epd.DisplayFrame();
  }

  oldTemperature = temperature;
  oldPressure = pressure;
  oldOutTemperature = outdoortemperature;
  oldHumidity = humidty;
  loopCounter++;
  delay(10000);
}