#include <Wire.h>
#include <LiquidCrystal_I2C.h>

const int TRIG_PIN = 7;
const int ECHO_PIN = 6;
const int BUZZER_PIN = 3;

const int ALARM_DISTANCE_CM = 50;

LiquidCrystal_I2C lcd(0x27, 16, 2);

bool alarmActive = false;

long readDistanceCM() {
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  long duration = pulseIn(ECHO_PIN, HIGH, 30000);
  if (duration == 0) return 999;

  return (duration * 0.0343) / 2;
}

void setup() {
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  digitalWrite(BUZZER_PIN, HIGH);

  Wire.begin();
  lcd.init();
  lcd.clear();
  lcd.backlight();
}

void loop() {
  long distance = readDistanceCM();

  if (distance < ALARM_DISTANCE_CM && !alarmActive) {
    alarmActive = true;

    digitalWrite(BUZZER_PIN, LOW);

    lcd.setCursor(0, 0);
    lcd.print("Status: ALERT! ");
    lcd.setCursor(0, 1);
    lcd.print("Dist: ");
    lcd.print(distance);
    lcd.print(" cm    ");

    delay(3000);

    digitalWrite(BUZZER_PIN, HIGH);
    alarmActive = false;
  } else if (!alarmActive) {
    digitalWrite(BUZZER_PIN, HIGH);

    lcd.setCursor(0, 0);
    lcd.print("Status: ARMED  ");
    lcd.setCursor(0, 1);
    
    if (distance == 999) {
      lcd.print("Dist: Out Range");
    } else {
      lcd.print("Dist: ");
      lcd.print(distance);
      lcd.print(" cm    ");
    }
  }

  delay(150);
}