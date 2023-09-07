void setup() {
	// put your setup code here, to run once:
	pinMode(13, OUTPUT); // Set pin 13 as an output.
}

void loop() {
	// put your main code here, to run repeatedly:
	digitalWrite(13, HIGH); // Turn the LED on.
	delay(1000);            // Wait for 1 sec (1000 milliseconds).
	digitalWrite(13, LOW);  // Turn the LED off.
	delay(1000);            // Wait for 1 sec.
}
