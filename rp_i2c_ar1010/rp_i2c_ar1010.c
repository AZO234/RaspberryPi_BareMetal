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
void c_irq_handler(void) {
}

//------------------------------------------------------------------------
void delay_ms(uint32_t ms) {
	uint32_t time = GET32(BCM283X_STIMER_CLO);
	while(GET32(BCM283X_STIMER_CLO) < time + ms * 1000);
}

//------------------------------------------------------------------------
void uart_putc(const uint8_t c) {
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
	uint_t ra;

	/* GPIO function set */
	ra = GET32(BCM283X_GPIO_GPFSEL1);
	ra &= ~(7<<12); /* GPIO14 */
	ra |=   2<<12 ; /* ALT5 TXD1 */
	ra &= ~(7<<15); /* GPIO15 */
	ra |=   2<<15 ;	/* ALT5 RXD1 */
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
void uart_puts_uint_val(const uint_t d) {
	uint_t ra = d;
	uint_t rb = 1;
	uint_t rc = 0;
	uint_t rd;

	while(1) {
		if(d < rb) break;
		if(rb >= 1000000000) break;
		rb *= 10;
		rc++;
	}
	if(rb != 1000000000 && d < rb && rc > 0) rc--;

	while(1) {
		rb = 1;
		for(rd = 0; rd < rc; rd++) rb *= 10;
		rd = azo_uint32_div(ra, rb);
		uart_putc(rd + '0');
		if(rc == 0) break;
		ra -= rb * rd;
		rc--;
	}
}

//------------------------------------------------------------------------
void uart_puts_val(const int_t d) {
	int_t ra = d;

	if(d < 0) {
		uart_putc('-');
		ra *= -1;
	}
	uart_puts_uint_val(ra);
}

//------------------------------------------------------------------------
void uart_puts_hex(const uint_t d) {
	uint_t ra = d;
	uint_t rb = 1;
	uint_t rc = 0;
	uint_t rd;

	while(1) {
		if(d < rb) break;
#ifdef AZO_64BIT
		if(rb >= 0x1000000000000000) break;
#else
		if(rb >= 0x10000000) break;
#endif	/* AZO_64BIT */
		rb <<= 4;
		rc++;
	}

#ifdef AZO_64BIT
	if(rb != 0x1000000000000000 && d < rb && rc > 0) rc--;
#else
	if(rb != 0x10000000 && d < rb && rc > 0) rc--;
#endif	/* AZO_64BIT */

	while(1) {
		rb = 1;
		for(rd = 0; rd < rc; rd++) rb <<= 4;
		rd = azo_uint32_div(ra, rb);
		if(rd <= 9) {
			uart_putc(rd + '0');
		} else {
			uart_putc((rd - 10) + 'A');
		}
		if(rc == 0) break;
		ra -= rb * rd;
		rc--;
	}
}

//------------------------------------------------------------------------
void uart_puts_u32_hex(const uint32_t d) {
	uint_t rb;
	uint_t rc;

	rb = 32;
	while(1) {
		rb -= 4;
		rc = (d >> rb) & 0xF;
		if(rc > 9) rc += 0x37; else rc += 0x30;
		uart_putc(rc);
		if(rb == 0) break;
	}
}

//------------------------------------------------------------------------
void uart_puts_u16_hex(const uint16_t d) {
	uint_t rb;
	uint_t rc;

	rb = 16;
	while(1) {
		rb -= 4;
		rc = (d >> rb) & 0xF;
		if(rc > 9) rc += 0x37; else rc += 0x30;
		uart_putc(rc);
		if(rb == 0) break;
	}
}

//------------------------------------------------------------------------
void uart_puts_u8_hex(const uint8_t d) {
	uint_t rb;
	uint_t rc;

	rb = 8;
	while(1) {
		rb -= 4;
		rc = (d >> rb) & 0xF;
		if(rc > 9) rc += 0x37; else rc += 0x30;
		uart_putc(rc);
		if(rb == 0) break;
	}
}

//------------------------------------------------------------------------
void uart_puts_ln(void) {
	uart_putc(0x0D);
	uart_putc(0x0A);
}

//------------------------------------------------------------------------
void uart_puts(const char* str) {
	uint_t loc = 0;

	if(str != 0) {
		while(1) {
			if(str[loc] == '\0') break;
			uart_putc(str[loc++]);
		}
	}
}

//------------------------------------------------------------------------
uint_t uart_gethex(const uint_t c) {
	uint_t count = c;
	uint_t res = 0, val;

	if(c != 0) {
		while(1) {
			val = uart_getc();
			if((val >= '0' && val <= '9') || (val >= 'a' && val <= 'f') || (val >= 'A' && val <= 'F')) {
				res <<= 4;
				if(val >= '0' && val <= '9') {
					res += val - '0';
				} else if(val >= 'a' && val <= 'f') {
					res += val - 'a' + 10;
				} else if(val >= 'A' && val <= 'F') {
					res += val - 'A' + 10;
				}
				count--;
				if(count == 0) break;
			}
		}
	}

	return res;
}

//------------------------------------------------------------------------
void i2c1_init(void) {
	uint_t ra;

	/* GPIO function set */
	ra = GET32(BCM283X_GPIO_GPFSEL0);
	ra &= ~(7<< 6); /* GPIO2 */
	ra |=   4<< 6 ; /* ALT0 SDA1 */
	ra &= ~(7<< 9); /* GPIO3 */
	ra |=   4<< 9 ;	/* ALT0 SCL1 */
	PUT32(BCM283X_GPIO_GPFSEL0, ra);

	PUT32(BCM283X_I2C1_BASE + BCM283X_I2C_C, 0x8030);	/* Enable I2C, FIFO Clear */
	PUT32(BCM283X_I2C1_BASE + BCM283X_I2C_DIV, 1500);	/* 100kHz = 150MHz/1500 (default, SLOW mode, AVR too) */
}

//------------------------------------------------------------------------
void i2c1_setaddr(const uint8_t addr) {
	PUT32(BCM283X_I2C1_BASE + BCM283X_I2C_A, addr);
}

//------------------------------------------------------------------------
void i2c1_transfer(void) {
	PUT32(BCM283X_I2C1_BASE + BCM283X_I2C_S, 0x302);	/* Clock Stretch Timeout clear, ACK Error clear, Transfer Done clear */
	PUT32(BCM283X_I2C1_BASE + BCM283X_I2C_C, GET32(BCM283X_I2C1_BASE + BCM283X_I2C_C) | 0xB0);
}

//------------------------------------------------------------------------
/* It can to 100ms wait between write after and pre read, maybe */
uint_t i2c1_write(const uint8_t* data, const uint_t len) {
	uint_t c;
	uint_t loc;
	uint_t res = 0;

	c = BCM283X_I2C1_BASE + BCM283X_I2C_S;
	while(GET32(c) & 1);

	PUT32(BCM283X_I2C1_BASE + BCM283X_I2C_DLEN, len);

	PUT32(BCM283X_I2C1_BASE + BCM283X_I2C_C, GET32(BCM283X_I2C1_BASE + BCM283X_I2C_C) & ~1);	/* Write */
	i2c1_transfer();

	for(loc = 0; loc < len; loc++) {
//		while(GET32(c) & 4);	/* Wait for write (FIFO is full) */
		PUT32(BCM283X_I2C1_BASE + BCM283X_I2C_FIFO, data[loc]);
	}
	while((GET32(c) & 2) == 0);	/* Wait for done */

	loc = GET32(c);
	if(loc & 0x200) {
		res = 1;
	}
	if(loc & 0x100) {
		res = 2;
	}

	return res;
}

//------------------------------------------------------------------------
uint_t i2c1_read(uint8_t* data, const uint_t len) {
	uint_t c;
	uint_t loc;
	uint_t res = 0;

	c = BCM283X_I2C1_BASE + BCM283X_I2C_S;
	while(GET32(c) & 1);

	PUT32(BCM283X_I2C1_BASE + BCM283X_I2C_DLEN, len);

	PUT32(BCM283X_I2C1_BASE + BCM283X_I2C_C, GET32(BCM283X_I2C1_BASE + BCM283X_I2C_C) | 1);	/* Read */
	i2c1_transfer();

	for(loc = 0; loc < len; loc++) {
		while((GET32(c) & 32) == 0);	/* Wait for read */
		data[loc] = (uint8_t)GET32(BCM283X_I2C1_BASE + BCM283X_I2C_FIFO);
	}
	/* probably done */

	loc = GET32(c);
	if(loc & 0x200) {
		res = 1;
	}
	if(loc & 0x100) {
		res = 2;
	}

	return res;
}

//------------------------------------------------------------------------
/*
    9-27-09
    Copyright Spark Fun Electronicsｩ 2009
    Nathan Seidle
    spark at sparkfun.com

    Example Interface to AR1000
	Using ATmega168 at 8MHz
    altanated for AR1010 and Raspberry Pi by AZO
*/ 

#define sbi(port, bit_mask)   ((port) |= (uint8_t)(1 << bit_mask))
#define cbi(port, bit_mask)   ((port) &= (uint8_t)~(1 << bit_mask))

//#define AR1010_W 0x20 //Write address of AR1010
//#define AR1010_R 0x21 //read address
#define AR1010_AD 0x10

#define ADDR_STATUS	0x13 // the address of status register
#define MASK_STC (1<<5) //0x0020  // Seek/Tune/PowerOn complete  D5 in  adress 13H 
#define MASK_SF	 (1<<4) //0x0010  //  Seek Fail D4 in address 13H
#define MASK_ST	 (1<<3) //0x0008  //  Stereo  D3 in address 13H
#define MASK_READCHAN 0xFF80 // D7~D15 in address 13H
#define SHIFT_READCHAN 7

#define AR1010_MUTE_ON  { uint16_t temp = ar1010_read(1); ar1010_write(1, temp | (1<<1)); } //Reg_Data[1].BIT.B1 = ON;
#define AR1010_MUTE_OFF { uint16_t temp = ar1010_read(1); ar1010_write(1, temp & ~(1<<1)); } //Reg_Data[1].BIT.B1 = OFF;}

#define AR1010_TUNE_ON 	{ uint16_t temp = ar1010_read(2); ar1010_write(2, temp | (1<<9)); } // Reg_Data[2].BIT.B9 = ON;
#define AR1010_TUNE_OFF { uint16_t temp = ar1010_read(2); ar1010_write(2, temp & ~(1<<9)); } //Reg_Data[2].BIT.B9 = OFF;

#define AR1010_SEEK_ON 	{ uint16_t temp = ar1010_read(3); ar1010_write(3, temp | (1<<14)); } //Reg_Data[3].BIT.B14 = ON;}
#define AR1010_SEEK_OFF { uint16_t temp = ar1010_read(3); ar1010_write(3, temp & ~(1<<14)); } //Reg_Data[3].BIT.B14 = OFF;}

//I2CDEBUG will turn on '!' serial characters and TWI status debugging
//Comment out the define to turn off debug characters
//#define I2CDEBUG 

void set_array_value(void);
void print_array(void);
void ar1010_main(void);
uint16_t ar1010_read(uint8_t address);
void ar1010_write(char reg_address, uint16_t reg_value);
void ar1010_readall(void);
void ar1010_write_array(void);
void ar1010_init(void);
void ar1010_setvolume(uint8_t volume_level);
void ar1010_status(void);
void ar1010_tuneto(uint16_t freq_kHz);
void ar1010_tune_hilo(uint16_t freq_kHz);
void ar1010_seek(void);
uint16_t ar1010_rssi(void);

//Register conaining default values for the AR1010, these are the default values from the programming guide.
uint16_t register_values[18] = 
{	
	
	0xFFFF, //R0
	0x5B15, //R1
	0xF4B9, //R2 Tune/Channel
	0x8012, //R3 seekTHD = 18
	0x0400, //R4
	0x28AA, //R5
	0x4400, //R6
	0x1EE7,	// R7
	0x7141,	// R8
	0x007D,	// R9
	0x82C6,	// R10  disable wrap
	0x4F55,	// R11. <--- (disable xo_output)
	0x970C,	// R12.
	0xB845,	// R13
	0xFC2D,	// R14 : Volume control 2
	0x8097,	// R15
	0x04A1,	// R16
	0xDF6A	// R17
	
	//Bad values from the Airoha example code - they don't work for me
	/*0xFF7B,	// R0 -- the first writable register .  (disable xo_en)
	0x5B15,		// R1
	0xD0B9,		// R2 
	0xA010,		// R3  seekTHD = 16
	0x0780,		// R4
	0x28AB,		// R5
	0x6400,		// R6
	0x1EE7,		// R7
	0x7141,		// R8
	0x007D,		// R9
	0x82C6,		// R10  disable wrap
	0x4F55,		// R11. <--- (disable xo_output)
	0x970C,		// R12.
	0xB845,		// R13
	0xFC2D,		// R14 : Volume control 2
	0x8097,		// R15
	0x04A1,		// R16
	0xDF6A	*/  // R17
};

// volume control (increasing)
unsigned char AR1010vol[22] =
{ 
	0x0F,	// step 0
	0xCF,	// 1
	0xDF,	// 2
	0xEF,	// 3
	0xFF,	// 4
	0xEE,	// 5
	0xFE,	// 6
	0xED,	// 7
	0xFD,	// 8
	0xFB,	// 9
	0xFA,	// 10
	0xF9,	// 11
	0xF7,	// 12
	0xE6,	// 13
	0xF6,	// 14
	0xE5,	// 15
	0xF5,	// 16
	0xE3,	// 17
	0xF3,	// 18
	0xF2,	// 19
	0xF1,	// 20
	0xF0	// 21 <------ default setting
};

//-------------------------------------------------------------------------
int notmain(unsigned int earlypc) {
	uart_init();
	i2c1_init();

	ar1010_main();

	while(1);

	return(0);
}

//-------------------------------------------------------------------------
void ar1010_main(void)
{
	char option;
	char vol = 21;
	
	i2c1_setaddr(AR1010_AD);

	while(1)
	{
		uart_puts("\r\n\r\n----AR1010 Configuration----");
		uart_puts("\r\n1) Send array values");
		uart_puts("\r\n2) Read array values");
		uart_puts("\r\n3) Set array value");	
		uart_puts("\r\n4) Read All Register values");
		uart_puts("\r\n5) Begin Seek");		
		uart_puts("\r\n6) Tune to 97.3");		
		uart_puts("\r\n7) Check Status");		
		uart_puts("\r\n8) Raise volume");		
		uart_puts("\r\n9) Lower volume");		
		uart_puts("\r\na) Check RSSI");		
		uart_puts("\r\nb) Special tune hi/low to 97.3");		
		uart_puts("\r\n");		

		option = uart_getc();
		uart_putc(option);
		uart_puts("\r\n");		
		
		if(option == '1')
		{
			uart_puts("\r\n\r\n\tSending Array Values\r\n");
			ar1010_write_array();
			uart_puts("\r\n\t\tDone\r\n");
		}
		
		else if(option == '2')
		{
			print_array();
		}
		
		else if(option == '3')	
		{
			set_array_value();
		}
		
		else if(option == '4')
		{
			uart_puts("\r\n\r\n\tRead All\r\n");
			ar1010_readall();
		}
		
		else if(option == '5')
		{
			ar1010_seek();
		}
		
		else if(option == '6')
		{
			uart_puts("\r\n\t Tune to 97.3\r\n");
			ar1010_tuneto(973);
		}

		else if(option == '7')
		{
			ar1010_status();
		}

		else if(option == '8')
		{
			if(vol < 21) ar1010_setvolume(++vol);
			uart_puts("\r\nVolume = "); uart_puts_val(vol);
		}
		else if(option == '9')
		{
			if(vol > 0) ar1010_setvolume(--vol);
			uart_puts("\r\nVolume = "); uart_puts_val(vol);
		}
		else if(option == 'a')
		{
			option = ar1010_rssi();
			uart_puts("\r\nRSSI = "); uart_puts_val(option);
		}
		else if(option == 'b')
		{
			ar1010_tune_hilo(973);
		}
	}
}

uint16_t ar1010_rssi(void)
{
#define ADDR_RSSI	0x12 
#define MASK_RSSI 	0xFE00
#define SHIFT_RSSI  9

	uint16_t rssi;
	
	rssi = ar1010_read(ADDR_RSSI);
	rssi &= MASK_RSSI;
	rssi >>= 9;

	return(rssi);
}

// Volume Control
// There are two different fields about volume control in AR1010
//  Volume : D7~D10 in register R3
//  Volume2 : D12~D15 in register R14
//  22 combinations of (volume2 + volume) are recommended.
void ar1010_setvolume(uint8_t volume_level)
{
	uint16_t reg3, reg14;
	
	reg3 = ar1010_read(3) & 0xF87F; //Zero out bits D7-D10
	reg3 |= ( (AR1010vol[volume_level] & 0x0F) << 7); //Mask in D7-D10
	
	reg14 = ar1010_read(14) & 0x0FFF; //Zero out bits D12-D15
	reg14 |= ((AR1010vol[volume_level] & 0xF0) << 8); //Mask in D12-D15
	
	ar1010_write(3, reg3);
	ar1010_write(14, reg14);
}

//Tunes the AR1010 to a given station.
//Calculate AR1010 CHAN id  :  Freq (MHz) = 69 + 0.1*CHAN
//Example, sending 973 will tune to 97.3MHz
void ar1010_tuneto(uint16_t freq_kHz)
{
	uint16_t channel, temp;

	/*
	1) Set hmute Bit 
	2) Clear TUNE Bit 
	3) Clear SEEK Bit
	4) Set BAND/SPACE/CHAN Bits
	5) Enable TUNE Bit 
	6) Wait STC flag (Seek/Tune Comlete, in Status Register
	7) Clear hmute Bit 
	8) Update Functions (optional) 
	*/

	//Clear tune bit
	AR1010_TUNE_OFF;

	//Set Channel
	channel = freq_kHz - 690;
	temp = ar1010_read(2); //Read 
	temp &= 0xFE00; //Mask
	temp |= channel;
	ar1010_write(2, temp); //Write
	
	//Enable tune bit
	AR1010_TUNE_ON;
	
	//Wait for tune to stabilize (STC flag)
	temp = 0;
	while(temp == 0)
	{
		temp = ar1010_read(ADDR_STATUS) & MASK_STC;
		uart_puts("!");
	}
	
}

//This is some weird function in AR1000 example code provided by Airoha
//Looks like it takes the RSSI into account and then fine tunes the station
//I can't hear much of a difference, but it looks fancy.
void ar1010_tune_hilo(uint16_t freq_kHz)
{
	uint16_t temp;

	AR1010_MUTE_ON; //Set mute ON before TUNE
	AR1010_SEEK_OFF; //Clear seek

	//Read Low-Side LO Injection
	//R11 --> clear  D15,  clear D0/D2,  D3 is the same as default
	temp = ar1010_read(11) & 0x7FFA;
	ar1010_write(11, temp);

	//TUNE to FreqKHz with current setting
	ar1010_tuneto(freq_kHz); //This function turns on TUNE and waits for STC flag
	//Low-side TUNE Ends 

	uart_puts("\r\nLow complete");

	uint16_t status = ar1010_read(ADDR_RSSI);
	uint16_t rssi = (status & MASK_RSSI);

	uart_puts("\r\nRSSI 1 = "); uart_puts_val(rssi >> SHIFT_RSSI);

	//Read Hi-Side LO Injection
	// R11-->set D15, set D0/D2,  D3 is the same as default
	temp = ar1010_read(11) | 0x8005;
	ar1010_write(11, temp);
	
	//TUNE to FreqKHz with current setting
	ar1010_tuneto(freq_kHz); //This function turns on TUNE and waits for STC flag
	//High-side TUNE Ends 

	uart_puts("\r\nHigh complete");
	

	status = ar1010_read(ADDR_RSSI);
	uart_puts("\nRSSI 2 = "); uart_puts_val((status & MASK_RSSI) >> SHIFT_RSSI);
	rssi = rssi - (status & MASK_RSSI);	
	if (rssi < 0) //errata in 0.82
	{ 	
		// LO
		// R11--> clear D15, set D0/D2, D3 is the same as default
		temp = (ar1010_read(11) & 0x7FFF) | 0x0005;
		ar1010_write(11, temp);
	}
	else
	{ 
		//HI
		//R11-->  set D15, clear D0/D2, D3 is the same as default
		temp = (ar1010_read(11) | 0x8000) & 0xFFFA;
		ar1010_write(11, temp);
	}

	//Fine-tune!!
	//TUNE to FreqKHz with current setting
	ar1010_tuneto(freq_kHz); //This function turns on TUNE and waits for STC flag

	AR1010_MUTE_OFF;

	uart_puts("\r\nLow/Hi tuning complete");

}

//Starts scanning the stations for a minimum set threshold. I found the bit to enable
//wrapping so the every time the function is called, it searches up, and wrap back to 88MHz
//if it doesn't find a good station.
void ar1010_seek(void)
{
	#define ADDR_SEEK_SETTING	0x11
	#define SEEK_SETTING	0x2000
	#define SEEK_MASK		0xC3FF
	#define SEEK_TH_MASK	0xFF80
	#define SEEK_TH			5 //A higher threshold causes stronger stations to be found
	
	uint16_t temp;
	
	char space = 1; //0.1MHz scanning
	char updown = 1; //Seek up
	char band = 0; //US/Europe radio band
	
	AR1010_MUTE_ON;
	AR1010_TUNE_OFF;
	AR1010_SEEK_OFF;
	
	//Enable wrap during seek - I found bit D3 enables wrap, by trial and error. Seems to work
	temp = ar1010_read(10) | (1<<3); //0x82C6 = 1000 0010 1100 0110
	ar1010_write(10, temp);
	
	//Setting before seek
	temp = (ar1010_read(17) & SEEK_MASK) | SEEK_SETTING;
	ar1010_write(17, temp);

	uart_puts("\r\n\r\nBegin searching:");

	AR1010_SEEK_ON;
	temp = ar1010_read(3);
	if(space == 1)	temp |= (1<<13); //Set space
	if(updown == 1)	temp |= (1<<15); //Set seek up or down
	temp = (temp & 0xE7FF) | band; //Set Band
	temp &= SEEK_TH_MASK; //Clear out the seek threshold
	temp |= SEEK_TH; //Set threshold
	ar1010_write(3, temp);
	
	//Wait for tune to stabilize (STC flag)
	temp = 0;
	while(temp == 0)
	{
		temp = ar1010_read(ADDR_STATUS) & MASK_STC;
		uart_puts(".");
	}

	temp = ar1010_read(ADDR_STATUS) & MASK_SF;
	if(temp != 0)
	{
		uart_puts("\r\nSeek failed!");
		return;
	}
	uart_puts("\r\nSeek success!");
	
	temp = ar1010_read(ADDR_STATUS) & MASK_READCHAN;
	uint16_t freq_kHz = 690 + (temp >> SHIFT_READCHAN); //Determine what channel we found
	
	uart_puts("\r\nNow on channel ");  uart_puts_val(azo_uint32_div(freq_kHz, 10)); uart_puts("."); uart_puts_val(azo_uint32_mod(freq_kHz, 10)); uart_puts("MHz");

	//Restore setting after seek
	ar1010_write(17, register_values[17]);

	//Fine-tune with auto hilo rejection
	ar1010_tune_hilo(freq_kHz);

	AR1010_MUTE_OFF;     
}

//This function reads in an address and array value, and stores the value into the array address
//it does not change the values in the ar1010, you will need to use the "send array valus"
//function in order to send the new values to the ar1010	
void set_array_value(void)
{
	uint8_t array_address;
	uint16_t array_value;
	
	uart_puts("\r\n\r\nArray Address (2 digit hex value): ");
	array_address = uart_gethex(2);
	uart_puts("\r\nArray Address is: "); uart_puts_u16_hex(array_address);
    
   	uart_puts("\r\n\r\nArray value (4 digit hex value): ");
	array_value = uart_gethex(4);
    uart_puts("\r\nArray value is: "); uart_puts_u16_hex(array_value);
    
    register_values[array_address] = array_value;
}

void print_array(void)
{
	int i;
	//prints the values of the array that is used to control the radio
	
	uart_puts("\r\n\r\nArray position: value\r\n");

	for(i = 0 ; i < 18 ; i++)
	{
		uart_puts("0x"); uart_puts_u8_hex(i); uart_puts(": 0x"); uart_puts_u16_hex(register_values[i]); uart_puts_ln();
	}
}

//Reads a memory register from the AR1000
uint16_t ar1010_read(uint8_t address_to_read)
{
	uint8_t bytes[2];
	uint_t ack;
	
AGAIN:
	bytes[0] = address_to_read;	//Send address to read
	ack = i2c1_write(bytes, 1);
	if(ack != 0)
	{
		#ifdef I2CDEBUG
			uart_puts("!"); //No ACK!
		#endif
		goto AGAIN;
	}
	
	(void)i2c1_read(bytes, 2);
	
	//Combine two bytes into one 16-bit word
	int16_t temp = bytes[0] << 8;	
	temp |= bytes[1];
	
	return(temp);
}

void ar1010_write(char reg_address, uint16_t reg_value)
{
	uint8_t bytes[3];
	uint_t ack;

	bytes[0] = reg_address;
	bytes[1] = (reg_value & 0xFF00) >> 8;
	bytes[2] = (reg_value & 0x00FF);

AGAIN:
	ack = i2c1_write(bytes, 3);
	if(ack != 0)
	{
		#ifdef I2CDEBUG
			uart_puts("!"); //No ACK!
		#endif
		goto AGAIN;
	}
}

void ar1010_write_array(void)
{
	int i;
	// This code writes the array values to the ar1010, it is used to calibrate the ar1010
	// on power up and it can send the modified array values needed for the seeking tuning etc
	
	//The example AR1000 code disables the analog and digital blocks
	// then write to the 0x01 to 0x11 registers
	// then enables the analog and digital blocks - so that's what we will do as well

	//Write the first register
	ar1010_write(0, register_values[0] & 0xFFFE); //<--- Notice we force the enable bit to zero

	for(i = 1 ; i < 18 ; i++)
		ar1010_write(i, register_values[i]); //Write registers 1 to 17 to AR1010
	
	ar1010_write(0, register_values[0]); //Re-write the first register, this will set the enable bit
}

//Reads and prints all 16 registers (16-bits wide) of the AR1010
void ar1010_readall(void)
{
	uint16_t x, register_value;
	for(x = 0 ; x < 0x1D ; x++)
	{
		register_value = ar1010_read(x);
		uart_puts("0x"); uart_puts_u8_hex(x); uart_puts(": 0x"); uart_puts_u16_hex(register_value); uart_puts_ln();
	}
}

void ar1010_init(void)
{
	delay_ms(100); //Wait for power to stabilize
	
	ar1010_write_array(); //Init the AR1010 by writing the initial recommended values

	uint16_t status = 0;
	while(status == 0)
	{
		status = ar1010_read(ADDR_STATUS);
	
		uart_puts("\r\nAR1010 status : 0x"); uart_puts_u16_hex(status);

		status = status & MASK_STC;
	}
}

//Read the status register (0x13) of the AR1010
void ar1010_status(void)
{
	uint16_t status;
	
	status = ar1010_read(ADDR_STATUS);
	
	uart_puts("\r\n\r\nAR1010 status : 0x"); uart_puts_u16_hex(status);
	
	uint16_t channel = status & MASK_READCHAN;
	channel >>= SHIFT_READCHAN;
	channel += 690;
	uart_puts("\r\nChannel : "); uart_puts_val(azo_uint32_div(channel, 10)); uart_puts("."); uart_puts_val(azo_uint32_mod(channel, 10)); uart_puts("MHz");
	
	if(status & MASK_STC) 
		uart_puts(" (Seek/Tune Complete)");
	else
		uart_puts(" (Seek/Tune Incomplete)");
	
	if(status & MASK_SF) 
		uart_puts(" (Seek Fail)");
	else
		uart_puts(" (Seek Successful)");
		
	if(status & MASK_ST) 
		uart_puts(" (Stereo)");
	else
		uart_puts(" (Mono)");
}

//-------------------------------------------------------------------------

