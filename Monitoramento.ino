// Bibliotecas Utilizadas
#include <WebServer.h>
#include <WiFi.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Wire.h>
#include <DHT11.h>

// Pinos dos Sensores
#define dhtPin 32
#define uvPin 35
#define gasPin 34

// Funções Declaradas
void lerSensores();
void mostrarDisplay();
void handleRoot();
String classificarUV(float valor);
String classificarAr(int valor);

// Variáveis Globais
int temperatura, umidade, qualidadeAr;
float uvIndex;

// Ponto de Acesso
const char* ssid = "MonitorWMA";
const char* password = "12345678";
WebServer server(80);

// Objetos para Temperatura e Display
DHT11 dht(dhtPin);
Adafruit_SSD1306 display(128, 64, &Wire, -1);


void setup(){
  // Inicializar Display
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.clearDisplay();
  display.setTextColor(SSD1306_WHITE);
  display.setTextSize(1);
  display.setCursor(0, 0);
  display.println("Monitor Iniciado...");
  display.display();
  delay(2000);

  // Inicializar Ponto de Acesso
  WiFi.softAP(ssid, password);
  IPAddress IP = WiFi.softAPIP();
  server.on("/", handleRoot);
  server.begin();

}

void loop() {
  lerSensores();
  mostrarDisplay();
  server.handleClient();
}

void lerSensores(){
  float uvRaw = analogRead(uvPin);
  float tensaoUV = (uvRaw / 4095.0) * 3.3;

  qualidadeAr = analogRead(gasPin);
  temperatura = dht.readTemperature();
  umidade = dht.readHumidity();
  uvIndex = tensaoUV;
}

void mostrarDisplay(){
  display.clearDisplay();
  display.setTextSize(1);
  
  // Mostrar Temperatura
  display.setCursor(0,0);
  display.print("Temp: ");
  display.print(temperatura);
  display.println(" C");

  // Mostrar Umidade
  display.setCursor(0, 16);
  display.print("Umidade: ");
  display.print(umidade);
  display.println(" %");

  // Mostrar UV e Classificação
  display.setCursor(0, 32);
  display.print("UV: ");
  display.print(classificarUV(uvIndex));
  display.print(" (");
  display.print(uvIndex);
  display.println(") ");

  // Qualidade do Ar e Classificação
  display.setCursor(0, 48);
  display.print("Ar: ");
  display.print(classificarAr(qualidadeAr));
  display.print(" (");
  display.print(qualidadeAr);
  display.println(") ");

  display.display();

}

String classificarUV(float valor){
  if(valor <= 0.318) return "Baixo";
    else if(valor >= 0.408 && valor <= 0.606) return "Moderado";
      else if(valor >= 0.696 && valor <= 0.795) return "Alto";
        else if(valor >= 0.881 && valor <= 1.079) return "Muito ALto";
          else return "Extremo";
}

String classificarAr(int valor){
  if (valor < 1401) return "Bom";
    else if (valor < 2202) return "Moderado";
      else return "Ruim";
}

// "Aplicativo" Web que mostrará as informações dos sensores.
void handleRoot() {
  String html = "<!DOCTYPE html><html><head><meta charset='UTF-8'><title>Monitor Ambiental</title>";
  html += "<style>body{font-family:sans-serif;padding:20px;}h1{color:#333;}p{font-size:18px;}</style></head><body>";
  html += "<h1>Monitor Ambiental</h1>";
  html += "<p><b>Temperatura:</b> " + String(temperatura) + " &deg;C</p>";
  html += "<p><b>Umidade:</b> " + String(umidade) + " %</p>";
  html += "<p><b>Índice UV:</b> " + classificarUV(uvIndex) + "</p>";
  html += "<p><b>Qualidade do Ar:</b> " + classificarAr(qualidadeAr) + "</p>";
  html += "</body></html>";

  server.send(200, "text/html", html);
}




