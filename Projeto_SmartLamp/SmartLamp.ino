// SmartLamp - Firmware ESP32
// Comunicação serial com protocolo simples para driver Linux

#define LED_PIN     13      // GPIO do LED
#define LDR_PIN     34      // GPIO do LDR (ADC)

int ledValue = 0;           // Intensidade atual do LED (0-100)
const int LDR_MAX = 4095;   // Valor máximo do ADC do ESP32 (12 bits)

void setup() {
  Serial.begin(9600);       // Baud rate usado pelo driver
  pinMode(LED_PIN, OUTPUT);
  analogWrite(LED_PIN, 0);  // LED inicia apagado
  Serial.println("SmartLamp Initialized.");
}

void loop() {
  if (Serial.available() > 0) {
    String command = Serial.readStringUntil('\n');
    command.trim();
    processCommand(command);
  }
  delay(50);
}

// Lê o LDR e normaliza para 0-100
int getLdrValue() {
  int raw = analogRead(LDR_PIN);
  int percent = map(raw, 0, LDR_MAX, 0, 100);
  if (percent > 100) percent = 100;
  if (percent < 0) percent = 0;
  return percent;
}

// Atualiza o LED (valor 0-100 → PWM 0-255)
void setLed(int value) {
  if (value >= 0 && value <= 100) {
    ledValue = value;
    analogWrite(LED_PIN, map(value, 0, 100, 0, 255));
    Serial.printf("RES SET_LED 1\n");
  } else {
    Serial.printf("RES SET_LED -1\n");
  }
}

void processCommand(String cmd) {
  if (cmd.startsWith("SET_LED")) {
    // Exemplo: SET_LED 75
    int space = cmd.indexOf(' ');
    if (space > 0) {
      int val = cmd.substring(space + 1).toInt();
      setLed(val);
    } else {
      Serial.println("ERR Invalid SET_LED");
    }
  }
  else if (cmd == "GET_LED") {
    Serial.printf("RES GET_LED %d\n", ledValue);
  }
  else if (cmd == "GET_LDR") {
    Serial.printf("RES GET_LDR %d\n", getLdrValue());
  }
  else {
    Serial.println("ERR Unknown command.");
  }
}