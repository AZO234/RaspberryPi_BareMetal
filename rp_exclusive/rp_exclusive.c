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

extern void core0stop(void);
extern void core1stop(void);
extern void core2stop(void);
extern void core3stop(void);

//-------------------------------------------------------------------------
void c_irq_handler(void)
{
}

//-------------------------------------------------------------------------
static volatile __attribute__ ((aligned (0x4000))) uint32_t page_table[4096];
static volatile __attribute__ ((aligned (0x20))) uint32_t my_lock;

volatile unsigned int corestopper0 = 0;
volatile unsigned int corestopper1 = 0;
volatile unsigned int corestopper2 = 0;
volatile unsigned int corestopper3 = 0;

volatile unsigned int coreready1 = 0;
volatile unsigned int coreready2 = 0;
volatile unsigned int coreready3 = 0;

//-------------------------------------------------------------------------
void azo_put_char(char c) {
	while(1) {
		if((GET32(BCM283X_AUX_MU_LSR_REG) & 0x20) != 0) break;
	}
	PUT32(BCM283X_AUX_MU_IO_REG,c);
}

//-------------------------------------------------------------------------
void azo_put_str(char* str) {
	int i = 0;
	while(str[i] != '\0') {
		azo_put_char(str[i]);
		i++;
	}
}

//-------------------------------------------------------------------------
void azo_put_val(unsigned int i) {
	int j;
	char str[11];
	char c;
	for(j = 0; j < 8; j++) {
		c = ((i >> (28 - 4 * j)) & 0xF);
		if(c <= 9) {
			str[j] = c + '0';
		} else {
			str[j] = c + 'A' - 10;
		}
	}
	str[8] = '\r';
	str[9] = '\n';
	str[10] = '\0';
	azo_put_str(str);
}

//-------------------------------------------------------------------------
void azo_put_addr_val(unsigned int addr) {
	azo_put_val(GET32(addr));
}

//------------------------------------------------------------------------
void azo_init_tlb(volatile uint32_t o_pTLB[]) {
	uint32_t base = 0;

	/* initialize page_table */
	/* 1008MB(1024MB - 16MB) of kernel memory */
	for(; base < 1008; base++) {
		/*
		section descriptor (1 MB)

		31:20 : PA[31:20]	Page Address
		   19 : NS		Non-secure
		   18 : 0
		   17 : nG		Not Global
		   16 : S		Shareable
		   15 : AP[2]	Access Permissions
		14:12 : TEX[2:0]	Memory region attribute
		11:10 : AP[1:0]	Access Permissions
		    9 : IMP		Implementation defined
		 8: 5 : Domain	Domain field
		    4 : XN		Execute-never
		    3 : C		Memory region attribute
		    2 : B		Memory region attribute
		    1 : 1
		    0 : 0

		TEX C B 
		000 0 0 : Strongly-ordered, (MemoryType)Strongly-ordered, Shareable
		000 0 1 : Shareable Device, (MT)Device, Shareable
		000 1 0 : Outer and Inner Write-Through, no Write-Allocate, (MT)Normal, S bit
		000 1 1 : Outer and Inner Write-Back, no Write-Allocate, (MT)Normal, S bit
		001 0 0 : Outer and Inner Non-cacheable, (MT)Normal, S bit
		001 0 1 : Reserved
		001 1 x : Outer and Inner Write-Back, Write-Allocate, (MT)Normal, S bit
		010 0 0 : Non-shareable Device, (MT)Device, Non-shareable
		010 0 1 : Reserved
		010 1 x : Reserved
		1BB A A : Cacheability, AA=Inner attribute BB=Outer attribute (MT)Normal, S bit

		Non-secure, outer and inner write back, write allocate, shareable
		o_pTLB[base] = (base << 20) | 0x91C0E;
		Non-secure, outer and inner write through, no write allocate, shareable
		o_pTLB[base] = (base << 20) | 0x90C0A;
		Secure, outer and inner write back, write allocate, shareable
		o_pTLB[base] = (base << 20) | 0x11C0E;
		Secure, outer and inner write through, no write allocate, shareable
		o_pTLB[base] = (base << 20) | 0x10C0A;
		outer and inner write through, no write allocate, shareable
		o_pTLB[base] = (base << 20) | 0x1040A;
		*/
//		o_pTLB[base] = (base << 20) | 0x01C0A;
		/* outer and inner write through, no write allocate, not share */
		o_pTLB[base] = (base << 20) | 0x00C0A;
	}

	/* 16MB+1MB peripherals at 0x3F000000 - 0x400FFFFF */
	for(; base < 1025; base++) {
		// shared device, never execute
		o_pTLB[base] = (base << 20) | 0x10416;
	}

	/* unused to 0xFFFFFFFF */
	for(; base < 4096; base++) {
		o_pTLB[base] = 0;
	}
}

