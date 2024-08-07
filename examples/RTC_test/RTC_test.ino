#include <Wire.h>
#include <RTClib.h>
#include <avr/sleep.h>
#include <avr/wdt.h>

RTC_DS3231 rtc;

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

  // Set the first alarm for 1 minute from now
  setAlarmInMinutes(1);

  // Enable alarm interrupts
  pinMode(2, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(2), wakeUp, LOW);
}

void loop() {
  // Stay awake for 1 minute
  Serial.println("Awake...");
  delay(60000); // Delay for 1 minute

  // Set the alarm for 1 minute from now
  setAlarmInMinutes(1);

  // Put the Arduino to sleep
  Serial.println("Going to sleep...");
  enterSleep();
}

void setAlarmInMinutes(int minutes) {
  DateTime now = rtc.now();
  DateTime alarmTime = now + TimeSpan(0, 0, minutes, 0);
  rtc.setAlarm1(alarmTime, DS3231_A1_Minute);
  rtc.clearAlarm(1);
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
