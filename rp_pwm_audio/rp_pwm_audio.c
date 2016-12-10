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
typedef struct azo_pcmdata {
	uint32_t size;
	uint32_t channels;
	uint32_t samplerate;
	uint32_t bitswidth;
	uint32_t loopcount;
	uint32_t endless;
	void* data;
} azo_pcmdata_t;

//-------------------------------------------------------------------------
uint16_t azo_sin_1k_48khz16bit[48] = {
	0x0000, 0x10B5, 0x2120, 0x30FB, 0x3FFF, 0x4DEB, 0x5A82, 0x658C, 0x6ED9, 0x7641, 0x7BA3, 0x7EE7, 0x7FFF, 0x7EE7, 0x7BA3, 0x7641, 
	0x6ED9, 0x658C, 0x5A82, 0x4DEB, 0x4000, 0x30FB, 0x2120, 0x10B5, 0x0000, 0xEF4B, 0xDEE0, 0xCF05, 0xC001, 0xB215, 0xA57E, 0x9A74, 
	0x9127, 0x89BF, 0x845D, 0x8119, 0x8000, 0x8119, 0x845D, 0x89BF, 0x9127, 0x9A74, 0xA57E, 0xB215, 0xC000, 0xCF05, 0xDEE0, 0xEF4B,
};

uint8_t azo_sin_1k_96khz24bit[288] = {
	0x00, 0x00, 0x00, 0x21, 0x5F, 0x08, 0x15, 0xB5, 0x10, 0xB8, 0xF8, 0x18, 0xFB, 0x20, 0x21,
	0xED, 0x24, 0x29, 0xC5, 0xFB, 0x30, 0xEA, 0x9C, 0x38, 0xFF, 0xFF, 0x3F, 0xEC, 0x1C, 0x47,
	0xE4, 0xEB, 0x4D, 0x71, 0x65, 0x54, 0x79, 0x82, 0x5A, 0x49, 0x3C, 0x60, 0x9A, 0x8C, 0x65,
	0x98, 0x6D, 0x6A, 0xEB, 0xD9, 0x6E, 0xB9, 0xCC, 0x72, 0xAF, 0x41, 0x76, 0x01, 0x35, 0x79,
	0x75, 0xA3, 0x7B, 0x5F, 0x8A, 0x7D, 0xAA, 0xE7, 0x7E, 0xD7, 0xB9, 0x7F, 0xFF, 0xFF, 0x7F,
	0xD7, 0xB9, 0x7F, 0xAA, 0xE7, 0x7E, 0x5F, 0x8A, 0x7D, 0x75, 0xA3, 0x7B, 0x01, 0x35, 0x79,
	0xAF, 0x41, 0x76, 0xB9, 0xCC, 0x72, 0xEB, 0xD9, 0x6E, 0x98, 0x6D, 0x6A, 0x9A, 0x8C, 0x65,
	0x49, 0x3C, 0x60, 0x79, 0x82, 0x5A, 0x71, 0x65, 0x54, 0xE4, 0xEB, 0x4D, 0xEC, 0x1C, 0x47,
	0x00, 0x00, 0x40, 0xEA, 0x9C, 0x38, 0xC5, 0xFB, 0x30, 0xED, 0x24, 0x29, 0xFB, 0x20, 0x21,
	0xB8, 0xF8, 0x18, 0x15, 0xB5, 0x10, 0x21, 0x5F, 0x08, 0x00, 0x00, 0x00, 0xDF, 0xA0, 0xF7,
	0xEB, 0x4A, 0xEF, 0x48, 0x07, 0xE7, 0x05, 0xDF, 0xDE, 0x13, 0xDB, 0xD6, 0x3B, 0x04, 0xCF,
	0x16, 0x63, 0xC7, 0x01, 0x00, 0xC0, 0x14, 0xE3, 0xB8, 0x1C, 0x14, 0xB2, 0x8F, 0x9A, 0xAB,
	0x87, 0x7D, 0xA5, 0xB7, 0xC3, 0x9F, 0x66, 0x73, 0x9A, 0x68, 0x92, 0x95, 0x15, 0x26, 0x91,
	0x47, 0x33, 0x8D, 0x51, 0xBE, 0x89, 0xFF, 0xCA, 0x86, 0x8B, 0x5C, 0x84, 0xA1, 0x75, 0x82,
	0x56, 0x18, 0x81, 0x29, 0x46, 0x80, 0x00, 0x00, 0x80, 0x29, 0x46, 0x80, 0x56, 0x18, 0x81,
	0xA1, 0x75, 0x82, 0x8B, 0x5C, 0x84, 0xFF, 0xCA, 0x86, 0x51, 0xBE, 0x89, 0x47, 0x33, 0x8D,
	0x15, 0x26, 0x91, 0x68, 0x92, 0x95, 0x66, 0x73, 0x9A, 0xB7, 0xC3, 0x9F, 0x87, 0x7D, 0xA5,
	0x8F, 0x9A, 0xAB, 0x1C, 0x14, 0xB2, 0x14, 0xE3, 0xB8, 0x00, 0x00, 0xC0, 0x16, 0x63, 0xC7,
	0x3B, 0x04, 0xCF, 0x13, 0xDB, 0xD6, 0x05, 0xDF, 0xDE, 0x48, 0x07, 0xE7, 0xEB, 0x4A, 0xEF,
	0xDF, 0xA0, 0xF7,
};

