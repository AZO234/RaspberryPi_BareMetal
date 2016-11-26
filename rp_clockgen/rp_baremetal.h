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
#define BCM283X_GPIO_GPFSEL0	(BCM283X_GPIO_BASE + 0x00)	/* GPIO Function Select 0 32bit R/W */
#define BCM283X_GPIO_GPFSEL1	(BCM283X_GPIO_BASE + 0x04)	/* GPIO Function Select 1 32bit R/W */
#define BCM283X_GPIO_GPFSEL2	(BCM283X_GPIO_BASE + 0x08)	/* GPIO Function Select 2 32bit R/W */
#define BCM283X_GPIO_GPFSEL4	(BCM283X_GPIO_BASE + 0x10)	/* GPIO Function Select 4 32bit R/W */
#define BCM283X_GPIO_GPSET0	(BCM283X_GPIO_BASE + 0x1C)
#define BCM283X_GPIO_GPCLR0	(BCM283X_GPIO_BASE + 0x28)
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
 *  Interrupt Controler
 */
#define BCM283X_IRQ_BASE (BCM283X_PER_BASE + 0xB200)
#define BCM283X_IRQ_BASIC		(BCM283X_IRQ_BASE + 0x00)
#define BCM283X_IRQ_PEND1		(BCM283X_IRQ_BASE + 0x04)
#define BCM283X_IRQ_PEND2		(BCM283X_IRQ_BASE + 0x08)
#define BCM283X_IRQ_FIQ_CONTROL		(BCM283X_IRQ_BASE + 0x0C)
#define BCM283X_IRQ_ENABLE1		(BCM283X_IRQ_BASE + 0x10)
#define BCM283X_IRQ_ENABLE2		(BCM283X_IRQ_BASE + 0x14)
#define BCM283X_IRQ_ENABLE_BASIC	(BCM283X_IRQ_BASE + 0x18)
#define BCM283X_IRQ_DISABLE1		(BCM283X_IRQ_BASE + 0x1C)
#define BCM283X_IRQ_DISABLE2		(BCM283X_IRQ_BASE + 0x20)
#define BCM283X_IRQ_DISABLE_BASIC	(BCM283X_IRQ_BASE + 0x24)

/*
 *  DMA
 */
#define BCM283X_DMA_BASE (BCM283X_PER_BASE + 0x7000)
#define BCM283X_DMA0_BASE	(BCM283X_DMA_BASE + 0x000)	/* DMA Channel 0 Register Set */
#define BCM283X_DMA1_BASE	(BCM283X_DMA_BASE + 0x100)	/* DMA Channel 1 Register Set */
#define BCM283X_DMA2_BASE	(BCM283X_DMA_BASE + 0x200)	/* DMA Channel 2 Register Set */
#define BCM283X_DMA3_BASE	(BCM283X_DMA_BASE + 0x300)	/* DMA Channel 3 Register Set */
#define BCM283X_DMA4_BASE	(BCM283X_DMA_BASE + 0x400)	/* DMA Channel 4 Register Set */
#define BCM283X_DMA5_BASE	(BCM283X_DMA_BASE + 0x500)	/* DMA Channel 5 Register Set */
#define BCM283X_DMA6_BASE	(BCM283X_DMA_BASE + 0x600)	/* DMA Channel 6 Register Set */
#define BCM283X_DMA7_BASE	(BCM283X_DMA_BASE + 0x700)	/* DMA Channel 7 Register Set */
#define BCM283X_DMA8_BASE	(BCM283X_DMA_BASE + 0x800)	/* DMA Channel 8 Register Set */
#define BCM283X_DMA9_BASE	(BCM283X_DMA_BASE + 0x900)	/* DMA Channel 9 Register Set */
#define BCM283X_DMA10_BASE	(BCM283X_DMA_BASE + 0xA00)	/* DMA Channel 10 Register Set */
#define BCM283X_DMA11_BASE	(BCM283X_DMA_BASE + 0xB00)	/* DMA Channel 11 Register Set */
#define BCM283X_DMA12_BASE	(BCM283X_DMA_BASE + 0xC00)	/* DMA Channel 12 Register Set */
#define BCM283X_DMA13_BASE	(BCM283X_DMA_BASE + 0xD00)	/* DMA Channel 13 Register Set */
#define BCM283X_DMA14_BASE	(BCM283X_DMA_BASE + 0xE00)	/* DMA Channel 14 Register Set */
#define BCM283X_DMA_INT_STATUS	(BCM283X_DMA_BASE + 0xFE0)	/* Interrupt Status of each DMA Channel */
#define BCM283X_DMA_ENABLE	(BCM283X_DMA_BASE + 0xFF0)	/* Global Enable bits for each DMA Channel */
#define BCM283X_DMA15_BASE (BCM283X_PER_BASE + 0xE05000)	/* DMA Channel 15 Register Set */

