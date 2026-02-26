/*
 * main.c
 *
 * Created: 3/25/2024 9:14:29 AM
 *  Author: Samuli Juutinen
 */ 

#include <xc.h>
#define F_CPU 8000000UL
#define BAUD_RATE 9600
#define MYUBRR F_CPU/16/BAUD_RATE-1

void USART_Init(unsigned int ubrr) {
	// Set baud rate
	UBRR0H = (unsigned char)(ubrr>>8);
	UBRR0L = (unsigned char)ubrr;
	// Enable receiver and transmitter
	UCSR0B = (1<<RXEN0)|(1<<TXEN0);
	// Set frame format: 8data, 1stop bit
	UCSR0C = (1<<UCSZ00)|(1<<UCSZ01);
}

void USART_Transmit(unsigned char data) {
	// Wait for empty transmit buffer
	while (!(UCSR0A & (1<<UDRE0)));
	// Put data into buffer, sends the data
	UDR0 = data;
}

unsigned char USART_Receive(void) {
	// Wait for data to be received
	while (!(UCSR0A & (1<<RXC0)));
	// Get and return received data from buffer
	return UDR0;
}

int main(void) {
	// Initialize USART communication
	USART_Init(MYUBRR);
	
	while (1) {
		// Receive message from XBee module
		unsigned char receivedChar = USART_Receive();
		
		// Echo back the received message
		USART_Transmit(receivedChar);
	}
}