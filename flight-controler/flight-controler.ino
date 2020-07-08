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

#include <SoftwareSerial.h>

const int PIN_APC_RX  = 11;
const int PIN_APC_TX  = 12;
const int PIN_APC_SET = 13;

SoftwareSerial APC(PIN_APC_RX, PIN_APC_TX);

// buffers & RC variables
const byte NUM_CHARS = 32;
char RECEIVED_CHARS[NUM_CHARS];
char START_MARKER = '<';
char END_MARKER = '>';
boolean NEW_DATA = false;

// ESC variables
boolean ESC_CALIBRATED_MIN = false;
boolean ESC_CALIBRATED_MAX = false;

// ================================================================================

void setup() {
    //Serial.println("<ARDUINO config...");
    Serial.begin(9600);
    //Serial.println("<Arduino is ready>");
    
    //Serial.println("<APC config>");
    apc_set_configuration();
    //apc_get_configuration();
    //Serial.println("<APC is ready>");

    //Serial.println("<ESC config...>");
    esc_setup_attach();
    //Serial.println("<ESC are ready...>");

    APC.write("<");
    APC.write("0");
    APC.write(">");
}

// ================================================================================

void loop() {
    //sendWithStartEndMarkers();
    recvWithStartEndMarkersOnSerial();
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
  Serial.println("");
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
 
  while(APC.available()) {
    Serial.write(APC.read());
  }
  
  digitalWrite(PIN_APC_SET, HIGH);
  delay(200);
}

// ================================================================================

void recvWithStartEndMarkers() {
    static boolean recv_in_progress = false;
    static byte ndx = 0;
    char rc;
 
    while ((APC.available() > 0) && (NEW_DATA == false)) {
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
              // terminate the string
                RECEIVED_CHARS[ndx] = '\0';

// CHECK
/*
                APC.write('<');
                for (int i = 0; i < ndx; i++) {
                  APC.write(RECEIVED_CHARS[i]);
                }
                APC.write('>');
                //APC.write('\0');
*/
// CHECK
                
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


void recvWithStartEndMarkersOnSerial() {
    static boolean recv_in_progress = false;
    static byte ndx = 0;
    char rc;
 
    while ((Serial.available() > 0) && (NEW_DATA == false)) {
        rc = Serial.read();

        if (recv_in_progress == true) {
            if (rc != END_MARKER) {
                RECEIVED_CHARS[ndx] = rc;
                ndx++;
                if (ndx >= NUM_CHARS) {
                    ndx = NUM_CHARS - 1;
                }
            }
            else {
              // terminate the string
                RECEIVED_CHARS[ndx] = '\0';

// CHECK
/*
                APC.write('<');
                for (int i = 0; i < ndx; i++) {
                  APC.write(RECEIVED_CHARS[i]);
                }
                APC.write('>');
                //APC.write('\0');
*/
// CHECK
                
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
      
        if (ESC_CALIBRATED_MIN and ESC_CALIBRATED_MAX) {
          parseData();
          APC.write('<');
          APC.write('V');
          APC.write('>');
        }

        else {
          if (RECEIVED_CHARS[0] == '-') {
            APC.write('<');
            APC.write('m');
            APC.write('>');
            //APC.println("<MIN THROTTLE RECEIVED>");
            Serial.println("MIN");
            esc_setup_min();
            ESC_CALIBRATED_MIN = true;
          }

          if (RECEIVED_CHARS[0] == '+') {
            APC.write('<');
            APC.write('M');
            APC.write('>');
            //APC.println("<MAX THROTTLE RECEIVED>");
            Serial.println("MAX");
            esc_setup_max();
            ESC_CALIBRATED_MAX = true;
          }
        }
        
        NEW_DATA = false;
    }
}

void parseData() {
    char * strtokIndx; // this is used by strtok() as an index
    
    strtokIndx = strtok(RECEIVED_CHARS, ",");      // get the first part - the string
    int motor_1 = atoi(strtokIndx);

    strtokIndx = strtok(NULL, ",");
    int motor_2 = atoi(strtokIndx);

    strtokIndx = strtok(NULL, ",");
    int motor_3 = atoi(strtokIndx);

    strtokIndx = strtok(NULL, ",");
    int motor_4 = atoi(strtokIndx);

    Serial.println(motor_1);
    Serial.println(motor_2);
    Serial.println(motor_3);
    Serial.println(motor_4);
    esc_send_pulse(motor_1, motor_2, motor_3, motor_4);
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
