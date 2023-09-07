/*
1. Connect LED anode to digital pin (pin 13) on the Arduino board.
2. Connect LED catode to resiston (200-ohm) and the to the ground (GND) of the Arduino board.
3. Connect one leg of the push button to the digital pin (pin 2).
4. Connect the other leg of the push button to the ground (GND) of the Arduino board.
*/

// Define variables
int buttonPin = 2; // Digital pin where the button is connected.
int ledPin = 13;   // Digital pin where the LED is connected.
bool buttonState = false;

void setup() {
    pinMode(ledPin, OUTPUT);    // LED pin as an output.
    pinMode(buttonPin, INPUT);  // Button pin as an input.
}

void loop() {
    // Read the state of the button
    buttonState = digitalRead(buttonPin);

    // Check if the button is pressed
    if (buttonState == HIGH) {
        digitalWrite(ledPin, HIGH);   // Turn LED on.
    }
    else {
        digitalWrite(ledPin, LOW);    // Turn LED off.
    }
}