uint32_t azo_sin_1k_48khz32bit[48] = {
	0x00000000, 0x10B5150F, 0x2120FB83, 0x30FBC54D,
	0x3FFFFFFF, 0x4DEBE4FE, 0x5A827999, 0x658C9A2D,
	0x6ED9EBA1, 0x7641AF3C, 0x7BA3751D, 0x7EE7AA4B,
	0x7FFFFFFF, 0x7EE7AA4B, 0x7BA3751D, 0x7641AF3C,
	0x6ED9EBA1, 0x658C9A2D, 0x5A827999, 0x4DEBE4FE,
	0x40000000, 0x30FBC54D, 0x2120FB83, 0x10B5150F,
	0x00000000, 0xEF4AEAF1, 0xDEDF047D, 0xCF043AB3,
	0xC0000001, 0xB2141B02, 0xA57D8667, 0x9A7365D3,
	0x9126145F, 0x89BE50C4, 0x845C8AE3, 0x811855B5,
	0x80000000, 0x811855B5, 0x845C8AE3, 0x89BE50C4,
	0x9126145F, 0x9A7365D3, 0xA57D8667, 0xB2141B02,
	0xC0000000, 0xCF043AB3, 0xDEDF047D, 0xEF4AEAF1,
};

//------------------------------------------------------------------------
void pwm_init(const uint32_t samplerate) {
	uint32_t value;

	if(samplerate == 0) {
		return;
	}

	/* GPIO setting */
#ifdef AZO_BOARD_RPZERO
	value = GET32(BCM283X_GPIO_GPFSEL1);
	value &= ~(7 <<  6); /* GPIO12 */
	value |=   4 <<  6 ; /* ALT0 */
	value &= ~(7 <<  9); /* GPIO13 */
	value |=   4 <<  9 ; /* ALT0 */
	PUT32(BCM283X_GPIO_GPFSEL1, value);
#else
	value = GET32(BCM283X_GPIO_GPFSEL4);
	value &= ~(7 <<  0); /* GPIO40 */
	value |=   4 <<  0 ; /* ALT0 */
	value &= ~(7 << 15); /* GPIO45 */
	value |=   4 << 15 ; /* ALT0 */
	PUT32(BCM283X_GPIO_GPFSEL4, value);
#endif /* AZO_BOARD_RPZERO */

	/* Clock setting 250MHz */
	PUT32(BCM283X_CM_PWMDIV, BCM283X_CM_PASSWORD + 0x2000);	/* Div by 2 */
	PUT32(BCM283X_CM_PWMCTL, BCM283X_CM_PASSWORD + 0x16);	/* Enable Clock Generator, PLLD */

	/* PWM init */
	PUT32(BCM283X_PWM_RNG1, azo_uint32_div(250000000, samplerate));
	PUT32(BCM283X_PWM_RNG2, azo_uint32_div(250000000, samplerate));
	PUT32(BCM283X_PWM_CTL, 0x2161); /* Channel 1&2 Enable + Use FIFO, Clear FIFO */
}
//------------------------------------------------------------------------
void pwm_play(const azo_pcmdata_t* pcmdata) {
	uint32_t loc;
	uint32_t nowloopcount;
	uint32_t value;

	if(pcmdata->data == 0) {
		return;
	}

	do {
		for(nowloopcount = 0; nowloopcount < pcmdata->loopcount; nowloopcount++) {
			for(loc = 0; loc < pcmdata->size; loc += pcmdata->bitswidth / 8 * pcmdata->channels) {
				/* FIFO full wait */
				while(GET32(BCM283X_PWM_STA) & 1);
				/* FIFO write */
				if(pcmdata->channels == 1) {
					switch(pcmdata->bitswidth) {
					case 16:
						value = ((uint16_t*)(pcmdata->data + loc))[0] >> 3;
						PUT32(BCM283X_PWM_FIF1, value);	/* Channel 1 */
						/* FIFO full wait */
						while(GET32(BCM283X_PWM_STA) & 1);
						PUT32(BCM283X_PWM_FIF1, value);	/* Channel 2 */
						break;
					case 24:
						value = (((uint8_t*)(pcmdata->data + loc))[1] >> 3) | ((uint16_t)(((uint8_t*)(pcmdata->data + loc))[2]) << 5);
						PUT32(BCM283X_PWM_FIF1, value);	/* Channel 1 */
						/* FIFO full wait */
						while(GET32(BCM283X_PWM_STA) & 1);
						PUT32(BCM283X_PWM_FIF1, value);	/* Channel 2 */
						break;
					case 32:
						value = ((uint32_t*)(pcmdata->data + loc))[0] >> 19;
						PUT32(BCM283X_PWM_FIF1, value);	/* Channel 1 */
						/* FIFO full wait */
						while(GET32(BCM283X_PWM_STA) & 1);
						PUT32(BCM283X_PWM_FIF1, value);	/* Channel 2 */
						break;
					}
				} else {
					switch(pcmdata->bitswidth) {
					case 16:
						PUT32(BCM283X_PWM_FIF1, ((uint16_t*)(pcmdata->data + loc))[0] >> 3);	/* Channel 1 */
						/* FIFO full wait */
						while(GET32(BCM283X_PWM_STA) & 1);
						PUT32(BCM283X_PWM_FIF1, ((uint16_t*)(pcmdata->data + loc))[1] >> 3);	/* Channel 2 */
						break;
					case 24:
						PUT32(BCM283X_PWM_FIF1, (((uint8_t*)(pcmdata->data + loc))[1] >> 3) | ((uint16_t)(((uint8_t*)(pcmdata->data + loc))[2]) << 5));	/* Channel 1 */
						/* FIFO full wait */
						while(GET32(BCM283X_PWM_STA) & 1);
						PUT32(BCM283X_PWM_FIF1, (((uint8_t*)(pcmdata->data + loc))[4] >> 3) | ((uint16_t)(((uint8_t*)(pcmdata->data + loc))[5]) << 5));	/* Channel 2 */
						break;
					case 32:
						PUT32(BCM283X_PWM_FIF1, ((uint32_t*)(pcmdata->data + loc))[0] >> 19);	/* Channel 1 */
						/* FIFO full wait */
						while(GET32(BCM283X_PWM_STA) & 1);
						PUT32(BCM283X_PWM_FIF1, ((uint32_t*)(pcmdata->data + loc))[1] >> 19);	/* Channel 2 */
						break;
					}
				}
			}
		}
	} while(pcmdata->endless);
}
//------------------------------------------------------------------------
int notmain(unsigned int earlypc)
{
	azo_pcmdata_t pcmdata = {
		96,
		1,
		48000,
		16,
		1,
		1,
		azo_sin_1k_48khz16bit
	};

/*	azo_pcmdata_t pcmdata = {
		288,
		1,
		96000,
		24,
		1,
		1,
		azo_sin_1k_96khz24bit
	};
*/
/*	azo_pcmdata_t pcmdata = {
		192,
		1,
		48000,
		32,
		1,
		1,
		azo_sin_1k_48khz32bit
	};
*/
	pwm_init(pcmdata.samplerate);
	pwm_play(&pcmdata);

	return(0);
}

//-------------------------------------------------------------------------