#define BCM283X_DMA_CS		0x00	/* DMA Channel 0..14 Control & Status */
#define BCM283X_DMA_CONBLK_AD	0x04	/* DMA Channel 0..14 Control Block Address */
#define BCM283X_DMA_TI		0x08	/* DMA Channel 0..14 CB Word 0 (Transfer Information) */
#define BCM283X_DMA_SOURCE_AD	0x0C	/* DMA Channel 0..14 CB Word 1 (Source Address) */
#define BCM283X_DMA_DEST_AD	0x10	/* DMA Channel 0..14 CB Word 2 (Destination Address) */
#define BCM283X_DMA_TXFR_LEN	0x14	/* DMA Channel 0..14 CB Word 3 (Transfer Length) */
#define BCM283X_DMA_STRIDE	0x18	/* DMA Channel 0..14 CB Word 4 (2D Stride) */
#define BCM283X_DMA_NEXTCONBK	0x1C	/* DMA Channel 0..14 CB Word 5 (Next CB Address) */
#define BCM283X_DMA_DEBUG	0x20	/* DMA Channel 0..14 Debug */

/*
 *  Random Number Generator(RNG)
 */
#define BCM283X_RNG_BASE	(BCM283X_PER_BASE + 0x104000)
#define BCM283X_RNG_CTRL	(BCM283X_RNG_BASE + 0x0)
#define BCM283X_RNG_STATUS	(BCM283X_RNG_BASE + 0x4)
#define BCM283X_RNG_DATA	(BCM283X_RNG_BASE + 0x8)

/*
 *  Mailbox
 */
#define BCM283X_MAIL_BASE	(BCM283X_PER_BASE + 0xB880)	/* Mailbox Base Address */
#define BCM283X_MAIL_READ	(BCM283X_MAIL_BASE + 0x00)	/* Mailbox Read Register */
#define BCM283X_MAIL_STATUS	(BCM283X_MAIL_BASE + 0x18)	/* Mailbox Status Register */
#define BCM283X_MAIL_CONFIG	(BCM283X_MAIL_BASE + 0x1C)	/* Mailbox Config Register */
#define BCM283X_MAIL_WRITE	(BCM283X_MAIL_BASE + 0x20)	/* Mailbox Write Register */

#define BCM283X_MAIL_EMPTY	0x40000000	/* Mailbox Status Register: Mailbox Empty (There is nothing to read from the Mailbox) */
#define BCM283X_MAIL_FULL	0x80000000	/* Mailbox Status Register: Mailbox Full  (There is no space to write into the Mailbox) */

#define BCM283X_MAIL_POWER	0x0	/* Mailbox Channel 0: Power Management Interface */
#define BCM283X_MAIL_FB		0x1	/* Mailbox Channel 1: Frame Buffer */
#define BCM283X_MAIL_VUART	0x2	/* Mailbox Channel 2: Virtual UART */
#define BCM283X_MAIL_VCHIQ	0x3	/* Mailbox Channel 3: VCHIQ Interface */
#define BCM283X_MAIL_LEDS	0x4	/* Mailbox Channel 4: LEDs Interface */
#define BCM283X_MAIL_BUTTONS	0x5	/* Mailbox Channel 5: Buttons Interface */
#define BCM283X_MAIL_TOUCH	0x6	/* Mailbox Channel 6: Touchscreen Interface */
#define BCM283X_MAIL_COUNT	0x7	/* Mailbox Channel 7: Counter */
#define BCM283X_MAIL_TAGS	0x8	/* Mailbox Channel 8: Tags (ARM to VC) */

