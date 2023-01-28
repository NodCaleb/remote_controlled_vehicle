#define PIN_ENA 9 // Вывод управления скоростью вращения мотора №1
#define PIN_ENB 3 // Вывод управления скоростью вращения мотора №2
#define PIN_IN1 7 // Вывод управления направлением вращения мотора №1
#define PIN_IN2 6 // Вывод управления направлением вращения мотора №1
#define PIN_IN3 5 // Вывод управления направлением вращения мотора №2
#define PIN_IN4 4 // Вывод управления направлением вращения мотора №2
uint8_t power = 105; // Значение ШИМ (или скорости вращения)
void setup() {
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
  // Вращаем моторы в одну сторону с разной скоростью
  while(power < 255) {
    analogWrite(PIN_ENA, power); // Устанавливаем скорость 1-го мотора
    analogWrite(PIN_ENB, power); // Устанавливаем скорость 2-го мотора
    // Задаём направление для 1-го мотора
    digitalWrite(PIN_IN1, HIGH);
    digitalWrite(PIN_IN2, LOW);
    // Задаём направление для 2-го мотора
    digitalWrite(PIN_IN3, HIGH);
    digitalWrite(PIN_IN4, LOW);
    delay(3000); // Пауза 3 секунды
    power += 30; // Увеличиваем скорость
  }
  power = 225;
  // Вращаем моторы в другую сторону с разной скоростью
  while(power > 105) {
    analogWrite(PIN_ENA, power); // Устанавливаем скорость 1-го мотора
    analogWrite(PIN_ENB, power); // Устанавливаем скорость 2-го мотора
    // Задаём направление для 1-го мотора
    digitalWrite(PIN_IN1, LOW);
    digitalWrite(PIN_IN2, HIGH);
    // Задаём направление для 2-го мотора
    digitalWrite(PIN_IN3, LOW);
    digitalWrite(PIN_IN4, HIGH);
    delay(3000); // Пауза 3 секунды
    power -= 30; // Уменьшаем скорость
  }
  power = 105;
}