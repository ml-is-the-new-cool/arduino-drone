#include <Servo.h>


#define MIN_PULSE_LENGTH 1000 // µs
#define MAX_PULSE_LENGTH 2000 // µs
#define MOTORS_N 4
int MOTORS_PIN[MOTORS_N] = {2, 3, 4, 5};
Servo MOTORS[MOTORS_N];

// ================================================================================

void escSetupAttach() {
  for(int i = 0; i < MOTORS_N; i++)
    MOTORS[i].attach(MOTORS_PIN[i], MIN_PULSE_LENGTH, MAX_PULSE_LENGTH);
}

// ================================================================================

void escSetupMin() {
  for(int i = 0; i < MOTORS_N; i++)
    MOTORS[i].writeMicroseconds(MIN_PULSE_LENGTH);
}

// ================================================================================

void escSetupMax() {
  for(int i = 0; i < MOTORS_N; i++)
    MOTORS[i].writeMicroseconds(MAX_PULSE_LENGTH);
}

// ================================================================================

void escSendPulses(int *pulses) {
  int pulse_constrain = 0;
  
  for(int i = 0; i < MOTORS_N; i++) {
    pulse_constrain = constrain(pulses[i], MIN_PULSE_LENGTH, MAX_PULSE_LENGTH);
    MOTORS[i].writeMicroseconds(pulse_constrain);
  }
}

/*
#define FREQ 250


unsigned int  period; // Sampling period in µs
unsigned long loop_timer;


unsigned long pulse_length_esc1 = 1000,
        pulse_length_esc2 = 1000,
        pulse_length_esc3 = 1000,
        pulse_length_esc4 = 1000;


void esc_setup() {
  DDRD |= B11110000;

  period = (1000000/FREQ);

  loop_timer = micros();
}


void esc_apply_speed() {
  while ((now = micros()) - loop_timer < period);

  loop_timer = now;
  
  PORTD |= B11110000;

  while (PORTD >= 16) {
        now        = micros();
        difference = now - loop_timer;

        if (difference >= pulse_length_esc1) PORTD &= B11101111; // Set pin #4 LOW
        if (difference >= pulse_length_esc2) PORTD &= B11011111; // Set pin #5 LOW
        if (difference >= pulse_length_esc3) PORTD &= B10111111; // Set pin #6 LOW
        if (difference >= pulse_length_esc4) PORTD &= B01111111; // Set pin #7 LOW
  }
}
*/
