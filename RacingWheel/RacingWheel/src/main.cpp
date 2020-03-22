#include <Arduino.h>
#include <Joystick.h> // comes from https://github.com/MHeironimus/ArduinoJoystickLibrary

// Encoder Vars
  #define ENC_WHEEL_A                    3 // This needs to be updated!! pin 3 maps to ISR 0
  #define ENC_WHEEL_B                    4 // This needs to be updated!!
  volatile bool _EncWheelASet;
  volatile bool _EncWheelBSet;
  volatile long _EncWheelTicks     =     0;
  #define ENC_WHEEL_PPR                600
  #define ENC_WHEEL_QUAD_USE             2
  #define AXIS_WHEEL_MAX              4095
  #define AXIS_WHEEL_MIN             -4095
  #define ROT_WHEEL_MAX               1500
  #define ROT_WHEEL_MIN              -1500
  
// Wheel Vars
  //double wheel_angle               =   0.0;
  int wheel_pos                    =     0;

// Loop Vars
#define LOOP_PER                        50
unsigned long next_loop            =     0;
int loop_catch                     =     0;

// Joystick
Joystick_ Joystick(JOYSTICK_DEFAULT_REPORT_ID,JOYSTICK_TYPE_GAMEPAD,
  0, 0,                    // Button Count, Hat Switch Count
  false, false, false,     // X, Y, Z
  false, false, false,     // Rx, Ry, Rz
  false, false,            // rudder, throttle
  false, false, true);     // accelerator, brake, steering


void UpdateEncoder_Wheel() {
  _EncWheelASet = digitalRead(ENC_WHEEL_A);
  _EncWheelBSet = digitalRead(ENC_WHEEL_B);
  if (_EncWheelASet) {
    _EncWheelTicks -= _EncWheelBSet ? -1 : +1;
  } else {
    _EncWheelTicks += _EncWheelBSet ? -1 : +1;
  }
  
}

double WheelAngle() {
  // Converts wheel encoder position to angle (degrees)
  double WheelAngle = (360 * (double)_EncWheelTicks) / (ENC_WHEEL_PPR * ENC_WHEEL_QUAD_USE);
  return WheelAngle;
}

void UpdateWheel() {
  //int wheel_pos;
  if (_EncWheelTicks >= 0) {
    if (_EncWheelTicks >= ROT_WHEEL_MAX) {
      wheel_pos = AXIS_WHEEL_MAX;
    } else {
      wheel_pos = map(_EncWheelTicks,0,ROT_WHEEL_MAX,0,AXIS_WHEEL_MAX);
    }
  } else {
    if (_EncWheelTicks <= ROT_WHEEL_MIN) {
      wheel_pos = AXIS_WHEEL_MIN;
    } else {
      wheel_pos = map(_EncWheelTicks,ROT_WHEEL_MIN,0,AXIS_WHEEL_MIN,0);
    }
  }
  //return wheel_pos;
}

void setup() {
  Serial.begin(9600);

  pinMode(ENC_WHEEL_A,INPUT_PULLUP);
  pinMode(ENC_WHEEL_B,INPUT_PULLUP);

  attachInterrupt(digitalPinToInterrupt(ENC_WHEEL_A), UpdateEncoder_Wheel, CHANGE);


  Joystick.setSteeringRange(AXIS_WHEEL_MIN,AXIS_WHEEL_MAX);
  Joystick.begin();
}

void loop() {
  if (millis() >= next_loop) {
    next_loop = millis() + LOOP_PER;

    if (loop_catch < 20) {
      Serial.print(millis()); Serial.print(",");Serial.println(next_loop);
      loop_catch++;
    }
    UpdateWheel();
    Joystick.setSteering(wheel_pos);
    Serial.print("Wheel Pos: "); Serial.println(wheel_pos);
  }
}