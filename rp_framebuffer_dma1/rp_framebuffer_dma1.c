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

//-------------------------------------------------------------------------
static __attribute__ ((aligned (0x10))) uint32_t armtovc[256];
static uint32_t my_fb1[1920 * 1080];
static uint32_t my_fb2[1920 * 1080];

//------------------------------------------------------------------------
static __attribute__ ((aligned (32))) bcm283x_dma_cb_t dma_cb_data;

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
void* bcm283x_display_init(bcm283x_display_config_t* config) {
	uint32_t locate, i;
	uint32_t* fbp;
	uint32_t* fb;

	while(1) {
		locate = 1;

		armtovc[locate] = 0x00000000;	/* Buffer Request/Response Code */
						/* Request Codes: 0x00000000 Process Request Response Codes: 0x80000000 Request Successful, 0x80000001 Partial Response */
		locate++;

		/* Sequence Of Concatenated Tags */
		armtovc[locate] = BCM283X_TAGS_SET_PHYSICAL_DISPLAY;	/* Tag Identifier */
		locate++;
		armtovc[locate] = 0x00000008;	/* Value Buffer Size In Bytes */
		locate++;
		armtovc[locate] = 0x00000008;	/* 1 bit (MSB) Request/Response Indicator (0=Request, 1=Response), 31 bits (LSB) Value Length In Bytes */
		locate++;
		armtovc[locate] = config->size_x;	/* Value Buffer */
		locate++;
		armtovc[locate] = config->size_y;	/* Value Buffer */
		locate++;

		armtovc[locate] = BCM283X_TAGS_SET_VIRTUAL_BUFFER;	/* Tag Identifier */
		locate++;
		armtovc[locate] = 0x00000008;	/* Value Buffer Size In Bytes */
		locate++;
		armtovc[locate] = 0x00000008;	/* 1 bit (MSB) Request/Response Indicator (0=Request, 1=Response), 31 bits (LSB) Value Length In Bytes */
		locate++;
		armtovc[locate] = config->size_x;	/* Value Buffer */
		locate++;
		armtovc[locate] = config->size_y * 2;	/* Value Buffer */
		locate++;

		armtovc[locate] = BCM283X_TAGS_SET_DEPTH;	/* Tag Identifier */
		locate++;
		armtovc[locate] = 0x00000004;	/* Value Buffer Size In Bytes */
		locate++;
		armtovc[locate] = 0x00000004;	/* 1 bit (MSB) Request/Response Indicator (0=Request, 1=Response), 31 bits (LSB) Value Length In Bytes */
		locate++;
		armtovc[locate] = config->depth;	/* Value Buffer */
		locate++;

		armtovc[locate] = BCM283X_TAGS_SET_VIRTUAL_OFFSET;	/* Tag Identifier */
		locate++;
		armtovc[locate] = 0x00000008;	/* Value Buffer Size In Bytes */
		locate++;
		armtovc[locate] = 0x00000008;	/* 1 bit (MSB) Request/Response Indicator (0=Request, 1=Response), 31 bits (LSB) Value Length In Bytes */
		locate++;
		armtovc[locate] = 0;	/* Value Buffer */
		locate++;
		armtovc[locate] = 0;	/* Value Buffer */
		locate++;

		if(config->palette_count != 0 && config->palette != (uint32_t*)0) {
			armtovc[locate] = BCM283X_TAGS_SET_PALETTE;	/* Tag Identifier */
			locate++;
			armtovc[locate] = 0x00000010 + config->palette_count * 4;	/* Value Buffer Size In Bytes */
			locate++;
			armtovc[locate] = 0x00000010 + config->palette_count * 4;	/* 1 bit (MSB) Request/Response Indicator (0=Request, 1=Response), 31 bits (LSB) Value Length In Bytes */
			locate++;
			armtovc[locate] = 0;	/* Value Buffer (Offset: First Palette Index To Set (0-255)) */
			locate++;
			armtovc[locate] = config->palette_count;	/* Value Buffer (Length: Number Of Palette Entries To Set (1-256)) */
			locate++;
			for(i = 0; i < config->palette_count; i++) {
				armtovc[locate] = config->palette[i];	/* RGBA Palette Values (Offset To Offset+Length-1) */
				locate++;
			}
		}

		armtovc[locate] = BCM283X_TAGS_ALLOCATE_BUFFER;	/* Tag Identifier */
		locate++;
		armtovc[locate] = 0x00000008;	/* Value Buffer Size In Bytes */
		locate++;
		armtovc[locate] = 0x00000008;	/* 1 bit (MSB) Request/Response Indicator (0=Request, 1=Response), 31 bits (LSB) Value Length In Bytes */
		locate++;
		fbp = &(armtovc[locate]);	/* Frame Buffer point */
		armtovc[locate] = 0;	/* Value Buffer */
		locate++;
		armtovc[locate] = 0;	/* Value Buffer */
		locate++;

		/* End Tag */
		armtovc[locate] = 0;
		locate++;
		armtovc[locate] = 0;
		locate++;

		armtovc[0] = locate * 4;

		/* Mail Box Write */
		PUT32(BCM283X_MAIL_WRITE, (uint32_t)armtovc + BCM283X_MAIL_TAGS);

		while((armtovc[1] & 0x80000000) == 0);

		if(*fbp != 0) break;
	}

	while((BCM283X_MAIL_STATUS & BCM283X_MAIL_EMPTY) != 0);
		GET32(BCM283X_MAIL_READ);

	fb = (uint32_t*)(*(fbp) & 0x3FFFFFFF);	/* Convert Mail Box Frame Buffer Pointer From BUS Address To Physical Address ($CXXXXXXX -> $3XXXXXXX) */
	*fbp = (uint32_t)fb;

	return fb;
}

