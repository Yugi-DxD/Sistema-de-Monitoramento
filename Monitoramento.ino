#include <Adafruit_GFX.h>
#include <Adafruit_GrayOLED.h>
#include <Adafruit_SPITFT.h>
#include <Adafruit_SPITFT_Macros.h>
#include <gfxfont.h>
#include <Adafruit_SSD1306.h>
#include <splash.h>
#include <Wire.h>
#include <DHT11.h>

float temperatura, umidade, resultado;
float uvIndex;
DHT11 dht(2);
Adafruit_SSD1306 display(128, 64, &Wire, -1);

void setup(){
  Serial.begin(9600);

  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)){
    Serial.println(F("Falha ao iniciar o display"));
    while (true);
  }

  display.clearDisplay();
  display.setTextColor(SSD1306_WHITE);
  display.setTextSize(1);
  display.setCursor(0, 0);
  display.println(F("Monitor Iniciado..."));
  display.display();
  delay(2000);
}

void loop() {
  float uvRaw = analogRead(A0);
  temperatura = dht.readTemperature();
  umidade = dht.readHumidity();
  uvIndex = map(uvRaw, 0, 1023, 0, 11);

  Serial.print("Temperatura: ");
  Serial.println(temperatura);
  Serial.print("Umidade: ");
  Serial.println(umidade);
  Serial.print("Radiação UV: ");
  Serial.println(uvIndex);
  Serial.println("==========");

  // Exibição no OLED
  display.clearDisplay();
  display.setTextSize(1);
  display.setCursor(0, 0);
  display.print("Temp: ");
  display.print(temperatura);
  display.println(" C");

  display.setCursor(0, 16);
  display.print("Umidade: ");
  display.print(umidade);
  display.println(" %");

  display.setCursor(0, 32);
  display.print("UV Index: ");
  display.println(uvIndex);

  display.setCursor(0, 48);
  display.print("Nivel: ");
  if (uvIndex < 3) display.println("Baixo");
  else if (uvIndex < 6) display.println("Moderado");
  else if (uvIndex < 8) display.println("Alto");
  else if (uvIndex < 11) display.println("Muito Alto");
  else display.println("Extremo");

  display.display();
  delay(500);

}
