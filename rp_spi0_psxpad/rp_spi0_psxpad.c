/*-------------------------------------------------------------------------
Copyright (c) 2017 AZO typesylph@gmail.com
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

//------------------------------------------------------------------------
#define PSXPAD_MAXPADNUM 2
#if PSXPAD_MAXPADNUM == 0 || PSXPAD_MAXPADNUM > 2
#error PSXPAD_MAXPADNUM must be 1-2
#endif

typedef enum {
	PSXPAD_KEYSTATE_DIGITAL = 0,
	PSXPAD_KEYSTATE_ANALOG1,
	PSXPAD_KEYSTATE_ANALOG2,
	PSXPAD_KEYSTATE_UNKNOWN
} PSXPad_KeyStateType_t;

typedef struct PSXPad_KeyState {
	PSXPad_KeyStateType_t tType;
	/* PSXPAD_KEYSTATE_DIGITAL */
	uint8_t bSel;
	uint8_t bStt;
	uint8_t bU;
	uint8_t bR;
	uint8_t bD;
	uint8_t bL;
	uint8_t bL2;
	uint8_t bR2;
	uint8_t bL1;
	uint8_t bR1;
	uint8_t bTri;
	uint8_t bCir;
	uint8_t bCrs;
	uint8_t bSqr;
	/* PSXPAD_KEYSTATE_ANALOG1 */
	uint8_t bL3;
	uint8_t bR3;
	uint8_t u8RX;
	uint8_t u8RY;
	uint8_t u8LX;
	uint8_t u8LY;
	/* PSXPAD_KEYSTATE_ANALOG2 */
	uint8_t u8AR;
	uint8_t u8AL;
	uint8_t u8AU;
	uint8_t u8AD;
	uint8_t u8ATri;
	uint8_t u8ACir;
	uint8_t u8ACrs;
	uint8_t u8ASqr;
	uint8_t u8AL1;
	uint8_t u8AR1;
	uint8_t u8AL2;
	uint8_t u8AR2;
} PSXPad_KeyState_t;