//------------------------------------------------------------------------
void bcm283x_display_setscreenno(const bcm283x_display_config_t* config, uint32_t screen_no) {
	uint32_t locate = 1;

	/* Sequence Of Concatenated Tags */
	armtovc[locate] = 0x00000000;
	locate++;

	armtovc[locate] = BCM283X_TAGS_SET_VIRTUAL_OFFSET;	/* Tag Identifier */
	locate++;
	armtovc[locate] = 0x00000008;	/* Value Buffer Size In Bytes */
	locate++;
	armtovc[locate] = 0x00000008;	/* 1 bit (MSB) Request/Response Indicator (0=Request, 1=Response), 31 bits (LSB) Value Length In Bytes */
	locate++;
	armtovc[locate] = 0;	/* Value Buffer */
	locate++;
	armtovc[locate] = config->size_y * screen_no;	/* Value Buffer */
	locate++;

	/* End Tag */
	armtovc[locate] = 0;
	locate++;
	armtovc[locate] = 0;
	locate++;

	armtovc[0] = locate * 4;

	/* Mail Box Write */
	while(BCM283X_MAIL_STATUS & BCM283X_MAIL_FULL);
	PUT32(BCM283X_MAIL_WRITE, (uint32_t)armtovc + BCM283X_MAIL_TAGS);
	while((armtovc[1] & 0x80000000) == 0);
	while((BCM283X_MAIL_STATUS & BCM283X_MAIL_EMPTY) != 0);
		GET32(BCM283X_MAIL_READ);
}

