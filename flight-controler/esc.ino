/**
 * (1, jaune, 5)
 * (3, vert,  6)
 * 
 * (2, bleu,  7)
 * (4, rouge, 8)
 *
 * hélices : (2,4,blanc) (1,3,rien)
 */

/**
 * <Servo.h> is used to control the speed of the motors since they work as servomotors.
 */
#include <Servo.h>

/**
 * Constants for ESC/MOTORS.
 * 
 * Pulse lengths are expressed in µs.
 */
#define MIN_PULSE_LENGTH 1000
#define MAX_PULSE_LENGTH 2000

#define MOTORS_LEN 4
int MOTORS_PIN[MOTORS_LEN] {5, 6, 7, 8};
Servo MOTORS[MOTORS_LEN];

// ================================================================================

void esc_setup_attach() {
    for(int i = 0; i < MOTORS_LEN; i++) {
        MOTORS[i].attach(MOTORS_PIN[i], MIN_PULSE_LENGTH, MAX_PULSE_LENGTH);
    }
}

// ================================================================================

void esc_setup_min() {
    for(int i = 0; i < 4; i++) {
        MOTORS[i].writeMicroseconds(MIN_PULSE_LENGTH);
    }
}

// ================================================================================

void esc_setup_max() {
    for(int i = 0; i < 4; i++) {
        MOTORS[i].writeMicroseconds(MAX_PULSE_LENGTH);
    }
}

// ================================================================================

void esc_send_pulse(int pulse1, int pulse2, int pulse3, int pulse4) {
    pulse1 = constrain(pulse1, MIN_PULSE_LENGTH, MAX_PULSE_LENGTH);
    pulse2 = constrain(pulse2, MIN_PULSE_LENGTH, MAX_PULSE_LENGTH);
    pulse3 = constrain(pulse3, MIN_PULSE_LENGTH, MAX_PULSE_LENGTH);
    pulse4 = constrain(pulse4, MIN_PULSE_LENGTH, MAX_PULSE_LENGTH);

    MOTORS[0].writeMicroseconds(pulse1);
    MOTORS[1].writeMicroseconds(pulse2);
    MOTORS[2].writeMicroseconds(pulse3);
    MOTORS[3].writeMicroseconds(pulse4);
}

// ================================================================================

/**
 * Displays instructions to user
 */
void displayInstructions()
{  
    //write_data_from_apc_220("READY - PLEASE SEND INSTRUCTIONS AS FOLLOWING :");
    //write_data_from_apc_220("\t0 : Send min throttle");
    //write_data_from_apc_220("\t1 : Send max throttle");
    //write_data_from_apc_220("\t2 : Run test function\n");
}

// ================================================================================

/*
void loop() {
    recvWithEndMarker();
    showNewNumber();
    
  //float voltage = 0;
  //voltage = battery_voltage();
  //Serial.println(voltage);
  //delay(1000);

  //IR_has_data();
  //delay(1000);

  //configure_motors();
  //read_data_from_apc_220();
  
  byte rx_byte_total, rx_byte_current;
  char buffer[4];

  if (apc220.available()) {
    
    Serial.print("--- received from rc : ");

    while (apc220.available()) {
      rx_byte_current = apc220.read();
      rx_byte_current
      
      Serial.print(rx_byte);
    }

    Serial.println("\n--- done reading from rc.");
    //configure_motors(rx_byte);
  }
}
*/
