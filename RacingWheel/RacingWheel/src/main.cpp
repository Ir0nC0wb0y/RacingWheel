#include <Arduino.h>
// #include <Joystick.h> // comes from https://github.com/MHeironimus/ArduinoJoystickLibrary

// Encoder Vars
  #define ENC_WHEEL_A                    3 // This needs to be updated!! pin 3 maps to ISR 0
  #define ENC_WHEEL_B                    4 // This needs to be updated!!
  volatile bool _EncWheelASet;
  volatile bool _EncWheelBSet;
  volatile long _EncWheelTicks     =     0;
  //#define readA bitRead(PIND, ENC_WHEEL_A)
  //#define readB bitRead(PIND, ENC_WHEEL_B)

// Wheel Vars
  double wheel_angle               =   0.0;

// Loop Vars
#define LOOP_PER                       250
unsigned long next_loop            =     0;
int loop_catch                     =     0;

void UpdateEncoder_Wheel() {
  _EncWheelASet = digitalRead(ENC_WHEEL_A);
  _EncWheelBSet = digitalRead(ENC_WHEEL_B);
  if (_EncWheelASet) {
    _EncWheelTicks -= _EncWheelBSet ? -1 : +1;
  } else {
    _EncWheelTicks += _EncWheelBSet ? -1 : +1;
  }
  
}

void setup() {
  Serial.begin(9600);

  pinMode(ENC_WHEEL_A,INPUT_PULLUP);
  pinMode(ENC_WHEEL_B,INPUT_PULLUP);

  attachInterrupt(digitalPinToInterrupt(ENC_WHEEL_A), UpdateEncoder_Wheel, CHANGE);

}

void loop() {
  if (millis() >= next_loop) {
    next_loop = millis() + LOOP_PER;

    if (loop_catch < 20) {
      Serial.print(millis()); Serial.print(",");Serial.println(next_loop);
      loop_catch++;
    }

    Serial.print("Wheel Angle: "); Serial.println(360 * (double)_EncWheelTicks / 1200.0);

    
    //Serial.print("Encoder count: "); Serial.println(_EncWheelTicks);
    //Serial.print("A: "); Serial.print(digitalRead(ENC_WHEEL_A)); Serial.print(", B: "); Serial.println(digitalRead(ENC_WHEEL_B));
    //next_loop = millis() + 250;
  }
}