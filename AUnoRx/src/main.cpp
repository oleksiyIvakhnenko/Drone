
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
  Serial.begin(9600);

  radio.begin();

  tm.init();
  tm.set(4);

  radio.openReadingPipe(1, 0xF0F0F0F0E1LL); // Встановлення такої самої адреси, як і на передавачі
  radio.startListening();                   // Перехід в режим прийому
}

void loop()
{
  char text[32] = "";
  String sString;
  String strs[] = {};
  int stringCount = 0;

  if (radio.available())
  {
    radio.read(&text, sizeof(text)); // Зчитування отриманих даних

    sString = String(text);

    // Split the string into substrings
    while (sString.length() > 0)
    {
      int index = sString.indexOf(',');

      if (index == -1) // No comma found
      {
        strs[stringCount++] = sString;
        break;
      }
      else
      {
        strs[stringCount++] = sString.substring(0, index);
        sString = sString.substring(index + 1);
      }
    }

    pValue = atoi(strs[0].c_str());
  }

  displayNumber(pValue);
}

void displayNumber(int num)
{
  tm.display(3, num % 10);
  tm.display(2, num / 10 % 10);
  tm.display(1, num / 100 % 10);
  tm.display(0, num / 1000 % 10);
}