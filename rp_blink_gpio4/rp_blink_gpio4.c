/*-------------------------------------------------------------------------
Copyright (c) 2017 AZO typesylph@gmail.com
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

//------------------------------------------------------------------------
static inline uint32_t GET32(const intptr_t addr) {
	return *(volatile uint32_t*)addr;
}
static inline void PUT32(const intptr_t addr, const uint32_t val) {
	*(volatile uint32_t*)addr = val;
}

//------------------------------------------------------------------------
void gpio_init(void) {
	uint32_t ra;

	/* GPIO function set */
	ra = GET32(BCM283X_GPIO_GPFSEL0);
	ra &= ~(7<<12); /* GPIO04 */
	ra |=   1<<12 ; /* output */
	PUT32(BCM283X_GPIO_GPFSEL0, ra);

	/* PullUD disable */
	PUT32(BCM283X_GPIO_GPPUD, 0);
	for(ra = 0; ra < 150; ra++) dummy();
	PUT32(BCM283X_GPIO_GPPUDCLK0, (1 << 4));
	for(ra = 0; ra < 150; ra++) dummy();
	PUT32(BCM283X_GPIO_GPPUDCLK0, 0);
}

//------------------------------------------------------------------------
int notmain(unsigned int earlypc) {
	uint32_t next, light = 1;

	gpio_init();

	next = azo_uint32_div(GET32(BCM283X_STIMER_CLO), 1000000) * 1000000 + 1000000;

	while(1) {
		if(next < GET32(BCM283X_STIMER_CLO)) {
			if(light == 1) {
				PUT32(BCM283X_GPIO_GPSET0, 1 << 4);
				light = 0;
			} else {
				PUT32(BCM283X_GPIO_GPCLR0, 1 << 4);
				light = 1;
			}
			next += 1000000;
		}
	}

	return(0);
}

//-------------------------------------------------------------------------
void c_irq_handler(void) {
}

//-------------------------------------------------------------------------

