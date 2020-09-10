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
unsigned int GET32(const unsigned int addr) {
	return *(volatile unsigned int*)addr;
}

//-------------------------------------------------------------------------
void PUT32(const unsigned int addr, const unsigned int val) {
	*(volatile unsigned int*)addr = val;
}

//-------------------------------------------------------------------------
static __attribute__ ((aligned (0x10))) uint32_t armtovc[256];

//------------------------------------------------------------------------
typedef struct rp_display_config {
	uint32_t size_x;
	uint32_t size_y;
	uint32_t depth;
	uint32_t offset_x;
	uint32_t offset_y;
	uint32_t palette_count;
	uint32_t* palette;
} rp_display_config_t;

//------------------------------------------------------------------------
uint8_t* rp_display_init(rp_display_config_t* config) {
	uint32_t locate, i;
	uint32_t* fbp;
	uint32_t* fb;

	while(1) {
		locate = 1;

		armtovc[locate] = 0x00000000;	// Buffer Request/Response Code
						// Request Codes: 0x00000000 Process Request Response Codes: 0x80000000 Request Successful, 0x80000001 Partial Response
		locate++;

		// Sequence Of Concatenated Tags
		armtovc[locate] = RP_TAGS_SET_PHYSICAL_DISPLAY;	// Tag Identifier
		locate++;
		armtovc[locate] = 0x00000008;	// Value Buffer Size In Bytes
		locate++;
		armtovc[locate] = 0x00000008;	// 1 bit (MSB) Request/Response Indicator (0=Request, 1=Response), 31 bits (LSB) Value Length In Bytes
		locate++;
		armtovc[locate] = config->size_x;	// Value Buffer
		locate++;
		armtovc[locate] = config->size_y;	// Value Buffer
		locate++;

		armtovc[locate] = RP_TAGS_SET_VIRTUAL_BUFFER;	// Tag Identifier
		locate++;
		armtovc[locate] = 0x00000008;	// Value Buffer Size In Bytes
		locate++;
		armtovc[locate] = 0x00000008;	// 1 bit (MSB) Request/Response Indicator (0=Request, 1=Response), 31 bits (LSB) Value Length In Bytes
		locate++;
		armtovc[locate] = config->size_x;	// Value Buffer
		locate++;
		armtovc[locate] = config->size_y;	// Value Buffer
		locate++;

		armtovc[locate] = RP_TAGS_SET_DEPTH;	// Tag Identifier
		locate++;
		armtovc[locate] = 0x00000004;	// Value Buffer Size In Bytes
		locate++;
		armtovc[locate] = 0x00000004;	// 1 bit (MSB) Request/Response Indicator (0=Request, 1=Response), 31 bits (LSB) Value Length In Bytes
		locate++;
		armtovc[locate] = config->depth;	// Value Buffer
		locate++;

		armtovc[locate] = RP_TAGS_SET_VIRTUAL_OFFSET;	// Tag Identifier
		locate++;
		armtovc[locate] = 0x00000008;	// Value Buffer Size In Bytes
		locate++;
		armtovc[locate] = 0x00000008;	// 1 bit (MSB) Request/Response Indicator (0=Request, 1=Response), 31 bits (LSB) Value Length In Bytes
		locate++;
		armtovc[locate] = config->offset_x;	// Value Buffer
		locate++;
		armtovc[locate] = config->offset_y;	// Value Buffer
		locate++;

		if(config->palette_count != 0 && config->palette != (uint32_t*)0) {
			armtovc[locate] = RP_TAGS_SET_PALETTE;	// Tag Identifier
			locate++;
			armtovc[locate] = 0x00000010 + config->palette_count * 4;	// Value Buffer Size In Bytes
			locate++;
			armtovc[locate] = 0x00000010 + config->palette_count * 4;	// 1 bit (MSB) Request/Response Indicator (0=Request, 1=Response), 31 bits (LSB) Value Length In Bytes
			locate++;
			armtovc[locate] = 0;	// Value Buffer (Offset: First Palette Index To Set (0-255))
			locate++;
			armtovc[locate] = config->palette_count;	// Value Buffer (Length: Number Of Palette Entries To Set (1-256))
			locate++;
			for(i = 0; i < config->palette_count; i++) {
				armtovc[locate] = config->palette[i];	// RGBA Palette Values (Offset To Offset+Length-1)
				locate++;
			}
		}

		armtovc[locate] = RP_TAGS_ALLOCATE_BUFFER;	// Tag Identifier
		locate++;
		armtovc[locate] = 0x00000008;	// Value Buffer Size In Bytes
		locate++;
		armtovc[locate] = 0x00000008;	// 1 bit (MSB) Request/Response Indicator (0=Request, 1=Response), 31 bits (LSB) Value Length In Bytes
		locate++;
		fbp = &(armtovc[locate]);	// Frame Buffer point
		armtovc[locate] = 0;	// Value Buffer
		locate++;
		armtovc[locate] = 0;	// Value Buffer
		locate++;

		// End Tag
		armtovc[locate] = 0;
		locate++;
		armtovc[locate] = 0;
		locate++;

		armtovc[0] = locate * 4;

		// Mail Box Write
		PUT32(RP_MAIL_WRITE + RP_MAIL_TAGS, (uint32_t)armtovc + RP_MAIL_TAGS);

		for(i=0;i<100;i++);

		if(*fbp != 0) break;
	}

	fb = (uint32_t*)(*(fbp) & 0x3FFFFFFF);	// Convert Mail Box Frame Buffer Pointer From BUS Address To Physical Address ($CXXXXXXX -> $3XXXXXXX)
	*fbp = (uint32_t)fb;

	return (uint8_t*)fb;
}

