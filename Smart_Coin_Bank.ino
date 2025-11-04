#include <Servo.h>               // 서보모터 헤더파일
#include <LiquidCrystal_I2C.h>   // LCD 헤더파일
#include <Keypad.h>              // 키패드 헤더파일

LiquidCrystal_I2C lcd(0x27, 16, 2); // LCD 핀번호 선언

int buzzer = 10;     // 부저 핀 설정
int tru = 0;         // 맞춘 비밀번호 개수
int count = 0;       // 입력된 개수
int a = 0;           // LCD 위치 조정용
int angle = 0;       // 서보 각도

char PW[4] = {'1','2','3','A'};  // 비밀번호 설정

const byte ROWS = 4; // 행의 개수
const byte COLS = 4; // 열의 개수

byte rowPins[ROWS] = {9, 8, 7, 6};     // 키패드 행 핀
byte colPins[COLS] = {5, 4, 3, 2};     // 키패드 열 핀

char keys[ROWS][COLS] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};

Servo myservo; // 서보모터 객체 생성
Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

void setup() {
  lcd.init();         // LCD 초기화
  lcd.backlight();    // LCD 백라이트 ON
  myservo.attach(11); // 서보모터 11번 핀 설정
  pinMode(buzzer, OUTPUT);
}

void loop() {
  char key = keypad.getKey(); // 키패드 입력 받기

  if (key) {
    lcd.setCursor(a, 0);
    a++;
    lcd.print(key); // LCD에 입력된 키 표시

    // 입력된 키가 비밀번호와 일치하면 tru++
    if (key == PW[count]) {
      count++;
      tru++;
    } else {
      count++;
    }

    // 비밀번호 4자리 입력 완료 시 확인
    if (count == 4) {
      if (tru == 4) {
        Su();  // 성공 함수
      } else {
        Fa();  // 실패 함수
      }
      tru = 0;
      count = 0;
      a = 0;
    }

    // 비밀번호 초기화 키 (‘*’)
    if (key == '*') {
      re();
    }

    // 비밀번호 전체 리셋 키 (‘#’)
    if (key == '#') {
      cl();
    }
  }
}

// ==============================
// 성공 함수
// ==============================
void Su() {
  lcd.setCursor(0, 1);
  lcd.print("open the door");
  for (angle = 0; angle < 90; angle++) {
    myservo.write(angle);
    delay(10);
  }
  delay(300);
  lcd.clear();
}

// ==============================
// 실패 함수
// ==============================
void Fa() {
  lcd.setCursor(0, 1);
  lcd.print("wrong");
  tone(buzzer, 1000, 500);
  delay(300);
  lcd.clear();
}

// ==============================
// 비밀번호 초기화 ('*')
// ==============================
void re() {
  lcd.setCursor(0, 1);
  tru = 0;
  count = 0;
  lcd.print("password reset");
  delay(300);
  lcd.clear();
}

// ==============================
// LCD 클리어 ('#')
// ==============================
void cl() {
  lcd.setCursor(0, 1);
  tru = 0;
  count = 0;
  a = 0;
  lcd.clear();
}
