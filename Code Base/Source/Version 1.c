/*********

*********/

#include <Arduino.h>

#include <Wire.h>              //wire library
#include <LiquidCrystal_I2C.h> //liquid crystal with I2C library

/*Pinout Definitions*/

#define Relay_one 15
#define Relay_two 17
#define buzzer 5 // set the pin for the buzzer

//  LED Pins
static const int greenLED = 23; // the pin that connects the green LED of RGB LED

// constants won't change. They're used here to set pin numbers:
const int button1Pin = 32; // the number of the pushbutton pin
const int button2Pin = 33; // the number of the pushbutton pin

// variables will change:
int button1State = 0; // variable for reading the pushbutton status
int button2State = 0; // variable for reading the pushbutton status
/*Configuration*/

// LCD
//  set the LCD number of columns and rows
int lcdColumns = 16;
int lcdRows = 2;
// set LCD address, number of columns and rows
LiquidCrystal_I2C lcd(0x27, lcdColumns, lcdRows);

// These 5 arrays paint the bars that go across the screen.
byte lcdzero[] = {
    B00000,
    B00000,
    B00000,
    B00000,
    B00000,
    B00000,
    B00000,
    B00000};
byte lcdone[] = {
    B10000,
    B10000,
    B10000,
    B10000,
    B10000,
    B10000,
    B10000,
    B10000};

byte lcdtwo[] = {
    B11000,
    B11000,
    B11000,
    B11000,
    B11000,
    B11000,
    B11000,
    B11000};

byte lcdthree[] = {
    B11100,
    B11100,
    B11100,
    B11100,
    B11100,
    B11100,
    B11100,
    B11100};

byte lcdfour[] = {
    B11110,
    B11110,
    B11110,
    B11110,
    B11110,
    B11110,
    B11110,
    B11110};

byte lcdfive[] = {
    B11111,
    B11111,
    B11111,
    B11111,
    B11111,
    B11111,
    B11111,
    B11111};

/* Function Declarations */

void (*resetFunc)(void) = 0; // declare reset function @ address 0

// Progress bar code
void updateProgressBar(unsigned long count, unsigned long totalCount, int lineToPrintOn)
{
  double factor = totalCount / 80.0;
  int percent = (count + 1) / factor;
  int number = percent / 5;
  int remainder = percent % 5;
  if (number > 0)
  {
    for (int j = 0; j < number; j++)
    {
      lcd.setCursor(j, lineToPrintOn);
      lcd.write(5);
    }
  }
  lcd.setCursor(number, lineToPrintOn);
  lcd.write(remainder);
  if (number < 16) // If using a 20 character display, this should be 20!
  {
    for (int j = number + 1; j <= 16; j++) // If using a 20 character display, this should be 20!
    {
      lcd.setCursor(j, lineToPrintOn);
      lcd.write(0);
    }
  }
}

void setup()
{

  Serial.begin(115200);

  /*Atmega ESP Communication*/
  Serial.println(" Device Connected");

  // initialize LCD
  lcd.init();
  // turn on LCD backlight
  lcd.backlight();
  lcd.begin(16, 2);

  /*Bootup Routine*/

  lcd.createChar(0, lcdzero);
  lcd.createChar(1, lcdone);
  lcd.createChar(2, lcdtwo);
  lcd.createChar(3, lcdthree);
  lcd.createChar(4, lcdfour);
  lcd.createChar(5, lcdfive);

  lcd.setCursor(0, 0);
  lcd.print("Home Automation");
  lcd.setCursor(0, 1);
  lcd.print(" Setup ");
  delay(2000);
  lcd.clear();

  // Output

  // set the pinmode for the buzzer
  pinMode(buzzer, OUTPUT);

  // pin mode for LED
  pinMode(greenLED, OUTPUT);

  pinMode(Relay_one, OUTPUT);
  pinMode(Relay_two, OUTPUT);

  // initialize the pushbutton pin as an input:
  pinMode(button1Pin, INPUT);
  pinMode(button2Pin, INPUT);

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("System Startup");
  delay(500);
  lcd.clear();

  for (size_t i = 0; i < 10; i = i + 3)
  {
    lcd.setCursor(0, 0);
    lcd.print("Initializing");
    updateProgressBar(i, 10, 1);
    delay(1000);
    lcd.clear();
  }
}

// the loop function runs over and over again forever
void loop()
{
 digitalWrite(Relay_one, LOW);
    digitalWrite(Relay_two, LOW);
  // read the state of the pushbutton value:
  button1State = digitalRead(button1Pin);
  button2State = digitalRead(button2Pin);
  // check if the pushbutton is pressed. If it is, the buttonState is HIGH:
  if (button1State == HIGH)
  {
    // turn LED on:
    digitalWrite(greenLED, HIGH); // turn the LED on (HIGH is the voltage level)
    digitalWrite(Relay_one, HIGH);
    digitalWrite(Relay_two, LOW);
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Relay One is ON");
    delay(500);
  }
  else if (button2State == HIGH)
  {
    // turn LED off:
    digitalWrite(greenLED, HIGH); // turn the LED on (HIGH is the voltage level)
    digitalWrite(Relay_two, HIGH);
    digitalWrite(Relay_one, LOW);
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Relay Two is ON");
    delay(500);
  }

  // Normally Open configuration, send LOW signal to let current flow
  // (if you're usong Normally Closed configuration send HIGH signal)
  else
  {
    digitalWrite(greenLED, LOW);
    // digitalWrite(Relay_one, LOW);
    // digitalWrite(Relay_two, LOW);
  }

  // Serial.println("Current Flowing");
  // delay(5000); // wait for a second

  // // Normally Open configuration, send HIGH signal stop current flow
  // // (if you're usong Normally Closed configuration send LOW signal)
  // 
  // Serial.println("Current not Flowing");
  // delay(5000);
}
