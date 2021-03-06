/**
 * |Motor| Pin | Rotation | ESC |
 * -----------------------------|
 * |  1  |  5  |    CCW   |  1  |
 * |  2  |  6  |    CW    | -1  |
 * |  3  |  7  |    CCW   |  1  |
 * |  4  |  8  |    CW    | -1  |
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
    //recvWithStartEndMarkersOnSerial();
    recvWithStartEndMarkers();
    showNewData();

/*
    int pulses[4];
    for(int power = 1000; power < 1400; power += 100) {
      for(int i = 0; i < 4; i++) {
        pulses[i] = power;
      }
      Serial.println(power);
      esc_send_pulse(pulses);
      delay(2000);
    }
*/
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
/*
  while (APC.available()) {
    Serial.print(APC.read());
  }
*/
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
      if (RECEIVED_CHARS[0] == '-') {
            APC.write('<');
            APC.write('m');
            APC.write('>');
            //APC.println("<MIN THROTTLE RECEIVED>");
            Serial.println("MIN");
            esc_setup_min();
            ESC_CALIBRATED_MIN = true;
          }

          else if (RECEIVED_CHARS[0] == '+') {
            APC.write('<');
            APC.write('M');
            APC.write('>');
            //APC.println("<MAX THROTTLE RECEIVED>");
            Serial.println("MAX");
            esc_setup_max();
            ESC_CALIBRATED_MAX = true;
          }

        else {
         parseData();
          APC.write('<');
          APC.write('V');
          APC.write('>');
     
        }
        
        
        NEW_DATA = false;
    }
}

void parseData() {
    char * strtokIndx; // this is used by strtok() as an index
    int pulses[4];
    
    strtokIndx = strtok(RECEIVED_CHARS, ",");      // get the first part - the string
    pulses[0] = atoi(strtokIndx);

    strtokIndx = strtok(NULL, ",");
    pulses[1] = atoi(strtokIndx);

    strtokIndx = strtok(NULL, ",");
    pulses[2] = atoi(strtokIndx);

    strtokIndx = strtok(NULL, ",");
    pulses[3] = atoi(strtokIndx);

    esc_send_pulse(pulses);
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