#define BCM283X_TAGS_GET_FIRMWARE_REVISION	0x00000001	/* VideoCore: Get Firmware Revision (Response: Firmware Revision) */
#define BCM283X_TAGS_GET_BOARD_MODEL		0x00010001	/* Hardware: Get Board Model (Response: Board Model) */
#define BCM283X_TAGS_GET_BOARD_REVISION		0x00010002	/* Hardware: Get Board Revision (Response: Board Revision) */
#define BCM283X_TAGS_GET_BOARD_MAC_ADDRESS	0x00010003	/* Hardware: Get Board MAC Address (Response: MAC Address In Network Byte Order) */
#define BCM283X_TAGS_GET_BOARD_SERIAL		0x00010004	/* Hardware: Get Board Serial (Response: Board Serial) */
#define BCM283X_TAGS_GET_ARM_MEMORY		0x00010005	/* Hardware: Get ARM Memory (Response: Base Address In Bytes, Size In Bytes) */
#define BCM283X_TAGS_GET_VC_MEMORY		0x00010006	/* Hardware: Get VC Memory (Response: Base Address In Bytes, Size In Bytes) */
#define BCM283X_TAGS_GET_CLOCKS			0x00010007	/* Hardware: Get Clocks (Response: Parent Clock ID (0 For A Root Clock), Clock ID) */
#define BCM283X_TAGS_GET_POWER_STATE		0x00020001	/* Power: Get Power State (Response: Device ID, State) */
#define BCM283X_TAGS_GET_TIMING			0x00020002	/* Power: Get Timing (Response: Device ID, Enable Wait Time In Microseconds) */
#define BCM283X_TAGS_SET_POWER_STATE		0x00028001	/* Power: Set Power State (Response: Device ID, State) */
#define BCM283X_TAGS_GET_CLOCK_STATE		0x00030001	/* Clocks: Get Clock State (Response: Clock ID, State) */
#define BCM283X_TAGS_GET_CLOCK_RATE		0x00030002	/* Clocks: Get Clock Rate (Response: Clock ID, Rate In Hz) */
#define BCM283X_TAGS_GET_VOLTAGE		0x00030003	/* Voltage: Get Voltage (Response: Voltage ID, Value Offset From 1.2V In Units Of 0.025V) */
#define BCM283X_TAGS_GET_MAX_CLOCK_RATE		0x00030004	/* Clocks: Get Max Clock Rate (Response: Clock ID, Rate In Hz) */
#define BCM283X_TAGS_GET_MAX_VOLTAGE		0x00030005	/* Voltage: Get Max Voltage (Response: Voltage ID, Value Offset From 1.2V In Units Of 0.025V) */
#define BCM283X_TAGS_GET_TEMPERATURE		0x00030006	/* Voltage: Get Temperature (Response: Temperature ID, Value In Degrees C) */
#define BCM283X_TAGS_GET_MIN_CLOCK_RATE		0x00030007	/* Clocks: Get Min Clock Rate (Response: Clock ID, Rate In Hz) */
#define BCM283X_TAGS_GET_MIN_VOKTAGE		0x00030008	/* Voltage: Get Min Voltage (Response: Voltage ID, Value Offset From 1.2V In Units Of 0.025V) */
#define BCM283X_TAGS_GET_TURBO			0x00030009	/* Clocks: Get Turbo (Response: ID, Level) */
#define BCM283X_TAGS_GET_MAX_TEMPERATURE	0x0003000A	/* Voltage: Get Max Temperature (Response: Temperature ID, Value In Degrees C) */
#define BCM283X_TAGS_ALLOCATE_MEMORY		0x0003000C	/* Memory: Allocates Contiguous Memory On The GPU (Response: Handle) */
#define BCM283X_TAGS_LOCK_MEMORY		0x0003000D	/* Memory: Lock Buffer In Place, And Return A Bus Address (Response: Bus Address) */
#define BCM283X_TAGS_UNLOCK_MEMORY		0x0003000E	/* Memory: Unlock Buffer (Response: Status) */
#define BCM283X_TAGS_RELEASE_MEMORY		0x0003000F	/* Memory: Free The Memory Buffer (Response: Status) */
#define BCM283X_TAGS_EXECUTE_CODE		0x00030010	/* Memory: Calls The Function At Given (Bus) Address And With Arguments Given */
#define BCM283X_TAGS_EXECUTE_QPU		0x00030011	/* QPU: Calls The QPU Function At Given (Bus) Address And With Arguments Given (Response: Number Of QPUs, Control, No Flush, Timeout In ms) */
#define BCM283X_TAGS_ENABLE_QPU			0x00030012	/* QPU: Enables The QPU (Response: Enable State) */
#define BCM283X_TAGS_GET_EDID_BLOCK		0x00030020	/* HDMI: Read Specificed EDID Block From Attached HDMI/DVI Device (Response: Block Number, Status, EDID Block (128 Bytes)) */
#define BCM283X_TAGS_SET_CLOCK_STATE		0x00038001	/* Clocks: Set Clock State (Response: Clock ID, State) */
#define BCM283X_TAGS_SET_CLOCK_RATE		0x00038002	/* Clocks: Set Clock Rate (Response: Clock ID, Rate In Hz) */
#define BCM283X_TAGS_SET_VOLTAGE		0x00038003	/* Voltage: Set Voltage (Response: Voltage ID, Value Offset From 1.2V In Units Of 0.025V) */
#define BCM283X_TAGS_SET_TURBO			0x00038009	/* Clocks: Set Turbo (Response: ID, Level) */
#define BCM283X_TAGS_ALLOCATE_BUFFER		0x00040001	/* Frame Buffer: Allocate Buffer (Response: Frame Buffer Base Address In Bytes, Frame Buffer Size In Bytes) */
#define BCM283X_TAGS_BLANK_SCREEN		0x00040002	/* Frame Buffer: Blank Screen (Response: State) */
#define BCM283X_TAGS_GET_PHYSICAL_DISPLAY	0x00040003	/* Frame Buffer: Get Physical (Display) Width/Height (Response: Width In Pixels, Height In Pixels) */
#define BCM283X_TAGS_GET_VIRTUAL_BUFFER		0x00040004	/* Frame Buffer: Get Virtual (Buffer) Width/Height (Response: Width In Pixels, Height In Pixels) */
#define BCM283X_TAGS_GET_DEPTH			0x00040005	/* Frame Buffer: Get Depth (Response: Bits Per Pixel) */
#define BCM283X_TAGS_GET_PIXEL_ORDER		0x00040006	/* Frame Buffer: Get Pixel Order (Response: State) */
#define BCM283X_TAGS_GET_ALPHA_MODE		0x00040007	/* Frame Buffer: Get Alpha Mode (Response: State) */
#define BCM283X_TAGS_GET_PITCH			0x00040008	/* Frame Buffer: Get Pitch (Response: Bytes Per Line) */
#define BCM283X_TAGS_GET_VIRTUAL_OFFSET		0x00040009	/* Frame Buffer: Get Virtual Offset (Response: X In Pixels, Y In Pixels) */
#define BCM283X_TAGS_GET_OVERSCAN		0x0004000A	/* Frame Buffer: Get Overscan (Response: Top In Pixels, Bottom In Pixels, Left In Pixels, Right In Pixels) */
#define BCM283X_TAGS_GET_PALETTE		0x0004000B	/* Frame Buffer: Get Palette (Response: RGBA Palette Values (Index 0 To 255)) */
#define BCM283X_TAGS_TEST_PHYSICAL_DISPLAY	0x00044003	/* Frame Buffer: Test Physical (Display) Width/Height (Response: Width In Pixels, Height In Pixels) */
#define BCM283X_TAGS_TEST_VIRTUAL_BUFFER	0x00044004	/* Frame Buffer: Test Virtual (Buffer) Width/Height (Response: Width In Pixels, Height In Pixels) */
#define BCM283X_TAGS_TEST_DEPTH			0x00044005	/* Frame Buffer: Test Depth (Response: Bits Per Pixel) */
#define BCM283X_TAGS_TEST_PIXEL_ORDER		0x00044006	/* Frame Buffer: Test Pixel Order (Response: State) */
#define BCM283X_TAGS_TEST_ALPHA_MODE		0x00044007	/* Frame Buffer: Test Alpha Mode (Response: State) */
#define BCM283X_TAGS_TEST_VIRTUAL_OFFSET	0x00044009	/* Frame Buffer: Test Virtual Offset (Response: X In Pixels, Y In Pixels) */
#define BCM283X_TAGS_TEST_OVERSCAN		0x0004400A	/* Frame Buffer: Test Overscan (Response: Top In Pixels, Bottom In Pixels, Left In Pixels, Right In Pixels) */
#define BCM283X_TAGS_TEST_PALETTE		0x0004400B	/* Frame Buffer: Test Palette (Response: RGBA Palette Values (Index 0 To 255)) */
#define BCM283X_TAGS_RELEASE_BUFFER		0x00048001	/* Frame Buffer: Release Buffer (Response: Releases And Disables The Frame Buffer) */
#define BCM283X_TAGS_SET_PHYSICAL_DISPLAY	0x00048003	/* Frame Buffer: Set Physical (Display) Width/Height (Response: Width In Pixels, Height In Pixels) */
#define BCM283X_TAGS_SET_VIRTUAL_BUFFER		0x00048004	/* Frame Buffer: Set Virtual (Buffer) Width/Height (Response: Width In Pixels, Height In Pixels) */
#define BCM283X_TAGS_SET_DEPTH			0x00048005	/* Frame Buffer: Set Depth (Response: Bits Per Pixel) */
#define BCM283X_TAGS_SET_PIXEL_ORDER		0x00048006	/* Frame Buffer: Set Pixel Order (Response: State) */
#define BCM283X_TAGS_SET_ALPHA_MODE		0x00048007	/* Frame Buffer: Set Alpha Mode (Response: State) */
#define BCM283X_TAGS_SET_VIRTUAL_OFFSET		0x00048009	/* Frame Buffer: Set Virtual Offset (Response: X In Pixels, Y In Pixels) */
#define BCM283X_TAGS_SET_OVERSCAN		0x0004800A	/* Frame Buffer: Set Overscan (Response: Top In Pixels, Bottom In Pixels, Left In Pixels, Right In Pixels) */
#define BCM283X_TAGS_SET_PALETTE		0x0004800B	/* Frame Buffer: Set Palette (Response: RGBA Palette Values (Index 0 To 255)) */
#define BCM283X_TAGS_GET_COMMAND_LINE		0x00050001	/* Config: Get Command Line (Response: ASCII Command Line String) */
#define BCM283X_TAGS_GET_DMA_CHANNELS		0x00060001	/* Shared Resource Management: Get DMA Channels (Response: Bits 0-15: DMA channels 0-15) */

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

