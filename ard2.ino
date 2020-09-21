#include <SoftwareSerial.h>
#include <LiquidCrystal.h>

SoftwareSerial soft(13,12);

int raised_state = 2 * 1000;
int down_state = 1 * 1000;
int wait = 20 * 1000;
char c = 'c';

const int RS = 8;
const int enable = 6;
const int DB4 = 5;
const int DB5 = 4;
const int DB6 = 3;
const int DB7 = 2;

LiquidCrystal lcd(RS, enable, DB4, DB5, DB6, DB7);

void setup()
{
  Serial.begin(9600);
  pinMode(14, OUTPUT);
  pinMode(15, OUTPUT);
  
  pinMode(12,INPUT_PULLUP);
  soft.begin(3000);
  lcd.begin(16,2);
}

void loop()
{
  lcd.clear();
  int counter = 0;
  while(Serial.available())
  {
   	c = Serial.read();
    Serial.println(c);
    switch(c){
      case '1': 
      while(Serial.read() != 'n'){
      	digitalWrite(17, HIGH);
        delayMicroseconds(raised_state);
        digitalWrite(17, LOW);
        delayMicroseconds(wait-raised_state);
      }
      break;
      case '2':
      while(Serial.read() != 'n'){
      	digitalWrite(17, HIGH);
        delayMicroseconds(down_state);
        digitalWrite(17, LOW);
        delayMicroseconds(wait-down_state);
      }
      break;
      
      case '3': 
      while(Serial.read() != 'n'){
      	digitalWrite(16, HIGH);
        delayMicroseconds(raised_state);
        digitalWrite(16, LOW);
        delayMicroseconds(wait-raised_state);
      }
      break;
      case '4':
      while(Serial.read() != 'n'){
      	digitalWrite(16, HIGH);
        delayMicroseconds(down_state);
        digitalWrite(16, LOW);
        delayMicroseconds(wait-down_state);
      }
      break;
      
      case '5': 
      while(Serial.read() != 'n'){
      	digitalWrite(15, HIGH);
        delayMicroseconds(raised_state);
        digitalWrite(15, LOW);
        delayMicroseconds(wait-raised_state);
      }
      break;
      case '6':
      while(Serial.read() != 'n'){
      	digitalWrite(15, HIGH);
        delayMicroseconds(down_state);
        digitalWrite(15, LOW);
        delayMicroseconds(wait-down_state);
      }
      break;
      
      case '7': 
      while(Serial.read() != 'n'){
      	digitalWrite(14, HIGH);
        delayMicroseconds(raised_state);
        digitalWrite(14, LOW);
        delayMicroseconds(wait-raised_state);
      }
      break;
      case '8':
      while(Serial.read() != 'n'){
      	digitalWrite(14, HIGH);
        delayMicroseconds(down_state);
        digitalWrite(14, LOW);
        delayMicroseconds(wait-down_state);
      }
      break;
      
    }
  }   
  if(soft.available()){
  	while(soft.available()){
      if(counter ==16){
        lcd.setCursor(0, 1);
        lcd.noCursor();
      }
      char c = soft.read();
      lcd.write(c);
      counter++;
  	}
    delay(500);
  }
  else{
    lcd.clear();
  }
  
}