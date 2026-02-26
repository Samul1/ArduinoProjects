// Author Samuli Juutinen
#include <TFT_eSPI.h>
#include <User_Setup.h>
#include "Free_Fonts.h"
#include <SPI.h>
#include <math.h>
#include <SoftwareSerial.h>
#include <string.h>

#define USER_SETUP_INFO "User_Setup"

#define RX_PIN 32 // RX
#define TX_PIN 33 // TX
#define BAUD_RATE 4800

#define TFT_CS   27 // Chip select control pin
#define TFT_RST  26 // 
#define TFT_DC   14 // Data Command control pin

uint16_t textColor = TFT_WHITE;
uint16_t screenColor = TFT_BLACK;
uint16_t drawColor = TFT_WHITE;

TFT_eSPI tft = TFT_eSPI();  // Invoke library, pins defined in User_Setup.h
SoftwareSerial softSerial(RX_PIN, TX_PIN);
int menuDraw = 0; // Check if menu has been drawn, 1 = no, 0 = yes

int isThereMailSignal = 0;
int mailSymbol = 0;
int temperature = 0;
int oldTemperature = 0;
int lighting = 0;
int lightSymbol = 0;
int batteryStage = 0;
int batterySymbol = 0;
int howManyTimesMail = 0;

unsigned long currentTime;
unsigned long hours;
unsigned long minutes;
unsigned long seconds;

// Funktioiden esittely:
void drawMenu();
void displayMessage(String input);
void weather();
void isThereMail();
void settings();
void symbol(int valinta);
void drawDiamondRect(int cx, int cy, int width, int height, uint16_t color);

void setup() 
{
  
  Serial.begin(BAUD_RATE);    // Initialize Serial Monitor
  softSerial.begin(BAUD_RATE);
  //Serial2.begin(BAUD_RATE, SERIAL_8N1, RX_PIN, TX_PIN); // Initialize XBee serial communication
  Serial.println("XBee Communication Test");
  
  // Initialize TFT display
  tft.begin();
  Serial.println("TFT initialized");
  
  Serial.println("TFT initialized successfully");
  
  //tft.setRotation(1);  // Rotate display if needed
  tft.fillScreen(screenColor);
  tft.setTextColor(textColor);
  tft.setTextSize(3);

  // Aloitus animaatio -----------------------------------------------------------------------------------------------------------
  // Kuviot:
  symbol(12);
  delay(150); // 150ms delay
  symbol(13);
  delay(150); // 150ms delay
  symbol(14);
  delay(150); // 150ms delay

  // Teksti:
  tft.drawString("SMART",40,200);
  delay(150); // 150ms delay
  tft.drawString("MAIL", 140,200);
  delay(5000); // 5s delay

  // -------------------------------------------------------------------------------------------------------------------------
  tft.setTextSize(2);
}

