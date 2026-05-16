#include <DHT.h>

// ---------------- 핀 설정 ----------------
#define DHTPIN 7
#define DHTTYPE DHT11   // DHT22면 DHT22로 바꾸기

#define RED_PIN 9
#define GREEN_PIN 10
#define BLUE_PIN 11

#define BUZZER_PIN 8
#define MOTOR_PIN 5   // DC 모터 제어 핀 추가

// ---------------- 객체 ----------------
DHT dht(DHTPIN, DHTTYPE);

// ---------------- 기준 ----------------
const float TEMP_LOW = 24.0;
const float TEMP_HIGH = 27.0;
const float HUMI_LOW = 34.0;
const float HUMI_HIGH = 70.0;

void setup() {
  Serial.begin(9600);
  dht.begin();

  pinMode(RED_PIN, OUTPUT);
  pinMode(GREEN_PIN, OUTPUT);
  pinMode(BLUE_PIN, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(MOTOR_PIN, OUTPUT);   // 모터 핀 출력 설정

  digitalWrite(MOTOR_PIN, LOW); // 시작할 때 모터 꺼두기

  Serial.println("시스템 시작");
}

void loop() {
  float humidity = dht.readHumidity();
  float temperature = dht.readTemperature();

  // 센서 오류 체크
  if (isnan(humidity) || isnan(temperature)) {
    Serial.println("센서 오류");

    // 전부 켜서 오류 표시
    digitalWrite(RED_PIN, HIGH);
    digitalWrite(GREEN_PIN, HIGH);
    digitalWrite(BLUE_PIN, HIGH);

    digitalWrite(BUZZER_PIN, LOW);
    digitalWrite(MOTOR_PIN, LOW);   // 오류 시 모터 끄기
    delay(100);
    return;
  }

  // 시리얼 출력
  Serial.print("온도: ");
  Serial.print(temperature);
  Serial.print(" °C / 습도: ");
  Serial.print(humidity);
  Serial.println(" %");

  // ---------------- LED 제어 ----------------
  // 먼저 다 끄기
  digitalWrite(RED_PIN, LOW);
  digitalWrite(GREEN_PIN, LOW);
  digitalWrite(BLUE_PIN, LOW);

  // ---------------- 온도 + 모터 제어 ----------------
  if (temperature < TEMP_LOW) {
    digitalWrite(BLUE_PIN, HIGH);   // 파랑
    digitalWrite(MOTOR_PIN, LOW);   // 모터 OFF
  }
  else if (temperature < TEMP_HIGH) {
    digitalWrite(GREEN_PIN, HIGH);  // 초록
    digitalWrite(MOTOR_PIN, LOW);   // 모터 OFF
  }
  else {
    digitalWrite(RED_PIN, HIGH);    // 빨강
    digitalWrite(MOTOR_PIN, HIGH);  // 모터 ON
  }

  // ---------------- 버저 제어 ----------------
  if (humidity < HUMI_LOW) {
    digitalWrite(BUZZER_PIN, HIGH);
    delay(150);

    digitalWrite(BUZZER_PIN, LOW);
    delay(150);
  }
  else if (humidity > HUMI_HIGH) {
    digitalWrite(BUZZER_PIN, HIGH);
    delay(50);

    digitalWrite(BUZZER_PIN, LOW);
    delay(50);
  }
  else {
    digitalWrite(BUZZER_PIN, LOW);
  }

  delay(100);
}