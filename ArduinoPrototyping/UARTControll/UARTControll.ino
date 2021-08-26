#include <SoftwareSerial.h>

#define PIN_ENA 9 // Вывод управления скоростью вращения мотора №1
#define PIN_IN1 8 // Вывод управления направлением вращения мотора №1
#define PIN_IN2 7 // Вывод управления направлением вращения мотора №1
#define PIN_ENB 6 // Вывод управления скоростью вращения мотора №2
#define PIN_IN3 5 // Вывод управления направлением вращения мотора №2
#define PIN_IN4 4 // Вывод управления направлением вращения мотора №2

SoftwareSerial mySerial(2, 3); // указываем пины rx и tx соответственно

uint8_t power_l = 0;
uint8_t power_r = 0;
uint8_t direction_l = 0; //0 - forward, 1 - backward
uint8_t direction_r = 0; //0 - forward, 1 - backward

int motor = -1; //0 - left, 1 - right
int direct = -1; //0 - forward, 1 - backward

void setup()  {
  pinMode(2,INPUT);
  pinMode(3,OUTPUT);
  Serial.begin(9600);
  mySerial.begin(38400);
  Serial.println("start prg");

  // Установка всех управляющих пинов в режим выхода
  pinMode(PIN_ENA, OUTPUT);
  pinMode(PIN_ENB, OUTPUT);
  pinMode(PIN_IN1, OUTPUT);
  pinMode(PIN_IN2, OUTPUT);
  pinMode(PIN_IN3, OUTPUT);
  pinMode(PIN_IN4, OUTPUT);
  // Команда остановки двум моторам
  digitalWrite(PIN_IN1, LOW);
  digitalWrite(PIN_IN2, LOW);
  digitalWrite(PIN_IN3, LOW);
  digitalWrite(PIN_IN4, LOW);
}

void loop() {
  
  if (mySerial.available()) {
    char c = mySerial.read(); // читаем из software-порта
    //Serial.println(c); // пишем в hardware-порт

    if (motor == -1){
      if(c == 'L'){
        motor = 0;
        //Serial.println("Left");
      }
      else if(c == 'R'){
        motor = 1;
        //Serial.println("Right");
      }
    }
    else if(direct == -1){
      if(c == 'F'){
        direct = 0;
        //Serial.println("Forward");
      }
      else if(c == 'B'){
        direct = 1;
        //Serial.println("Backward");
      }
    }
    else{
      byte power = (byte)c;
      //Serial.println(power);

      if (motor == 0){
        direction_l = direct;
        power_l = 0;
        power_l += power;
      }
      else if (motor == 1){
        direction_r = direct;
        power_r = 0;
        power_r += power;
      }

      motor = -1;
      direct = -1;

//      Serial.print("Left: ");
//      if (direction_l == 0) Serial.print("fwd ");
//      else Serial.print("rwd ");
//      Serial.println(power_l);
//
//      Serial.print("Right: ");
//      if (direction_r == 0) Serial.print("fwd ");
//      else Serial.print("rwd ");
//      Serial.println(power_r);
    }

    //Задаём направление для 1-го мотора
    if(direction_l == 0){
      digitalWrite(PIN_IN1, HIGH);
      digitalWrite(PIN_IN2, LOW);
    }
    else{
      digitalWrite(PIN_IN1, LOW);
      digitalWrite(PIN_IN2, HIGH);
    }

    //Задаём направление для 2-го мотора
    if(direction_r == 0){
      digitalWrite(PIN_IN3, HIGH);
      digitalWrite(PIN_IN4, LOW);
    }
    else{
      digitalWrite(PIN_IN3, LOW);
      digitalWrite(PIN_IN4, HIGH);
    }

    analogWrite(PIN_ENA, power_l); // Устанавливаем скорость 1-го мотора
    analogWrite(PIN_ENB, power_r); // Устанавливаем скорость 2-го мотора
  }
}
