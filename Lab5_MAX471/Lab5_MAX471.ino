/*****************************************************************************
Jännite / virta mittauksia MAX471

Samuli Juutinen TTV23SA
Harri Hankonen TTV23SA
14.02.2024

MAX471 - Virtalähde:
Vin - 6V
GND

MAX471 - DC-moottori:
Vout - DC-Moottori
GND

MAX471 - Arduino uno:
VT - A1(arduino uno)
AT - A0(arduino uno)
GND


******************************************************************************/
// alustetaan pinnit:
#define VT_PIN A1 
#define AT_PIN A0

void setup()
{
  Serial.begin(9600);
  
}

void loop()
{
  // Luetaan analogi pinnit:
  int vt_read = analogRead(VT_PIN);
  int at_read = analogRead(AT_PIN);

  // Lasketaan jännite:
  float voltage = vt_read * (5.0 / 1024.0) * 5.0;
  // Lasketaan virta:
  float current = (at_read * (5.0 / 1024.0))*1000;
  // Lasketaan teho:
  float watts = voltage * current;
  
  // Tulostetaan serial monitorille:
  Serial.print("V: "); 
  Serial.print(voltage, 3);
  Serial.print("\tmA: ");
  Serial.print(current,3);
 // Serial.print("\tWatts: ");
 // Serial.println(watts,3);
  Serial.println();
  
  delay(500);
  
}