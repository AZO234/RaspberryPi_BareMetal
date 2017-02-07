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
void gpclk_init(void) {
	uint32_t ra;

	/* GPIO function set */
	ra = GET32(BCM283X_GPIO_GPFSEL0);
	ra &= ~(7<<12); /* GPIO4 GPCLK0 */
	ra |=   4<<12 ; /* ALT0 */
	ra &= ~(7<<15); /* GPIO5 GPCLK1 */
	ra |=   4<<15 ;	/* ALT0 */
	ra &= ~(7<<18); /* GPIO6 GPCLK2 */
	ra |=   4<<18 ;	/* ALT0 */
	PUT32(BCM283X_GPIO_GPFSEL0, ra);

	/* PullUD disable */
	PUT32(BCM283X_GPIO_GPPUD, 0);					/* disable pullUD */
	for(ra = 0; ra < 150; ra++) dummy();				/* wait 150 cycles */
	PUT32(BCM283X_GPIO_GPPUDCLK0, (1 << 4) | (1 << 5) | (1 << 6));	/* GPIO4/5/6 pullUD disable */
	for(ra = 0; ra < 150; ra++) dummy();				/* wait 150 cycles */
	PUT32(BCM283X_GPIO_GPPUDCLK0, 0);				/* remove the clock */
}

//------------------------------------------------------------------------
int notmain(unsigned int earlypc) {
	gpclk_init();

	/* Clock setting */
	/*
		0: GND
		1: oscillator 19.2 MHz
		2: testdebug0
		3: testdebug1
		4: PLLA 650 MHz per
		*: PLLB 400 MHz
		5: PLLC 200 MHz per (changes with overclock settings to 1000 MHz)
		6: PLLD 500 MHz per
		7: HDMI auxiliary 216 MHz

		GPCLK0, GPCLK1, and GPCLK2.
		Don't use GPCLK1 (it's probably used for the Ethernet clock).
	*/
	/* GPnCLK  8.1920MHz: PLLD 500 MHz /  8.1920MHz = 61.0351 : I = 61 : F = (0.0351 * 4096) =  143 */
	/* GPnCLK 11.2896MHz: PLLD 500 MHz / 11.2896MHz = 44.2885 : I = 44 : F = (0.2885 * 4096) = 1181 */
	/* GPnCLK 12.2880MHz: PLLD 500 MHz / 12.2880MHz = 40.6901 : I = 40 : F = (0.6901 * 4096) = 2826 */
	/* GPnCLK 16.3840MHz: PLLD 500 MHz / 16.3840MHz = 30.5175 : I = 30 : F = (0.5175 * 4096) = 2119 */
	/* GPnCLK 16.9344MHz: PLLD 500 MHz / 16.9344MHz = 29.5256 : I = 29 : F = (0.5256 * 4096) = 2152 */
	/* GPnCLK 18.4320MHz: PLLD 500 MHz / 18.4320MHz = 27.1267 : I = 27 : F = (0.1267 * 4096) =  518 */
	/* GPnCLK 22.5792MHz: PLLD 500 MHz / 22.5792MHz = 22.1442 : I = 22 : F = (0.1442 * 4096) =  590 */
	/* GPnCLK 24.5760MHz: PLLD 500 MHz / 24.5760MHz = 20.3450 : I = 20 : F = (0.3450 * 4096) = 1413 */
	/* GPnCLK 32.7680MHz: PLLD 500 MHz / 32.7680MHz = 15.2587 : I = 15 : F = (0.2587 * 4096) = 1059 */
	/* GPnCLK 33.8688MHz: PLLD 500 MHz / 33.8688MHz = 14.7628 : I = 14 : F = (0.7628 * 4096) = 3124 */
	/* GPnCLK 36.8640MHz: PLLD 500 MHz / 36.8640MHz = 13.5633 : I = 13 : F = (0.5633 * 4096) = 2307 */
	/* GPnCLK 45.1584MHz: PLLD 500 MHz / 45.1584MHz = 11.0721 : I = 11 : F = (0.0721 * 4096) =  295 */
	/* GPnCLK 49.1520MHz: PLLD 500 MHz / 49.1520MHz = 10.1725 : I = 10 : F = (0.1725 * 4096) =  706 */

	/* GP0CLK 24.5760MHz */
	PUT32(BCM283X_CM_GP0CTL, BCM283X_CM_PASSWORD +  0x26);		/* Disable Clock Generator, oscillator */
	PUT32(BCM283X_CM_GP0DIV, BCM283X_CM_PASSWORD + 0x14585);	/* Div by I=20:F=1413 */
	PUT32(BCM283X_CM_GP0CTL, BCM283X_CM_PASSWORD + 0x216);		/* Enable Clock Generator, 1stage MASH, PLLD */
	/* GP2CLK 22.5792MHz */
	PUT32(BCM283X_CM_GP2CTL, BCM283X_CM_PASSWORD +  0x26);		/* Disable Clock Generator, oscillator */
	PUT32(BCM283X_CM_GP2DIV, BCM283X_CM_PASSWORD + 0x161FD);	/* Div by I=22:F= 590 */
	PUT32(BCM283X_CM_GP2CTL, BCM283X_CM_PASSWORD + 0x216);		/* Enable Clock Generator, 1stage MASH, PLLD */

	while(1);

	return(0);
}

//-------------------------------------------------------------------------

