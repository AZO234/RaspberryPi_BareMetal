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
typedef struct azo_rawpwmdata {
	uint32_t size;
	uint32_t samplerate;
	uint32_t bitswidth;
	uint32_t loopcount;
	uint32_t endless;
	void* data;
} azo_rawpwmdata_t;

//-------------------------------------------------------------------------
static __attribute__ ((aligned (32))) bcm283x_dma_cb_t dma_cb_data;

//-------------------------------------------------------------------------
static __attribute__ ((aligned (4))) uint8_t azo_sin_1k_raw[6144];

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
	value = GET32(BCM283X_GPIO_GPFSEL4);
	value &= ~(7 <<  0); /* GPIO40 */
	value |=   4 <<  0 ; /* ALT0 */
	value &= ~(7 << 15); /* GPIO45 */
	value |=   4 << 15 ; /* ALT0 */
	PUT32(BCM283X_GPIO_GPFSEL4, value);

	/* Clock setting 250MHz */
	PUT32(BCM283X_CM_PWMDIV, BCM283X_CM_PASSWORD + 0x2000);	/* Div by 2 */
	PUT32(BCM283X_CM_PWMCTL, BCM283X_CM_PASSWORD + 0x16);	/* Enable Clock Generator, PLLD */

	/* PWM init */
	PUT32(BCM283X_PWM_RNG1, azo_uint32_div(250000000, samplerate));
	PUT32(BCM283X_PWM_RNG2, azo_uint32_div(250000000, samplerate));
	PUT32(BCM283X_PWM_CTL, 0x2161); /* Channel 1&2 Enable + Use FIFO, Clear FIFO */
}

//------------------------------------------------------------------------
/*
	16bit 1channel -> 13bit 2channels : x4   size
	16bit 2channel -> 13bit 2channels : x2   size
	24bit 1channel -> 13bit 2channels : 8/3  size
	24bit 2channel -> 13bit 2channels : 4/3  size
	32bit 1channel -> 13bit 2channels : same size
	32bit 2channel -> 13bit 2channels : 1/2  size
*/
void convert_pcmtoraw(azo_rawpwmdata_t* rawdata, const azo_pcmdata_t* pcmdata) {
	uint32_t loc, loc2;

	if(pcmdata->data == 0) {
		return;
	}
	if(rawdata->data == 0) {
		return;
	}

	if(pcmdata->channels == 0) {
		return;
	}
	switch(pcmdata->samplerate) {
	case 8000:
	case 16000:
	case 32000:
	case 44100:
	case 48000:
	case 96000:
	case 192000:
	case 384000:
	case 768000:
		break;
	default:
		return;
	}
	switch(pcmdata->bitswidth) {
	case 16:
	case 24:
	case 32:
		break;
	default:
		return;
	}

	rawdata->samplerate = pcmdata->samplerate;
	rawdata->bitswidth = pcmdata->bitswidth;
	rawdata->loopcount = pcmdata->loopcount;
	rawdata->endless = pcmdata->endless;
	if(rawdata->loopcount == 0 && rawdata->endless != 0) {
		rawdata->loopcount = 1;
	}

	loc2 = 0;
	for(loc = 0; loc < pcmdata->size;) {
		switch(pcmdata->bitswidth) {
		case 16:
			if(pcmdata->channels == 1) {
				((uint32_t*)((uint32_t)rawdata->data + loc2))[0] = ((uint32_t*)((uint32_t)rawdata->data + loc2))[1] = ((uint16_t*)((uint32_t)pcmdata->data + loc))[0] >> 3;
				loc += 2;
			} else {
				((uint32_t*)((uint32_t)rawdata->data + loc2))[0] = ((uint16_t*)((uint32_t)pcmdata->data + loc))[0] >> 3;
				((uint32_t*)((uint32_t)rawdata->data + loc2))[1] = ((uint16_t*)((uint32_t)pcmdata->data + loc))[0] >> 3;
				loc += 2 * pcmdata->channels;
			}
			loc2 += 8;
			break;
		case 24:
			if(pcmdata->channels == 1) {
				((uint32_t*)((uint32_t)rawdata->data + loc2))[0] = ((uint32_t*)((uint32_t)rawdata->data + loc2))[1] = ((uint16_t)(((uint8_t*)((uint32_t)pcmdata->data + loc))[2]) << 5) | ((uint16_t)(((uint8_t*)((uint32_t)pcmdata->data + loc))[1]) >> 3);
				loc += 3;
			} else {
				((uint32_t*)((uint32_t)rawdata->data + loc2))[0] = ((uint16_t)(((uint8_t*)((uint32_t)pcmdata->data + loc))[2]) << 5) | ((uint16_t)(((uint8_t*)((uint32_t)pcmdata->data + loc))[1]) >> 3);
				((uint32_t*)((uint32_t)rawdata->data + loc2))[1] = ((uint16_t)(((uint8_t*)((uint32_t)pcmdata->data + loc))[5]) << 5) | ((uint16_t)(((uint8_t*)((uint32_t)pcmdata->data + loc))[4]) >> 3);
				loc += 3 * pcmdata->channels;
			}
			loc2 += 8;
			break;
		case 32:
			if(pcmdata->channels == 1) {
				((uint32_t*)((uint32_t)rawdata->data + loc2))[0] = ((uint32_t*)((uint32_t)rawdata->data + loc2))[1] = ((uint32_t*)((uint32_t)pcmdata->data + loc))[0] >> 19;
				loc += 4;
			} else {
				((uint32_t*)((uint32_t)rawdata->data + loc2))[0] = ((uint32_t*)((uint32_t)pcmdata->data + loc))[0] >> 19;
				((uint32_t*)((uint32_t)rawdata->data + loc2))[1] = ((uint32_t*)((uint32_t)pcmdata->data + loc))[1] >> 19;
				loc += 4 * pcmdata->channels;
			}
			loc2 += 8;
			break;
		}
	}

	rawdata->size = loc2;
}