//------------------------------------------------------------------------
void azo_framebuffer_drawpixel_RGB24(uint8_t* framebuffer, uint32_t x, uint32_t y, uint32_t max_x, uint8_t r, uint8_t g, uint8_t b) {
	if(max_x == 0) return;
	framebuffer[x * 3 + (max_x * 3) * y    ] = r;
	framebuffer[x * 3 + (max_x * 3) * y + 1] = g;
	framebuffer[x * 3 + (max_x * 3) * y + 2] = b;
}

//-------------------------------------------------------------------------
void rng_init(void) {
	// initial numbers write
	PUT32(RP_RNG_STATUS, 0xFFFFFFFF);

	// enable RNG
	PUT32(RP_RNG_CTRL, 1);
}

//-------------------------------------------------------------------------
uint32_t rng_get(void) {
	// wait
	while((GET32(RP_RNG_STATUS) >> 24) == 0);
	// get random number
	return GET32(RP_RNG_DATA);
}

//------------------------------------------------------------------------
void draw01(uint32_t* fb) {
	uint32_t loc;

	while(1) {
		for(loc = 0; loc < 1920 * 1080; loc++) {
			fb[loc] = rng_get();
		}
	}
}

//------------------------------------------------------------------------
void draw01_2(uint32_t* fb) {
	uint32_t loc;
	uint32_t c = 0xFFFFFFFF;

	while(1) {
		for(loc = 0; loc < 1920 * 1080; loc++) {
			if(loc % 1920 == 0) c *= rng_get();
			c = c / 7 * 9;
			fb[loc] = c;
		}
	}
}

//------------------------------------------------------------------------
void draw01_3(uint32_t* fb) {
	uint32_t loc;
	uint32_t c = 0xFFFFFFFF;

	while(1) {
		for(loc = 0; loc < 1920 * 1080; loc++) {
			if(loc % (1920 * 12) == 0) c *= rng_get();
			c = c / 7 * 9;
			fb[loc] = c;
		}
	}
}

//------------------------------------------------------------------------
void draw01_4(uint32_t* fb) {
	uint32_t loc;
	uint32_t c = 0xFFFFFFFF;

	while(1) {
		// 24bit
		for(loc = 0; loc < 1920 * 810; loc++) {
			if(loc % (1920 * 9) == 0) c *= rng_get();
			c = c / 7 * 9;
			fb[loc] = c;
		}
	}
}

//------------------------------------------------------------------------
#define RP_DISPLAY_RGBTO32(r,g,b) ((((b) & 0xFF) << 16) | (((g) & 0xFF) << 8) | ((r) & 0xFF))