void loop() 
{
  // Valikko muuttujat:
  int alku = 1;
  int userInput = 9;
  // Lähetysmuuttujat:
  int firstIndex = 0;
  int secondIndex = 0;
  int thirdIndex = 0;
  char etsi = ',';
  char mail[25];
  char temp[25];
  char light[25];
  char battery[25];
  int isThereMailSignalCheck = 0;

  // Draw menu screen if it is now yet drawn:
  if(menuDraw == 0)
    {
      drawMenu();
      menuDraw = 1;
    }

  // Ajan laskua----------------------------------------------------------------------------------------------------------------------------------------------

  currentTime = millis();
  seconds = currentTime / 1000; // Convert milliseconds to seconds
  minutes = seconds / 60;
  hours = minutes / 60;

  int oldMinutes = minutes;

  // Calculate hours, minutes, and seconds
  hours = hours % 24; // Keep hours within 0-23 range
  minutes = minutes % 60; // Keep minutes within 0-59 range
  seconds = seconds % 60; // Keep seconds within 0-59 range
  if(hours > 24)
    hours = 0;
  if (minutes > 60)
    minutes = 0;
  if(seconds > 60)
    seconds = 0;

  if(oldMinutes != minutes || alku == 1)
  {
    alku = 0;
    // Clear Clock:
    tft.fillRect(160, 7, 70, 23, screenColor);
    tft.drawRect(160, 7, 70, 23, textColor);

    // Display time
    tft.setCursor(165, 10);
    if (hours < 10) tft.print("0");
    tft.print(hours);
    tft.print(":");
    if (minutes < 10) tft.print("0");
    tft.print(minutes); 
  }

  // ------------------------------------------------------------------------------------------------------------------------------------------------------------

  // Read input from serial monitor if available-----------------------------------------------------------------------------------------------------------------
  if (Serial.available() > 0) 
  {
    // Muuta valikko a,b,c,d... char
    int input = Serial.parseInt(); // Read integer from serial port
    Serial.print("You entered: ");
    Serial.println(input);
    userInput = input;
    //String message = String(input);
    if (userInput != 0) 
    {
        // Valinnan logiikka:  
      switch(userInput)
      {
        case 1:
          weather();
          // Clear Screen:
          tft.fillScreen(screenColor);
          break;
        case 2:
          isThereMail();
          // Clear Screen:
          tft.fillScreen(screenColor);
          break;
        case 3:
          //hours++;
          break;
        case 4:
          //minutes++;
          break;
        case 5:
          settings();
          // Clear Screen:
          tft.fillScreen(screenColor);
          break;
        default:
          break;
      } 
    }
    else
    {
      displayMessage("invalid input");
    }
      
  }
//--------------------------------------------------------------------------------------------------------------------------------------------------------

  // Tässä lopussa päivitetään lämpötila ja relatiivinen kosteusprosentti: -------------------------------------------------------------------------------

  //XBee data otetaan vastaan ja näytetään näytöllä----------------------------------------------------------------------------------
  if (softSerial.available() > 0) 
  {
    int oldLight = lighting;
    int oldMail = isThereMailSignal;
    oldTemperature = temperature;
    int oldMailTimes = howManyTimesMail;
    int batteryStageTest = 0;
    int isThereMailTest = 0;

    //menuDraw = 0;
    /*
    tft.setTextSize(2);
    tft.setTextColor(TFT_BLUE);
    tft.setCursor(40, 255); // x,y
    tft.print("TOIMII");*/
    
    //char receivedChar = Serial2.read();
    //char receivedChar = softSerial.read();
    //int receivedInt = softSerial.parseInt();

    // TESTERI ===========================================================================
    /*
    String testeri = softSerial.readStringUntil('\n');
    testeri = testeri.toInt();
    tft.setTextSize(4);
    tft.setTextColor(TFT_RED);
    tft.setCursor(40, 225); // x,y
    tft.print(testeri);
    */
    // ===================================================================================

    // Tähän kohtaan signaalin lukeminen ja pilkkominen --------------------------------

    String receivedString = softSerial.readStringUntil('\n');
    char received[100] = {'0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0'};
    //strcpy(received, receivedString);
    for(int d = 0; d < 25; d++)
    {
      received[d] = receivedString[d];
    }
    delay(10);
    // DebugViesti:
    Serial.print("receivedString: ");
    Serial.println(receivedString);
    // DebugViesti:
    Serial.print("muutettu muoto: ");
    Serial.println(received);
    

    // Haetaan ensimmäisen pilkun indeksi:
    for(int i = 0; i < 24; i++)
    {
      if(received[i] == etsi)
        {
          firstIndex = i;
          break;
        }
    }
    delay(10);
    // DebugViesti:
    Serial.print("firstIndex: ");
    Serial.println(firstIndex);

    if(firstIndex == 0)
    {
      // DebugViesti:
      Serial.print("Ensimmäinen indeksi oli ',', joten signaali hylätään.");
    }
    else
    {
      // Haetaan toisen pilkun indeksi:
      for(int j = firstIndex+1; j < 24-firstIndex; j++)
      {
        if(received[j] == etsi)
          {
            secondIndex = j;
            break;
          }
      }
      delay(10);
      // DebugViesti:
      Serial.print("secondIndex: ");
      Serial.println(secondIndex);

      for(int g = secondIndex+1; g < 24-secondIndex; g++)
      {
        if(received[g] == etsi)
        {
          thirdIndex = g;
          break;
        }
      }
      delay(10);
      // DebugViesti:
      Serial.print("thirdIndex: ");
      Serial.println(thirdIndex);

      // Tallennetaan mail char taulukkoon.
      for(int k = 0; k < firstIndex; k++)
        mail[k] = received[k];
      delay(10);

      // Muutetaan char taulukko string muotoon:
      String itm = String(mail[0]);
      // Muutetaan string intiksi:
      isThereMailTest = itm.toInt();
      if(isThereMailTest >= 10)
      {
        (isThereMailTest / 1) % 10;
      }
      isThereMailSignalCheck = isThereMailTest; // 4 = postitullut
      // DebugViesti:
      Serial.print("isThereMailSignalCheck: ");
      Serial.println(isThereMailSignalCheck);
      // Jos saatu arvo on 4:
      if(isThereMailSignalCheck == 4)
      {
        isThereMailSignal = isThereMailSignalCheck;
        howManyTimesMail++;
      }
      else
      {
        // Ei uutta postia.
      }

      int ind = 0;
      // Tallennetaan temp taulukkoon.
      for(int l = firstIndex+1; l < secondIndex; l++)
      {
        temp[ind] = received[l];
        ind++;
      }
      delay(10);

      // DebugViesti:
      Serial.print("tempTaulukko: ");
      Serial.println(temp);
      // Muutetaan char taulukko string muotoon:
      String ts = String(temp);
      // DebugViesti:
      Serial.print("ts: ");
      Serial.println(ts);
      // Muutetaan string intiksi:
      temperature = ts.toInt();
      // DebugViesti:
      Serial.print("temperature: ");
      Serial.println(temperature);

      ind = 0;
      // Tallennetaan light taulukkoon.
      for(int n = secondIndex+1; n < thirdIndex; n++)
      {
        light[ind] = received[n];
        ind++;
      }
      delay(10);

      // DebugViesti:
      Serial.print("lightTaulukko: ");
      Serial.println(light);
      // Muutetaan char taulukko string muotoon:
      String ls = String(light[0]);
      // DebugViesti:
      Serial.print("ls: ");
      Serial.println(ls);
      // Muutetaan string intiksi:
      lighting = ls.toInt();
      // DebugViesti:
      Serial.print("lighting: ");
      Serial.println(lighting);

      ind = 0;
      // Tallenetaan akuntila taulukkoon.
      for(int s = thirdIndex+1; s < 25-thirdIndex; s++)
      {
        battery[ind] = received[s];
        ind++;
      }
      delay(10);
      // Debugviesti:
      Serial.print("batteryTaulukko: ");
      Serial.println(battery);
      // Muutetaan char taulukko string muotoon:
      String bs = String(battery[0]);
      // Debugviesti:
      Serial.print("bs: ");
      Serial.println(bs);
      batteryStageTest = bs.toInt();
      if(batteryStageTest <= 10)
        (batteryStageTest / 1) % 10;
      Serial.print("batteryStageTest: ");
      Serial.println(batteryStageTest);
      if(batteryStageTest >= 5)
        batteryStageTest = 4;
      // Muutetaan string intiksi:
      batteryStage = batteryStageTest;
      // Debugviesti:
      Serial.print("batteryStage: ");
      Serial.println(batteryStage);


      // -----------------------------------------------------------------------------------------------

      // Onko tullut postia: ----------------------------------------------------------------------------
      //String receivedString = softSerial.readStringUntil('\n');
      //isThereMailSignal = receivedString.toInt(); // 4 = postitullut

      // Lämpötila --------------------------------------------------------------------------------------
      //receivedString = softSerial.readStringUntil('\n');
      //temperature = receivedString.toInt();
      //Serial.print("Temperature: ");
      //Serial.println(temperature);
      if(temperature < 0 && temperature != oldTemperature)
      {
        tft.fillRect(11,181,103,138, screenColor);
        tft.setTextSize(4);
        tft.setTextColor(TFT_BLUE);
        if(temperature <= -10)
        {
          tft.setTextSize(3);
          tft.setCursor(10, 225); // x,y
          tft.print(temperature);
        }
        else
        {
          tft.setCursor(30, 225); // x,y
          tft.print(temperature);
        }

        tft.setCursor(80, 225);
        tft.print("C");
      }
      else if(temperature >= 0 && temperature != oldTemperature)
      {
        tft.fillRect(11,181,103,138, screenColor);
        tft.setTextSize(4);
        tft.setTextColor(TFT_RED);
        if(temperature >= 10)
        {
          tft.setCursor(30, 225); // x,y
          tft.print(temperature);
        }
        else
        {
          tft.setCursor(50, 225); // x,y
          tft.print(temperature);
        }
        tft.setCursor(80, 225);
        tft.print("C");
      }

      // Valoisuus -----------------------------------------------------------------------------------
      //receivedString = softSerial.readStringUntil('\n');
      //lighting = receivedString.toInt(); // 1 = Pimeää, 2 = Valoisaa

      // Jos yöaika ja valoisaa...
      if(hours < 6 || hours >= 22 && lighting == 2)
      {
        lighting = 3; // 1 = pilvinen, 2 = Aurinko, 3 = Kuu
      }
      // Jos yöaika ja pimeää...
      else if(hours < 6 || hours >= 22 && lighting == 1)
      {
        lighting = 3; // 1 = pilvinen, 2 = Aurinko, 3 = Kuu
      }
      // Jos päiväaika ja pilvistä...
      else if(hours >= 6 && hours < 22 && lighting == 1)
      {
        lighting = 1;
      }
      else if(hours >= 6 && hours < 22 && lighting == 2)
      {
        lighting = 2;
      }
      // Jos saatu signaali on 0:
      else if(lighting == 0)
      {
        lighting = 1;
      }

      Serial.print("lightingCheck: ");
      Serial.println(lighting);

      // Ruudun päivitys: valo, onko tullut posti, 

      // TODO: KUVAKKEIDEN PÄIVITYS!
      // symbol 4-5 mail symbols: ----------------------------------------------------------
      if(isThereMailSignal == 0 && mailSymbol != 5)
      {
        tft.fillRect(126,31,103,138, screenColor);
        symbol(5); // Avonainen kirjekuori
        mailSymbol = 5;
      }
      else if(isThereMailSignal != 0 && mailSymbol != 4 && howManyTimesMail != oldMailTimes)
      {
        tft.fillRect(126,31,103,138, screenColor);
        symbol(isThereMailSignal); // 4, Avaamaton kirjekuori
        tft.setTextSize(2); 
        tft.setTextColor(TFT_WHITE); 
        tft.setCursor(195, 120); // x,y
        tft.print(howManyTimesMail);  
        tft.setTextColor(textColor);
        mailSymbol = 4;
      }
      else if(isThereMailSignal != 0 && mailSymbol == 4 && howManyTimesMail != oldMailTimes)
      {
        tft.fillRect(126,31,103,138, screenColor);
        symbol(isThereMailSignal); // 4, Avaamaton kirjekuori
        tft.setTextSize(2); 
        tft.setTextColor(TFT_WHITE); 
        if(howManyTimesMail >= 10)
        {
          tft.setCursor(190, 120); // x,y
          tft.print(howManyTimesMail); 
        }
        else
        {
          tft.setCursor(195, 120); // x,y
          tft.print(howManyTimesMail); 
        } 
        tft.setTextColor(textColor);
        mailSymbol = 4;
      }
      //----------------------------------------------------------------------------------------
      // Sää symbolin päivitys: // 1 = pilvinen, 2 = Aurinko, 3 = Kuu
      if(lighting == 0 && lightSymbol != 2)
      {
        tft.fillRect(11,31,103,138, screenColor);
        symbol(2);
        lightSymbol = 2;
      }
      else if(lighting == 1 && lightSymbol != 1)
      {
        tft.fillRect(11,31,103,138, screenColor);
        symbol(lighting);
        lightSymbol = 1;
      } 
      else if(lighting == 2 && lightSymbol != 2)
      {
        tft.fillRect(11,31,103,138, screenColor);
        symbol(lighting);
        lightSymbol = 2;
      }
      else if(lighting == 3 && lightSymbol != 3)
      {
        tft.fillRect(11,31,103,138, screenColor);
        symbol(lighting);
        lightSymbol = 3;
      }
      // Battery --------------------------------------------------------------------------------------------------
      // Battery symbol, 7 = 4block, 8 = 3block, 9 = 2block, 10 = 1block, 11 = 0block:

      if(batteryStage == 4 && batterySymbol != 7)
      {
        // tyhjennä ruudun alue:
        tft.fillRect(10,10,80,20, screenColor); 
        // piirrä symboli uudestaan:
        symbol(7);
        // batterySymbol:
        batterySymbol = 7;
      }
      else if(batteryStage == 3 && batterySymbol != 8)
      {
        // tyhjennä ruudun alue:
        tft.fillRect(10,10,80,20, screenColor); 
        // piirrä symboli uudestaan:
        symbol(8);
        // batterySymbol:
        batterySymbol = 8;
      }
      else if(batteryStage == 2 && batterySymbol != 9)
      {
        // tyhjennä ruudun alue:
        tft.fillRect(10,10,80,20, screenColor);
        // piirrä symboli uudestaan:
        symbol(9);
        // batterySymbol:
        batterySymbol = 9;
      }
      else if(batteryStage == 1 && batterySymbol != 10)
      {
        // tyhjennä ruudun alue:
        tft.fillRect(10,10,80,20, screenColor); 
        // piirrä symboli uudestaan:
        symbol(10);
        // batterySymbol:
        batterySymbol = 10;
      }
      else if(batteryStage == 0 && batterySymbol != 11)
      {
        // tyhjennä ruudun alue:
        tft.fillRect(10,10,80,20, screenColor); 
        // piirrä symboli uudestaan:
        symbol(11);
        // batterySymbol:
        batterySymbol = 11;
      }
      else if(batteryStage >= 5)
      {
        // BatteryStage 5 -->
      }
    }

    

    // ----------------------------------------------------------------------------------------------------------
    
  }//------------------------------------------------------------------------
  // Jos uutta signaalia ei tule...
  else
  {
    if(temperature < 0 && oldTemperature != temperature)
    {
      tft.fillRect(11,181,103,138, screenColor);
      tft.setTextSize(4);
      tft.setTextColor(TFT_BLUE);
      if(temperature <= -10)
      {
        tft.setTextSize(3);
        tft.setCursor(10, 225); // x,y
        tft.print(temperature);
      }
      else
      {
        tft.setCursor(30, 225); // x,y
        tft.print(temperature);
      }
      tft.setCursor(80, 225);
      tft.print("C");
    }
    else if( temperature >= 0 && oldTemperature != temperature)
    {
      tft.fillRect(11,181,103,138, screenColor);
      tft.setTextSize(4);
      tft.setTextColor(TFT_RED);
      if(temperature >= 10)
      {
        tft.setCursor(30, 225); // x,y
        tft.print(temperature);
      }
      else
      {
        tft.setCursor(50, 225); // x,y
        tft.print(temperature);
      }
      tft.setCursor(80, 225);
      tft.print("C");
    }

  }
  //---------------------------------------------------------------------------------------------------------------------------------

  tft.setTextColor(textColor);
  tft.setTextSize(2);
  delay(930);
  Serial.flush();
}

