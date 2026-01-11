

#define UART_RX 16
#define UART_TX 17
#define BAUD_RATE 115200

void setup() {
  // USB Serial (PC Monitor)
  Serial.begin(115200);
  delay(1000);
  Serial.println("ESP32 UART -> USB Bridge Started");

  // Hardware UART (Serial1)
  Serial1.begin(BAUD_RATE, SERIAL_8N1, UART_RX, UART_TX);
}

void loop() {
  // Forward UART -> USB
  while (Serial1.available()) {
    Serial.write(Serial1.read());
  }

  // Optional: Forward USB -> UART (interactive console)
  while (Serial.available()) {
    Serial1.write(Serial.read());
  }
}
