
#include <LiquidCrystal_I2C.h>
#define BLYNK_PRINT Serial
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <DHT.h>

# define BLYNK_TEMPLATE_ID "TMPLc012ND7k"
# define BLYNK_DEVICE_NAME "micro"
# define BLYNK_AUTH_TOKEN "c60SfMQwDj06dM7cY1-yCbsigSGy5v8W"



char auth[] = "c60SfMQwDj06dM7cY1—yCbsigSGy5v8W";
char ssid[] = "UIU";
char pass[] = "123456789";

LiquidCrystal_I2C lcd(0x27, 16, 2);
DHT dht(D3, DHT11);
BlynkTimer timer;
bool pirbutton = 0;


#define Buzzer D0
#define MQ2 A0
#define trig D4
#define echo D5
#define PIR D6



String fire_event "fire_event";
String gas_event = "gas_leaked";
String temp_event = "temp";
String ultrasonic_event = "water";


BLYNK_WRITE(V0) {
  digitalWrite(D0, param.asInt());
}

void setup() {
  pinMode(D0, OUTPUT);
  Serial.begin(9600);
  lcd.init();
  lcd.backlight();
  pinMode(Buzzer, OUTPUT);
  pinMode(PIR, INPUT);
  pinMode(trig, OUTPUT);
  pinMode(echo, INPUT);

  Blynk.begin(auth, ssid, pass, "blynk.cloud", 80);
  
  dht.begin();

  lcd.setCursor(0, 0);
  lcd.print("Home Automation");
  lcd.setCursor(4, 1);
  lcd.print("System");
  delay(4000);
  lcd.clear();


  timer.setInterval(100L, gassensor);
  timer.setInterval(100L, DHT11sensor);
  timer.setInterval(100L, pirsensor);
  timer.setInterval(100L, ultrasonic);
}


void gassensor() {
  int value = analogRead(MQ2);
  Serial.println(value);
  value = map(value, 0, 1024, 0, 100);
  if (value <= 55) {
    digitalWrite(Buzzer, LOW);
  } else if (value > 55) {
    Blynk.notify("Warning! Gas leak detected");
    digitalWrite(Buzzer, HIGH);
  }
  Blynk.virtualWrite(V1, value);
  lcd.setCursor(0, 0);
  lcd.print("G:");
  lcd.print(" ");
  lcd.print(value);

}



void DHT11sensor() {
  float h = dht.readHumidity();
  float t = dht.readTemperature();

  if (isnan(h) || isnan(t)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }
  Blynk.virtualWrite(V2, t);
  Blynk.virtualWrite(V3, h);

  lcd.setCursor(8, 0);
  lcd.print("T:");
  lcd.print(t);

  lcd.setCursor(0, 1);
  lcd.print("H:");
  lcd.print(h);

}


void pirsensor() {
  bool value = digitalRead(PIR);
  if (pirbutton == 1) {
    if (value == 0) {
      digitalWrite(Buzzer, LOW);
    } else if (value == 1) {
      Blynk.notify("Warning! Please check your security system");
      digitalWrite(Buzzer, HIGH);
    }
  }
}


void ultrasonic() {
  digitalWrite(trig, LOW);
  delayMicroseconds(4);
  digitalWrite(trig, HIGH);
  delayMicroseconds(10);
  digitalWrite(trig, LOW);
  long t = pulseIn(echo, HIGH);
  long cm = t / 29 / 2;
  Blynk.virtualWrite(V4, cm);

  lcd.setCursor(8, 1);
  lcd.print("W:");
  lcd.print(cm);
  lcd.print("  ");
}


void loop() {
  Blynk.run();
  timer.run();
}