//-----------------------------------------------------------------------------------------------------------------------------------------------------------

// Method will draw the menu screen: ------------------------------------------------------------------------------------------------------------------------
void drawMenu()
{
  tft.fillScreen(screenColor); 
  // set text color:
  tft.setTextColor(textColor);
  
  // Menu screen:
  // Battery symbol, 7 = 4block, 8 = 3block, 9 = 2block, 10 = 1block, 11 = 0block:
  symbol(7);
  //tft.setCursor(10, 10);
  //tft.print("Smart Mail");
  tft.drawRect(10,30,105,140, drawColor); //x, y, width, height
  tft.drawRect(125,30,105,140, drawColor); //x, y, width, height
  tft.drawRect(10,180,105,140, drawColor); //x, y, width, height
  tft.drawRect(125,180,105,140, drawColor); //x, y, width, height

  if(temperature < 0)
    {
      tft.fillRect(11,181,103,138, screenColor);
      tft.setTextSize(4);
      tft.setTextColor(TFT_BLUE);
      if(temperature >= 10)
      {
        tft.setCursor(10, 225); // x,y
        tft.print(temperature);
      }
      else
      {
        tft.setCursor(30, 225); // x,y
        tft.print(temperature);
      }
      tft.setCursor(80, 225);
      tft.print("C");
    }
    else if( temperature >= 0)
    {
      tft.fillRect(11,181,103,138, screenColor);
      tft.setTextSize(4);
      tft.setTextColor(TFT_RED);
      if(temperature >= 10)
      {
        tft.setCursor(30, 225); // x,y
        tft.print(temperature);
      }
      else
      {
        tft.setCursor(50, 225); // x,y
        tft.print(temperature);
      }
      tft.setCursor(80, 225);
      tft.print("C");
    }

  // Valinta tekstit:
  //tft.setCursor(65, 95); // x,y
  tft.setTextDatum(MC_DATUM);
  //tft.drawString("Weather", 65, 95);
  // symbol 1-3 weather symbols:
  if(lighting == 1)
  {
    symbol(lighting);
    lightSymbol = 1;
  }
  else if(lighting == 2)
  {
    symbol(lighting);
    lightSymbol = 2;
  }
  else if(lighting == 3)
  {
    symbol(lighting);
    lightSymbol = 3;
  }
  else
  {
    symbol(1);
    lightSymbol = 1;
  }
  
  //tft.drawString("Mail", 180, 95);
  // symbol 4-5 mail symbols:
  if(isThereMailSignal == 4)
  {
    symbol(isThereMailSignal); // 4, Avaamaton kirjekuori
    tft.setTextSize(2); 
    tft.setTextColor(TFT_WHITE); 
    tft.setCursor(195, 120); // x,y
    tft.print(howManyTimesMail);  
    tft.setTextColor(textColor);
    mailSymbol = 4;
  }
  else 
  {
    symbol(5); // Avonainen kirjekuori 
    mailSymbol = 5; 
  }
  
  //tft.setCursor(130, 245); // x,y
  //tft.print("Settings");
  // symbol 6 settings symbol:
  symbol(6);
  tft.setTextColor(textColor);
  tft.setTextSize(2);
  currentTime += 50;
  delay(50);
}
//----------------------------------------------------------------------------------------------------------------------------------------------------------------

