#define BLYNK_TEMPLATE_ID "TMPL6DPB_MhaJ"
#define BLYNK_TEMPLATE_NAME "Smart Lamp"
#define BLYNK_AUTH_TOKEN "cIKEYBolIohXM6Mu6Nl0wSuv_AurZBWh"

#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>

char ssid[] = "ya";     // Ganti dengan SSID WiFi kamu
char pass[] = "tehgelas"; // Ganti dengan password WiFi kamu

int relay_2 = 4;
bool manualControl = false;
bool relayState = false;

BlynkTimer timer;

#define VPIN_LAMPU V0 // Virtual pin di Blynk untuk kontrol manual

void setup() {
  Serial.begin(115200);
  pinMode(relay_2, OUTPUT);
  digitalWrite(relay_2, HIGH); // Matikan relay saat start

  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);
  timer.setInterval(60000L, toggleRelay); // Otomatis ON/OFF tiap 60 detik
}

// Fungsi toggle otomatis relay
void toggleRelay() {
  if (!manualControl) {
    relayState = !relayState;
    digitalWrite(relay_2, relayState ? LOW : HIGH); // Relay aktif LOW
    Blynk.virtualWrite(VPIN_LAMPU, relayState ? 1 : 0); // Update status ke Blynk
  }
}

// Kontrol manual dari Blynk
BLYNK_WRITE(VPIN_LAMPU) {
  int val = param.asInt();
  manualControl = true; // Prioritaskan kontrol dari aplikasi
  relayState = (val == 1);
  digitalWrite(relay_2, relayState ? LOW : HIGH);
}

// Reset ke mode otomatis via Blynk (opsional, misal tombol di V1)
BLYNK_WRITE(V1) {
  int val = param.asInt();
  if (val == 1) {
    manualControl = false;
  }
}

void loop() {
  Blynk.run();
  timer.run();
}