void draw02(uint32_t* fb) {
	uint32_t x, y;
	uint32_t r,g,b,c;

	for(y = 0; y < 1080; y++) {
		for(x = 0; x < 1920; x++) {
			r = 255 - (255*y)/1080;
			g = (255*x)/1920;
			b = (255*y)/1080;
			c = RP_DISPLAY_RGBTO32(r,g,b);
			fb[y * 1920 + x] = c;
		}
	}
}

//------------------------------------------------------------------------
void draw03(uint32_t* fb) {
	uint32_t x, y;
	uint32_t c = 0x0000FF00;

	for(y = 0; y < 1080; y++) {
		for(x = 0; x < 1920; x++) {
			fb[y * 1920 + x] = c;
		}
	}
}

//------------------------------------------------------------------------
void draw04(uint32_t* fb) {
	uint32_t i;
	uint32_t r,g,b,c;

	for(b = 0; b < 256; b++) {
		for(r = 0; r < 256; r++) {
			for(i = 0; i < 28; i++) {
				g = 256/28 * i;
				c = RP_DISPLAY_RGBTO32(r,g,b);
				fb[((i/7)*256 + b) * 1920 + ((i%7)*256) + r] = c;
			}
		}
	}
}

//------------------------------------------------------------------------
void draw05(uint32_t* fb) {
	uint32_t x,y;
	uint32_t c;

	// ARIB STD-B28

	// 40% GRAY
	for(y = 0; y < 670; y++) {
		for(x = 0; x < 240; x++) {
			c = RP_DISPLAY_RGBTO32(102,102,102);
			fb[(0 + y) * 1920 + (0 + x)] = c;
		}
	}
	// 75% WHITE
	for(y = 0; y < 670; y++) {
		for(x = 0; x < 206; x++) {
			c = RP_DISPLAY_RGBTO32(192,192,192);
			fb[(0 + y) * 1920 + (240 + x)] = c;
		}
	}
	// 75% YELLOW
	for(y = 0; y < 670; y++) {
		for(x = 0; x < 206; x++) {
			c = RP_DISPLAY_RGBTO32(192,192,0);
			fb[(0 + y) * 1920 + (446 + x)] = c;
		}
	}
	// 75% CYAN
	for(y = 0; y < 670; y++) {
		for(x = 0; x < 206; x++) {
			c = RP_DISPLAY_RGBTO32(0,192,192);
			fb[(0 + y) * 1920 + (652 + x)] = c;
		}
	}
	// 75% GREEN
	for(y = 0; y < 670; y++) {
		for(x = 0; x < 206; x++) {
			c = RP_DISPLAY_RGBTO32(0,192,0);
			fb[(0 + y) * 1920 + (858 + x)] = c;
		}
	}
	// 75% MAGENTA
	for(y = 0; y < 670; y++) {
		for(x = 0; x < 206; x++) {
			c = RP_DISPLAY_RGBTO32(192,0,192);
			fb[(0 + y) * 1920 + (1064 + x)] = c;
		}
	}
	// 75% RED
	for(y = 0; y < 670; y++) {
		for(x = 0; x < 206; x++) {
			c = RP_DISPLAY_RGBTO32(192,0,0);
			fb[(0 + y) * 1920 + (1270 + x)] = c;
		}
	}
	// 75% BLUE
	for(y = 0; y < 670; y++) {
		for(x = 0; x < 204; x++) {
			c = RP_DISPLAY_RGBTO32(0,0,192);
			fb[(0 + y) * 1920 + (1476 + x)] = c;
		}
	}
	// 40% GRAY
	for(y = 0; y < 670; y++) {
		for(x = 0; x < 240; x++) {
			c = RP_DISPLAY_RGBTO32(102,102,102);
			fb[(0 + y) * 1920 + (1680 + x)] = c;
		}
	}

	// 100% CYAN
	for(y = 0; y < 88; y++) {
		for(x = 0; x < 240; x++) {
			c = RP_DISPLAY_RGBTO32(0,255,255);
			fb[(670 + y) * 1920 + (0 + x)] = c;
		}
	}
	// 100% WHITE
	for(y = 0; y < 88; y++) {
		for(x = 0; x < 206; x++) {
			c = RP_DISPLAY_RGBTO32(255,255,255);
			fb[(670 + y) * 1920 + (240 + x)] = c;
		}
	}
	// 75% WHITE
	for(y = 0; y < 88; y++) {
		for(x = 0; x < 1234; x++) {
			c = RP_DISPLAY_RGBTO32(192,192,192);
			fb[(670 + y) * 1920 + (446 + x)] = c;
		}
	}
	// 100% BLUE
	for(y = 0; y < 88; y++) {
		for(x = 0; x < 240; x++) {
			c = RP_DISPLAY_RGBTO32(0,0,255);
			fb[(670 + y) * 1920 + (1680 + x)] = c;
		}
	}

	// 100% YELLOW
	for(y = 0; y < 88; y++) {
		for(x = 0; x < 240; x++) {
			c = RP_DISPLAY_RGBTO32(255,255,0);
			fb[(758 + y) * 1920 + (0 + x)] = c;
		}
	}
	// Y-RAMP
	for(y = 0; y < 88; y++) {
		for(x = 0; x < 1440; x++) {
			c = RP_DISPLAY_RGBTO32(x*178/1440+76,x*178/1440+76,x*178/1440+76);
			fb[(758 + y) * 1920 + (240 + x)] = c;
		}
	}
	// 100% RED
	for(y = 0; y < 88; y++) {
		for(x = 0; x < 240; x++) {
			c = RP_DISPLAY_RGBTO32(255,0,0);
			fb[(758 + y) * 1920 + (1680 + x)] = c;
		}
	}

	// 15% WHITE
	for(y = 0; y < 234; y++) {
		for(x = 0; x < 240; x++) {
			c = RP_DISPLAY_RGBTO32(38,38,38);
			fb[(846 + y) * 1920 + (0 + x)] = c;
		}
	}
	// 0% BLACK
	for(y = 0; y < 234; y++) {
		for(x = 0; x < 206; x++) {
			c = RP_DISPLAY_RGBTO32(0,0,0);
			fb[(846 + y) * 1920 + (240 + x)] = c;
		}
	}
	// 100% WHITE
	for(y = 0; y < 234; y++) {
		for(x = 0; x < 412; x++) {
			c = RP_DISPLAY_RGBTO32(255,255,255);
			fb[(846 + y) * 1920 + (446 + x)] = c;
		}
	}
	// 0% BLACK
	for(y = 0; y < 234; y++) {
		for(x = 0; x < 206; x++) {
			c = RP_DISPLAY_RGBTO32(0,0,0);
			fb[(846 + y) * 1920 + (858 + x)] = c;
		}
	}
	// -2%(0%) BLACK
	for(y = 0; y < 234; y++) {
		for(x = 0; x < 82; x++) {
			c = RP_DISPLAY_RGBTO32(0,0,0);
			fb[(846 + y) * 1920 + (1064 + x)] = c;
		}
	}
	// 0% BLACK
	for(y = 0; y < 234; y++) {
		for(x = 0; x < 82; x++) {
			c = RP_DISPLAY_RGBTO32(0,0,0);
			fb[(846 + y) * 1920 + (1146 + x)] = c;
		}
	}
	// 2% BLACK
	for(y = 0; y < 234; y++) {
		for(x = 0; x < 82; x++) {
			c = RP_DISPLAY_RGBTO32(5,5,5);
			fb[(846 + y) * 1920 + (1228 + x)] = c;
		}
	}
	// 0% BLACK
	for(y = 0; y < 234; y++) {
		for(x = 0; x < 82; x++) {
			c = RP_DISPLAY_RGBTO32(0,0,0);
			fb[(846 + y) * 1920 + (1310 + x)] = c;
		}
	}
	// 4% BLACK
	for(y = 0; y < 234; y++) {
		for(x = 0; x < 84; x++) {
			c = RP_DISPLAY_RGBTO32(10,10,10);
			fb[(846 + y) * 1920 + (1392 + x)] = c;
		}
	}
	// 0% BLACK
	for(y = 0; y < 234; y++) {
		for(x = 0; x < 204; x++) {
			c = RP_DISPLAY_RGBTO32(0,0,0);
			fb[(846 + y) * 1920 + (1476 + x)] = c;
		}
	}
	// 15% WHITE
	for(y = 0; y < 234; y++) {
		for(x = 0; x < 240; x++) {
			c = RP_DISPLAY_RGBTO32(38,38,38);
			fb[(846 + y) * 1920 + (1680 + x)] = c;
		}
	}
}

