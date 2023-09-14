/*
Materials:
LED
Resistor 220Ω
potentiometer

1. Connect LED anode to digital pin 9 of the arduino board.
2. Connect LED cathode to resistor.
3. Connect resistor to GND of the arduino board.
4. Connect potentiometer VCC to 5V output of the arduino board.
5. Connect potentiometer GND to GND of the arduino board.
6. Connect potentiometer SIG to Analog pin 0 of the arduino board.
*/

// Define variables
int ledPin = 9;   // Digital pin for the LED.
int potPin = A0;  // Analog pin for the potentiometer.
int potValue = 0; // Variable to store potentiomater value.

void setup() {
	pinMode(ledPin, OUTPUT);  // LED pin as an output. 
	Serial.begin(9600);       // Initialize serial communication 
}

void loop() {
	// Read the potentiometer value (0-1023)
	potValue = analogRead(potPin);

	// Print the potentiometer value to the Serial Monitor
	Serial.println(potValue);

	// Map the potentiometer value to LED brightness (0-255)
	int brightness = map(potValue, 0, 1023, 0, 255);

	// Set the LED brightness using PWM
	analogWrite(ledPin, brightness);
}
