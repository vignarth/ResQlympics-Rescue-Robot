#include <Arduino.h>
#include "sbus.h"

using namespace bfs;

// ================= SBUS =================
#define SBUS_RX_PIN 16
#define SBUS_TX_PIN 17
SbusRx sbus(&Serial2, SBUS_RX_PIN, SBUS_TX_PIN, true);

// ============== LEFT MOTOR (IBT-2) ==============
#define L_RPWM 25
#define L_LPWM 26
#define L_REN  27
#define L_LEN  14

// ============== RIGHT MOTOR (IBT-2) ==============
#define R_RPWM 33
#define R_LPWM 32
#define R_REN  15
#define R_LEN  4

// ============== SERVO PINS ==============
#define SERVO1_PIN 19   // CH8
#define SERVO2_PIN 18   // CH7
#define SERVO3_PIN 5    // CH6

// PWM settings
#define PWM_FREQ_MOTOR 20000
#define PWM_FREQ_SERVO 50
#define PWM_RES_MOTOR  8
#define PWM_RES_SERVO  16

// PWM Channels
#define CH_L_RPWM 0
#define CH_L_LPWM 1
#define CH_R_RPWM 2
#define CH_R_LPWM 3
#define CH_SERVO1 4
#define CH_SERVO2 5
#define CH_SERVO3 6

// SBUS range
#define SBUS_MIN 172
#define SBUS_MID 992
#define SBUS_MAX 1811
#define DEADZONE 40

// Servo pulse (16-bit, 50Hz)
#define SERVO_MIN 1638   // ~500us
#define SERVO_MAX 8192   // ~2500us

// ================= UTILS =================

int sbusToPWM(int value) {
  if (abs(value - SBUS_MID) < DEADZONE) return 0;
  return map(value, SBUS_MIN, SBUS_MAX, -255, 255);
}

uint32_t sbusToServo(int value) {
  value = constrain(value, SBUS_MIN, SBUS_MAX);
  return map(value, SBUS_MIN, SBUS_MAX, SERVO_MIN, SERVO_MAX);
}

void driveMotor(int pwm, int ch_r, int ch_l) {
  pwm = constrain(pwm, -255, 255);

  if (pwm > 0) {
    ledcWrite(ch_r, pwm);
    ledcWrite(ch_l, 0);
  } else if (pwm < 0) {
    ledcWrite(ch_r, 0);
    ledcWrite(ch_l, -pwm);
  } else {
    ledcWrite(ch_r, 0);
    ledcWrite(ch_l, 0);
  }
}

// ================= SETUP =================

void setup() {
  Serial.begin(115200);

  // Enable BTS7960
  pinMode(L_REN, OUTPUT);
  pinMode(L_LEN, OUTPUT);
  pinMode(R_REN, OUTPUT);
  pinMode(R_LEN, OUTPUT);

  digitalWrite(L_REN, HIGH);
  digitalWrite(L_LEN, HIGH);
  digitalWrite(R_REN, HIGH);
  digitalWrite(R_LEN, HIGH);

  // Motor PWM
  ledcSetup(CH_L_RPWM, PWM_FREQ_MOTOR, PWM_RES_MOTOR);
  ledcSetup(CH_L_LPWM, PWM_FREQ_MOTOR, PWM_RES_MOTOR);
  ledcSetup(CH_R_RPWM, PWM_FREQ_MOTOR, PWM_RES_MOTOR);
  ledcSetup(CH_R_LPWM, PWM_FREQ_MOTOR, PWM_RES_MOTOR);

  ledcAttachPin(L_RPWM, CH_L_RPWM);
  ledcAttachPin(L_LPWM, CH_L_LPWM);
  ledcAttachPin(R_RPWM, CH_R_RPWM);
  ledcAttachPin(R_LPWM, CH_R_LPWM);

  // Servo PWM
  ledcSetup(CH_SERVO1, PWM_FREQ_SERVO, PWM_RES_SERVO);
  ledcSetup(CH_SERVO2, PWM_FREQ_SERVO, PWM_RES_SERVO);
  ledcSetup(CH_SERVO3, PWM_FREQ_SERVO, PWM_RES_SERVO);

  ledcAttachPin(SERVO1_PIN, CH_SERVO1);
  ledcAttachPin(SERVO2_PIN, CH_SERVO2);
  ledcAttachPin(SERVO3_PIN, CH_SERVO3);

  // SBUS
  sbus.Begin();

  Serial.println("SBUS + MOTORS + 3 SERVOS READY");
}

// ================= LOOP =================

void loop() {
  if (sbus.Read()) {

    SbusData data = sbus.data();

    // FAILSAFE
    if (data.failsafe || data.lost_frame) {
      driveMotor(0, CH_L_RPWM, CH_L_LPWM);
      driveMotor(0, CH_R_RPWM, CH_R_LPWM);
      return;
    }

    // ---- MOTORS ----
    int throttle = sbusToPWM(data.ch[1]); // CH2
    int steering = sbusToPWM(data.ch[0]); // CH1

    int leftMotor  = throttle + steering;
    int rightMotor = throttle - steering;

    driveMotor(leftMotor, CH_L_RPWM, CH_L_LPWM);
    driveMotor(rightMotor, CH_R_RPWM, CH_R_LPWM);

    // ---- SERVOS ----
    ledcWrite(CH_SERVO1, sbusToServo(data.ch[7])); // CH8
    ledcWrite(CH_SERVO2, sbusToServo(data.ch[6])); // CH7
    ledcWrite(CH_SERVO3, sbusToServo(data.ch[5])); // CH6
  }
}