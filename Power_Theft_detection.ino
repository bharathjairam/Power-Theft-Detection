//LCD display
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd1(0x27, 16, 2); // << Address 1
LiquidCrystal_I2C lcd2(0x21, 16, 2); // << Address 2

//Current sensors
//Distribution sensor
const int currentPinD = A0;
int sensitivity = 66;
int adcValueD = 0;
int offsetVoltage = 2500;
double adcVoltageD = 0;
double currentValueD = 0;

//Consumer sensor
const int currentPinC = A1;
//int sensitivity = 66;
int adcValueC = 0;
//int offsetVoltage = 2500;
double adcVoltageC = 0;
double currentValueC = 0;

//SENSOR 1
int MinS1;
int MaxS1;
int AnalogS1;


//SENSOR 2
int MinS2;
int MaxS2;
int AnalogS2;

//Buzzer
const int buzzer = 9;






void setup()
{

  Serial.begin(9600);
  delay(2000);

  //LCD display
  lcd1.init();
  lcd2.init();
  lcd1.backlight();
  lcd2.backlight();

  pinMode(buzzer, OUTPUT);

}


void loop()
{

  MinS1 = 1023;     //Initilize/reset to limit
  MaxS1 = 0;        //Initilize/reset to limit

  MinS2 = 1023;     //Initilize/reset to limit
  MaxS2 = 0;        //Initilize/reset to limit


  //Take 1000 readings, find min, max, and average.  This loop takes about 100ms.
  for (int i = 0; i < 1000; i++)
  {
    AnalogS1 = analogRead(currentPinD);

    if (AnalogS1 < MinS1)
      MinS1 = AnalogS1;

    if (AnalogS1 > MaxS1)
      MaxS1 = AnalogS1;

    AnalogS2 = analogRead(currentPinC);

    if (AnalogS2 < MinS2)
      MinS2 = AnalogS2;

    if (AnalogS2 > MaxS2)
      MaxS2 = AnalogS2;
  }


  // print results
  //Serial.print ( " MinS1 = ");
  //Serial.print (MinS1);

  //Serial.print ( " MaxS1 = ");
  //Serial.println(MaxS1);

  int ValueS1 = (MaxS1 - MinS1);
  Serial.println(ValueS1);

  // print results
  //Serial.print ( " MinS2 = ");
  //Serial.print (MinS2);

  //Serial.print ( " MaxS2 = ");
  //Serial.println(MaxS2);

  int ValueS2 = (MaxS2 - MinS2);
  Serial.println(ValueS2);

  delay(500);    //Optional delay for readability

  double D = Sensor1();
  Serial.print("Distribution side");
  Serial.println(D);
  lcd1.setCursor(2, 0);
  lcd1.print("Current: ");
  lcd1.setCursor(10, 0);
  lcd1.print(D);
  lcd1.setCursor(14, 0);
  lcd1.print(" A");
  lcd1.setCursor(0, 1);
  lcd1.print("  Line is Safe");
  //lcd1.clear();

  double C = Sensor2();
  Serial.print("Consumer side");
  Serial.println(C);
  lcd2.setCursor(2, 0);
  lcd2.print("Current: ");
  lcd2.setCursor(10, 0);
  lcd2.print(C);
  lcd2.setCursor(14, 0);
  lcd2.print(" A");
  lcd2.setCursor(0, 1);
  lcd2.print("Current Consumed");
  delay(5000);
  //lcd2.clear();

  if ((ValueS1 >= 12) && (ValueS2 <= 10)) {
    Serial.println("Electricity Theft detected");
    tone(buzzer, 1000); // Send 1KHz sound signal...
    //delay(5000);        // ...for 1 sec
    lcd1.setCursor(2, 0);
    lcd1.print(" WARNING !!!");
    //lcd1.setCursor(10, 0);
    //lcd1.print(D);
    lcd1.setCursor(0, 1);
    lcd1.print("Theft Detected!");
    delay(5000);
    lcd1.clear();
    noTone(buzzer);     // Stop sound...
    delay(1000);
  }

  else if ((ValueS1 >= 28) && (ValueS2 >= 18)) {
    Serial.println("Electricity Theft detected");
    tone(buzzer, 1000); // Send 1KHz sound signal...
    //delay(5000);        // ...for 1 sec
    lcd1.setCursor(2, 0);
    lcd1.print(" WARNING !!!");
    //lcd1.setCursor(10, 0);
    //lcd1.print(D);
    lcd1.setCursor(0, 1);
    lcd1.print("Theft Detected!");
    delay(5000);
    lcd1.clear();
    noTone(buzzer);     // Stop sound...
    delay(1000);
  }


}

double Sensor1() {

  adcValueD = analogRead(currentPinD);
  adcVoltageD = (adcValueD / 1024.0) * 5000;
  currentValueD = ((adcVoltageD - offsetVoltage) / sensitivity);

  //Serial.print("Raw Sensor Value = " );
  //Serial.print(adcValueD);
  //delay(2000);
  //Serial.print("\t Voltage(mV) = ");
  //Serial.print(adcVoltageD, 3);
  //delay(2000);
  //Serial.print("\t Distribution Current = ");
  //Serial.println(currentValueD, 3);
  //delay(2000);
  double S1 = abs(currentValueD);
  return S1;

}

double Sensor2() {

  adcValueC = analogRead(currentPinC);
  adcVoltageC = (adcValueC / 1024.0) * 5000;
  currentValueC = ((adcVoltageC - offsetVoltage) / sensitivity);

  //Serial.print("Raw Sensor Value = " );
  //Serial.print(adcValueC);
  //delay(2000);
  //Serial.print("\t Voltage(mV) = ");
  //Serial.print(adcVoltageC, 3);
  //delay(2000);
  //Serial.print("\t Consumer Current = ");
  //Serial.println(currentValueC, 3);
  //delay(2000);
  double S2 = abs(currentValueC);
  return S2;
}
