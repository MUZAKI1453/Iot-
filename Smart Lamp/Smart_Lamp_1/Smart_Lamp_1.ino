int relay_2 = 4;

void setup() {
  pinMode(relay_2, OUTPUT);
  digitalWrite(relay_2, HIGH);
}

void loop() {
  digitalWrite(relay_2, LOW);
  delay(60000);
  digitalWrite(relay_2, HIGH);
  delay(60000);
}
