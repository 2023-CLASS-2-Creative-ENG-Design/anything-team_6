#include <LiquidCrystal.h>
#include <Wire.h>
#include <SPI.h>

#include <time.h>

#define MAX 5
#define btn1 9
#define btn2 10
#define btn3 12
#define btn4 11
#define resetbtn 13

int randNumber[MAX];

LiquidCrystal lcd(7,6,5,4,3,2);
enum direction {UP, DOWN, RIGHT, LEFT};

char buff[32];

void print_init(){
    byte up1[8] = {
        B00000,
        B00000,
        B00000,
        B00000,
        B00001,
        B00010,
        B00100,
        B01000
    };
    byte up2[8] = {
        B00100,
        B01110,
        B10101,
        B00100,
        B00100,
        B00100,
        B00100,
        B00100
    };

    byte down4[8] = { // left4
        B01000,
        B00100,
        B00010,
        B00001,
        B00000,
        B00000,
        B00000,
        B00000
    };
    byte down5[8] = {
        B00100,
        B00100,
        B00100,
        B00100,
        B00100,
        B10101,
        B01110,
        B00100
    };
    byte down6[8] = { // right6
        B00010,
        B00100,
        B01000,
        B10000,
        B00000,
        B00000,
        B00000,
        B00000
    };

    byte right3[8] = {
        B00000,
        B00000,
        B10000,
        B01000,
        B00100,
        B00010,
        B11111,
        B00000,
    };

    byte left1[8] = {
        B00000,
        B00000,
        B00001,
        B00010,
        B00100,
        B01000,
        B11111,
        B00000
    };

    lcd.createChar(1, up1);
    lcd.createChar(2, up2);
    lcd.createChar(3, down4);
    lcd.createChar(4, down5);
    lcd.createChar(5, down6);
    lcd.createChar(6, right3);
    lcd.createChar(7, left1);
}

void print_arrow(int i, direction j){ //i는 슬롯(0~4) j는 화살표 방향
    if(j==UP){
        lcd.setCursor(3*i, 0);
        lcd.write(byte(1));

        lcd.setCursor(3*i+1, 0);
        lcd.write(byte(2));

        lcd.setCursor(3*i+2, 0);
        lcd.write(byte(B10100100));

        lcd.setCursor(3*i+1, 1);
        lcd.write(byte(B01111100));
    }
    else if (j==DOWN){
        lcd.setCursor(3*i+1, 0);
        lcd.write(byte(B01111100));

        lcd.setCursor(3*i, 1);
        lcd.write(byte(3));

        lcd.setCursor(3*i+1, 1);
        lcd.write(byte(4));

        lcd.setCursor(3*i+2, 1);
        lcd.write(byte(5));
    }
    else if (j==RIGHT){
        lcd.setCursor(3*i, 0);
        lcd.write(byte(B01011111));

        lcd.setCursor(3*i+1, 0);
        lcd.write(byte(B01011111));

        lcd.setCursor(3*i+2, 0);
        lcd.write(byte(6));

        lcd.setCursor(3*i+2, 1);
        lcd.write(byte(5));
    }
    else if (j==LEFT){
        lcd.setCursor(3*i, 0);
        lcd.write(byte(7));

        lcd.setCursor(3*i+1, 0);
        lcd.write(byte(B01011111));

        lcd.setCursor(3*i+2, 0);
        lcd.write(byte(B01011111));

        lcd.setCursor(3*i, 1);
        lcd.write(byte(3));
    }
}

void make_delay(int i,int j){
    delay(i);
    lcd.noDisplay();
    delay(j);
    lcd.display();
}

void random_sign(){
        // 랜덤값 생성 코드
    for(int i = 0; i < MAX ; i++)
    {
        randNumber[i] = random(0,4);
    }

    for(int i = 0; i < MAX; i++){
        sprintf(buff, "%d ", randNumber[i]);
        Serial.write(buff);
    }
    //lcd 화살표 출력 코드
    lcd.clear();
    for(int i = 0; i < MAX; i++){
        print_arrow(i, (direction)(randNumber[i]));
    }
    make_delay(100, 500);
    return;
}

int input_btn(){
    //버튼 입력 함수
    int btn1_state = 0,
        btn2_state = 0,
        btn3_state = 0,
        btn4_state = 0,
        resetbtn_state = 0;
        
    btn1_state = digitalRead(btn1);
    btn2_state = digitalRead(btn2);
    btn3_state = digitalRead(btn3);
    btn4_state = digitalRead(btn4);
    resetbtn_state = digitalRead(resetbtn);
    delay(50);
    
    if (btn1_state == 1)
    {
        Serial.write("111111111111111111111111111111111\n");
        return 1;
    }
    else if(btn2_state == 1)
    {
        Serial.write("2222222222222222222222222222222222\n");
        return 2;
    }
    else if(btn3_state == 1)
    {
        Serial.write("33333333333333333333333333333333333\n");
        return 3;
    }
    else if(btn4_state == 1)
    {
        Serial.write("4444444444444444444444444444444444\n");
        return 4;
    }
    else if(resetbtn_state  == 1)
    {
        Serial.write("55555555555555555555555555555555555\n");
        return 5;
    }
    else
    {
      return 0;
    }

}

int check_btn(int rand_numbers, int btn_value){
    //랜덤 생성 넘버와 비교
    if(rand_numbers == btn_value){
        return 1;
    }
    else{
        return 0;
    }
}

int start_check(){
    int cnt = 0;

    while (1)
    {
        
        if(cnt > 2){
          lcd.clear();
            return 1;
        }
            
            
        random_sign();
        Serial.write("111111111\n");
        int check_cnt = 0;
        int reset_cnt = 0;
        while (1)
        {
            int btn_value = 0;
            btn_value = input_btn();
            delay(50);
            sprintf(buff, "%d", btn_value);
            Serial.write(buff);

            if(check_cnt > 4){//버튼 횟수 5회 이상
                Serial.write("44444444444444\n");
                return 1;
            }
            
            if(!(btn_value == 1 || btn_value == 2 || btn_value == 3 || btn_value == 4 || btn_value == 5)){
                btn_value = 0;
                continue;
            }
            
            if (reset_cnt > 3)//리셋 버튼 횟수 3회 이상
            {
                lcd.clear();
                return 0;
            }
            
            if (btn_value == 5)//리셋 버튼 눌렀을 때
            {
                reset_cnt++;
                make_delay(0, 500);
                Serial.write("6666666666666666666666666\n");
                continue;
            }
            
            if(check_btn(randNumber[check_cnt], (btn_value-1))){//버튼 값 확인
                //맞았을 때
                make_delay(0, 500);
                check_cnt++;
            }
            else
            {
                //틀렸습니다.
                cnt = -1;// 카운트 초기화
                Serial.write("555555555555555555555\n");
                break;
            }
            Serial.write("3333333333\n");
            delay(500);
        }
        cnt++;
    }
}



void setup() {
    lcd.begin(16, 2);
    print_init();

    Serial.begin(9600);

    randomSeed(analogRead(0));
    pinMode(btn1, INPUT);
    pinMode(btn2, INPUT);
    pinMode(btn3, INPUT);
    pinMode(btn4, INPUT);
    pinMode(resetbtn, INPUT);
}

void loop() {

    if(Serial.available()>0){
        char c=Serial.read();

        if(c=='s'){
            if(start_check()){
                Serial.println("p");
            }
            else{
                Serial.println("f");
            }
        }
    }

    
    
}
