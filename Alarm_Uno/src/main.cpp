#include <Arduino.h>
#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include <DS1302.h>
#define CHECK_S 10

const int set_hour = 0;
const int set_min = 0;
const int set_sec = 10;

const int CLK = 4;  // Clock 을  5번핀 설정
const int DAT = 3;  // Data를  6번핀 설정
const int RST = 2;  // Reset을  7번핀 설정
DS1302 rtc(RST, DAT, CLK);  // DS1302  객체 설정

const int pressurePin = A0;  // 압력 센서 연결 핀
const int buzzerPin = 5;  // 부저 연결 핀

int tones[] = {261, 277, 294, 311, 330, 349, 370, 392};
int numTones = 8;

int pressureThreshold = 500;  // 압력 감지 임계값 (조절 필요)


void Serial_char(char a){ // 한글자를 시리얼로 보내는 함수
  char buff[111];
  sprintf(buff,"%c\n",a);
  Serial.write(buff);
}

void Serial_unsigned_long(unsigned long a){ // unsigned long을 시리얼로 보내는 함수
  char buff[111];
  sprintf(buff,"%lu\n",a);
  Serial.write(buff);
}

void buzzer_on() {
  for(int i = 0; i < numTones; i++)  {
    tone(buzzerPin, tones[i]);
    delay(500);
  }
  noTone(buzzerPin);
  delay(1000);
}

int pressure_check(int check_time){
  unsigned long startTime = millis();  // 측정 시작 시간
  unsigned long pressureDetectedTime = 0; // 압력 감지 시간
  unsigned long pressure;
  while (millis()-startTime < check_time*1000) {  // check_time(초) 동안 반복
    pressure = analogRead(pressurePin);  // 압력 센서 값 읽기
    if (pressure > pressureThreshold) 
      pressureDetectedTime += 1;  // 압력이 임계값 이상이면 감지 시간 1초 증가

    Serial_unsigned_long(pressure);
    delay(1000);
  }

  if (pressureDetectedTime*2 >= check_time) return 1; // 압력 감지 시간이 50% 이상이면 1 반환
  else return 0; // 아니면 0 반환
}

void start_moring(){
  buzzer_on();
  Serial_char('s');
  while(1){
    if(Serial.available()>0){
      char c=Serial.read();
      if(c=='p'){
        for(int i=0;i<3;i++){
          if(pressure_check(CHECK_S)){
            Serial.println("4321");
            buzzer_on();
            Serial_char('s');
            break;
          }
        }
        return;
      }
      else if(c=='f') return;
    }
  }
}

void setup() {
  Serial.begin(9600);
  srand(time(NULL));

  rtc.halt(false);  
  rtc.writeProtect(false);   

  rtc.setDOW(FRIDAY);     // Set Day-of-Week  
  rtc.setTime(0, 0, 0);    // Set the time (HH:MM:SS)  
  rtc.setDate(6, 12, 2023);   // Set the date (DD.MM.YY)

  pinMode(pressurePin, INPUT);
  pinMode(buzzerPin, OUTPUT);

  
}

const int test = 0;

void loop() {

  if(test){
    start_moring();
    Serial.println("1234");
    delay(90000);
  }
  else{
    Time t=rtc.getTime();  // 현재 시간을 t에 저장
    Serial.print("data: ");
    Serial.print(t.date);
    Serial.print(" ");
    Serial.print(t.mon);
    Serial.print(" ");
    Serial.println(t.year);
    
    Serial.print(" time: ");
    Serial.print(t.hour);
    Serial.print(":");
    Serial.print(t.min);
    Serial.print(":");
    Serial.println(t.sec);

    if(t.hour == set_hour && t.min == set_min && t.sec == set_sec){ // 설정한 시간이 되면
      start_moring();
    }

    delay(1000);
  }

  
}