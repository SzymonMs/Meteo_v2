#include <Arduino.h>
#include <stdio.h>
#include <SPI.h>


#include "Adafruit_BMP085.h"
#include "epd1in54_V2.h"
#include "imagedata.h"
#include "epdpaint.h"

#define COLORED 0
#define UNCOLORED 1


Adafruit_BMP085 bmp;
float temperature = 0.0f;
float pressure = 0.0f;

Epd epd;
unsigned char image[1024];
Paint paint(image,0,0);
char tempStr[10];
char pressStr[10];



void setup() {
  Serial.begin(9600);
  bmp.begin();

  epd.LDirInit();
  epd.Clear();
  paint.SetWidth(200);
  paint.SetHeight(24);
}
  
void loop() {
  pressure = bmp.readPressure()/100;
  temperature = bmp.readTemperature();
  dtostrf(temperature, 4, 1, tempStr);   // np. "23.5"
  dtostrf(pressure,    5, 0, pressStr);  // np. "1013"
  
  paint.Clear(COLORED);
  paint.DrawStringAt(30, 4,"Temperatura [*C]", &Font12, UNCOLORED);
  epd.SetFrameMemory(paint.GetImage(), 0, 10, paint.GetWidth(), paint.GetHeight());

  paint.Clear(UNCOLORED);
  paint.DrawStringAt(30, 4, tempStr, &Font12, COLORED);
  epd.SetFrameMemory(paint.GetImage(), 0, 30, paint.GetWidth(), paint.GetHeight());

  
  paint.Clear(COLORED);
  paint.DrawStringAt(30, 4, "Cisnienie [hPa]", &Font12, UNCOLORED);
  epd.SetFrameMemory(paint.GetImage(), 0, 50, paint.GetWidth(), paint.GetHeight());

  paint.Clear(UNCOLORED);
  paint.DrawStringAt(30, 4, pressStr, &Font12, COLORED);
  epd.SetFrameMemory(paint.GetImage(), 0, 70, paint.GetWidth(), paint.GetHeight());
  epd.DisplayFrame();


  Serial.print("Temperature = ");
  Serial.print(temperature);
  Serial.println(" *C");
  
  Serial.print("Pressure = ");
  Serial.print(pressure);
  Serial.println(" hPa");

  Serial.println();
  delay(10000);

}