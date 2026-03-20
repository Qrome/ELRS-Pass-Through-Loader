/*
  ---------------------------------------------------------------------------
   RP2040‑Zero ELRS Serial Passthrough Loader
   https://github.com/Qrome/ELRS-Pass-Through-Loader.git
  ---------------------------------------------------------------------------

   Description:
     This firmware turns the Waveshare RP2040‑Zero into a high‑speed
     USB‑to‑UART passthrough tool specifically optimized for flashing
     ExpressLRS (ELRS) receivers. It provides a clean, transparent serial
     bridge between the ExpressLRS Configurator and any UART‑based ELRS
     receiver without requiring a flight controller.
       • Fully compatible with ExpressLRS Configurator
       • Select Flashing Method Betaflight Passthrough and the com port

     The sketch uses:
       • USB CDC Serial (host connection)
       • UART1 on GPIO0 (TX) and GPIO1 (RX) for ELRS communication
       • 420000 baud for both sides (ELRS bootloader default)
       • Built‑in WS2812 RGB LED on GPIO16 for status indication

   LED Behavior:
       • Solid BLUE  – Device powered, idle, and ready for connection
       • Flashing GREEN – Active data transfer (firmware flashing or bootloader traffic)
         LED returns to BLUE after ~150 ms of inactivity

   Wiring:
       RP2040‑Zero        ELRS Receiver
       --------------------------------
       GPIO0 (TX)   -->   RX
       GPIO1 (RX)   -->   TX
       5V           -->   5V
       GND          -->   GND

   Notes:
       • No MSP, CRSF, or Betaflight passthrough emulation is used.
       • This is a pure, low‑latency UART bridge ideal for ELRS firmware updates.
       • Requires the Adafruit_NeoPixel library for LED control.

   Author:
       David (Qrome)
       https://github.com/Qrome/ELRS-Pass-Through-Loader.git

  ---------------------------------------------------------------------------
*/

#include <Adafruit_NeoPixel.h>

// Built‑in RGB LED on RP2040‑Zero
#define LED_PIN     16
#define LED_COUNT   1

// UART1 pins for passthrough
#define UART_TX_PIN 0   // RP2040 TX → ELRS RX
#define UART_RX_PIN 1   // RP2040 RX → ELRS TX

Adafruit_NeoPixel led(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);

// LED colors
const uint32_t COLOR_BLUE  = 0x0000FF;
const uint32_t COLOR_GREEN = 0x00FF00;
const uint32_t COLOR_OFF   = 0x000000;

// Activity timer
unsigned long lastActivity = 0;
const unsigned long ACTIVITY_TIMEOUT = 150;  // ms LED stays green after traffic

void setLED(uint32_t color) {
  led.setPixelColor(0, color);
  led.show();
}

void setup() {
  // LED init
  led.begin();
  led.show();
  setLED(COLOR_BLUE);   // idle/connected state

  // USB serial
  Serial.begin(420000);   // ELRS bootloader uses 420k
  while (!Serial) { delay(10); }

  // UART1 serial passthrough
  Serial1.setTX(UART_TX_PIN);
  Serial1.setRX(UART_RX_PIN);
  Serial1.begin(420000);
}

void loop() {
  bool activity = false;

  // USB → UART
  while (Serial.available()) {
    uint8_t b = Serial.read();
    Serial1.write(b);
    activity = true;
  }

  // UART → USB
  while (Serial1.available()) {
    uint8_t b = Serial1.read();
    Serial.write(b);
    activity = true;
  }

  // LED activity logic
  if (activity) {
    lastActivity = millis();
    setLED(COLOR_GREEN);   // flashing green during firmware transfer
  } else {
    if (millis() - lastActivity > ACTIVITY_TIMEOUT) {
      setLED(COLOR_BLUE);  // idle/connected
    }
  }
}