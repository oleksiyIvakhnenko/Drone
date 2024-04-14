
#include <SPI.h>
#include <RF24.h>
#include <TM1637.h>

void displayNumber(int num);

TM1637 tm(2, 3);

// CE 6
// CSN  7
// SCK  13
// MOSI 11
// MISO 12

RF24 radio(6, 7); // Підключення пінів CE та CSN до 6 і 7 відповідно

int pValue = 0;

void setup()
{

  radio.begin();

  tm.init();
  tm.set(4);

  radio.openReadingPipe(1, 0xF0F0F0F0E1LL); // Встановлення такої самої адреси, як і на передавачі
  radio.startListening();                   // Перехід в режим прийому
}

void loop()
{
  String sString;
  if (radio.available())
  {
    char text[32] = "";
    radio.read(&text, sizeof(text)); // Зчитування отриманих даних

    sString = String(text);
    size_t found = sString.indexOf("=");

    String key = sString.substring(0, found);    // get first part
    String value = sString.substring(found + 1); // get second part

    if (key == "joystickX")
    {
      pValue = atoi(value.c_str());
    }
  }

  displayNumber(pValue);

  // displayNumber(125);
}

void displayNumber(int num)
{
  tm.display(3, num % 10);
  tm.display(2, num / 10 % 10);
  tm.display(1, num / 100 % 10);
  tm.display(0, num / 1000 % 10);
}