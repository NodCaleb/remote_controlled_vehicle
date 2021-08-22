#include <SoftwareSerial.h>

SoftwareSerial mySerial(2, 3); // указываем пины rx и tx соответственно

void setup()  {
  pinMode(2,INPUT);
  pinMode(3,OUTPUT);
  Serial.begin(9600);
  mySerial.begin(38400);
  Serial.println("start prg");
}

void loop() {
  if (mySerial.available()) {
    char c = mySerial.read(); // читаем из software-порта
    Serial.print(c); // пишем в hardware-порт
  }
  if (Serial.available()) {
    char c = Serial.read(); // читаем из hardware-порта
    mySerial.write(c); // пишем в software-порт
  }
}
