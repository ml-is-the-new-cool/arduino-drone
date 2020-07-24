/*
* Arduino Wireless Communication Tutorial
*     Example 1 - Transmitter Code
*                
* by Dejan Nedelkovski, www.HowToMechatronics.com
* 
* Library: TMRh20/RF24, https://github.com/tmrh20/RF24/
* Documentation : https://tmrh20.github.io/RF24/
* Examples from official library : https://tmrh20.github.io/RF24/examples.html
* Tutorial : https://forum.arduino.cc/index.php?topic=421081
* 
* pipes (similar as the support used) =/= addresses (similar as a phone number)
* 
* ============
* Data packets
* ============
* maximum 32 bytes in a single message
* 2 modes of operations
* {a} fixed payload size (default to 32 bytes) using setPayloadSize(x)
* {b} dynamic payload using enableDynamicPayloads() and checkable with getDynamicPayloadSize()
* 
* on transmitter (TX) : if the data that was sent is not correctly received, the TX will
* consider the transimssion to have failed and will automatically retry the transmission up to
* 5 times (default, max 15) before giving up.
* 
* on receiver (RX) : if radio.available() > 0 then you are sure it will be correct and fully
* received.
* 
*/
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

#define PIN_RADIO_CE 9
#define PIN_RADIO_CSN 10

const byte address[6] = "00001";
unsigned long currentTime;
unsigned long prevTime;
unsigned long TX_INTERVAL_TIME = 2000;
RF24 radio(PIN_RADIO_CE, PIN_RADIO_CSN);

int data_tx = 0;

void setup() {
  /**
   * Arduino configuration
   */
  Serial.begin(9600);

  /**
   * Radio configuration
   */
  radio.begin();
  radio.setAutoAck(false);
  radio.setRetries(3, 5); // delay, count
  radio.setDataRate(RF24_250KBPS);
  radio.setPALevel(RF24_PA_LOW);
  radio.openWritingPipe(address);
  radio.stopListening();
}

void loop() {
  currentTime = millis();

  if (currentTime - prevTime >= TX_INTERVAL_TIME) {
    tx();
    prevTime = millis();
  }
}

void tx() {
  bool isSuccess;

  isSuccess = radio.write(&data_tx, sizeof(data_tx));

  if (isSuccess) {
    Serial.println("success");
  }
  else {
    Serial.println("Failed");
  }
}