//------------------------------------------------------------------------
void draw05(uint32_t* fb) {
	uint32_t x, y;
	uint32_t c;

	/* ARIB STD-B28 */

	/* 40% GRAY */
	for(y = 0; y < 670; y++) {
		for(x = 0; x < 240; x++) {
			c = BCM283X_DISPLAY_RGBTO32(102,102,102);
			fb[(0 + y) * 1920 + (0 + x)] = c;
		}
	}
	/* 75% WHITE */
	for(y = 0; y < 670; y++) {
		for(x = 0; x < 206; x++) {
			c = BCM283X_DISPLAY_RGBTO32(192,192,192);
			fb[(0 + y) * 1920 + (240 + x)] = c;
		}
	}
	/* 75% YELLOW */
	for(y = 0; y < 670; y++) {
		for(x = 0; x < 206; x++) {
			c = BCM283X_DISPLAY_RGBTO32(192,192,0);
			fb[(0 + y) * 1920 + (446 + x)] = c;
		}
	}
	/* 75% CYAN */
	for(y = 0; y < 670; y++) {
		for(x = 0; x < 206; x++) {
			c = BCM283X_DISPLAY_RGBTO32(0,192,192);
			fb[(0 + y) * 1920 + (652 + x)] = c;
		}
	}
	/* 75% GREEN */
	for(y = 0; y < 670; y++) {
		for(x = 0; x < 206; x++) {
			c = BCM283X_DISPLAY_RGBTO32(0,192,0);
			fb[(0 + y) * 1920 + (858 + x)] = c;
		}
	}
	/* 75% MAGENTA */
	for(y = 0; y < 670; y++) {
		for(x = 0; x < 206; x++) {
			c = BCM283X_DISPLAY_RGBTO32(192,0,192);
			fb[(0 + y) * 1920 + (1064 + x)] = c;
		}
	}
	/* 75% RED */
	for(y = 0; y < 670; y++) {
		for(x = 0; x < 206; x++) {
			c = BCM283X_DISPLAY_RGBTO32(192,0,0);
			fb[(0 + y) * 1920 + (1270 + x)] = c;
		}
	}
	/* 75% BLUE */
	for(y = 0; y < 670; y++) {
		for(x = 0; x < 204; x++) {
			c = BCM283X_DISPLAY_RGBTO32(0,0,192);
			fb[(0 + y) * 1920 + (1476 + x)] = c;
		}
	}
	/* 40% GRAY */
	for(y = 0; y < 670; y++) {
		for(x = 0; x < 240; x++) {
			c = BCM283X_DISPLAY_RGBTO32(102,102,102);
			fb[(0 + y) * 1920 + (1680 + x)] = c;
		}
	}

	/* 100% CYAN */
	for(y = 0; y < 88; y++) {
		for(x = 0; x < 240; x++) {
			c = BCM283X_DISPLAY_RGBTO32(0,255,255);
			fb[(670 + y) * 1920 + (0 + x)] = c;
		}
	}
	/* 100% WHITE */
	for(y = 0; y < 88; y++) {
		for(x = 0; x < 206; x++) {
			c = BCM283X_DISPLAY_RGBTO32(255,255,255);
			fb[(670 + y) * 1920 + (240 + x)] = c;
		}
	}
	/* 75% WHITE */
	for(y = 0; y < 88; y++) {
		for(x = 0; x < 1234; x++) {
			c = BCM283X_DISPLAY_RGBTO32(192,192,192);
			fb[(670 + y) * 1920 + (446 + x)] = c;
		}
	}
	/* 100% BLUE */
	for(y = 0; y < 88; y++) {
		for(x = 0; x < 240; x++) {
			c = BCM283X_DISPLAY_RGBTO32(0,0,255);
			fb[(670 + y) * 1920 + (1680 + x)] = c;
		}
	}

	/* 100% YELLOW */
	for(y = 0; y < 88; y++) {
		for(x = 0; x < 240; x++) {
			c = BCM283X_DISPLAY_RGBTO32(255,255,0);
			fb[(758 + y) * 1920 + (0 + x)] = c;
		}
	}
	/* Y-RAMP */
	for(y = 0; y < 88; y++) {
		for(x = 0; x < 1440; x++) {
			c = BCM283X_DISPLAY_RGBTO32(x*178/1440+76,x*178/1440+76,x*178/1440+76);
			fb[(758 + y) * 1920 + (240 + x)] = c;
		}
	}
	/* 100% RED */
	for(y = 0; y < 88; y++) {
		for(x = 0; x < 240; x++) {
			c = BCM283X_DISPLAY_RGBTO32(255,0,0);
			fb[(758 + y) * 1920 + (1680 + x)] = c;
		}
	}

	/* 15% WHITE */
	for(y = 0; y < 234; y++) {
		for(x = 0; x < 240; x++) {
			c = BCM283X_DISPLAY_RGBTO32(38,38,38);
			fb[(846 + y) * 1920 + (0 + x)] = c;
		}
	}
	/* 0% BLACK */
	for(y = 0; y < 234; y++) {
		for(x = 0; x < 206; x++) {
			c = BCM283X_DISPLAY_RGBTO32(0,0,0);
			fb[(846 + y) * 1920 + (240 + x)] = c;
		}
	}
	/* 100% WHITE */
	for(y = 0; y < 234; y++) {
		for(x = 0; x < 412; x++) {
			c = BCM283X_DISPLAY_RGBTO32(255,255,255);
			fb[(846 + y) * 1920 + (446 + x)] = c;
		}
	}
	/* 0% BLACK */
	for(y = 0; y < 234; y++) {
		for(x = 0; x < 206; x++) {
			c = BCM283X_DISPLAY_RGBTO32(0,0,0);
			fb[(846 + y) * 1920 + (858 + x)] = c;
		}
	}
	/* -2%(0%) BLACK */
	for(y = 0; y < 234; y++) {
		for(x = 0; x < 82; x++) {
			c = BCM283X_DISPLAY_RGBTO32(0,0,0);
			fb[(846 + y) * 1920 + (1064 + x)] = c;
		}
	}
	/* 0% BLACK */
	for(y = 0; y < 234; y++) {
		for(x = 0; x < 82; x++) {
			c = BCM283X_DISPLAY_RGBTO32(0,0,0);
			fb[(846 + y) * 1920 + (1146 + x)] = c;
		}
	}
	/* 2% BLACK */
	for(y = 0; y < 234; y++) {
		for(x = 0; x < 82; x++) {
			c = BCM283X_DISPLAY_RGBTO32(5,5,5);
			fb[(846 + y) * 1920 + (1228 + x)] = c;
		}
	}
	/* 0% BLACK */
	for(y = 0; y < 234; y++) {
		for(x = 0; x < 82; x++) {
			c = BCM283X_DISPLAY_RGBTO32(0,0,0);
			fb[(846 + y) * 1920 + (1310 + x)] = c;
		}
	}
	/* 4% BLACK */
	for(y = 0; y < 234; y++) {
		for(x = 0; x < 84; x++) {
			c = BCM283X_DISPLAY_RGBTO32(10,10,10);
			fb[(846 + y) * 1920 + (1392 + x)] = c;
		}
	}
	/* 0% BLACK */
	for(y = 0; y < 234; y++) {
		for(x = 0; x < 204; x++) {
			c = BCM283X_DISPLAY_RGBTO32(0,0,0);
			fb[(846 + y) * 1920 + (1476 + x)] = c;
		}
	}
	/* 15% WHITE */
	for(y = 0; y < 234; y++) {
		for(x = 0; x < 240; x++) {
			c = BCM283X_DISPLAY_RGBTO32(38,38,38);
			fb[(846 + y) * 1920 + (1680 + x)] = c;
		}
	}
}

