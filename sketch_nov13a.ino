#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <HTTPClient.h>
#include <DHT.h>

#define DHTPIN  4
#define RED_LED  2
#define GREEN_LED  15
#define DHTTYPE DHT11

const char* WIFI_SSID = "";
const char* WIFI_PASS = "";
const char* FIREBASE_URL = "";

DHT dht(DHTPIN, DHTTYPE);

unsigned long ultimoEnvio = 0;
const long intervaloEnvio = 30000; 

void conectarWiFi() {
  WiFi.begin(WIFI_SSID, WIFI_PASS);
  Serial.print("Conectando a WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi conectado");
}

void enviarAFirebase(float temp, float hum) {
  if (WiFi.status() == WL_CONNECTED) {
    WiFiClientSecure client;
    client.setInsecure();               
    HTTPClient http;
    http.begin(client, FIREBASE_URL);
    http.addHeader("Content-Type", "application/json");
    
    String json = "{\"temperatura\":" + String(temp) +
                  ",\"humedad\":" + String(hum) +
                  ",\"ts\":{\".sv\":\"timestamp\"}}";
                  
    int code = http.PUT(json);
    Serial.println("Firebase HTTP Response: " + String(code));
    http.end();
  }
}

void setup() {
  Serial.begin(9600);
  pinMode(RED_LED, OUTPUT);
  pinMode(GREEN_LED, OUTPUT);
  
  dht.begin();
  conectarWiFi();
}

void loop() {
  if (WiFi.status() != WL_CONNECTED) {
    conectarWiFi();
  }

  delay(2000);

  float temp = dht.readTemperature();
  float hum = dht.readHumidity();

  if (isnan(temp) || isnan(hum)) {
    Serial.println("Error al leer del sensor DHT11");
    return;
  }

  if (temp > 35) {
    digitalWrite(RED_LED, HIGH);   
    digitalWrite(GREEN_LED, LOW);  
  } else {
    digitalWrite(RED_LED, LOW);    
    digitalWrite(GREEN_LED, HIGH); 
  }

  Serial.print("Temperatura: ");
  Serial.print(temp);
  Serial.print(" °C | Humedad: ");
  Serial.print(hum);
  Serial.println(" %");

  unsigned long tiempoActual = millis();
  if (tiempoActual - ultimoEnvio >= intervaloEnvio) {
    ultimoEnvio = tiempoActual;
    Serial.println("Subiendo datos a Firebase...");
    enviarAFirebase(temp, hum);
  }
}
