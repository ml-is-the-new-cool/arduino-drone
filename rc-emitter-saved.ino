/*
#include <Servo.h>

Servo ESC;     // create servo object to control the ESC
int potValue;  // value from the analog pin

void setup() {
  Serial.begin(9600);
  // Attach the ESC on pin 9
  ESC.attach(9,1000,2000); // (pin, min pulse width, max pulse width in microseconds) 
}


void loop() {
  potValue = analogRead(A0);   // reads the value of the potentiometer (value between 0 and 1023)
  potValue = map(potValue, 0, 1023, 0, 180);   // scale it to use it with the servo library (value between 0 and 180)
  ESC.write(potValue);    // Send the signal to the ESC
}
*/

#include <Servo.h>
#include <SoftwareSerial.h>

const int PIN_APC_RX  = 11; // vert
const int PIN_APC_TX  = 12; // blanc
const int PIN_APC_SET = 13; // jaune

SoftwareSerial APC(PIN_APC_RX, PIN_APC_TX);

const byte NUM_CHARS = 32;
char RECEIVED_CHARS[NUM_CHARS];
char START_MARKER = '<';
char END_MARKER = '>';
boolean NEW_DATA = false;

// ================================================================================

void setup() {
    //Serial.println("<ARDUINO config...");
    Serial.begin(9600);
    //Serial.println("<Arduino is ready>");
    
    //Serial.println("<APC config>");
    apc_set_configuration();
    //apc_get_configuration();
    //Serial.println("<APC is ready>");
}

// ================================================================================

void loop() {
    sendWithStartEndMarkers();
    recvWithStartEndMarkers();
    showNewData();
}

// ================================================================================

void apc_set_configuration() {
  pinMode(PIN_APC_SET, HIGH);
  APC.begin(9600);

  // on met le APC en mode configuration
  digitalWrite(PIN_APC_SET, LOW);
  delay(10);

  // on configure le APC avec les valeurs par défault, voir fiche constructeur
  // TODO : à quoi correspond les valeurs
  //const long apc_frequency = 431000; // 418_000MHz to 455MHz
  //const int apc_air_rate  = 3;      // (0, 2400bps) (1, 4800) (2, 9600) (3, 19200)
  //const int apc_jsaispas  = 3;
  APC.println("WR 433900 3 9 3 0");
  delay(10);

// TODO
  while (APC.available()) {
    Serial.print(APC.read());
  }
  Serial.println();
// TODO

  // on remet le APC en mode opérationnel
  digitalWrite(PIN_APC_SET, HIGH);
  delay(200);
}

// ================================================================================

void apc_get_configuration() {
  digitalWrite(PIN_APC_SET, LOW);
  delay(10);
  APC.println("RD");
  delay(10);

  Serial.println("\t<APC config>");
  Serial.print("\t\t");
  while(APC.available() > 0) {
    char rx_byte = APC.read();
    Serial.print(rx_byte);
  }
  Serial.println("");
  
  while(APC.available() > 0) {
    Serial.print(APC.read());
  }
  Serial.println();
  
  digitalWrite(PIN_APC_SET, HIGH);
  delay(200);
}

// ================================================================================

void recvWithStartEndMarkers() {
    static boolean recv_in_progress = false;
    static byte ndx = 0;
    char rc;
 
    while (APC.available() > 0 && NEW_DATA == false) {
        rc = APC.read();

        if (recv_in_progress == true) {
            if (rc != END_MARKER) {
                RECEIVED_CHARS[ndx] = rc;
                ndx++;
                if (ndx >= NUM_CHARS) {
                    ndx = NUM_CHARS - 1;
                }
            }
            else {
                RECEIVED_CHARS[ndx] = '\0'; // terminate the string
                recv_in_progress = false;
                ndx = 0;
                NEW_DATA = true;
            }
        }

        else if (rc == START_MARKER) {
            recv_in_progress = true;
        }
    }
}

// ================================================================================

void showNewData() {
    if (NEW_DATA == true) {
        Serial.print("Received from drone -> ");
        Serial.println(RECEIVED_CHARS);
        NEW_DATA = false;
    }
}

// ================================================================================

void sendWithStartEndMarkers() {
  char rc;
  while (Serial.available() > 0) {
    rc = Serial.read();
    APC.write(rc);
    //Serial.println(rc);
  }
}