/*
 *  PWM
 */
#define BCM283X_PWM_BASE (BCM283X_PER_BASE + 0x20C000)
#define BCM283X_PWM_CTL		(BCM283X_PWM_BASE + 0x00)	/* PWM Control */
#define BCM283X_PWM_STA		(BCM283X_PWM_BASE + 0x04)	/* PWM Status */
#define BCM283X_PWM_DMAC	(BCM283X_PWM_BASE + 0x08)	/* PWM DMA Configuration */
#define BCM283X_PWM_RNG1	(BCM283X_PWM_BASE + 0x10)	/* PWM Channel 1 Range */
#define BCM283X_PWM_DAT1	(BCM283X_PWM_BASE + 0x14)	/* PWM Channel 1 Data */
#define BCM283X_PWM_FIF1	(BCM283X_PWM_BASE + 0x18)	/* PWM FIFO Input */
#define BCM283X_PWM_RNG2	(BCM283X_PWM_BASE + 0x20)	/* PWM Channel 2 Range */
#define BCM283X_PWM_DAT2	(BCM283X_PWM_BASE + 0x24)	/* PWM Channel 2 Data */

/*
 *  System Timer
 */
#define BCM283X_STIMER_BASE (BCM283X_PER_BASE + 0x3000)
#define BCM283X_STIMER_CS	(BCM283X_STIMER_BASE + 0x00)
#define BCM283X_STIMER_CLO	(BCM283X_STIMER_BASE + 0x04)
#define BCM283X_STIMER_CHI	(BCM283X_STIMER_BASE + 0x08)
#define BCM283X_STIMER_C0	(BCM283X_STIMER_BASE + 0x0C)
#define BCM283X_STIMER_C1	(BCM283X_STIMER_BASE + 0x10)
#define BCM283X_STIMER_C2	(BCM283X_STIMER_BASE + 0x14)
#define BCM283X_STIMER_C3	(BCM283X_STIMER_BASE + 0x18)

