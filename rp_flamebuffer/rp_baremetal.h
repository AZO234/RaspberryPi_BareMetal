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
 * Random Number Generator(RNG)
 */
#define BCM283X_RNG_BASE	(BCM283X_PER_BASE + 0x104000)
#define BCM283X_RNG_CTRL	(BCM283X_RNG_BASE + 0x0)
#define BCM283X_RNG_STATUS	(BCM283X_RNG_BASE + 0x4)
#define BCM283X_RNG_DATA	(BCM283X_RNG_BASE + 0x8)

/*
 * Mailbox
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

#endif	/* AZO_MACRO_ONLY */
#endif	/* RP_BAREMETAL_H */