//------------------------------------------------------------------------
void pwm_raw_play(azo_rawpwmdata_t* rawdata) {
	uint32_t nowloopcount;
	uint32_t value;

	if(rawdata->data == 0) {
		return;
	}

	switch(rawdata->samplerate) {
	case 8000:
	case 16000:
	case 32000:
	case 44100:
	case 48000:
	case 96000:
	case 192000:
	case 384000:
	case 768000:
		break;
	default:
		return;
	}
	switch(rawdata->bitswidth) {
	case 16:
	case 24:
	case 32:
		break;
	default:
		return;
	}
	if(rawdata->loopcount == 0 && rawdata->endless == 0) {
		return;
	}

	/* DMA0 enable */
	PUT32(BCM283X_DMA_ENABLE, 0x1);
	/* DMA Control Block */
	dma_cb_data.transfer_information = 0x50140;	/* Peripheral number 5(PWM), Control Destination Writes with DREQ. Destination address does not change. Source Address Increment 4 */
	dma_cb_data.src_address = (uint32_t)(rawdata->data);
	dma_cb_data.dst_address = ARMTOVC_PER_ADDRESS(BCM283X_PWM_FIF1);
	dma_cb_data.length = rawdata->size;
	dma_cb_data.tdmode_stride = 0;
	dma_cb_data.next_control_block_address = 0;
	dma_cb_data.reserved1 = 0;
	dma_cb_data.reserved2 = 0;
	if(rawdata->endless != 0) {
		/* loop */
		dma_cb_data.next_control_block_address = (uint32_t)&dma_cb_data;
	}
	/* DMA set CB */
	PUT32(BCM283X_DMA0_BASE + BCM283X_DMA_CONBLK_AD, (uint32_t)&dma_cb_data);

	/* PWM DMAC Register */
	/*
		bit31    : ENAB : DMA Enable
		bit 7: 0 : DREQ : DMA Threshold for DREQ signal
	*/
	value  = GET32(BCM283X_PWM_DMAC);
	value &= ~(0x800000FF);
	value |=   0x80000008 ;	/* DMA Enable, DMA Threshold for DREQ signal level 8 */
	PUT32(BCM283X_PWM_DMAC, value);

	if(rawdata->endless == 0) {
		for(nowloopcount = 0; nowloopcount < rawdata->loopcount; nowloopcount++) {
			/* DMA activate */
			PUT32(BCM283X_DMA0_BASE + BCM283X_DMA_CS, 0x1);
			/* wait DMA transfer end */
			while((GET32(BCM283X_DMA0_BASE + BCM283X_DMA_CS) & 0x2) == 0);
		}
	} else {
		/* DMA activate */
		PUT32(BCM283X_DMA0_BASE + BCM283X_DMA_CS, 0x1);
	}
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
	azo_rawpwmdata_t rawdata = {
		0,
		0,
		0,
		0,
		0,
		azo_sin_1k_raw
	};

	pwm_init(pcmdata.samplerate);
	convert_pcmtoraw(&rawdata, &pcmdata);
	pwm_raw_play(&rawdata);

	return(0);
}

//-------------------------------------------------------------------------