/*
 *  Auxiliary
 */
#define BCM283X_AUX_BASE (BCM283X_PER_BASE + 0x215000)
#define BCM283X_AUX_IRQ			(BCM283X_AUX_BASE + 0x00)	/* Auxiliary Interrupt status 3 */
#define BCM283X_AUX_ENABLES		(BCM283X_AUX_BASE + 0x04)	/* Auxiliary enables 3bit */
#define BCM283X_AUX_MU_IO_REG		(BCM283X_AUX_BASE + 0x40)	/* Mini Uart I/O Data 8bit */
#define BCM283X_AUX_MU_IER_REG		(BCM283X_AUX_BASE + 0x44)	/* Mini Uart Interrupt Enable 8bit */
#define BCM283X_AUX_MU_IIR_REG		(BCM283X_AUX_BASE + 0x48)	/* Mini Uart Interrupt Identify 8bit */
#define BCM283X_AUX_MU_LCR_REG		(BCM283X_AUX_BASE + 0x4C)	/* Mini Uart Line Control 8bit */
#define BCM283X_AUX_MU_MCR_REG		(BCM283X_AUX_BASE + 0x50)	/* Mini Uart Modem Control 8bit */
#define BCM283X_AUX_MU_LSR_REG		(BCM283X_AUX_BASE + 0x54)	/* Mini Uart Line Status 8bit */
#define BCM283X_AUX_MU_MSR_REG		(BCM283X_AUX_BASE + 0x58)	/* Mini Uart Modem Status 8bit */
#define BCM283X_AUX_MU_SCRATCH		(BCM283X_AUX_BASE + 0x5C)	/* Mini Uart Scratch 8bit */
#define BCM283X_AUX_MU_CNTL_REG		(BCM283X_AUX_BASE + 0x60)	/* Mini Uart Extra Control 8bit */
#define BCM283X_AUX_MU_STAT_REG		(BCM283X_AUX_BASE + 0x64)	/* Mini Uart Extra Status 32bit */
#define BCM283X_AUX_MU_BAUD_REG		(BCM283X_AUX_BASE + 0x68)	/* Mini Uart Baudrate 16bit */
#define BCM283X_AUX_SPI0_CNTL0_REG	(BCM283X_AUX_BASE + 0x80)	/* SPI 1 Control register 0 32bit */
#define BCM283X_AUX_SPI0_CNTL1_REG	(BCM283X_AUX_BASE + 0x84)	/* SPI 1 Control register 1 8bit */
#define BCM283X_AUX_SPI0_STAT_REG	(BCM283X_AUX_BASE + 0x88)	/* SPI 1 Status 32bit */
#define BCM283X_AUX_SPI0_IO_REG		(BCM283X_AUX_BASE + 0x90)	/* SPI 1 Data 32bit */
#define BCM283X_AUX_SPI0_PEEK_REG	(BCM283X_AUX_BASE + 0x94)	/* SPI 1 Peek 16bit */
#define BCM283X_AUX_SPI1_CNTL0_REG	(BCM283X_AUX_BASE + 0xC0)	/* SPI 2 Control register 0 32bit */
#define BCM283X_AUX_SPI1_CNTL1_REG	(BCM283X_AUX_BASE + 0xC4)	/* SPI 2 Control register 1 8bit */

#if defined(AZO_CHIP_BCM2836) || defined(AZO_CHIP_BCM2837)
#define BCM283X_PER_BASE_40000000 (0x40000000)

/*
 *  Core Timer
 */
