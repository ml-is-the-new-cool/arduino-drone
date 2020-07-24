/*
* Arduino Wireless Communication Tutorial
*     Example 1 - Transmitter Code
*                
* by Dejan Nedelkovski, www.HowToMechatronics.com
* 
* Library: TMRh20/RF24, https://github.com/tmrh20/RF24/
*/
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

#define PIN_LED 2
#define PIN_RADIO_CE 9
#define PIN_RADIO_CSN 10

const byte address[6] = "00001";
char data_tx[32];
unsigned long currentTime;
unsigned long prevTime;
unsigned long RX_INTERVAL_TIME = 1000;
RF24 radio(PIN_RADIO_CE, PIN_RADIO_CSN);

int data_rx = 0;

void setup() {
  /**
   * Arduino configuration
   */
  Serial.begin(9600);

  /**
   * ESC configuration
   */
   escSetupAttach();

  /**
   * Radio configuration
   */
  radio.begin();
  radio.setAutoAck(false);
  radio.setDataRate(RF24_250KBPS);
  radio.setPALevel(RF24_PA_LOW);
  radio.openReadingPipe(1, address); // pipe, address
  radio.startListening();
}

void loop() {
  rx();
  
  if (data_rx == "ESC-SETUP-MIN") {
    escSetupMin();
  }
  else if (data_rx == "ESC-SETUP-MAX") {
    escSetupMax();
  }
  else {
    int pulses[4] = {1100, 1100, 1100, 1100};
    escSendPulses(pulses);
  }
}

void rx() {
  currentTime = millis();

  if (currentTime - prevTime >= RX_INTERVAL_TIME) {
    data_rx = -1;
  }

  if (radio.available()) {
    radio.read(&data_rx, sizeof(data_rx));
    prevTime = millis();
  }
}
