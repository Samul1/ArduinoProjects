/*
Materials:
Arduino board UNO or equilevant
Atleast 5kpl LEDs
Atleast 5kpl 200 ohm Resistors

1. Connect LEDs anode to resistor other leg.
2. Connect resistors other leg to digital pins 2, 3, 4, 5 and 6.
3. Connect LEDs cathode to ground (GND) of the arduino board.
*/

// Define variables:
const int numLEDs = 5; // Number of LEDs
int ledPins[] = { 2, 3, 4, 5, 6 }; // Array of LED pins

void setup() {
    // Initialize LED pins as outputs
    for (int i = 0; i < numLEDs; i++) {
        pinMode(ledPins[i], OUTPUT);
    }
}

void loop() {
    // Turn LEDs on one by one:
    for (int i = 0; i < numLEDs; i++) {
        digitalWrite(ledPins[i], HIGH); // Turn LED on
        delay(50); // Wait for 50ms
        digitalWrite(ledPins[i], LOW);  // Turn LED off
        delay(50); // Wait for 50ms
    }

}