#include <SPI.h>
#include <RF24.h>

// CE	6 white
// CSN	7 yellow
// SCK	13 green
// MOSI	11 blue
// MISO	12 pink

RF24 radio(6, 7); // Підключення пінів CE та CSN до 6 і 7 відповідно

byte joystickX = A0;
int xValue;

void setup()
{
  Serial.begin(9600);
  radio.begin();
}

void loop()
{
  String str = "joystickX=" + String(analogRead(joystickX));
  int str_len = str.length() + 1;

  Serial.println(str);


  char text[str_len];
  str.toCharArray(text, str_len);

  radio.openWritingPipe(0xF0F0F0F0E1LL); // Встановлення адреси отримувача
  radio.write(&text, sizeof(text));      // Відправлення даних по радіоканалу
}