#define BCM283X_CTIMER_CTL	(BCM283X_PER_BASE_40000000 + 0x00)	/* Control register */
#define BCM283X_CTIMER_PRE	(BCM283X_PER_BASE_40000000 + 0x08)	/* Core timer prescaler */
#define BCM283X_CTIMER_LS32B	(BCM283X_PER_BASE_40000000 + 0x1C)	/* Core timer access LS 32 bits */
#define BCM283X_CTIMER_MS32B	(BCM283X_PER_BASE_40000000 + 0x20)	/* Core timer access MS 32 bits */

/*
 *  GPU interrupt
 */
#define BCM283X_GPUINT_ROUTE	(BCM283X_PER_BASE_40000000 + 0x0C)	/* interrupts routing */

/*
 *  Performance Monitor
 */
#define BCM283X_PMINT_ROUTESET	(BCM283X_PER_BASE_40000000 + 0x10)	/* Performance Monitor Interrupts routing-set */
#define BCM283X_PMINT_ROUTECLR	(BCM283X_PER_BASE_40000000 + 0x14)	/* Performance Monitor Interrupts routing-clear */

/*
 *  Local Timer
 */
#define BCM283X_LTIMER_CTLSTA	(BCM283X_PER_BASE_40000000 + 0x34)	/* Local timer control & status */
#define BCM283X_LTIMER_WRITE	(BCM283X_PER_BASE_40000000 + 0x38)	/* Local timer write flags */
#define BCM283X_LTIMER_ROUTE	(BCM283X_PER_BASE_40000000 + 0x24)	/* Local Interrupt 0 [1-7] routing */

/*
 *  AXI Timer
 */
#define BCM283X_AXITIMER_CTL	(BCM283X_PER_BASE_40000000 + 0x2C)	/* Axi outstanding counters */
#define BCM283X_AXITIMER_IRQ	(BCM283X_PER_BASE_40000000 + 0x30)	/* Axi outstanding IRQ */

/*
 *  ARM Core Timer
 */
#define BCM283X_C0TIMER_INTCTL	(BCM283X_PER_BASE_40000000 + 0x40)	/* Core0 timers Interrupt control */
#define BCM283X_C1TIMER_INTCTL	(BCM283X_PER_BASE_40000000 + 0x44)	/* Core1 timers Interrupt control */
#define BCM283X_C2TIMER_INTCTL	(BCM283X_PER_BASE_40000000 + 0x48)	/* Core2 timers Interrupt control */
#define BCM283X_C3TIMER_INTCTL	(BCM283X_PER_BASE_40000000 + 0x4C)	/* Core3 timers Interrupt control */
/*
 *  ARM Core Mailbox interrupt
 */
#define BCM283X_C0MB_INTCTL	(BCM283X_PER_BASE_40000000 + 0x50)	/* Core0 Mailboxes Interrupt control */
#define BCM283X_C1MB_INTCTL	(BCM283X_PER_BASE_40000000 + 0x54)	/* Core1 Mailboxes Interrupt control */
#define BCM283X_C2MB_INTCTL	(BCM283X_PER_BASE_40000000 + 0x58)	/* Core2 Mailboxes Interrupt control */
#define BCM283X_C3MB_INTCTL	(BCM283X_PER_BASE_40000000 + 0x5C)	/* Core3 Mailboxes Interrupt control */
/*
 *  ARM Core IRQ・FIQ status
 */
#define BCM283X_C0_IRQSOURCE	(BCM283X_PER_BASE_40000000 + 0x60)	/* Core0 IRQ Source */
#define BCM283X_C1_IRQSOURCE	(BCM283X_PER_BASE_40000000 + 0x64)	/* Core1 IRQ Source */
#define BCM283X_C2_IRQSOURCE	(BCM283X_PER_BASE_40000000 + 0x68)	/* Core2 IRQ Source */
#define BCM283X_C3_IRQSOURCE	(BCM283X_PER_BASE_40000000 + 0x6C)	/* Core3 IRQ Source */
#define BCM283X_C0_FIQSOURCE	(BCM283X_PER_BASE_40000000 + 0x70)	/* Core0 FIQ Source */
#define BCM283X_C1_FIQSOURCE	(BCM283X_PER_BASE_40000000 + 0x74)	/* Core1 FIQ Source */
#define BCM283X_C2_FIQSOURCE	(BCM283X_PER_BASE_40000000 + 0x78)	/* Core2 FIQ Source */
#define BCM283X_C3_FIQSOURCE	(BCM283X_PER_BASE_40000000 + 0x7C)	/* Core3 FIQ Source */
/*
 *  ARM Core Mailbox
 */
