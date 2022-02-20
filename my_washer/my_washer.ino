#define SW_VALVE 9
#define SW_TORQUE 12
#define SW_TOGGLE 11
#define SW_DRY  10

#define M_VALVE 2
#define M_TORQUE  3
#define M_ROT_EN  4
#define M_ROT_DIR 5

#define BUZZER  A3

#define ON  1
#define OFF 0

#define ISPRESSED true

void setup() {
  // put your setup code here, to run once:
  pinMode(SW_VALVE, INPUT);
  pinMode(SW_TORQUE, INPUT);
  pinMode(SW_TOGGLE, INPUT);
  pinMode(SW_DRY, INPUT);

  // set pull up
  digitalWrite(SW_VALVE, HIGH);
  digitalWrite(SW_TORQUE, HIGH);
  digitalWrite(SW_TOGGLE, HIGH);
  digitalWrite(SW_DRY, HIGH);

  // set output
  pinMode(M_VALVE, OUTPUT);
  pinMode(M_TORQUE, OUTPUT);
  pinMode(M_ROT_EN, OUTPUT);
  pinMode(M_ROT_DIR, OUTPUT);
  pinMode(BUZZER, OUTPUT);

  Serial.begin(9600);
}

void beepOn() {
  digitalWrite(BUZZER, HIGH);
}

void beepOff() {
  digitalWrite(BUZZER, LOW);
}

void beep(int time) {
  beepOn();
  delay(time);
  beepOff();
}

bool isSwValve() {
  if (digitalRead(SW_VALVE) == LOW)
  return true;
  else return false;
}

bool isSwTorque() {
  if (digitalRead(SW_TORQUE) == LOW)
  return true;
  else return false;
}

bool isSwToggle() {
  if (digitalRead(SW_TOGGLE) == LOW)
  return true;
  else return false;
}

bool isSwDry() {
  if (digitalRead(SW_DRY) == LOW)
  return true;
  else return false;
}

void turnOnValve() {
  Serial.println("Turn ON Valve");
  digitalWrite(M_VALVE, HIGH);
}

void turnOffValve() {
  Serial.println("Turn OFF Valve");
  digitalWrite(M_VALVE, LOW);
}

void turnOnTorque() {
  Serial.println("Turn ON Torque");
  digitalWrite(M_TORQUE, HIGH);
}

void turnOffTorque() {
  Serial.println("Turn OFF Torque");
  digitalWrite(M_TORQUE, LOW);
}

void rotCW() {
  Serial.println("--->>>");
  digitalWrite(M_ROT_DIR, HIGH);
  delay(300);
  digitalWrite(M_ROT_EN, HIGH);
}

void rotCCW() {
  Serial.println("<<<---");
  digitalWrite(M_ROT_DIR, LOW);
  delay(300);
  digitalWrite(M_ROT_EN, HIGH);
}

void rotStop() {
  Serial.println("Stop Rotating");
  digitalWrite(M_ROT_EN, LOW);
}

void toggle() {
  Serial.println("Toggle");
  rotCW();
  delay(2000);
  rotStop();
  delay(2000);
  rotCCW();
  delay(2000);
  rotStop();
  delay(2000);
}

void stopToggle() {
  rotStop();
}

void dryStart() {
  Serial.println("Dry Start");
  turnOffValve();
  if ( digitalRead(M_TORQUE) == LOW ) {
    Serial.println("Please TURN ON Torque");
    return;
  } 
  beep(100);
  delay(50);
  beep(100);
  delay(50);
  beep(100);
  delay(1000);
  rotCW();
}
void dryStop() {
  Serial.println("Dry Stop");
  rotStop();
}

bool isValve() {
  if (digitalRead(M_VALVE) == HIGH) {
    return ON;
  } else return OFF;
}

bool isTorque() {
  if (digitalRead(M_TORQUE) == HIGH) {
    return ON;
  } else return OFF;
}

bool isRotMotor() {
  if (digitalRead(M_ROT_EN) == HIGH) {
    return ON;
  } else return OFF;
}

void loop() {
  // put your main code here, to run repeatedly:

  if ( isSwValve() == ISPRESSED ) {
    Serial.println("SW Valve is Pressed");
    beep(100);
    delay(200);
    if (isValve() == OFF) {
      turnOnValve();
    } else {
      turnOffValve();
    }
  }

  if (isSwTorque() == ISPRESSED) {
    Serial.println("SW Torque is Pressed");
    beep(100);
    delay(200);

    if (isTorque() == OFF) {
      turnOnTorque();
    } else {
      turnOffTorque();
    }
  }

  static int toggleEn = 0;
  if (isSwToggle() == ISPRESSED){
    Serial.println("SW Toggle is Pressed");
    beep(100);
    delay(200);

    if (isSwToggle() == ISPRESSED) {
      beep(1500);
      toggleEn = 0;
      stopToggle();
    } else {
      toggleEn = 1;
    }
  }

  static char toggleMode = 0;
  if (toggleEn) {
    Serial.println("Toggle ");
    Serial.println(toggleMode);
    
    switch (toggleMode) {
      case 0: {
        toggleMode = 1;
        
        rotCW();
        delay(1000);
        rotStop();
        delay(1000);
        break;
      }

      case 1: {
        toggleMode = 0;
        
        rotCCW();
        delay(1000);
        rotStop();
        delay(1000);
        break;
      }
    }
  }

  static int dryEn = 0;
  if (isSwDry() == ISPRESSED) {
    Serial.println("SW Dry is Pressed");
    beep(100);
    delay(200);
    if (toggleEn == 1) {
      toggleEn = 0;
      stopToggle();
    }
    if (isSwDry() == ISPRESSED) {
      beep(1500);
      dryEn = 0;
      dryStop();
    } else {
      dryStart();
      dryEn = 1;
    }
  }
}