//------------------------------------------------------------------------
int notmain ( unsigned int earlypc )
{
	uint32_t cx = 0, cy = 0;
	int32_t dx, dy;
	uint32_t fcount, pretime, currenttime;
	uint8_t* fb;
	bcm283x_display_config_t config = {1920, 1080, 32, 0, 0, 0, (uint32_t*)0};
	uint32_t screen_no = 0;

	uart_init();
	fb = bcm283x_display_init(&config);

	draw05(my_fb1);
	draw05(my_fb2);

	/* DMA0 enable */
	PUT32(BCM283X_DMA_ENABLE, 0x1);
	/* DMA Control Block */
	dma_cb_data.transfer_information = 0x332;	/* Source Address Increment 32, Destination Address Increment 32, 2D Mode */
	dma_cb_data.length = (1080 << 16) + (1920 * 4); /* 0:15 XLENGTH, 16:29 YLENGTH */
	dma_cb_data.tdmode_stride = 0;
	dma_cb_data.next_control_block_address = 0;
	dma_cb_data.reserved1 = 0;
	dma_cb_data.reserved2 = 0;

	pretime = 0;
	fcount = 0;
	dx = dy = 32;
	while(1) {
		currenttime = GET32(BCM283X_STIMER_CLO);
		if(pretime != currenttime) {
			if(currenttime - pretime >= 1000000) {
				/* output fps */
				decstrings(azo_uint32_div(fcount, azo_uint32_div(currenttime - pretime, 1000000)));
				uart_putc('f');
				uart_putc('p');
				uart_putc('s');
				uart_putc(0x0D);
				uart_putc(0x0A);
		
				pretime = currenttime;
				fcount = 0;
			}
		}

		/* origin */
		if(cx + dx >= 1920)
			cx -= 1920;
		if(cy + dy >= 1080)
			cy -= 1080;
		cx += dx;
		cy += dy;

		/* DMA src set */
		dma_cb_data.src_address = (uint32_t)my_fb1 + ((cy * 1920 + cx) << 2);
		/* DMA dest set */
		if(screen_no == 0) {
			/* write to screen 1 */
			dma_cb_data.dst_address = (uint32_t)fb + 1920*1080*4;
		} else {
			/* write to screen 0 */
			dma_cb_data.dst_address = (uint32_t)fb;
		}
		/* DMA set CB */
		PUT32(BCM283X_DMA0_BASE + BCM283X_DMA_CONBLK_AD, (uint32_t)&(dma_cb_data));
		/* DMA activate */
		PUT32(BCM283X_DMA0_BASE + BCM283X_DMA_CS, 0x1);

		/* wait DMA transfer end */
		while((GET32(BCM283X_DMA0_BASE + BCM283X_DMA_CS) & 0x2) == 0);
		/* clear end */
		PUT32(BCM283X_DMA0_BASE + BCM283X_DMA_CS, 0x2);

		/* flip screen */
		if(screen_no == 0) {
			/* screen 1 */
			bcm283x_display_setscreenno(&config, 1);
			screen_no = 1;
		} else {
			/* screen 0 */
			bcm283x_display_setscreenno(&config, 0);
			screen_no = 0;
		}

		fcount++;
	}

	return(0);
}

//-------------------------------------------------------------------------

