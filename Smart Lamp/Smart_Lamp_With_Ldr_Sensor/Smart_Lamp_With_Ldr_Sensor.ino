/* konsep dari sensor ldr adalah semakin terang cahaya yang didapat oleh sensor ldr maka nilai resistensi nya akan semakin kecil,
   sebaliknya bila semakin gelap cahaya yang didapat oleh sensor ldr maka nilai resistensi nya akan semakin besar */

#define relayPin 4       // pin relay/lampu
#define ldrSensor 35     // pin sensor LDR (misalnya di ESP32)

const int batasCahaya = 1000; // batas cahaya bisa diatur sesuai keadaan ruangan anda 

void setup() {
  pinMode(relayPin, OUTPUT);
  digitalWrite(relayPin, LOW);  // keadaan Lampu mati di awal
  Serial.begin(115200);
}

void loop() {
  int nilaiCahaya = analogRead(ldrSensor); // nilai analog dari sensor ldr
  Serial.print("Nilai Cahaya Saat ini : ");
  Serial.println(nilaiCahaya);

  if (nilaiCahaya < batasCahaya) {
    // GELAP → nyalakan lampu
    digitalWrite(relayPin, HIGH); // nyalakan lampu
  } else {
    // TERANG → matikan lampu
    digitalWrite(relayPin, LOW);  // matikan lampu
  }

  delay(200);
}
