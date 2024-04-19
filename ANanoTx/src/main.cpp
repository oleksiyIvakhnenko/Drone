#include <SPI.h>
#include <RF24.h>

// CE	6 white
// CSN	7 yellow
// SCK	13 green
// MOSI	11 blue
// MISO	12 pink

RF24 radio(6, 7); // Підключення пінів CE та CSN до 6 і 7 відповідно

byte joystickX = A0;
// uint16_t xValue;
unsigned int xValue = 0;
unsigned int filtr;

unsigned int toSend[] = {0};
bool isActive = false;

unsigned long tim2 = 0;

void setup()
{
  Serial.begin(9600);
  radio.begin();
}

void loop()
{
  radio.openWritingPipe(0xF0F0F0F0E1LL); // Встановлення адреси отримувача

  // int currentValues[] = {(millis()/1000)};

  unsigned long _tmpTime = millis();

  if (tim2 + 100 < _tmpTime)
  {
    xValue = analogRead(joystickX);
    tim2 = _tmpTime;
  }

  int currentValues[] = {xValue};

  for (int i = 0; i < (sizeof(toSend) / sizeof(int)); i++)
  {
    if (!(toSend[i] == currentValues[i]))
    {
Serial.println(currentValues[i]);

      toSend[i] = currentValues[i];

      isActive = true;
    }
  }

  if (isActive)
  {

    String str = "";

    unsigned int paramLength = (sizeof(toSend) / sizeof(unsigned int));

    for (unsigned int i = 0; i < paramLength; i++)
    {
      str = str + String(toSend[i]);
      if ((i + 1) < paramLength)
      {
        str = str + ",";
      }
    }

    char text[str.length() + 1];
    str.toCharArray(text, str.length() + 1);

    Serial.println(str);
    radio.write(&text, sizeof(text)); // Відправлення даних по радіоканалу

    isActive = false;
  }
}