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

//-------------------------------------------------------------------------
void c_irq_handler(void)
{
}

//------------------------------------------------------------------------
void uart_putc(uint8_t c) {
	while((GET32(BCM283X_AUX_MU_LSR_REG) & 0x20) == 0);
	PUT32(BCM283X_AUX_MU_IO_REG,c);
}

//------------------------------------------------------------------------
uint8_t uart_getc(void) {
	while((GET32(BCM283X_AUX_MU_LSR_REG) & 0x01) == 0);
	return(GET32(BCM283X_AUX_MU_IO_REG));
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
	PUT32(BCM283X_AUX_MU_BAUD_REG, 270);	/* 115200 = system clock 250MHz / (8 * (baud + 1)), baud = 270 */
	PUT32(BCM283X_AUX_MU_CNTL_REG, 3);	/* Enable Transmitter and Receiver */
}
//------------------------------------------------------------------------
void hexstrings(uint32_t d) {
	uint32_t rb;
	uint32_t rc;

	rb = 32;
	while(1) {
		rb -= 4;
		rc = (d >> rb) & 0xF;
		if(rc > 9) rc += 0x37; else rc += 0x30;
		uart_putc(rc);
		if(rb == 0) break;
	}
	uart_putc(0x20);
}

//------------------------------------------------------------------------
void hexstring(uint32_t d) {
	hexstrings(d);
	uart_putc(0x0D);
	uart_putc(0x0A);
}

//------------------------------------------------------------------------
void decstrings(uint32_t d) {
	uint32_t ra;
	uint32_t rb;
	uint32_t rc;

	ra = 0;
	rb = 1000000000;
	while(1) {
		rc = azo_uint32_div(d, rb);
		if(rc == 0 && ra == 1) {
			uart_putc('0');
		} else if(rc > 0) {
			uart_putc('0' + rc);
			ra = 1;
		}
		if(rb == 1) {
			if(ra == 0) {
				uart_putc('0');
			}
			break;
		}
		d -= rc * rb;
		rb = azo_uint32_div(rb, 10);
	}
	uart_putc(0x20);
}

//------------------------------------------------------------------------
void decstring(uint32_t d) {
	decstrings(d);
	uart_putc(0x0D);
	uart_putc(0x0A);
}

//------------------------------------------------------------------------
int notmain(unsigned int earlypc) {
	uint32_t pretime, nowtime, msec;
	uint32_t clock;

	uart_init();

	hexstring(0x12345678);

	pretime = 0xFFFFFFFF;
	while(1) {
		nowtime = GET32(BCM283X_STIMER_CLO);
		msec = azo_uint32_mod(nowtime, 1000000);
		if(msec == 0 && nowtime != pretime) {
			clock = bcm283x_get_cpuclock_med(0);
			decstring(clock);
			pretime = nowtime;
		}
	}

	return(0);
}

//-------------------------------------------------------------------------

