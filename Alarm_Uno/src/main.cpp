#include <Arduino.h>
#include <stdlib.h>
#include <time.h>
#include <stdio.h>

void setup() {
  Serial.begin(9600);
  srand(time(NULL));

  pinMode(6,OUTPUT);
  pinMode(7,OUTPUT);

  char a[111];
  
  sprintf(a,"%c",'s');
  Serial.write(a);
}

int n;

char b[100];

void loop() {
  if(Serial.available()>0){
    char c=Serial.read();
    if(c=='f'){
      digitalWrite(7, LOW);
      digitalWrite(6, HIGH);
    }
    else if(c=='p'){
      digitalWrite(6, LOW);
      digitalWrite(7, HIGH);
    }

  }
}
