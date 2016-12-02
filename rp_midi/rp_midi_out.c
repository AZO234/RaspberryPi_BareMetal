/*-------------------------------------------------------------------------
Copyright (c) 2016 AZO typesylph@gmail.com
  referred to David Welch https://github.com/dwelch67

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files
(the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge,
publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so,
subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE
FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
-------------------------------------------------------------------------*/

#include "rp_baremetal.h"

/* GM system on */
const uint8_t azo_midi_gmsystemon[6] = {0xF0, 0x7E, 0x7F, 0x09, 0x01, 0xF7};
/* C major */
const uint8_t azo_midi_keys[8] = {0x3C, 0x3E, 0x40, 0x41, 0x43, 0x45, 0x47, 0x48};

//-------------------------------------------------------------------------
void c_irq_handler(void) {
}

//------------------------------------------------------------------------
void uart_putc(uint8_t c) {
	while((GET32(BCM283X_AUX_MU_LSR_REG) & 0x20) == 0);
	PUT32(BCM283X_AUX_MU_IO_REG,c);
}

//------------------------------------------------------------------------
void uart_init(void) {
	uint32_t ra;

	/* GPIO function set */
	ra = GET32(BCM283X_GPIO_GPFSEL1);
	ra &= ~(7<<12); /* GPIO14 */
	ra |=   2<<12 ; /* ALT5 */
	ra &= ~(7<<15); /* GPIO15 */
	ra |=   2<<15 ;	/* ALT5 */
	PUT32(BCM283X_GPIO_GPFSEL1, ra);

	/* PullUD disable */
	PUT32(BCM283X_GPIO_GPPUD, 0);
	for(ra = 0; ra < 150; ra++) dummy();
	PUT32(BCM283X_GPIO_GPPUDCLK0, (1 << 14) | (1 << 15));
	for(ra = 0; ra < 150; ra++) dummy();
	PUT32(BCM283X_GPIO_GPPUDCLK0, 0);

	PUT32(BCM283X_AUX_ENABLES, 1);		/* Enable UART1 */
	PUT32(BCM283X_AUX_MU_IER_REG, 0);	/* Disable interrupt */
	PUT32(BCM283X_AUX_MU_CNTL_REG, 0);	/* Disable Transmitter and Receiver */
	PUT32(BCM283X_AUX_MU_LCR_REG, 3);	/* Works in 8-bit mode */
	PUT32(BCM283X_AUX_MU_MCR_REG, 0);	/* Disable RTS */
	PUT32(BCM283X_AUX_MU_IIR_REG, 0xC6);	/* Enable FIFO, Clear FIFO */
	PUT32(BCM283X_AUX_MU_BAUD_REG, 999);	/* 31250 = system clock 250MHz / (8 * (baud + 1)), baud = 999 */
	PUT32(BCM283X_AUX_MU_CNTL_REG, 3);	/* Enable Transmitter and Receiver */
}

//------------------------------------------------------------------------
int notmain(unsigned int earlypc) {
	uint32_t time;
	uint32_t key = 0;

	/* UART to MIDI */
	uart_init();

	time = GET32(BCM283X_STIMER_CLO);

	/* GM system on */
	uart_putc(azo_midi_gmsystemon[0]);
	uart_putc(azo_midi_gmsystemon[1]);
	uart_putc(azo_midi_gmsystemon[2]);
	uart_putc(azo_midi_gmsystemon[3]);
	uart_putc(azo_midi_gmsystemon[4]);
	uart_putc(azo_midi_gmsystemon[5]);

	/* wait 100ms */
	while(GET32(BCM283X_STIMER_CLO) < time + 100000);

	while(1) {
		time = GET32(BCM283X_STIMER_CLO);

		/* note on */
		uart_putc(0x90);
		uart_putc(azo_midi_keys[key]);
		uart_putc(0x7F);

		/* wait 500ms = 120BPM */
		while(GET32(BCM283X_STIMER_CLO) < time + 500000);

		/* note off */
		uart_putc(0x80);
		uart_putc(azo_midi_keys[key]);
		uart_putc(0x7F);

		key++;
		if(key >= 8) key = 0;
	}

	return(0);
}

//-------------------------------------------------------------------------