//------------------------------------------------------------------------
void uart_init(void)
{
	unsigned int ra;

	ra = GET32(BCM283X_GPIO_GPFSEL1);
	ra &= ~(7 << 12);	/* GPIO14 */
	ra |=   2 << 12 ;	/* ALT5 */
	ra &= ~(7 << 15);	/* GPIO15 */
	ra |=   2 << 15 ;	/* ALT5 */
	PUT32(BCM283X_GPIO_GPFSEL1, ra);

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
int notmain(unsigned int earlypc)
{
	switch(arm_get_coreid()) {
	case 0:
		arm_mmu_reset();
		arm_release_lock(&my_lock);
		uart_init();
		azo_init_tlb(page_table);
		arm_mmu_init((volatile uint32_t*)&page_table);
		arm_bp_enable();
		arm_icache_enable();
		arm_dcache_enable();

		azo_put_val(0x12345678);

		corestopper1 = 0x12345678;
		corestopper2 = 0x12345678;
		corestopper3 = 0x12345678;
		arm_data_memorybarrier();
		core0stop();

		while(1) {
			arm_acquire_lock(&my_lock);
			azo_put_char(arm_get_coreid() + '0');
			arm_release_lock(&my_lock);
		}
		break;
	case 1:
		core1stop();

		arm_mmu_reset();
		arm_mmu_init((volatile uint32_t*)&page_table);
		arm_bp_enable();
		arm_icache_enable();
		arm_dcache_enable();

		coreready1 = 0x12345678;
		arm_data_memorybarrier();
		while((coreready1 != 0x12345678) || (coreready2 != 0x12345678) || (coreready3 != 0x12345678)) {
			arm_data_memorybarrier();
		}
		corestopper0 = 0x12345678;
		arm_data_memorybarrier();

		while(1) {
			arm_acquire_lock(&my_lock);
			azo_put_char(arm_get_coreid() + '0');
			arm_release_lock(&my_lock);
		}
		break;
	case 2:
		core2stop();

		arm_mmu_reset();
		arm_mmu_init((volatile uint32_t*)&page_table);
		arm_bp_enable();
		arm_icache_enable();
		arm_dcache_enable();

		coreready2 = 0x12345678;
		arm_data_memorybarrier();

		while(1) {
			arm_acquire_lock(&my_lock);
			azo_put_char(arm_get_coreid() + '0');
			arm_release_lock(&my_lock);
		}
		break;
	case 3:
		core3stop();

		arm_mmu_reset();
		arm_mmu_init((volatile uint32_t*)&page_table);
		arm_bp_enable();
		arm_icache_enable();
		arm_dcache_enable();

		coreready3 = 0x12345678;
		arm_data_memorybarrier();

		while(1) {
			arm_acquire_lock(&my_lock);
			azo_put_char(arm_get_coreid() + '0');
			arm_release_lock(&my_lock);
		}
		break;
	}

	while(1);

	return(0);
}

//-------------------------------------------------------------------------

