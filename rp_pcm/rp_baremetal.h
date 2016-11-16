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

#ifndef RP_BAREMETAL_H
#define RP_BAREMETAL_H

//-------------------------------------------------------------------------
#if defined(AZO_CHIP_BCM2835)
#define BCM283X_PER_BASE	0x20000000
#elif defined(AZO_CHIP_BCM2836) || defined(AZO_CHIP_BCM2837)
#define BCM283X_PER_BASE	0x3F000000
#endif

/*
 *  GPIO
 */
#define BCM283X_GPIO_BASE (BCM283X_PER_BASE + 0x200000)
#define BCM283X_GPIO_GPFSEL1	(BCM283X_GPIO_BASE + 0x04)	/* GPIO Function Select 1 32bit R/W */
#define BCM283X_GPIO_GPFSEL2	(BCM283X_GPIO_BASE + 0x08)	/* GPIO Function Select 2 32bit R/W */
#define BCM283X_GPIO_GPFSEL4	(BCM283X_GPIO_BASE + 0x10)	/* GPIO Function Select 4 32bit R/W */
#define BCM283X_GPIO_GPPUD	(BCM283X_GPIO_BASE + 0x94)	/* GPIO Pin Pull-up/down Enable */
#define BCM283X_GPIO_GPPUDCLK0	(BCM283X_GPIO_BASE + 0x98)	/* GPIO Pin Pull-up/down Enable Clock 0 */
#define BCM283X_GPIO_GPPUDCLK1	(BCM283X_GPIO_BASE + 0x9C)	/* GPIO Pin Pull-up/down Enable Clock 1 */

/*
 *  Clock manager
 */
