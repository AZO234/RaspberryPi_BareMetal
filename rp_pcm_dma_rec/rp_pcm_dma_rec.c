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

#define BUFFERLEN 0x1000000

//-------------------------------------------------------------------------
void c_irq_handler(void)
{
}

//-------------------------------------------------------------------------
static __attribute__ ((aligned (32))) bcm283x_dma_cb_t dma_cb_data;

//-------------------------------------------------------------------------
typedef struct azo_pcmrecinfo {
	uint32_t maxsize;
	uint32_t channels;
	uint32_t samplerate;
	uint32_t bitswidth;
	void* data;
} azo_pcmrecinfo_t;

//-------------------------------------------------------------------------
static __attribute__ ((aligned (4))) uint8_t azo_pcmrecdata[BUFFERLEN];

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
void puts(const uint8_t* str) {
	uint32_t loc = 0;

	while(str[loc] != 0) uart_putc(str[loc++]);
}

//------------------------------------------------------------------------
void putsnl(const uint8_t* str) {
	puts(str);
	uart_putc(0x0D);
	uart_putc(0x0A);
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
void pcm_rec_init(const uint32_t bitswidth) {
	volatile uint32_t value, c;

	switch(bitswidth) {
	case 16:
	case 24:
	case 32:
		break;
	default:
		return;
	}

	/* GPIO setting */
	value = GET32(BCM283X_GPIO_GPFSEL1);
	value &= ~(7 << 24); /* GPIO18 */
	value |=   4 << 24 ; /* ALT0 PCM_CLK */
	value &= ~(7 << 27); /* GPIO19 */
	value |=   4 << 27 ; /* ALT0 PCM_FS */
	PUT32(BCM283X_GPIO_GPFSEL1, value);
	value = GET32(BCM283X_GPIO_GPFSEL2);
	value &= ~(7 <<  0); /* GPIO20 */
	value |=   4 <<  0 ; /* ALT0 PCM_DIN */
	PUT32(BCM283X_GPIO_GPFSEL2, value);
	PUT32(BCM283X_GPIO_GPPUD, 0);						/* disable pullUD */
	for(c = 0; c < 150; c++) value = GET32(BCM283X_GPIO_GPFSEL1);		/* wait 150 cycles */
	PUT32(BCM283X_GPIO_GPPUDCLK0, (1 << 18) | (1 << 19) | (1 << 20));	/* GPIO18/19/20 pullUD disable */
	for(c = 0; c < 150; c++) value = GET32(BCM283X_GPIO_GPFSEL1);		/* wait 150 cycles */
	PUT32(BCM283X_GPIO_GPPUDCLK0, 0);					/* remove the clock */

	/* PCM init */
	/* MODE_A Register */
	/*
		bit25     FRXP : Receive Frame Packed Mode
		 0 = The data from each channel is written into the RX FIFO.
		 1 = The data from both RX channels is merged.
		bit23     CLKM : PCM Clock Mode
		 0  = Master mode. The PCM CLK is an output and drives at the MCLK rate.
		 1* = Slave mode. The PCM CLK is an input.
		bit22     CLKI : Clock Invert this logically inverts the PCM_CLK signal.
		 0  = Outputs change on rising edge of clock, inputs are sampled on falling edge.
		 1* = Outputs change on falling edge of clock, inputs are sampled on rising edge.
		bit21      FSM : Frame Sync Invert This logically inverts the frame sync signal.
		 0  = Master mode.
		 1* = Slave mode.
		bit20      FSI : Frame Sync Invert This logically inverts the frame sync signal.
		 0* = FS is normally low and goes high to indicate frame sync.
		 1  = FS is normally high and goes low to indicate frame sync.
		bit19:10  FLEN : Frame Length - Sets the frame length to (FLEN+1) clocks.
		 Used only when FSM == 0.
		bit 9: 0 FSLEN : Frame Sync Length - Sets the frame sync length to (FSLEN) clocks.
		 Used only when FSM == 0.
		 PCM_FS will remain permanently active if FSLEN >= FLEN.
		 0  = frame sync pulse is off.
	*/
	PUT32(BCM283X_PCM_MODE_A, 0x0E0FC20);	/* FRXP=0, CLKM=1, CLKI=1, FSM=1, FLEN=64-1, FSLEN=32 */
	/* RXC_A Register */
	/*
		bit31    CH1WEX : Channel 1 Width Extension Bit
		 This is the MSB of the channel 1 width (CH1WID). It allows widths greater than 24 bits
		bit30     CH1EN : Channel 1 Enable
		bit29:20 CH1POS : Channel 1 Position
		 0  = indicates the first clock of frame.
		bit19:16 CH1WID : Channel 1 Width
		 width of channel 1 in bit clocks.
		 This field has been extended with the CH1WEX bit giving a total width of (CH1WEX*16)+CH1WID+8.
		 The Maximum supported width is 32 bits.
		 0  = 8 bits wide
		bit15    CH2WEX : Channel 2 Width Extension Bit
		bit14     CH2EN : Channel 2 Enable
		bit13: 4 CH2POS : Channel 2 Position
		bit 3: 0 CH2WID : Channel 2 Width
	*/
	switch(bitswidth) {
	case 16:
		PUT32(BCM283X_PCM_RXC_A, 0x40184218);	/* CH1WEX=0, CH1EN=CH2EN=1, CH1POS=1, CH2POS=33, CH1WID=CH2WID=8 */
		break;
	case 24:
		PUT32(BCM283X_PCM_RXC_A, 0xC010C210);	/* CH1WEX=1, CH1EN=CH2EN=1, CH1POS=1, CH2POS=33, CH1WID=CH2WID=0 */
		break;
	case 32:
//		PUT32(BCM283X_PCM_RXC_A, 0xC008C208);	/* CH1WEX=1, CH1EN=CH2EN=1, CH1POS=0, CH2POS=32, CH1WID=CH2WID=8 */ /* Left-Justified, not I2S */
		PUT32(BCM283X_PCM_RXC_A, 0xC018C218);	/* CH1WEX=1, CH1EN=CH2EN=1, CH1POS=1, CH2POS=33, CH1WID=CH2WID=8 */
		break;
	}
	/* CS_A Register */
	/*
		bit24      SYNC : PCM Clock sync helper.
		 a software synchronisation mechanism to allow the software to detect when 2 PCM clocks have occurred.
		bit23     RXSEX : RX Sign Extend
		 0* = No sign extension.
		 1  = Sign extend the RX data.
		bit20       RXD : Indicates that the RX FIFO contains data
		 0 = RX FIFO is empty.
		 1 = RX FIFO contains at least 1 sample.
		bit18       RXR : Indicates that the RX FIFO needs reading
		 0 = RX FIFO is less than RXTHR full.
		 1 = RX FIFO is RXTHR or more full.
		bit14    RXSYNC : RX FIFO Sync
		bit 8: 7  RXTHR : Sets the RX FIFO threshold at which point the RXR flag is set
		 00  = set when we have a single sample in the RX FIFO
		 01* = set when the RX FIFO is at least full
		 10  = set when the RX FIFO is at least
		 11  = set when the RX FIFO is full
		bit 4     RXCLR : Clear the RX FIFO
		bit 1      RXON : Enable reception
		bit 0        EN : Enable the PCM Audio Interface
	*/
//	PUT32(BCM283X_PCM_CS_A, 0x1000493);	/* SYNC=1, RXSYNC=1, RXTHR=1, RXCLR=1, RXON=1, EN=1 */
	PUT32(BCM283X_PCM_CS_A, 0x493);		/* SYNC=0, RXSYNC=1, RXTHR=1, RXCLR=1, RXON=1, EN=1 */

	/* RXCLR + SYNC and wait */
	while(GET32(BCM283X_PCM_CS_A) & ((1 << 4) | (1 << 24)));
}

//------------------------------------------------------------------------
void pcm_rec(azo_pcmrecinfo_t* recdata) {
	uint32_t value;

	if(recdata->data == 0) {
		return;
	}

	switch(recdata->samplerate) {
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
	switch(recdata->bitswidth) {
	case 16:
	case 24:
	case 32:
		break;
	default:
		return;
	}

	/* DMA0 enable */
	PUT32(BCM283X_DMA_ENABLE, 0x1);
	/* DMA Control Block */
	dma_cb_data.transfer_information = 0x30410;	/* Peripheral number 3(PCM RX). Control Source Reads with DREQ. Source address does not change. Destination address Address Increment 4. */
	
	dma_cb_data.src_address = ARMTOVC_PER_ADDRESS(BCM283X_PCM_FIFO_A);
	dma_cb_data.dst_address = (uint32_t)(recdata->data);
	dma_cb_data.length = recdata->maxsize;
	dma_cb_data.tdmode_stride = 0;
	dma_cb_data.next_control_block_address = 0;
	dma_cb_data.reserved1 = 0;
	dma_cb_data.reserved2 = 0;
	/* DMA set CB */
	PUT32(BCM283X_DMA0_BASE + BCM283X_DMA_CONBLK_AD, (uint32_t)&dma_cb_data);

	/* PCM DREQ_A Register */
	/*
		bit 6: 0 : RX : RX Request Level 0x0(0) - 0x3F(63)
	*/
	value  = GET32(BCM283X_PCM_DREQ_A);
	value &= ~(0x7F);
	value |=   0x30 ;	/* level 48 */
	PUT32(BCM283X_PCM_DREQ_A, value);
	/* PCM Enable DREQ */
	value  = GET32(BCM283X_PCM_CS_A);
	value |= 0x200;		/* DMA DREQ Enable */
	PUT32(BCM283X_PCM_CS_A, value);

	/* DMA activate */
	PUT32(BCM283X_DMA0_BASE + BCM283X_DMA_CS, 0x1);

	/* wait DMA transfer end */
	while((GET32(BCM283X_DMA0_BASE + BCM283X_DMA_CS) & 0x2) == 0);
}

//------------------------------------------------------------------------
uint8_t str1[] = "Hit space key to record.";
uint8_t str2[] = "processing...";
uint8_t str3[] = "Hit space key to playback.";

//------------------------------------------------------------------------
int notmain(unsigned int earlypc)
{
	uint32_t loc;

	azo_pcmrecinfo_t recdata = {
		BUFFERLEN,
		2,
		48000,
		16,
		azo_pcmrecdata
	};

/*	azo_pcmrecinfo_t recdata = {
		BUFFERLEN,
		2,
		192000,
		24,
		azo_pcmrecdata
	};
*/
/*	azo_pcmrecinfo_t recdata = {
		BUFFERLEN,
		2,
		384000,
		32,
		azo_pcmrecdata
	};
*/
/*	azo_pcmrecinfo_t recdata = {
		BUFFERLEN,
		2,
		768000,
		32,
		azo_pcmrecdata
	};
*/
	uart_init();
	gpclk_init();
	pwm_init(48000);

	/* GP0CLK 24.576MHz: PLLD 500 MHz / 24.576MHz = 20.345 : I = 20 : F = (0.345 * 4096) = 1413 */
	PUT32(BCM283X_CM_GP0CTL, BCM283X_CM_PASSWORD +  0x26);		/* Disable Clock Generator, oscillator */
	PUT32(BCM283X_CM_GP0DIV, BCM283X_CM_PASSWORD + 0x14585);	/* Div by I=20:F=1413 */
	PUT32(BCM283X_CM_GP0CTL, BCM283X_CM_PASSWORD + 0x216);		/* Enable Clock Generator, 1stage MASH, PLLD */

	putsnl(str1);
	while(uart_getc() != 0x20);
	putsnl(str2);
	pcm_rec_init(recdata.bitswidth);
	pcm_rec(&recdata);

	while(1) {
		putsnl(str3);
		while(uart_getc() != 0x20);
		putsnl(str2);

		for(loc = 0; loc < BUFFERLEN; loc += azo_uint32_div(recdata.samplerate, 48000) * 8) {
			/* FIFO full wait */
			while(GET32(BCM283X_PWM_STA) & 1);
			/* FIFO write */
			PUT32(BCM283X_PWM_FIF1, ((uint32_t*)(azo_pcmrecdata + loc))[0] >> 19);	/* Channel 1 */
			/* FIFO full wait */
			while(GET32(BCM283X_PWM_STA) & 1);
			PUT32(BCM283X_PWM_FIF1, ((uint32_t*)(azo_pcmrecdata + loc))[1] >> 19);	/* Channel 2 */
		}
	}

	return(0);
}

//-------------------------------------------------------------------------

