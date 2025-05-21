#define relayPin 4           // Pin relay/lampu
#define soundSensor 34       // Pin sensor suara

bool lampuNyala = false;     // Status awal: lampu mati
const int ambangSuara = 60;

void setup() {
  pinMode(relayPin, OUTPUT);
  digitalWrite(relayPin, LOW);  // Pastikan lampu mati di awal
  Serial.begin(115200);
}

void loop() {
  int nilaiSuara = analogRead(soundSensor);
  Serial.println(nilaiSuara);

  if (nilaiSuara > ambangSuara) {
    // Toggle lampu
    lampuNyala = !lampuNyala;
    digitalWrite(relayPin, lampuNyala ? HIGH : LOW);

    // Tunda sebentar agar tidak mendeteksi suara yang sama berulang
    delay(500);  // 500 ms untuk debounce kasar
  }

  // Tambahan delay kecil agar tidak spam Serial Monitor
  delay(50);
}
