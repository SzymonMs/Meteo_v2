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
// Definitions
#define COLORED 0
#define UNCOLORED 1

// Temperature and Pressure Sensor object
Adafruit_BMP085 bmp;
// E-Paper object
Epd epd;
unsigned char image[1024];
Paint paint(image, 0, 0);

// Variables for storing sensor data
float temperature = 0.0f;
float pressure = 0.0f;
float oldTemperature = 0.0f;
float oldPressure = 0.0f;
float temperatureDiff;
char tempStr[10];
char pressStr[10];

void setup()
{
  Serial.begin(9600);

  // Initialize BMP180 sensor and E-Paper display
  bmp.begin();
  epd.LDirInit();
  epd.Clear();
  paint.SetWidth(200);
  paint.SetHeight(24);

  // Create static labels on the display
  paint.Clear(COLORED);
  paint.DrawStringAt(0, 4, "Temperature [*C]", &Font16, UNCOLORED);
  epd.SetFrameMemory(paint.GetImage(), 0, 10, paint.GetWidth(), paint.GetHeight());
  paint.Clear(COLORED);
  paint.DrawStringAt(0, 4, "Pressure [hPa]", &Font16, UNCOLORED);
  epd.SetFrameMemory(paint.GetImage(), 0, 50, paint.GetWidth(), paint.GetHeight());
}

void loop()
{

  // Read temperature and pressure from BMP180
  pressure = bmp.readPressure() / 100;
  temperature = bmp.readTemperature();
  dtostrf(temperature, 4, 1, tempStr);
  dtostrf(pressure, 4, 0, pressStr);
  temperatureDiff = fabs(temperature - oldTemperature);
  // Update display only if significant change detected
  if (temperatureDiff > 0.2f || oldPressure != pressure)
  {
    paint.Clear(UNCOLORED);
    paint.DrawStringAt(0, 4, tempStr, &Font20, COLORED);
    epd.SetFrameMemory(paint.GetImage(), 0, 30, paint.GetWidth(), paint.GetHeight());
    paint.Clear(UNCOLORED);
    paint.DrawStringAt(0, 4, pressStr, &Font20, COLORED);
    epd.SetFrameMemory(paint.GetImage(), 0, 70, paint.GetWidth(), paint.GetHeight());
    epd.DisplayFrame();
  }
  oldTemperature = temperature;
  oldPressure = pressure;
  // Wait for 10 seconds before next reading
  delay(10000);
}
