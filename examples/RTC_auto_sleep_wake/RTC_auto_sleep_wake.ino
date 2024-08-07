#include <Wire.h>
#include <RTClib.h>
#include <avr/sleep.h>
#include <avr/wdt.h>

RTC_DS3231 rtc;

// Variables to store alarm times
const int wakeHours[] = {0, 7, 12, 17}; // Wake up times: midnight, 7am, 12pm, 5pm
const int wakeMinutes[] = {0, 0, 0, 0};

void setup() {
  Serial.begin(9600);
  Wire.begin();

  // Initialize the RTC
  if (!rtc.begin()) {
    Serial.println("Couldn't find RTC");
    while (1);
  }

  // Set RTC time to compile time if it lost power
  if (rtc.lostPower()) {
    Serial.println("RTC lost power, setting the time!");
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__))); // Set RTC to compile date and time
  }

  // Clear any existing alarms
  rtc.writeSqwPinMode(DS3231_OFF);
  rtc.clearAlarm(1);
  rtc.clearAlarm(2);

  // Set alarms
  setAlarms();

  // Enable alarm interrupts
  pinMode(2, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(2), wakeUp, LOW);
}

void loop() {
  // Put the Arduino to sleep
  enterSleep();
}

void setAlarms() {
  for (int i = 0; i < 4; i++) {
    DateTime now = rtc.now();
    DateTime alarmTime(now.year(), now.month(), now.day(), wakeHours[i], wakeMinutes[i], 0);

    if (i == 0) {
      rtc.setAlarm1(alarmTime, DS3231_A1_Hour);
    } else if (i == 1) {
      rtc.setAlarm2(alarmTime, DS3231_A2_Hour);
    } else if (i == 2) {
      rtc.setAlarm1(alarmTime, DS3231_A1_Hour);
    } else if (i == 3) {
      rtc.setAlarm2(alarmTime, DS3231_A2_Hour);
    }
  }
}

void enterSleep() {
  // Disable ADC
  ADCSRA = 0;

  // Set sleep mode
  set_sleep_mode(SLEEP_MODE_PWR_DOWN);
  sleep_enable();
  sleep_cpu();

  // CPU is asleep here

  // Wake up here
  sleep_disable();
  ADCSRA = (1 << ADEN); // Re-enable ADC

  Serial.println("Woke up!");
}

void wakeUp() {
  // This function is called when the interrupt pin goes low
  // It is intentionally left empty because its mere presence as an ISR will wake up the Arduino from sleep mode.
}