#define BCM283X_C0MB_0WS	(BCM283X_PER_BASE_40000000 + 0x80)	/* Core 0 Mailbox 0 write-set (WO) */
#define BCM283X_C0MB_1WS	(BCM283X_PER_BASE_40000000 + 0x84)	/* Core 0 Mailbox 1 write-set (WO) */
#define BCM283X_C0MB_2WS	(BCM283X_PER_BASE_40000000 + 0x88)	/* Core 0 Mailbox 2 write-set (WO) */
#define BCM283X_C0MB_3WS	(BCM283X_PER_BASE_40000000 + 0x8C)	/* Core 0 Mailbox 3 write-set (WO) */
#define BCM283X_C1MB_0WS	(BCM283X_PER_BASE_40000000 + 0x90)	/* Core 1 Mailbox 0 write-set (WO) */
#define BCM283X_C1MB_1WS	(BCM283X_PER_BASE_40000000 + 0x94)	/* Core 1 Mailbox 1 write-set (WO) */
#define BCM283X_C1MB_2WS	(BCM283X_PER_BASE_40000000 + 0x98)	/* Core 1 Mailbox 2 write-set (WO) */
#define BCM283X_C1MB_3WS	(BCM283X_PER_BASE_40000000 + 0x9C)	/* Core 1 Mailbox 3 write-set (WO) */
#define BCM283X_C2MB_0WS	(BCM283X_PER_BASE_40000000 + 0xA0)	/* Core 2 Mailbox 0 write-set (WO) */
#define BCM283X_C2MB_1WS	(BCM283X_PER_BASE_40000000 + 0xA4)	/* Core 2 Mailbox 1 write-set (WO) */
#define BCM283X_C2MB_2WS	(BCM283X_PER_BASE_40000000 + 0xA8)	/* Core 2 Mailbox 2 write-set (WO) */
#define BCM283X_C2MB_3WS	(BCM283X_PER_BASE_40000000 + 0xAC)	/* Core 2 Mailbox 3 write-set (WO) */
#define BCM283X_C3MB_0WS	(BCM283X_PER_BASE_40000000 + 0xB0)	/* Core 3 Mailbox 0 write-set (WO) */
#define BCM283X_C3MB_1WS	(BCM283X_PER_BASE_40000000 + 0xB4)	/* Core 3 Mailbox 1 write-set (WO) */
#define BCM283X_C3MB_2WS	(BCM283X_PER_BASE_40000000 + 0xB8)	/* Core 3 Mailbox 2 write-set (WO) */
#define BCM283X_C3MB_3WS	(BCM283X_PER_BASE_40000000 + 0xBC)	/* Core 3 Mailbox 3 write-set (WO) */
#define BCM283X_C0MB_0RC	(BCM283X_PER_BASE_40000000 + 0xC0)	/* Core 0 Mailbox 0 read & write-high-to-clear */
#define BCM283X_C0MB_1RC	(BCM283X_PER_BASE_40000000 + 0xC4)	/* Core 0 Mailbox 1 read & write-high-to-clear */
#define BCM283X_C0MB_2RC	(BCM283X_PER_BASE_40000000 + 0xC8)	/* Core 0 Mailbox 2 read & write-high-to-clear */
#define BCM283X_C0MB_3RC	(BCM283X_PER_BASE_40000000 + 0xCC)	/* Core 0 Mailbox 3 read & write-high-to-clear */
#define BCM283X_C1MB_0RC	(BCM283X_PER_BASE_40000000 + 0xD0)	/* Core 1 Mailbox 0 read & write-high-to-clear */
#define BCM283X_C1MB_1RC	(BCM283X_PER_BASE_40000000 + 0xD4)	/* Core 1 Mailbox 1 read & write-high-to-clear */
#define BCM283X_C1MB_2RC	(BCM283X_PER_BASE_40000000 + 0xD8)	/* Core 1 Mailbox 2 read & write-high-to-clear */
#define BCM283X_C1MB_3RC	(BCM283X_PER_BASE_40000000 + 0xDC)	/* Core 1 Mailbox 3 read & write-high-to-clear */
#define BCM283X_C2MB_0RC	(BCM283X_PER_BASE_40000000 + 0xE0)	/* Core 2 Mailbox 0 read & write-high-to-clear */
#define BCM283X_C2MB_1RC	(BCM283X_PER_BASE_40000000 + 0xE4)	/* Core 2 Mailbox 1 read & write-high-to-clear */
#define BCM283X_C2MB_2RC	(BCM283X_PER_BASE_40000000 + 0xE8)	/* Core 2 Mailbox 2 read & write-high-to-clear */
#define BCM283X_C2MB_3RC	(BCM283X_PER_BASE_40000000 + 0xEC)	/* Core 2 Mailbox 3 read & write-high-to-clear */
#define BCM283X_C3MB_0RC	(BCM283X_PER_BASE_40000000 + 0xF0)	/* Core 3 Mailbox 0 read & write-high-to-clear */
#define BCM283X_C3MB_1RC	(BCM283X_PER_BASE_40000000 + 0xF4)	/* Core 3 Mailbox 1 read & write-high-to-clear */
#define BCM283X_C3MB_2RC	(BCM283X_PER_BASE_40000000 + 0xF8)	/* Core 3 Mailbox 2 read & write-high-to-clear */
#define BCM283X_C3MB_3RC	(BCM283X_PER_BASE_40000000 + 0xFC)	/* Core 3 Mailbox 3 read & write-high-to-clear */
#endif

