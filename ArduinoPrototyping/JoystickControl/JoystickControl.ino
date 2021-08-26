#define PIN_ENA 9 // Вывод управления скоростью вращения мотора №1
#define PIN_IN1 8 // Вывод управления направлением вращения мотора №1
#define PIN_IN2 7 // Вывод управления направлением вращения мотора №1
#define PIN_ENB 6 // Вывод управления скоростью вращения мотора №2
#define PIN_IN3 5 // Вывод управления направлением вращения мотора №2
#define PIN_IN4 4 // Вывод управления направлением вращения мотора №2
#define PIN_X 0 // X джойстика
#define PIN_Y 1 // Y джойстика
#define PIN_SW 9 // Кнопка джойстика

uint8_t power_l = 0;
uint8_t power_r = 0;

void setup() {
  pinMode(PIN_SW, INPUT);
  digitalWrite(PIN_SW, HIGH);
  Serial.begin(9600);

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
  
  int x = analogRead(PIN_X);
  int y = analogRead(PIN_Y);

  Serial.print("X: ");
  Serial.println(x);
  Serial.print("Y: ");
  Serial.println(y);

  if (y < 412){ //Moving forward
    Serial.println("Forward");
    power_l = map(412-y, 0, 412, 0, 255);
    power_r = map(412-y, 0, 412, 0, 255);

    // Задаём направление для 1-го мотора
    digitalWrite(PIN_IN1, HIGH);
    digitalWrite(PIN_IN2, LOW);
    // Задаём направление для 2-го мотора
    digitalWrite(PIN_IN3, HIGH);
    digitalWrite(PIN_IN4, LOW);
  }
  else if (y > 612){ //Moving backward
    Serial.println("Backward");
    power_l = map(y - 612, 0, 412, 0, 255);
    power_r = map(y - 612, 0, 412, 0, 255);

    // Задаём направление для 1-го мотора
    digitalWrite(PIN_IN1, LOW);
    digitalWrite(PIN_IN2, HIGH);
    // Задаём направление для 2-го мотора
    digitalWrite(PIN_IN3, LOW);
    digitalWrite(PIN_IN4, HIGH);
  }
  else{ //Do not move
    Serial.println("Stop");
    power_l = 0;
    power_r = 0;  

    // Задаём направление для 1-го мотора
    digitalWrite(PIN_IN1, LOW);
    digitalWrite(PIN_IN2, LOW);
    // Задаём направление для 2-го мотора
    digitalWrite(PIN_IN3, LOW);
    digitalWrite(PIN_IN4, LOW);
  }

  if (x < 412) { //turn left

    uint8_t reduce_l = map(412-x, 0, 412, 0, 255);

    if (reduce_l < power_l){
        power_l -= reduce_l;
    }
    else{
      power_l = 0;  
    }
    
  }
  else if (x > 612){ //turn right    

    uint8_t reduce_r = map(x - 612, 0, 412, 0, 255);

    if (reduce_r < power_r){
        power_r -= reduce_r;
    }
    else{
      power_r = 0;  
    }
    
  }

  analogWrite(PIN_ENA, power_l); // Устанавливаем скорость 1-го мотора
  analogWrite(PIN_ENB, power_r); // Устанавливаем скорость 2-го мотора
  
  Serial.print("Power L: ");
  Serial.println(power_l);
  Serial.print("Power R: ");
  Serial.println(power_r);
  Serial.print("\n");
  delay(100);
}
