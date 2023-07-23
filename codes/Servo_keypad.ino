#include <Keypad.h>
#include <LiquidCrystal.h>
#include <Servo.h>

Servo myservo;

LiquidCrystal lcd(A0, A1, A2, A3, A4, A5);

#define Password_Lenght 7 

int pos = 0;    

char Data[Password_Lenght]; 
char Master[Password_Lenght] = "123456";  
byte data_count = 0, master_count = 0;
bool Pass_is_good;
char customKey;

const byte ROWS = 4;
const byte COLS = 3;
char keys[ROWS][COLS] = {
  {'1', '2', '3'},
  {'4', '5', '6'},
  {'7', '8', '9'},
  {'*', '0', '#'}
};
bool door = true;

byte rowPins[ROWS] = {8, 7, 6, 5}; 
byte colPins[COLS] = {4, 3, 2};

Keypad customKeypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS);

void setup()
{
  myservo.attach(9);
  ServoClose();
  lcd.begin(16, 2);
  
}

void loop()
{
  if (door == 0)
  {
    customKey = customKeypad.getKey();

    if (customKey == '#')

    {
      
    lcd.clear();
      lcd.setCursor (3,0);
        lcd.print("LOCKING");
        delay (700);
      lcd.setCursor (10,0);
        lcd.print(".");
        delay (800);
      lcd.setCursor (11,0);  
        lcd.print(".");
        delay (800);
      lcd.setCursor (12,0);
        lcd.print(".");
        delay (500);
        lcd.clear();
        delay (500);
        ServoClose();
      lcd.setCursor (5,0);
      lcd.print("LOCKED");
      delay (1700);
      
      door = 1;
    }
  }

  else Open();
}

void clearData()
{
  while (data_count != 0)
  { 
    Data[data_count--] = 0; 
  }
  return;
}

void ServoOpen()
{
  for (pos = 180; pos >= 0; pos -= 5) { 
    
    myservo.write(pos);
    delay(15);
  }
}

void ServoClose()
{
  for (pos = 0; pos <= 180; pos += 5) { 
    myservo.write(pos);
    delay(15);  
  }
}

void Open()
{
  lcd.setCursor(1, 0);
  lcd.print("INPUT PASSWORD");
  
  customKey = customKeypad.getKey();
  if (customKey) 
  {
   lcd.setCursor(0, 1); 
    lcd.print("******");     
    Data[data_count] = customKey; 
    lcd.setCursor(data_count, 1); 
    lcd.print(Data[data_count]); 
    data_count++; 
  }

  if (data_count == Password_Lenght - 1) 
  {
    if (!strcmp(Data, Master)) 
    {
      lcd.clear();
      ServoOpen();
      lcd.print(" ACCESS GRANTED");
      lcd.setCursor(0,1);
      lcd.print("press # to close");
      door = 0;
    }
    else
    {
      lcd.clear();

     lcd.setCursor(1,0); 
      lcd.print("ACCESS DENIED!");
      delay(1500);
      door = 1;
    }
    clearData();
  }
} 