// method will show message in TFT screen: -----------------------------------------------------------------------------------------------------------------------
void displayMessage(String input) {
  tft.fillScreen(screenColor); 
  tft.setCursor(10,120);
  tft.println(input);
  menuDraw = 0;
  currentTime += 5000;
  delay(5000);
  tft.fillScreen(screenColor);
}
//----------------------------------------------------------------------------------------------------------------------------------------------------------------

// Method will show weather in TFT screen:------------------------------------------------------------------------------------------------------------------------
void weather()
{
  tft.setTextDatum(MC_DATUM);
  tft.fillScreen(screenColor); 
  //tft.setCursor(160,100);
  tft.drawString("WEATHER:",120,100);
  //tft.setCursor(160,110);
  tft.drawString("Such empty",120,130);
  //tft.setCursor(160,120);
  tft.drawString("Much WOW",120,160);
  menuDraw = 0;
  currentTime += 5000;
  delay(5000);
}
//----------------------------------------------------------------------------------------------------------------------------------------------------------------

// Method will show weather there is mail or not:-----------------------------------------------------------------------------------------------------------------

void isThereMail()
{
  tft.setTextDatum(MC_DATUM);
  tft.fillScreen(screenColor); 
  isThereMailSignal = 0;
  howManyTimesMail = 0;
  //tft.setCursor(160,100);
  tft.drawString("MAIL DATA:",120,100);
  //tft.setCursor(160,110);
  tft.drawString("RESETED",120,130);
  //tft.setCursor(160,120);
  tft.drawString("Much WOW",120,160);
  menuDraw = 0;
  currentTime += 5000;
  delay(5000);
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------

// This is settings function. User can decide color mode and sete time here: ---------------------------------------------------------------------------------------
void settings()
{
  menuDraw = 0;
  // Variables:
  int userInput = 0;
  int userHour = 0;
  int userMinute = 0;
  // Center text:
  tft.setTextDatum(MC_DATUM);
  // Clean screen:
  tft.fillScreen(screenColor);

  while(userInput != 4)
  {
    tft.drawString("1. Dark mode",120,100);
    //tft.drawString("1. Set Time",120,100);
    tft.drawString("2. Light mode",120,130);
    tft.drawString("3. DOOM mode",120,160);
    //tft.drawString("4. Set time",120,190);
    tft.drawString("4. Cancel", 120, 220);
      
    // 2s delay:
    //currentTime += 2000;
    //delay(2000);
    userInput = Serial.parseInt(); // Read integer from serial port
    Serial.print("You entered: ");
    Serial.println(userInput);

    // Clean screen:
    tft.fillScreen(screenColor); 

    switch(userInput)
    {
      // Dark mode...
      case 1:
        screenColor = TFT_BLACK;
        delay(10);
        textColor = TFT_WHITE;
        delay(10);
        drawColor = TFT_WHITE;
        delay(10);
       currentTime += 30;
        break;
      // Light mode...
      case 2:
        screenColor = TFT_WHITE;
        delay(10);
        textColor = TFT_BLACK;
        delay(10);
        drawColor = TFT_BLACK;
        delay(10);
        currentTime += 30;
        break;
      // DOOM mode...
      case 3:
        screenColor = TFT_RED;
        delay(10);
        textColor = TFT_GREEN;
        delay(10);
        drawColor = TFT_GREEN;
        delay(10);
        currentTime += 30;
        break;
      // Jos käyttäjä haluaa asettaa ajan...
      case 4:
        //setTime(); 
        break;
      // Jos käyttäjä haluaa poistua...
      case 5:
        break;
      // Jos käyttäjä antaa random inputin...
      default:
        userInput = 9;
        delay(10);
        currentTime += 10;
        break;
    }
/*
    // Jos Dark mode:
    if(userInput == 1)
    {
      screenColor = TFT_BLACK;
      delay(50);
      textColor = TFT_WHITE;
      delay(50);
      drawColor = TFT_WHITE;
      delay(50);
      currentTime += 150;
    }
    // Jos Light mode:
    else if(userInput == 2)
    {
      screenColor = TFT_WHITE;
      delay(50);
      textColor = TFT_BLACK;
      delay(50);
      drawColor = TFT_BLACK;
      delay(50);
      currentTime += 150;
    }
    // Jos DOOM mode:
    else if(userInput == 3)
    {
      screenColor = TFT_RED;
      delay(50);
      textColor = TFT_GREEN;
      delay(50);
      drawColor = TFT_GREEN;
      delay(50);
      currentTime += 150;
    }
    else if(userInput == 0)
    {
      break;
    }
    else
    {
      userInput = 9;
      delay(10);
      currentTime += 10;
    }*/
    Serial.flush();
    delay(10);
    currentTime += 10;
  }
}
//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------

// This method will handle symbol drawing on menu: --------------------------------------------------------------------------------------------------------------------------
void symbol(int valinta)
{
  // Sää sympoli 1 puolipilvistä
  if(valinta == 1)
  {
    int x = 38, y = 100, radius = 20;
    tft.fillCircle(x+20, y-8, radius + 10, TFT_YELLOW);
    tft.drawCircle(x+20, y-8, radius + 10, drawColor);
    tft.fillCircle(x, y, radius, TFT_WHITE);
    tft.drawCircle(x, y, radius, drawColor);
    tft.fillCircle(x+50, y, radius, TFT_WHITE);
    tft.drawCircle(x+50, y, radius, drawColor);
  }
  // Sää sympoli 2, Aurinko
  if(valinta == 2)
  {
    // Ympyrä:
    int x = 62, y = 100, radius = 20;
    tft.fillCircle(x, y, radius, TFT_YELLOW);
    tft.drawCircle(x, y, radius, drawColor); //draw
    // Auringon "säteet":
    tft.fillTriangle(x, y-40, x+5, y-30, x-5, y-30, TFT_YELLOW); // Ylä Keski
    tft.drawTriangle(x, y-40, x+5, y-30, x-5, y-30, drawColor); // Ylä Keski draw
    tft.fillTriangle(x+25, y-27, x+25, y-17, x+15, y-27, TFT_YELLOW); //Ylä oikea
    tft.drawTriangle(x+25, y-27, x+25, y-17, x+15, y-27, drawColor); //Ylä oikea draw
    tft.fillTriangle(x+38, y, x+28, y+5, x+28, y-5, TFT_YELLOW); //Oikea keski
    tft.drawTriangle(x+38, y, x+28, y+5, x+28, y-5, drawColor); //Oikea keski draw
    tft.fillTriangle(x+25, y+27, x+25, y+17, x+15, y+27, TFT_YELLOW); //Ala oikea
    tft.drawTriangle(x+25, y+27, x+25, y+17, x+15, y+27, drawColor); //Ala oikea draw
    tft.fillTriangle(x, y+40, x+5, y+30, x-5, y+30, TFT_YELLOW); // Ala keski
    tft.drawTriangle(x, y+40, x+5, y+30, x-5, y+30, drawColor); // Ala keski draw
    tft.fillTriangle(x-25, y+27, x-15, y+27, x-25, y+17, TFT_YELLOW); //Ala vasen
    tft.drawTriangle(x-25, y+27, x-15, y+27, x-25, y+17, drawColor); //Ala vasen draw
    tft.fillTriangle(x-38, y, x-28, y-5, x-28, y+5, TFT_YELLOW); //Vasen keski
    tft.drawTriangle(x-38, y, x-28, y-5, x-28, y+5, drawColor); //Vasen keski draw
    tft.fillTriangle(x-25, y-27, x-15, y-27, x-25, y-17, TFT_YELLOW); //Ylä vasen
    tft.drawTriangle(x-25, y-27, x-15, y-27, x-25, y-17, drawColor); //Ylä vasen draw
  }
  // Sää sympoli 3, Kuun sirppi
  if(valinta == 3)
  {
    int x = 67, y = 100, radius = 30;
    tft.fillCircle(x, y, radius, TFT_YELLOW);
    tft.fillCircle(x+15, y, radius, screenColor);
  }
  // Posti symboli 1, Avaamaton kirje
  if(valinta == 4)
  {
    int x = 137, y = 80, w = 80, h = 50, r = 1;
    tft.fillRoundRect(x, y, w, h, r, TFT_WHITE);
    tft.drawRoundRect(x, y, w, h, r, TFT_BLACK);
    tft.drawTriangle(x, y, x+w, y, x+(w/2), y+30, TFT_BLACK);
    tft.fillRoundRect(x+w-30, y+h-15, 25, 25, r+3, TFT_RED);
  }
  // Posti symboli 2, Avattu kirje
  if(valinta == 5)
  {
    int x = 137, y = 80, w = 80, h = 50, r = 1;
    // Kirjekuori
    tft.fillRoundRect(x, y, w, h, r, TFT_WHITE);
    tft.drawRoundRect(x, y, w, h, r, TFT_BLACK);
    tft.fillTriangle(x, y, x+w, y, x+(w/2), y-30, TFT_WHITE);
    tft.drawTriangle(x, y, x+w, y, x+(w/2), y-30, TFT_BLACK);
    tft.fillTriangle(x+5, y, x+w-5, y, x+(w/2), y+30, TFT_DARKCYAN);
    tft.fillTriangle(x+5, y, x+w-5, y, x+(w/2), y-25, TFT_DARKCYAN);
    // Paperi osa
    tft.fillRect(x+10, y-15, w-20, h, TFT_WHITE);
    tft.drawRect(x+10, y-15, w-20, h, TFT_BLACK);
    // Paperin "tekstit"
    tft.drawLine(x+15, y-10, x+60, y-10, TFT_BLACK);
    tft.drawLine(x+15, y-5,  x+60, y-5,  TFT_BLACK);
    tft.drawLine(x+15, y,    x+60, y,    TFT_BLACK);
    // Kirjekuoren päällysosa:
    tft.fillRect(x+1, y+6, w-2, h-20, TFT_WHITE);
    tft.drawLine(x+5, y, x+(w/2), y+30, TFT_BLACK);
    tft.drawLine(x+75, y, x+(w/2), y+30, TFT_BLACK);
  }
  // Settings symboli
  if(valinta == 6)
  {
    int x = 177, y = 250, radius = 30, w = 20, h = 20;
    // Ympyrä:
    tft.fillCircle(x, y, radius, TFT_DARKCYAN);
    tft.fillCircle(x, y, radius-15, screenColor);
    tft.drawCircle(x, y, radius-15, TFT_BLACK);
    // Hampaat:
    tft.fillRect(x-(w/2), y-radius-(h/2), w, h, TFT_DARKCYAN); // Ylä keski
    tft.fillRect(x-(w/2), y+radius-(h/2), w, h, TFT_DARKCYAN); // Ala keski
    tft.fillRect(x-radius-(w/2), y-(h/2), w, h, TFT_DARKCYAN); // Vasen keski
    tft.fillRect(x+radius-(w/2), y-(h/2), w, h, TFT_DARKCYAN); // Oikea keski
    drawDiamondRect(x+radius-(h/2), y+radius-(h/2), w+8, h+8, TFT_DARKCYAN); // x, y, w, h, color Ala oikea
    drawDiamondRect(x-radius+(h/2), y+radius-(h/2), w+8, h+8, TFT_DARKCYAN); // x, y, w, h, color Ala vasen
    drawDiamondRect(x+radius-(h/2), y-radius+(h/2), w+8, h+8, TFT_DARKCYAN); // x, y, w, h, color Ylä oikea
    drawDiamondRect(x-radius+(h/2), y-radius+(h/2), w+8, h+8, TFT_DARKCYAN); // x, y, w, h, color Ylä vasen
  
  }
  // Battery stage: (4 blocks)
  if(valinta == 7)
  {
    int x = 15, y = 10, wcase = 29, wblock = 5, h = 15, r = 1;
    tft.drawRoundRect(x, y, wcase, h, r, TFT_GREEN);
    tft.drawRoundRect(x-2, y+3, wcase-27, h-6, r, TFT_GREEN);
    tft.fillRect(x+3, y+2, wblock, h-4, TFT_GREEN);
    tft.fillRect(x+4+wblock, y+2, wblock, h-4, TFT_GREEN);
    tft.fillRect(x+5+(wblock*2), y+2, wblock, h-4, TFT_GREEN);
    tft.fillRect(x+6+(wblock*3), y+2, wblock, h-4, TFT_GREEN);
  }
  // Battery stage: (3 blocks)
  if(valinta == 8)
  {
    int x = 15, y = 10, wcase = 29, wblock = 5, h = 15, r = 1;
    tft.drawRoundRect(x, y, wcase, h, r, TFT_GREEN);
    tft.drawRoundRect(x-2, y+3, wcase-27, h-6, r, TFT_GREEN);
    tft.fillRect(x+4+wblock, y+2, wblock, h-4, TFT_GREEN);
    tft.fillRect(x+5+(wblock*2), y+2, wblock, h-4, TFT_GREEN);
    tft.fillRect(x+6+(wblock*3), y+2, wblock, h-4, TFT_GREEN);
  }
  // Battery stage: (2 blocks)
  if(valinta == 9)
  {
    int x = 15, y = 10, wcase = 29, wblock = 5, h = 15, r = 1;
    tft.drawRoundRect(x, y, wcase, h, r, TFT_YELLOW);
    tft.drawRoundRect(x-2, y+3, wcase-27, h-6, r, TFT_YELLOW);
    tft.fillRect(x+5+(wblock*2), y+2, wblock, h-4, TFT_YELLOW);
    tft.fillRect(x+6+(wblock*3), y+2, wblock, h-4, TFT_YELLOW);
  }
  // Battery stage: (1 blocks)
  if(valinta == 10)
  {
    int x = 15, y = 10, wcase = 29, wblock = 5, h = 15, r = 1;
    tft.drawRoundRect(x, y, wcase, h, r, TFT_RED);
    tft.drawRoundRect(x-2, y+3, wcase-27, h-6, r, TFT_RED);
    tft.fillRect(x+6+(wblock*3), y+2, wblock, h-4, TFT_RED);
  }
  // Battery stage: (0 blocks)
  if(valinta == 11)
  {
    int x = 15, y = 10, wcase = 29, h = 15, r = 1;
    tft.drawRoundRect(x, y, wcase, h, r, TFT_RED);
    tft.drawRoundRect(x-2, y+3, wcase-27, h-6, r, TFT_RED);
  }
  // Startscreen stage 1:
  if(valinta == 12)
  {
    // Ympyrä:
    int x = 110, y = 110, radius = 20;
    tft.fillCircle(x, y, radius, TFT_YELLOW);
    tft.drawCircle(x, y, radius, drawColor); //draw
    // Auringon "säteet":
    tft.fillTriangle(x, y-40, x+5, y-30, x-5, y-30, TFT_YELLOW); // Ylä Keski
    tft.drawTriangle(x, y-40, x+5, y-30, x-5, y-30, drawColor); // Ylä Keski draw
    tft.fillTriangle(x+25, y-27, x+25, y-17, x+15, y-27, TFT_YELLOW); //Ylä oikea
    tft.drawTriangle(x+25, y-27, x+25, y-17, x+15, y-27, drawColor); //Ylä oikea draw
    tft.fillTriangle(x+38, y, x+28, y+5, x+28, y-5, TFT_YELLOW); //Oikea keski
    tft.drawTriangle(x+38, y, x+28, y+5, x+28, y-5, drawColor); //Oikea keski draw
    tft.fillTriangle(x+25, y+27, x+25, y+17, x+15, y+27, TFT_YELLOW); //Ala oikea
    tft.drawTriangle(x+25, y+27, x+25, y+17, x+15, y+27, drawColor); //Ala oikea draw
    tft.fillTriangle(x, y+40, x+5, y+30, x-5, y+30, TFT_YELLOW); // Ala keski
    tft.drawTriangle(x, y+40, x+5, y+30, x-5, y+30, drawColor); // Ala keski draw
    tft.fillTriangle(x-25, y+27, x-15, y+27, x-25, y+17, TFT_YELLOW); //Ala vasen
    tft.drawTriangle(x-25, y+27, x-15, y+27, x-25, y+17, drawColor); //Ala vasen draw
    tft.fillTriangle(x-38, y, x-28, y-5, x-28, y+5, TFT_YELLOW); //Vasen keski
    tft.drawTriangle(x-38, y, x-28, y-5, x-28, y+5, drawColor); //Vasen keski draw
    tft.fillTriangle(x-25, y-27, x-15, y-27, x-25, y-17, TFT_YELLOW); //Ylä vasen
    tft.drawTriangle(x-25, y-27, x-15, y-27, x-25, y-17, drawColor); //Ylä vasen draw
  }
  // Startscreen stage 2:
  if(valinta == 13)
  {
    int x = 110, y = 130, w = 80, h = 50, r = 1;
    tft.fillRoundRect(x, y, w, h, r, TFT_WHITE);
    tft.drawRoundRect(x, y, w, h, r, TFT_BLACK);
    tft.drawTriangle(x, y, x+w, y, x+(w/2), y+30, TFT_BLACK);
  }
  // Startscreen stage 3:
  if(valinta == 14)
  {
    int x = 110, y = 130, w = 80, h = 50, r = 1;
    // Kirjekuori
    tft.fillRoundRect(x, y, w, h, r, TFT_WHITE);
    tft.drawRoundRect(x, y, w, h, r, TFT_BLACK);
    tft.fillTriangle(x, y, x+w, y, x+(w/2), y-30, TFT_WHITE);
    tft.drawTriangle(x, y, x+w, y, x+(w/2), y-30, TFT_BLACK);
    tft.fillTriangle(x+5, y, x+w-5, y, x+(w/2), y+30, TFT_DARKCYAN);
    tft.fillTriangle(x+5, y, x+w-5, y, x+(w/2), y-25, TFT_DARKCYAN);
    // Paperi osa
    tft.fillRect(x+10, y-15, w-20, h, TFT_WHITE);
    tft.drawRect(x+10, y-15, w-20, h, TFT_BLACK);
    // Paperin "tekstit"
    tft.drawLine(x+15, y-10, x+60, y-10, TFT_BLACK);
    tft.drawLine(x+15, y-5,  x+60, y-5,  TFT_BLACK);
    tft.drawLine(x+15, y,    x+60, y,    TFT_BLACK);
    // Kirjekuoren päällysosa:
    tft.fillRect(x+1, y+6, w-2, h-20, TFT_WHITE);
    tft.drawLine(x+5, y, x+(w/2), y+30, TFT_BLACK);
    tft.drawLine(x+75, y, x+(w/2), y+30, TFT_BLACK);
  }
  else
  {
  }
}
// ---------------------------------------------------------------------------------------------------------------------------------------------------------------------------

// Function to draw a diamond rectangle: -------------------------------------------------------------------------------------------------------------------------------------
void drawDiamondRect(int cx, int cy, int width, int height, uint16_t color) {
  // Calculate the coordinates of the four corners of the diamond rectangle
  int x0 = cx - width / 2;
  int y0 = cy;
  int x1 = cx;
  int y1 = cy - height / 2;
  int x2 = cx + width / 2;
  int y2 = cy;
  int x3 = cx;
  int y3 = cy + height / 2;
  
  // Draw the diamond rectangle using lines
  tft.drawLine(x0, y0, x1, y1, color);
  tft.drawLine(x1, y1, x2, y2, color);
  tft.drawLine(x2, y2, x3, y3, color);
  tft.drawLine(x3, y3, x0, y0, color);
  
  // Fill the area of the diamond rectangle with the specified color
  tft.fillTriangle(x0, y0, x1, y1, x2, y2, color);
  tft.fillTriangle(x0, y0, x2, y2, x3, y3, color);
}
// ---------------------------------------------------------------------------------------------------------------------------------------------------------------------------

#ifndef LOAD_GLCD
//ERROR_Please_enable_LOAD_GLCD_in_User_Setup
#endif

#ifndef LOAD_GFXFF
ERROR_Please_enable_LOAD_GFXFF_in_User_Setup!
#endif
