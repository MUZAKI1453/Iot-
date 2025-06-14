#define float_switch 6

int led     = 2;
int buzzer = 3;

void setup() {
  pinMode(led, OUTPUT);
  pinMode(buzzer, OUTPUT);
  pinMode(float_switch, INPUT_PULLUP);
}

void loop() {
  if (digitalRead(float_switch) == HIGH) {
    digitalWrite(led, HIGH);
    digitalWrite(buzzer, HIGH);
  } else {
    digitalWrite(led, LOW);
    digitalWrite(buzzer, LOW);
  }
}