//-------------------------------------------------------------------------

#ifndef AZO_ASSEMBLY

//-------------------------------------------------------------------------
typedef char		  int8_t;
typedef unsigned char	 uint8_t;
typedef short		 int16_t;
typedef unsigned short	uint16_t;
typedef long		 int32_t;
typedef unsigned long	uint32_t;
typedef unsigned int	intptr_t;

extern void dummy(void);
extern uint32_t arm_get_coreid(void);
extern void arm_irq_enable(void);
extern void arm_irq_disable(void);
extern void arm_bp_enable(void);
extern void arm_bp_disable(void);
extern void arm_icache_enable(void);
extern void arm_icache_disable(void);
extern void arm_dcache_enable(void);
extern void arm_dcache_disable(void);
extern void arm_acquire_lock(volatile void* lock);
extern void arm_release_lock(volatile void* lock);
extern void arm_smp_enable(void);
extern void arm_smp_disable(void);
extern void arm_icache_invalidate(void);
extern void arm_dcache_invalidate(void);
extern void arm_dcache_clean(void);
extern void arm_dcache_clean_invalidate(void);
extern void arm_cache_invalidate(void);
extern void arm_bp_invalidate(void);
extern void arm_tlb_invalidate(void);
extern void arm_alignment_enable(void);
extern void arm_alignment_disable(void);
extern void arm_mmu_reset(void);
extern void arm_mmu_init(volatile uint32_t* tlb);
extern void arm_data_memorybarrier(void);
extern void arm_instruction_syncbarrier(void);
extern void arm_drain_write_buffer(void);

extern uint32_t azo_uint32_div(uint32_t val, uint32_t div);
extern uint32_t azo_uint32_mod(uint32_t val, uint32_t div);
extern uint32_t bcm283x_get_cpuclock_med(uint32_t count);

//-------------------------------------------------------------------------
/* DMA Control Block */
/*
	DMA Transfer Information.

	bit26 : NO_WIDE_BURSTS : Don t Do wide writes as a 2 beat burst
	bit25:21 : WAITS : Add Wait Cycles
	bit20:16 : PERMAP : Peripheral Mapping
	bit15:12 : BURST_LENGTH : Burst Transfer Length
	bit11 : SRC_IGNORE : Ignore Reads
	bit10 : SRC_DREQ : Control Source Reads with DREQ
	bit 9 : SRC_WIDTH : Source Transfer Width
		1 = Use 128-bit source read width.
		0 = Use 32-bit source read width.
	bit 8 : SRC_INC : Source Address Increment
		1 = Source address increments after each read. The
		    address will increment by 4, if S_WIDTH=0 else by 32.
		0 = Source address does not change.
	bit 7 : DEST_IGNORE : Ignore Writes
	bit 6 : DEST_DREQ : Control Destination Writes with DREQ
	bit 5 : DEST_WIDTH : Destination Transfer Width
		1 = Use 128-bit destination write width.
		0 = Use 32-bit destination write width.
	bit 4 : DEST_INC : Destination Address Increment
		1 = Destination address increments after each write The address will increment by 4,
		    if DEST_WIDTH=0 else by 32.
		0 = Destination address does not change.
	bit 3 : WAIT_RESP : Wait for a Write Response
	bit 1 : TDMODE : 2D Mode
		1 = 2D mode interpret the TXFR_LEN register as
		    YLENGTH number of transfers each of XLENGTH, and
		    add the strides to the address after each transfer.
		0 = Linear mode interpret the TXFR register as a single
		    transfer of total length {YLENGTH ,XLENGTH}.
	bit 0 : INTEN : Interrupt Enable
*/
typedef struct bcm283x_dma_cb {
	uint32_t transfer_information;
	uint32_t src_address;
	uint32_t dst_address;
	uint32_t length;
	uint32_t tdmode_stride;
	uint32_t next_control_block_address;
	uint32_t reserved1;
	uint32_t reserved2;
} bcm283x_dma_cb_t;

//-------------------------------------------------------------------------
#define ARMTOVC_RAM_ADDRESS(a) ((uint32_t)(a) + 0xC0000000)
#define VCTOARM_RAM_ADDRESS(a) ((uint32_t)(a) - 0xC0000000)
#define ARMTOVC_PER_ADDRESS(a) ((uint32_t)(a) - BCM283X_PER_BASE + 0x7E000000)

//-------------------------------------------------------------------------
inline unsigned int GET32(const unsigned int addr) {
	return *(volatile unsigned int*)addr;
}

//-------------------------------------------------------------------------
inline void PUT32(const unsigned int addr, const unsigned int val) {
	*(volatile unsigned int*)addr = val;
}

#endif	/* AZO_ASSEMBLY */
#endif	/* RP_BAREMETAL_H */