//------------------------------------------------------------------------
void draw05_2(uint8_t* fb) {
	uint32_t x,y;

	// 24 bit
	// ARIB STD-B28

	// 40% GRAY
	for(y = 0; y < 670; y++) {
		for(x = 0; x < 240; x++) {
			azo_framebuffer_drawpixel_RGB24(fb, x, y, 1920, 102, 102, 102);
		}
	}
	// 75% WHITE
	for(y = 0; y < 670; y++) {
		for(x = 0; x < 206; x++) {
			azo_framebuffer_drawpixel_RGB24(fb, x + 240, y, 1920, 192, 192, 192);
		}
	}
	// 75% YELLOW
	for(y = 0; y < 670; y++) {
		for(x = 0; x < 206; x++) {
			azo_framebuffer_drawpixel_RGB24(fb, x + 446, y, 1920, 192, 192, 0);
		}
	}
	// 75% CYAN
	for(y = 0; y < 670; y++) {
		for(x = 0; x < 206; x++) {
			azo_framebuffer_drawpixel_RGB24(fb, x + 652, y, 1920, 0, 192, 192);
		}
	}
	// 75% GREEN
	for(y = 0; y < 670; y++) {
		for(x = 0; x < 206; x++) {
			azo_framebuffer_drawpixel_RGB24(fb, x + 858, y, 1920, 0, 192, 0);
		}
	}
	// 75% MAGENTA
	for(y = 0; y < 670; y++) {
		for(x = 0; x < 206; x++) {
			azo_framebuffer_drawpixel_RGB24(fb, x + 1064, y, 1920, 192, 0, 192);
		}
	}
	// 75% RED
	for(y = 0; y < 670; y++) {
		for(x = 0; x < 206; x++) {
			azo_framebuffer_drawpixel_RGB24(fb, x + 1270, y, 1920, 192, 0, 0);
		}
	}
	// 75% BLUE
	for(y = 0; y < 670; y++) {
		for(x = 0; x < 204; x++) {
			azo_framebuffer_drawpixel_RGB24(fb, x + 1476, y, 1920, 0, 0, 192);
		}
	}
	// 40% GRAY
	for(y = 0; y < 670; y++) {
		for(x = 0; x < 240; x++) {
			azo_framebuffer_drawpixel_RGB24(fb, x + 1680, y, 1920, 102, 102, 102);
		}
	}

	// 100% CYAN
	for(y = 0; y < 88; y++) {
		for(x = 0; x < 240; x++) {
			azo_framebuffer_drawpixel_RGB24(fb, x, y + 670, 1920, 0, 255, 255);
		}
	}
	// 100% WHITE
	for(y = 0; y < 88; y++) {
		for(x = 0; x < 206; x++) {
			azo_framebuffer_drawpixel_RGB24(fb, x + 240, y + 670, 1920, 255, 255, 255);
		}
	}
	// 75% WHITE
	for(y = 0; y < 88; y++) {
		for(x = 0; x < 1234; x++) {
			azo_framebuffer_drawpixel_RGB24(fb, x + 446, y + 670, 1920, 192, 192, 192);
		}
	}
	// 100% BLUE
	for(y = 0; y < 88; y++) {
		for(x = 0; x < 240; x++) {
			azo_framebuffer_drawpixel_RGB24(fb, x + 1680, y + 670, 1920, 0, 0, 255);
		}
	}

	// 100% YELLOW
	for(y = 0; y < 88; y++) {
		for(x = 0; x < 240; x++) {
			azo_framebuffer_drawpixel_RGB24(fb, x, y + 758, 1920, 255, 255, 0);
		}
	}
	// Y-RAMP
	for(y = 0; y < 88; y++) {
		for(x = 0; x < 1440; x++) {
			azo_framebuffer_drawpixel_RGB24(fb, x + 240, y + 758, 1920, x*178/1440+76, x*178/1440+76, x*178/1440+76);
		}
	}
	// 100% RED
	for(y = 0; y < 88; y++) {
		for(x = 0; x < 240; x++) {
			azo_framebuffer_drawpixel_RGB24(fb, x + 1680, y + 758, 1920, 255, 0, 0);
		}
	}

	// 15% WHITE
	for(y = 0; y < 234; y++) {
		for(x = 0; x < 240; x++) {
			azo_framebuffer_drawpixel_RGB24(fb, x, y + 846, 1920, 38, 38, 38);
		}
	}
	// 0% BLACK
	for(y = 0; y < 234; y++) {
		for(x = 0; x < 206; x++) {
			azo_framebuffer_drawpixel_RGB24(fb, x + 240, y + 846, 1920, 0, 0, 0);
		}
	}
	// 100% WHITE
	for(y = 0; y < 234; y++) {
		for(x = 0; x < 412; x++) {
			azo_framebuffer_drawpixel_RGB24(fb, x + 446, y + 846, 1920, 255, 255, 255);
		}
	}
	// 0% BLACK
	for(y = 0; y < 234; y++) {
		for(x = 0; x < 206; x++) {
			azo_framebuffer_drawpixel_RGB24(fb, x + 858, y + 846, 1920, 0, 0, 0);
		}
	}
	// -2%(0%) BLACK
	for(y = 0; y < 234; y++) {
		for(x = 0; x < 82; x++) {
			azo_framebuffer_drawpixel_RGB24(fb, x + 1064, y + 846, 1920, 0, 0, 0);
		}
	}
	// 0% BLACK
	for(y = 0; y < 234; y++) {
		for(x = 0; x < 82; x++) {
			azo_framebuffer_drawpixel_RGB24(fb, x + 1146, y + 846, 1920, 0, 0, 0);
		}
	}
	// 2% BLACK
	for(y = 0; y < 234; y++) {
		for(x = 0; x < 82; x++) {
			azo_framebuffer_drawpixel_RGB24(fb, x + 1228, y + 846, 1920, 5, 5, 5);
		}
	}
	// 0% BLACK
	for(y = 0; y < 234; y++) {
		for(x = 0; x < 82; x++) {
			azo_framebuffer_drawpixel_RGB24(fb, x + 1310, y + 846, 1920, 0, 0, 0);
		}
	}
	// 4% BLACK
	for(y = 0; y < 234; y++) {
		for(x = 0; x < 84; x++) {
			azo_framebuffer_drawpixel_RGB24(fb, x + 1392, y + 846, 1920, 10, 10, 10);
		}
	}
	// 0% BLACK
	for(y = 0; y < 234; y++) {
		for(x = 0; x < 204; x++) {
			azo_framebuffer_drawpixel_RGB24(fb, x + 1476, y + 846, 1920, 0, 0, 0);
		}
	}
	// 15% WHITE
	for(y = 0; y < 234; y++) {
		for(x = 0; x < 240; x++) {
			azo_framebuffer_drawpixel_RGB24(fb, x + 1680, y + 846, 1920, 38, 38, 38);
		}
	}
}

//------------------------------------------------------------------------
void draw06(uint32_t* fb) {
	uint32_t x,y,c;

	while(1) {
		x = rng_get() % 1920;
		y = rng_get() % 1080;
		c = rng_get();
		fb[y * 1920 + x] = c;
	}
}

//------------------------------------------------------------------------
int notmain ( unsigned int earlypc )
{
//	uint32_t* fb;
	uint8_t* fb;
//	rp_display_config_t config = {1920, 1080, 32, 0, 0, 0, (uint32_t*)0};
	rp_display_config_t config = {1920, 1080, 24, 0, 0, 0, (uint32_t*)0};

	fb = rp_display_init(&config);
	rng_init();

	// 32bit
//	draw01(fb);
//	draw01_2(fb);
//	draw01_3(fb);
//	draw02(fb);
//	draw03(fb);
//	draw04(fb);
//	draw05(fb);
//	draw06(fb);

	// 24bit
//	draw01_4(fb);
	draw05_2(fb);

	while(1);

	return(0);
}

//-------------------------------------------------------------------------