#define BCM283X_CM_BASE (BCM283X_PER_BASE + 0x101000)
#define BCM283X_CM_GNRICCTL	(BCM283X_CM_BASE + 0x000)	/* Clock Manager Generic Clock Control */
#define BCM283X_CM_GNRICDIV	(BCM283X_CM_BASE + 0x004)	/* Clock Manager Generic Clock Divisor */
#define BCM283X_CM_VPUCTL	(BCM283X_CM_BASE + 0x008)	/* Clock Manager VPU Clock Control */
#define BCM283X_CM_VPUDIV	(BCM283X_CM_BASE + 0x00C)	/* Clock Manager VPU Clock Divisor */
#define BCM283X_CM_SYSCTL	(BCM283X_CM_BASE + 0x010)	/* Clock Manager System Clock Control */
#define BCM283X_CM_SYSDIV	(BCM283X_CM_BASE + 0x014)	/* Clock Manager System Clock Divisor */
#define BCM283X_CM_PERIACTL	(BCM283X_CM_BASE + 0x018)	/* Clock Manager PERIA Clock Control */
#define BCM283X_CM_PERIADIV	(BCM283X_CM_BASE + 0x01C)	/* Clock Manager PERIA Clock Divisor */
#define BCM283X_CM_PERIICTL	(BCM283X_CM_BASE + 0x020)	/* Clock Manager PERII Clock Control */
#define BCM283X_CM_PERIIDIV	(BCM283X_CM_BASE + 0x024)	/* Clock Manager PERII Clock Divisor */
#define BCM283X_CM_H264CTL	(BCM283X_CM_BASE + 0x028)	/* Clock Manager H264 Clock Control */
#define BCM283X_CM_H264DIV	(BCM283X_CM_BASE + 0x02C)	/* Clock Manager H264 Clock Divisor */
#define BCM283X_CM_ISPCTL	(BCM283X_CM_BASE + 0x030)	/* Clock Manager ISP Clock Control */
#define BCM283X_CM_ISPDIV	(BCM283X_CM_BASE + 0x034)	/* Clock Manager ISP Clock Divisor */
#define BCM283X_CM_V3DCTL	(BCM283X_CM_BASE + 0x038)	/* Clock Manager V3D Clock Control */
#define BCM283X_CM_V3DDIV	(BCM283X_CM_BASE + 0x03C)	/* Clock Manager V3D Clock Divisor */
#define BCM283X_CM_CAM0CTL	(BCM283X_CM_BASE + 0x040)	/* Clock Manager Camera 0 Clock Control */
#define BCM283X_CM_CAM0DIV	(BCM283X_CM_BASE + 0x044)	/* Clock Manager Camera 0 Clock Divisor */
#define BCM283X_CM_CAM1CTL	(BCM283X_CM_BASE + 0x048)	/* Clock Manager Camera 1 Clock Control */
#define BCM283X_CM_CAM1DIV	(BCM283X_CM_BASE + 0x04C)	/* Clock Manager Camera 1 Clock Divisor */
#define BCM283X_CM_CCP2CTL	(BCM283X_CM_BASE + 0x050)	/* Clock Manager CCP2 Clock Control */
#define BCM283X_CM_CCP2DIV	(BCM283X_CM_BASE + 0x054)	/* Clock Manager CCP2 Clock Divisor */
#define BCM283X_CM_DSI0ECTL	(BCM283X_CM_BASE + 0x058)	/* Clock Manager DSI0E Clock Control */
#define BCM283X_CM_DSI0EDIV	(BCM283X_CM_BASE + 0x05C)	/* Clock Manager DSI0E Clock Divisor */
#define BCM283X_CM_DSI0PCTL	(BCM283X_CM_BASE + 0x060)	/* Clock Manager DSI0P Clock Control */
#define BCM283X_CM_DSI0PDIV	(BCM283X_CM_BASE + 0x064)	/* Clock Manager DSI0P Clock Divisor */
#define BCM283X_CM_DPICTL	(BCM283X_CM_BASE + 0x068)	/* Clock Manager DPI Clock Control */
#define BCM283X_CM_DPIDIV	(BCM283X_CM_BASE + 0x06C)	/* Clock Manager DPI Clock Divisor */
#define BCM283X_CM_GP0CTL	(BCM283X_CM_BASE + 0x070)	/* Clock Manager General Purpose 0 Clock Control */
#define BCM283X_CM_GP0DIV	(BCM283X_CM_BASE + 0x074)	/* Clock Manager General Purpose 0 Clock Divisor */
#define BCM283X_CM_GP1CTL	(BCM283X_CM_BASE + 0x078)	/* Clock Manager General Purpose 1 Clock Control */
#define BCM283X_CM_GP1DIV	(BCM283X_CM_BASE + 0x07C)	/* Clock Manager General Purpose 1 Clock Divisor */
#define BCM283X_CM_GP2CTL	(BCM283X_CM_BASE + 0x080)	/* Clock Manager General Purpose 2 Clock Control */
#define BCM283X_CM_GP2DIV	(BCM283X_CM_BASE + 0x084)	/* Clock Manager General Purpose 2 Clock Divisor */
#define BCM283X_CM_HSMCTL	(BCM283X_CM_BASE + 0x088)	/* Clock Manager HSM Clock Control */
#define BCM283X_CM_HSMDIV	(BCM283X_CM_BASE + 0x08C)	/* Clock Manager HSM Clock Divisor */
#define BCM283X_CM_OTPCTL	(BCM283X_CM_BASE + 0x090)	/* Clock Manager OTP Clock Control */
#define BCM283X_CM_OTPDIV	(BCM283X_CM_BASE + 0x094)	/* Clock Manager OTP Clock Divisor */
#define BCM283X_CM_PCMCTL	(BCM283X_CM_BASE + 0x098)	/* Clock Manager PCM / I2S Clock Control */
#define BCM283X_CM_PCMDIV	(BCM283X_CM_BASE + 0x09C)	/* Clock Manager PCM / I2S Clock Divisor */
#define BCM283X_CM_PWMCTL	(BCM283X_CM_BASE + 0x0A0)	/* Clock Manager PWM Clock Control */
#define BCM283X_CM_PWMDIV	(BCM283X_CM_BASE + 0x0A4)	/* Clock Manager PWM Clock Divisor */
#define BCM283X_CM_SLIMCTL	(BCM283X_CM_BASE + 0x0A8)	/* Clock Manager SLIM Clock Control */
#define BCM283X_CM_SLIMDIV	(BCM283X_CM_BASE + 0x0AC)	/* Clock Manager SLIM Clock Divisor */
#define BCM283X_CM_SMICTL	(BCM283X_CM_BASE + 0x0B0)	/* Clock Manager SMI Clock Control */
#define BCM283X_CM_SMIDIV	(BCM283X_CM_BASE + 0x0B4)	/* Clock Manager SMI Clock Divisor */
#define BCM283X_CM_TCNTCTL	(BCM283X_CM_BASE + 0x0C0)	/* Clock Manager TCNT Clock Control */
#define BCM283X_CM_TCNTDIV	(BCM283X_CM_BASE + 0x0C4)	/* Clock Manager TCNT Clock Divisor */
#define BCM283X_CM_TECCTL	(BCM283X_CM_BASE + 0x0C8)	/* Clock Manager TEC Clock Control */
#define BCM283X_CM_TECDIV	(BCM283X_CM_BASE + 0x0CC)	/* Clock Manager TEC Clock Divisor */
#define BCM283X_CM_TD0CTL	(BCM283X_CM_BASE + 0x0D0)	/* Clock Manager TD0 Clock Control */
#define BCM283X_CM_TD0DIV	(BCM283X_CM_BASE + 0x0D4)	/* Clock Manager TD0 Clock Divisor */
#define BCM283X_CM_TD1CTL	(BCM283X_CM_BASE + 0x0D8)	/* Clock Manager TD1 Clock Control */
#define BCM283X_CM_TD1DIV	(BCM283X_CM_BASE + 0x0DC)	/* Clock Manager TD1 Clock Divisor */
#define BCM283X_CM_TSENSCTL	(BCM283X_CM_BASE + 0x0E0)	/* Clock Manager TSENS Clock Control */
#define BCM283X_CM_TSENSDIV	(BCM283X_CM_BASE + 0x0E4)	/* Clock Manager TSENS Clock Divisor */
#define BCM283X_CM_TIMERCTL	(BCM283X_CM_BASE + 0x0E8)	/* Clock Manager Timer Clock Control */
#define BCM283X_CM_TIMERDIV	(BCM283X_CM_BASE + 0x0EC)	/* Clock Manager Timer Clock Divisor */
#define BCM283X_CM_UARTCTL	(BCM283X_CM_BASE + 0x0F0)	/* Clock Manager UART Clock Control */
#define BCM283X_CM_UARTDIV	(BCM283X_CM_BASE + 0x0F4)	/* Clock Manager UART Clock Divisor */
#define BCM283X_CM_VECCTL	(BCM283X_CM_BASE + 0x0F8)	/* Clock Manager VEC Clock Control */
#define BCM283X_CM_VECDIV	(BCM283X_CM_BASE + 0x0FC)	/* Clock Manager VEC Clock Divisor */
#define BCM283X_CM_OSCCOUNT	(BCM283X_CM_BASE + 0x100)	/* Clock Manager Oscillator Count */
#define BCM283X_CM_PLLA		(BCM283X_CM_BASE + 0x104)	/* Clock Manager PLLA */
#define BCM283X_CM_PLLC		(BCM283X_CM_BASE + 0x108)	/* Clock Manager PLLC */
#define BCM283X_CM_PLLD		(BCM283X_CM_BASE + 0x10C)	/* Clock Manager PLLD */
#define BCM283X_CM_PLLH		(BCM283X_CM_BASE + 0x110)	/* Clock Manager PLLH */
#define BCM283X_CM_LOCK		(BCM283X_CM_BASE + 0x114)	/* Clock Manager Lock */
#define BCM283X_CM_EVENT	(BCM283X_CM_BASE + 0x118)	/* Clock Manager Event */
#define BCM283X_CM_INTEN	(BCM283X_CM_BASE + 0x118)	/* Clock Manager INTEN */
#define BCM283X_CM_DSI0HSCK	(BCM283X_CM_BASE + 0x120)	/* Clock Manager DSI0HSCK */
#define BCM283X_CM_CKSM		(BCM283X_CM_BASE + 0x124)	/* Clock Manager CKSM */
#define BCM283X_CM_OSCFREQI	(BCM283X_CM_BASE + 0x128)	/* Clock Manager Oscillator Frequency Integer */
#define BCM283X_CM_OSCFREQF	(BCM283X_CM_BASE + 0x12C)	/* Clock Manager Oscillator Frequency Fraction */
#define BCM283X_CM_PLLTCTL	(BCM283X_CM_BASE + 0x130)	/* Clock Manager PLLT Control */
#define BCM283X_CM_PLLTCNT0	(BCM283X_CM_BASE + 0x134)	/* Clock Manager PLLT0 */
#define BCM283X_CM_PLLTCNT1	(BCM283X_CM_BASE + 0x138)	/* Clock Manager PLLT1 */
#define BCM283X_CM_PLLTCNT2	(BCM283X_CM_BASE + 0x13C)	/* Clock Manager PLLT2 */
#define BCM283X_CM_PLLTCNT3	(BCM283X_CM_BASE + 0x140)	/* Clock Manager PLLT3 */
#define BCM283X_CM_TDCLKEN	(BCM283X_CM_BASE + 0x144)	/* Clock Manager TD Clock Enable */
#define BCM283X_CM_BURSTCTL	(BCM283X_CM_BASE + 0x148)	/* Clock Manager Burst Control */
#define BCM283X_CM_BURSTCNT	(BCM283X_CM_BASE + 0x14C)	/* Clock Manager Burst */
#define BCM283X_CM_DSI1ECTL	(BCM283X_CM_BASE + 0x158)	/* Clock Manager DSI1E Clock Control */
#define BCM283X_CM_DSI1EDIV	(BCM283X_CM_BASE + 0x15C)	/* Clock Manager DSI1E Clock Divisor */
#define BCM283X_CM_DSI1PCTL	(BCM283X_CM_BASE + 0x160)	/* Clock Manager DSI1P Clock Control */
#define BCM283X_CM_DSI1PDIV	(BCM283X_CM_BASE + 0x164)	/* Clock Manager DSI1P Clock Divisor */
#define BCM283X_CM_DFTCTL	(BCM283X_CM_BASE + 0x168)	/* Clock Manager DFT Clock Control */
#define BCM283X_CM_DFTDIV	(BCM283X_CM_BASE + 0x16C)	/* Clock Manager DFT Clock Divisor */
#define BCM283X_CM_PLLB		(BCM283X_CM_BASE + 0x170)	/* Clock Manager PLLB */
#define BCM283X_CM_PULSECTL	(BCM283X_CM_BASE + 0x190)	/* Clock Manager Pulse Clock Control */
#define BCM283X_CM_PULSEDIV	(BCM283X_CM_BASE + 0x194)	/* Clock Manager Pulse Clock Divisor */
#define BCM283X_CM_SDCCTL	(BCM283X_CM_BASE + 0x1A8)	/* Clock Manager SDC Clock Control */
#define BCM283X_CM_SDCDIV	(BCM283X_CM_BASE + 0x1AC)	/* Clock Manager SDC Clock Divisor */
#define BCM283X_CM_ARMCTL	(BCM283X_CM_BASE + 0x1B0)	/* Clock Manager ARM Clock Control */
#define BCM283X_CM_ARMDIV	(BCM283X_CM_BASE + 0x1B4)	/* Clock Manager ARM Clock Divisor */
#define BCM283X_CM_AVEOCTL	(BCM283X_CM_BASE + 0x1B8)	/* Clock Manager AVEO Clock Control */
#define BCM283X_CM_AVEODIV	(BCM283X_CM_BASE + 0x1BC)	/* Clock Manager AVEO Clock Divisor */
#define BCM283X_CM_EMMCCTL	(BCM283X_CM_BASE + 0x1C0)	/* Clock Manager EMMC Clock Control */
#define BCM283X_CM_EMMCDIV	(BCM283X_CM_BASE + 0x1C4)	/* Clock Manager EMMC Clock Divisor */

