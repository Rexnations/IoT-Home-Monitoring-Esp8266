#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <DHT.h>

#define DHTPIN D2 // Pin sensor DHT11
#define DHTTYPE DHT11 // Tipe sensor DHT11

DHT dht(DHTPIN, DHTTYPE);
int rainSensorPin = D4; // Pin sensor raindrop
int flameSensorPin = D3; // Pin sensor flame

const char* ssid = "5G"; // Nama SSID WiFi
const char* password = "12345678"; // Password WiFi

ESP8266WebServer server(80);

void handleRoot() {
  float h = dht.readHumidity();
  float t = dht.readTemperature();

  // Membaca nilai sensor raindrop dan flame
  int rainSensorValue = analogRead(rainSensorPin);
  int flameSensorValue = analogRead(flameSensorPin);

  // Mengonversi nilai sensor raindrop dan flame ke dalam persentase
  int rainPercentage = map(rainSensorValue, 0, 1023, 100, 0);
  int flamePercentage = map(flameSensorValue, 0, 1023, 0, 100);

  // Mengirimkan data suhu, kelembaban, indikator hujan, dan indikator api ke klien
  String html = "<html><body><h1>Monitoring Suhu dan Kelembaban</h1>";
  html += "<p>Suhu: " + String(t) + " &deg;C</p>";
  html += "<p>Kelembaban: " + String(h) + " %</p>";
  html += "<p>Indikator Hujan: " + String(rainPercentage) + " %</p>";
  html += "<p>Indikator Api: " + String(flamePercentage) + " %</p></body></html>";
  server.send(200, "text/html", html);
}

void setup() {
  dht.begin();
  pinMode(rainSensorPin, INPUT);
  pinMode(flameSensorPin, INPUT);

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }

  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  server.on("/", handleRoot);

  server.begin();
  Serial.println("HTTP server started");
}

void loop() {
  server.handleClient();
}
