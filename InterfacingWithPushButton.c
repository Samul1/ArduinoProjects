/*
Materials:
LED
Resistor 220Ω
Push button switch

1. Connect LED anode to digital pin 9 of the arduino board.
2. Connect LED cathode to resistor.
3. Connect resistor to GND of the arduino board.
4. Connect Button leg to digital pin 7 of the arduino board.
5. Connect Button other leg to GND of the arduino board.
*/

// Define variables:
int ledPin = 9;       // Digital pin for the LED.
int buttonPin = 7;    // Digital pin for the push button.
int buttonState = 0;  // Variable to store button state.

void setup() {
    pinMode(ledPin, OUTPUT);    // LED pin as an output.
    pinMode(buttonPin, INPUT);  // Button pin as an input.
}

void loop() {
    // Read the state of the button (HIGH or LOW).
    buttonState = digitalRead(buttonPin);

    // Check if the vutton is pressed (LOW).
    if (buttonState == LOW) {
        digitalWrite(ledPin, HIGH); // Turn on the LED.
    }
    else {
        digitalWrite(ledPin, LOW);  // Turn off the LED.
    }
}