#define BCM283X_CM_PASSWORD	0x5A000000

/*
 *  PCM/I2S
 */
#define BCM283X_PCM_BASE (BCM283X_PER_BASE + 0x203000) 
#define BCM283X_PCM_CS_A	(BCM283X_PCM_BASE + 0x00)	/* PCM Control and Status */
#define BCM283X_PCM_FIFO_A	(BCM283X_PCM_BASE + 0x04)	/* PCM FIFO Data */
#define BCM283X_PCM_MODE_A	(BCM283X_PCM_BASE + 0x08)	/* PCM Mode */
#define BCM283X_PCM_RXC_A	(BCM283X_PCM_BASE + 0x0C)	/* PCM Receive Configuration */
#define BCM283X_PCM_TXC_A	(BCM283X_PCM_BASE + 0x10)	/* PCM Transmit Configuration */
#define BCM283X_PCM_DREQ_A	(BCM283X_PCM_BASE + 0x14)	/* PCM DMA Request Level */
#define BCM283X_PCM_INTEN_A	(BCM283X_PCM_BASE + 0x18)	/* PCM Interrupt Enables */
#define BCM283X_PCM_INTSTC_A	(BCM283X_PCM_BASE + 0x1C)	/* PCM Interrupt Status & Clear */
#define BCM283X_PCM_GRAY	(BCM283X_PCM_BASE + 0x20)	/* PCM Gray Mode Control */
//-------------------------------------------------------------------------

#ifndef AZO_MACRO_ONLY

//-------------------------------------------------------------------------
typedef unsigned char	 uint8_t;
typedef unsigned short	uint16_t;
typedef unsigned int	uint32_t;
typedef unsigned int	intptr_t;

extern void dummy(void);
extern void arm_enable_irq (void);
extern void arm_disable_irq (void);
extern uint32_t arm_get_coreid(void);
extern void arm_acquire_lock(void*);
extern void arm_release_lock(void*);

//-------------------------------------------------------------------------
inline unsigned int GET32(const unsigned int addr) {
	return *(volatile unsigned int*)addr;
}

//-------------------------------------------------------------------------
inline void PUT32(const unsigned int addr, const unsigned int val) {
	*(volatile unsigned int*)addr = val;
}

//-------------------------------------------------------------------------

#endif	/* AZO_MACRO_ONLY */
#endif	/* RP_BAREMETAL_H */

