/* BSD 2-Clause license:

Copyright (c) 2013, Erwin Kooi
All rights reserved.

Redistribution and use in source and binary forms, with or without modification,
are permitted provided that the following conditions are met:

Redistributions of source code must retain the above copyright notice, this
list of conditions and the following disclaimer.

Redistributions in binary form must reproduce the above copyright notice, this
list of conditions and the following disclaimer in the documentation and/or
other materials provided with the distribution.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>
#include <avr/pgmspace.h>
#define LEDPORT	PORTB
#define LEFT	0x08	// PB3 (pin 2)
#define RIGHT	0x10	// PB4 (pin 3)
#define OFF		0x00
#define DISPLAY_FREQUENCY	((1200000 / 1024) / 20)
//                          ((F_CPU / CPU_PRESCALER / Hz)
#define LENGTH_DATA_ARRAY	64

volatile unsigned int iArrayPointer;
volatile unsigned char arData[LENGTH_DATA_ARRAY] PROGMEM = {
	LEFT, OFF, LEFT, OFF, LEFT, OFF, LEFT, OFF, 
	RIGHT, OFF, RIGHT, OFF, RIGHT, OFF, RIGHT, OFF,
	LEFT, OFF, LEFT, OFF, LEFT, OFF, LEFT, OFF, 
	RIGHT, OFF, RIGHT, OFF, RIGHT, OFF, RIGHT, OFF,
	LEFT, LEFT, OFF, OFF, RIGHT, RIGHT, OFF, OFF,
	LEFT, LEFT, OFF, OFF, RIGHT, RIGHT, OFF, OFF,
	LEFT, LEFT, OFF, OFF, RIGHT, RIGHT, OFF, OFF,
	LEFT, LEFT, OFF, OFF, RIGHT, RIGHT, OFF, OFF
};

int main (void) {
	// disable interrupts
	cli();
	// set sleep mode to
	set_sleep_mode(SLEEP_MODE_IDLE);
	// initialize output ports
	MCUCR = _BV(PUD);
	DDRB  = LEFT | RIGHT;			// enable pins for data output
	// setup clock
	TIMSK0 = _BV(OCIE0A);
	TCCR0A = _BV(WGM01);			// CTC mode, TOP = OCR0A
	TCCR0B = _BV(CS00) | _BV(CS02);	// 1024 prescale
	OCR0A = DISPLAY_FREQUENCY;
	// enable interrupts
	sei();
	// run main program
	while (1) {
		sleep_mode();
//		asm volatile("nop" ::);
	}
	return 0;   // we will never reach this...
}

ISR(TIM0_COMPA_vect)
{
	iArrayPointer++;
	if (iArrayPointer >= LENGTH_DATA_ARRAY) {
		iArrayPointer = 0;
	}
    LEDPORT = pgm_read_byte(&(arData[iArrayPointer]));
}
