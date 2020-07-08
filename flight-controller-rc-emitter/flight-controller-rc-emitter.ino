/**
 * Usage, according to documentation(https://www.firediy.fr/files/drone/HW-01-V4.pdf) :
 * 
 * 1. Plug your Arduino to your computer with USB cable, open terminal,
 *    then type 1 to send max throttle to every ESC to enter programming mode.
 * 2. Power up your ESCs.
 *    You must hear "beep1 beep2 beep3" tones meaning the power supply is OK.
 * 3. After 2sec, "beep beep" tone emits, meaning the throttle highest point has
 *    been correctly confirmed.
 * 4. Type 0 to send min throttle.
 * 5. Several "beep" tones emits, which means the quantity of the lithium battery cells
 *    (3 beeps for a 3 cells LiPo).
 * 6. A long beep tone emits meaning the throttle lowest point has been correctly confirmed.
 * 7. Type 2 to launch test function. This will send min to max throttle to ESCs to test them.
 *
 * @author mlisthenewcool <contact@hippolyte-debernardi.com>
 */

/**
 * (1, 3) red propellers
 * (2, 4) black propellers
 * 
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

 /*
  Serial.println("\t<APC config>");
  Serial.print("\t\t");
  while(APC.available() > 0) {
    char rx_byte = APC.read();
    Serial.print(rx_byte);
  }
  Serial.println("");
  */
  
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