const uint8_t PSX_CMD_INIT_PRESSURE[]	= {0x01, 0x40, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x00};
const uint8_t PSX_CMD_POLL[]         = {0x01, 0x42, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
const uint8_t PSX_CMD_ENTER_CFG[]    = {0x01, 0x43, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00};
const uint8_t PSX_CMD_EXIT_CFG[]     = {0x01, 0x43, 0x00, 0x00, 0x5A, 0x5A, 0x5A, 0x5A, 0x5A};
const uint8_t PSX_CMD_ENABLE_MOTOR[] = {0x01, 0x4D, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
const uint8_t PSX_CMD_ALL_PRESSURE[] = {0x01, 0x4F, 0x00, 0xFF, 0xFF, 0x03, 0x00, 0x00, 0x00};
const uint8_t PSX_CMD_AD_MODE[]      = {0x01, 0x44, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00};

typedef struct PSXPad {
	uint8_t lu8PoolCmd[sizeof(PSX_CMD_POLL)];
	uint8_t lu8Response[sizeof(PSX_CMD_POLL)];
	uint8_t u8AttPinNo;
	uint8_t bAnalog;
	uint8_t bLock;
	uint8_t bMotor1Enable;
	uint8_t bMotor2Enable;
	uint8_t u8Motor1Level;
	uint8_t u8Motor2Level;
	uint8_t lu8EnableMotor[sizeof(PSX_CMD_ENABLE_MOTOR)];
	uint8_t lu8ADMode[sizeof(PSX_CMD_AD_MODE)];
} PSXPad_t;

typedef struct PSXPads {
	uint8_t u8PadsNum;
	PSXPad_t ltPad[PSXPAD_MAXPADNUM];
} PSXPads_t;

#define REVERSE_BIT(x) ((((x) & 0x80) >> 7) | (((x) & 0x40) >> 5) | (((x) & 0x20) >> 3) | (((x) & 0x10) >> 1) | (((x) & 0x08) << 1) | (((x) & 0x04) << 3) | (((x) & 0x02) << 5) | (((x) & 0x01) << 7))

static PSXPads_t tPSXPads;
static PSXPad_KeyState_t tPSXKeyState;

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
void hex2strings(uint8_t d) {
	uint32_t rb;
	uint32_t rc;

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
void hexstring(uint32_t d) {
	hexstrings(d);
	uart_putc(0x0D);
	uart_putc(0x0A);
}

//------------------------------------------------------------------------
void wait_usec(uint32_t time) {
	uint32_t s = GET32(BCM283X_STIMER_CLO);
	while(s + time + 1 > GET32(BCM283X_STIMER_CLO));
}

//------------------------------------------------------------------------
void spi0_init(void) {
	uint32_t ra;

	/* GPIO function set */
	ra = GET32(BCM283X_GPIO_GPFSEL0);
	ra &= ~(7<<21); /* GPIO 7 */
	ra |=   4<<21 ; /* ALT0 */
	ra &= ~(7<<24); /* GPIO 8 */
	ra |=   4<<24 ; /* ALT0 */
	ra &= ~(7<<27); /* GPIO 9 */
	ra |=   4<<27 ; /* ALT0 */
	PUT32(BCM283X_GPIO_GPFSEL0, ra);
	ra = GET32(BCM283X_GPIO_GPFSEL1);
	ra &= ~(7<< 0); /* GPIO10 */
	ra |=   4<< 0 ; /* ALT0 */
	ra &= ~(7<< 3); /* GPIO11 */
	ra |=   4<< 3 ; /* ALT0 */
	PUT32(BCM283X_GPIO_GPFSEL1, ra);

	/* PullUD disable */
	PUT32(BCM283X_GPIO_GPPUD, 0);
	for(ra = 0; ra < 150; ra++) dummy();
	PUT32(BCM283X_GPIO_GPPUDCLK0, (1 << 7) | (1 << 8) | (1 << 9) | (1 << 10) | (1 << 11));
	for(ra = 0; ra < 150; ra++) dummy();
	PUT32(BCM283X_GPIO_GPPUDCLK0, 0);

	/* 250MHz / 1024 = 244kbps */
	PUT32(BCM283X_SPI0_CLK, 1024);
}

//------------------------------------------------------------------------
void PSXPads_StartDMA() {
	/* DMA activate */
	PUT32(BCM283X_DMA0_BASE + BCM283X_DMA_CS, 0x1);
	PUT32(BCM283X_DMA1_BASE + BCM283X_DMA_CS, 0x1);
}

void PSXPads_StopDMA() {
	/* DMA deactivate */
	PUT32(BCM283X_DMA0_BASE + BCM283X_DMA_CS, 0x0);
	PUT32(BCM283X_DMA1_BASE + BCM283X_DMA_CS, 0x0);
}

void PSXPads_Init(PSXPads_t* ptPSXPads, const uint8_t i_u8PadNum) {
	uint8_t u8PadNo, u8Loc;

	if(!ptPSXPads)
		return;
	if(i_u8PadNum == 0 || i_u8PadNum >= PSXPAD_MAXPADNUM)
		return;

	ptPSXPads->u8PadsNum = i_u8PadNum;

	for(u8PadNo = 0; u8PadNo < ptPSXPads->u8PadsNum; u8PadNo++) {
		for(u8Loc = 0; u8Loc < sizeof(PSX_CMD_POLL); u8Loc++)
			ptPSXPads->ltPad[u8PadNo].lu8PoolCmd[u8Loc] = PSX_CMD_POLL[u8Loc];
		for(u8Loc = 0; u8Loc < sizeof(PSX_CMD_ENABLE_MOTOR); u8Loc++)
			ptPSXPads->ltPad[u8PadNo].lu8EnableMotor[u8Loc] = PSX_CMD_ENABLE_MOTOR[u8Loc];
		for(u8Loc = 0; u8Loc < sizeof(PSX_CMD_AD_MODE); u8Loc++)
			ptPSXPads->ltPad[u8PadNo].lu8ADMode[u8Loc] = PSX_CMD_AD_MODE[u8Loc];
	}
}

void PSXPads_Command(const PSXPads_t* ptPSXPads, const uint8_t u8PadNo, const uint8_t i_lu8SendCmd[], uint8_t i_lu8Response[], const uint8_t i_u8SendCmdLen) {
	uint8_t u8Loc;

	if(!ptPSXPads)
		return;
	if(u8PadNo >= ptPSXPads->u8PadsNum)
		return;

	/* FIFO clear, mode 3 */
	PUT32(BCM283X_SPI0_CS, 0x000000BC + u8PadNo);
	wait_usec(10);
	for(u8Loc = 0; u8Loc < i_u8SendCmdLen; u8Loc++) {
		while(!(GET32(BCM283X_SPI0_CS) & (1 << 18)));
		PUT32(BCM283X_SPI0_FIFO, REVERSE_BIT(i_lu8SendCmd[u8Loc]));
		while(!(GET32(BCM283X_SPI0_CS) & (1 << 16)));
		while(!(GET32(BCM283X_SPI0_CS) & (1 << 17)));
		i_lu8Response[u8Loc] = GET32(BCM283X_SPI0_FIFO) & 0xFF;
		i_lu8Response[u8Loc] = REVERSE_BIT(i_lu8Response[u8Loc]);
	}
	wait_usec(5);
	PUT32(BCM283X_SPI0_CS, 0x00000000);
	wait_usec(5);
}

void PSXPads_Pool(PSXPads_t* ptPSXPads) {
	uint8_t u8PadNo;

	if(!ptPSXPads)
		return;

	for(u8PadNo = 0; u8PadNo < ptPSXPads->u8PadsNum; u8PadNo++)
		PSXPads_Command(ptPSXPads, u8PadNo, ptPSXPads->ltPad[u8PadNo].lu8PoolCmd, ptPSXPads->ltPad[u8PadNo].lu8Response, sizeof(PSX_CMD_POLL));
}

void PSXPads_SetADMode(PSXPads_t* ptPSXPads, const uint8_t u8PadNo, const uint8_t i_bAnalog, const uint8_t i_bLock) {
	if(!ptPSXPads)
		return;
	if(u8PadNo >= ptPSXPads->u8PadsNum)
		return;

	ptPSXPads->ltPad[u8PadNo].bAnalog = i_bAnalog ? 1 : 0;
	ptPSXPads->ltPad[u8PadNo].bLock   = i_bLock   ? 1 : 0;

	ptPSXPads->ltPad[u8PadNo].lu8ADMode[3] = ptPSXPads->ltPad[u8PadNo].bAnalog ? 0x01 : 0x00;
	ptPSXPads->ltPad[u8PadNo].lu8ADMode[4] = ptPSXPads->ltPad[u8PadNo].bLock   ? 0x03 : 0x00;

	PSXPads_Command(ptPSXPads, u8PadNo, PSX_CMD_ENTER_CFG, ptPSXPads->ltPad[u8PadNo].lu8Response, sizeof(PSX_CMD_ENTER_CFG));
	PSXPads_Command(ptPSXPads, u8PadNo, ptPSXPads->ltPad[u8PadNo].lu8ADMode, ptPSXPads->ltPad[u8PadNo].lu8Response, sizeof(PSX_CMD_AD_MODE));
//	PSXPads_Command(ptPSXPads, u8PadNo, PSX_CMD_INIT_PRESSURE, ptPSXPads->ltPad[u8PadNo].lu8Response, sizeof(PSX_CMD_INIT_PRESSURE));
//	PSXPads_Command(ptPSXPads, u8PadNo, PSX_CMD_ALL_PRESSURE, ptPSXPads->ltPad[u8PadNo].lu8Response, sizeof(PSX_CMD_ALL_PRESSURE));
	PSXPads_Command(ptPSXPads, u8PadNo, PSX_CMD_EXIT_CFG, ptPSXPads->ltPad[u8PadNo].lu8Response, sizeof(PSX_CMD_EXIT_CFG));
}

void PSXPads_SetEnableMotor(PSXPads_t* ptPSXPads, const uint8_t u8PadNo, const uint8_t i_bMotor1Enable, const uint8_t i_bMotor2Enable) {
	if(!ptPSXPads)
		return;
	if(u8PadNo >= ptPSXPads->u8PadsNum)
		return;

	ptPSXPads->ltPad[u8PadNo].bMotor1Enable = i_bMotor1Enable ? 1 : 0;
	ptPSXPads->ltPad[u8PadNo].bMotor2Enable = i_bMotor2Enable ? 1 : 0;

	ptPSXPads->ltPad[u8PadNo].lu8EnableMotor[3] = ptPSXPads->ltPad[u8PadNo].bMotor1Enable ? 0x00 : 0xFF;
	ptPSXPads->ltPad[u8PadNo].lu8EnableMotor[4] = ptPSXPads->ltPad[u8PadNo].bMotor2Enable ? 0x01 : 0xFF;

	PSXPads_Command(ptPSXPads, u8PadNo, PSX_CMD_ENTER_CFG, ptPSXPads->ltPad[u8PadNo].lu8Response, sizeof(PSX_CMD_ENTER_CFG));
	PSXPads_Command(ptPSXPads, u8PadNo, ptPSXPads->ltPad[u8PadNo].lu8EnableMotor, ptPSXPads->ltPad[u8PadNo].lu8Response, sizeof(PSX_CMD_ENABLE_MOTOR));
	PSXPads_Command(ptPSXPads, u8PadNo, PSX_CMD_EXIT_CFG, ptPSXPads->ltPad[u8PadNo].lu8Response, sizeof(PSX_CMD_EXIT_CFG));
}

void PSXPads_SetMotorLevel(PSXPads_t* ptPSXPads, const uint8_t u8PadNo, const uint8_t i_u8Motor1Level, const uint8_t i_u8Motor2Level) {
	if(!ptPSXPads)
		return;
	if(u8PadNo >= ptPSXPads->u8PadsNum)
		return;

 	ptPSXPads->ltPad[u8PadNo].u8Motor1Level = i_u8Motor1Level ? 0xFF : 0x00;
	ptPSXPads->ltPad[u8PadNo].u8Motor2Level = i_u8Motor2Level;

	ptPSXPads->ltPad[u8PadNo].lu8PoolCmd[3] = ptPSXPads->ltPad[u8PadNo].u8Motor1Level;
	ptPSXPads->ltPad[u8PadNo].lu8PoolCmd[4] = ptPSXPads->ltPad[u8PadNo].u8Motor2Level;
}

void PSXPads_GetKeyState(PSXPads_t* ptPSXPads, const uint8_t u8PadNo, PSXPad_KeyState_t* o_ptKeyState) {
	if(!ptPSXPads)
		return;
	if(u8PadNo >= ptPSXPads->u8PadsNum)
		return;
	if(!o_ptKeyState)
		return;

	o_ptKeyState->tType = PSXPAD_KEYSTATE_UNKNOWN;

	switch(ptPSXPads->ltPad[u8PadNo].lu8Response[1]) {
	case 0x79:
		o_ptKeyState->tType = PSXPAD_KEYSTATE_ANALOG2;
		o_ptKeyState->u8AR   = ptPSXPads->ltPad[u8PadNo].lu8Response[ 9];
		o_ptKeyState->u8AL   = ptPSXPads->ltPad[u8PadNo].lu8Response[10];
		o_ptKeyState->u8AU   = ptPSXPads->ltPad[u8PadNo].lu8Response[11];
		o_ptKeyState->u8AD   = ptPSXPads->ltPad[u8PadNo].lu8Response[12];
		o_ptKeyState->u8ATri = ptPSXPads->ltPad[u8PadNo].lu8Response[13];
		o_ptKeyState->u8ACir = ptPSXPads->ltPad[u8PadNo].lu8Response[14];
		o_ptKeyState->u8ACrs = ptPSXPads->ltPad[u8PadNo].lu8Response[15];
		o_ptKeyState->u8ASqr = ptPSXPads->ltPad[u8PadNo].lu8Response[16];
		o_ptKeyState->u8AL1  = ptPSXPads->ltPad[u8PadNo].lu8Response[17];
		o_ptKeyState->u8AR1  = ptPSXPads->ltPad[u8PadNo].lu8Response[18];
		o_ptKeyState->u8AL2  = ptPSXPads->ltPad[u8PadNo].lu8Response[19];
		o_ptKeyState->u8AR2  = ptPSXPads->ltPad[u8PadNo].lu8Response[20];
	case 0x73:
		if(o_ptKeyState->tType == PSXPAD_KEYSTATE_UNKNOWN)
			o_ptKeyState->tType = PSXPAD_KEYSTATE_ANALOG1;
		o_ptKeyState->u8RX = ptPSXPads->ltPad[u8PadNo].lu8Response[5];
		o_ptKeyState->u8RY = ptPSXPads->ltPad[u8PadNo].lu8Response[6];
		o_ptKeyState->u8LX = ptPSXPads->ltPad[u8PadNo].lu8Response[7];
		o_ptKeyState->u8LY = ptPSXPads->ltPad[u8PadNo].lu8Response[8];
		o_ptKeyState->bL3  = (ptPSXPads->ltPad[u8PadNo].lu8Response[3] & 0x02U) ? 0 : 1;
		o_ptKeyState->bR3  = (ptPSXPads->ltPad[u8PadNo].lu8Response[3] & 0x04U) ? 0 : 1;
	case 0x41:
		if(o_ptKeyState->tType == PSXPAD_KEYSTATE_UNKNOWN)
			o_ptKeyState->tType = PSXPAD_KEYSTATE_DIGITAL;
		o_ptKeyState->bSel = (ptPSXPads->ltPad[u8PadNo].lu8Response[3] & 0x01U) ? 0 : 1;
		o_ptKeyState->bStt = (ptPSXPads->ltPad[u8PadNo].lu8Response[3] & 0x08U) ? 0 : 1;
		o_ptKeyState->bU   = (ptPSXPads->ltPad[u8PadNo].lu8Response[3] & 0x10U) ? 0 : 1;
		o_ptKeyState->bR   = (ptPSXPads->ltPad[u8PadNo].lu8Response[3] & 0x20U) ? 0 : 1;
		o_ptKeyState->bD   = (ptPSXPads->ltPad[u8PadNo].lu8Response[3] & 0x40U) ? 0 : 1;
		o_ptKeyState->bL   = (ptPSXPads->ltPad[u8PadNo].lu8Response[3] & 0x80U) ? 0 : 1;
		o_ptKeyState->bL2  = (ptPSXPads->ltPad[u8PadNo].lu8Response[4] & 0x01U) ? 0 : 1;
		o_ptKeyState->bR2  = (ptPSXPads->ltPad[u8PadNo].lu8Response[4] & 0x02U) ? 0 : 1;
		o_ptKeyState->bL1  = (ptPSXPads->ltPad[u8PadNo].lu8Response[4] & 0x04U) ? 0 : 1;
		o_ptKeyState->bR1  = (ptPSXPads->ltPad[u8PadNo].lu8Response[4] & 0x08U) ? 0 : 1;
		o_ptKeyState->bTri = (ptPSXPads->ltPad[u8PadNo].lu8Response[4] & 0x10U) ? 0 : 1;
		o_ptKeyState->bCir = (ptPSXPads->ltPad[u8PadNo].lu8Response[4] & 0x20U) ? 0 : 1;
		o_ptKeyState->bCrs = (ptPSXPads->ltPad[u8PadNo].lu8Response[4] & 0x40U) ? 0 : 1;
		o_ptKeyState->bSqr = (ptPSXPads->ltPad[u8PadNo].lu8Response[4] & 0x80U) ? 0 : 1;
	}
}

//------------------------------------------------------------------------
int notmain(unsigned int earlypc) {
	spi0_init();
	uart_init();

	PSXPads_Init(&tPSXPads, 1);
	PSXPads_SetADMode(&tPSXPads, 0, 1, 1);	/* Analog mode, Lock */

	while(1) {
		PSXPads_Pool(&tPSXPads);
		PSXPads_GetKeyState(&tPSXPads, 0, &tPSXKeyState);

		if(tPSXKeyState.bU) {
			uart_putc('U');
			uart_putc(' ');
		}
		if(tPSXKeyState.bD) {
			uart_putc('D');
			uart_putc(' ');
		}
		if(tPSXKeyState.bL) {
			uart_putc('L');
			uart_putc(' ');
		}
		if(tPSXKeyState.bR) {
			uart_putc('R');
			uart_putc(' ');
		}
		if(tPSXKeyState.bTri) {
			uart_putc('T');
			uart_putc('r');
			uart_putc('i');
			uart_putc(' ');
		}
		if(tPSXKeyState.bSqr) {
			uart_putc('S');
			uart_putc('q');
			uart_putc('r');
			uart_putc(' ');
		}
		if(tPSXKeyState.bCrs) {
			uart_putc('C');
			uart_putc('r');
			uart_putc('s');
			uart_putc(' ');
		}
		if(tPSXKeyState.bCir) {
			uart_putc('C');
			uart_putc('i');
			uart_putc('r');
			uart_putc(' ');
		}
		if(tPSXKeyState.bL1) {
			uart_putc('L');
			uart_putc('1');
			uart_putc(' ');
		}
		if(tPSXKeyState.bR1) {
			uart_putc('R');
			uart_putc('1');
			uart_putc(' ');
		}
		if(tPSXKeyState.bL2) {
			uart_putc('L');
			uart_putc('2');
			uart_putc(' ');
		}
		if(tPSXKeyState.bR2) {
			uart_putc('R');
			uart_putc('2');
			uart_putc(' ');
		}
		if(tPSXKeyState.bSel) {
			uart_putc('S');
			uart_putc('e');
			uart_putc('l');
			uart_putc(' ');
		}
		if(tPSXKeyState.bStt) {
			uart_putc('S');
			uart_putc('t');
			uart_putc('t');
			uart_putc(' ');
		}
		if(tPSXKeyState.bL3) {
			uart_putc('L');
			uart_putc('3');
			uart_putc(' ');
		}
		if(tPSXKeyState.bR3) {
			uart_putc('R');
			uart_putc('3');
			uart_putc(' ');
		}
		uart_putc(0x0D);
		uart_putc(0x0A);

/*
		uart_putc('U');
		uart_putc(':');
		hex2strings(tPSXKeyState.u8AU);
		uart_putc(' ');
		uart_putc('D');
		uart_putc(':');
		hex2strings(tPSXKeyState.u8AD);
		uart_putc(' ');
		uart_putc('L');
		uart_putc(':');
		hex2strings(tPSXKeyState.u8AL);
		uart_putc(' ');
		uart_putc('R');
		uart_putc(':');
		hex2strings(tPSXKeyState.u8AR);
		uart_putc(' ');
		uart_putc('T');
		uart_putc('r');
		uart_putc('i');
		uart_putc(':');
		hex2strings(tPSXKeyState.u8ATri);
		uart_putc(' ');
		uart_putc('S');
		uart_putc('q');
		uart_putc('r');
		uart_putc(':');
		hex2strings(tPSXKeyState.u8ASqr);
		uart_putc(' ');
		uart_putc('C');
		uart_putc('r');
		uart_putc('s');
		uart_putc(':');
		hex2strings(tPSXKeyState.u8ACrs);
		uart_putc(' ');
		uart_putc('C');
		uart_putc('i');
		uart_putc('r');
		uart_putc(':');
		hex2strings(tPSXKeyState.u8ACir);
		uart_putc(' ');
		uart_putc('L');
		uart_putc('1');
		uart_putc(':');
		hex2strings(tPSXKeyState.u8AL1);
		uart_putc(' ');
		uart_putc('R');
		uart_putc('1');
		uart_putc(':');
		hex2strings(tPSXKeyState.u8AR1);
		uart_putc(' ');
		uart_putc('L');
		uart_putc('2');
		uart_putc(':');
		hex2strings(tPSXKeyState.u8AL1);
		uart_putc(' ');
		uart_putc('R');
		uart_putc('2');
		uart_putc(':');
		hex2strings(tPSXKeyState.u8AR2);
		uart_putc(' ');
		uart_putc('L');
		uart_putc('X');
		uart_putc(':');
		hex2strings(tPSXKeyState.u8LX);
		uart_putc(' ');
		uart_putc('L');
		uart_putc('Y');
		uart_putc(':');
		hex2strings(tPSXKeyState.u8LY);
		uart_putc(' ');
		uart_putc('R');
		uart_putc('X');
		uart_putc(':');
		hex2strings(tPSXKeyState.u8RX);
		uart_putc(' ');
		uart_putc('R');
		uart_putc('Y');
		uart_putc(':');
		hex2strings(tPSXKeyState.u8RY);
		uart_putc(0x0D);
		uart_putc(0x0A);
*/
	}

	return(0);
}

//-------------------------------------------------------------------------
void c_irq_handler(void) {
}

//-------------------------------------------------------------------------

