#include <DHT.h>

// --- PİN TANIMLAMALARI ---
#define DHTPIN 4
#define DHTTYPE DHT11
const int soilPin = 34;
const int relayPin = 5;
const int mq2Pin = 35;
const int buzzerPin = 18;

// --- EŞİK (THRESHOLD) DEĞERLERİ ---
const int KURU_ESIK = 2800;
const int ISLAK_ESIK = 2000;
const int GAZ_ESIK = 2000;

DHT dht(DHTPIN, DHTTYPE);

void setup() {
  Serial.begin(115200);
  
  pinMode(relayPin, OUTPUT);
  pinMode(buzzerPin, OUTPUT);
  
  digitalWrite(relayPin, HIGH);
  digitalWrite(buzzerPin, LOW);
  
  dht.begin();
  
  Serial.println("=====================================");
  Serial.println("  AKILLI SERA SISTEMI BASLATILIYOR  ");
  Serial.println("=====================================");
  delay(2000);
}

void loop() {
  Serial.println("\n--- SENSOR VERILERI ---");

  float sicaklik = dht.readTemperature();
  float havaNemi = dht.readHumidity();
  
  if (isnan(sicaklik) || isnan(havaNemi)) {
    Serial.println("HATA: DHT11 sensorunden veri alinamiyor!");
  } else {
    Serial.print("Sicaklik: "); 
    Serial.print(sicaklik); 
    Serial.print(" °C  |  Hava Nemi: %"); 
    Serial.println(havaNemi);
  }

  int gazDegeri = analogRead(mq2Pin);
  Serial.print("Gaz/Duman Seviyesi: ");
  Serial.print(gazDegeri);
  
  if (gazDegeri > GAZ_ESIK) {
    digitalWrite(buzzerPin, HIGH);
    Serial.println("  [!!! TEHLIKE: GAZ/DUMAN ALGILANDI !!!]");
  }
  else if(gazDegeri>1500 & gazDegeri<2500) {
    digitalWrite(buzzerPin, LOW);
    Serial.println(" Ortam Havasız.");
  }
   else {
    digitalWrite(buzzerPin, LOW);
    Serial.println("  [Guvenli]");
  }

  int toprakNemi = analogRead(soilPin);
  Serial.print("Toprak Kuruluk Seviyesi: ");
  Serial.print(toprakNemi);
  
  if (toprakNemi > KURU_ESIK) { 
    digitalWrite(relayPin, LOW);
    Serial.println("  [Durum: KURU -> Pompa CALISIYOR 🌊]");
  } 
  else if (toprakNemi < ISLAK_ESIK) {
    digitalWrite(relayPin, HIGH);
    Serial.println("  [Durum: ISLAK -> Pompa DURDU 🛑]");
  } 
  else {
    Serial.println("  [Durum: IDEAL -> Sistem Beklemede]");
  }
  
  Serial.println("-----------------------");
  delay(2500); 
}