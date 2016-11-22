.text
	.globl _start
_start:
#ifndef AZO_CHIP_BCM2835
	mrc  p15, 0, r0, c0, c0, 5	/* core0 only */
	ands r0, r0, #0x3
	bne  loop
#endif	/* AZO_CHIP_BCM2835 */

	ldr r3, =bcm283x_per
	ldr r3, [r3]
	ldr r2, =0x200008	/* GPFSEL2 */
	add r2, r2, r3
	ldr r0, [r2]
	mov r1, #7		/* FSEL22 */
	lsl r1, r1, #6
	mvn r1, r1
	and r0, r0, r1
	mov r1, #3		/* ALT4 */
	lsl r1, r1, #6
	orr r0, r0, r1
	mov r1, #7		/* FSEL23 */
	lsl r1, r1, #9
	mvn r1, r1
	and r0, r0, r1
	mov r1, #3		/* ALT4 */
	lsl r1, r1, #9
	orr r0, r0, r1
	mov r1, #7		/* FSEL24 */
	lsl r1, r1, #12
	mvn r1, r1
	and r0, r0, r1
	mov r1, #3		/* ALT4 */
	lsl r1, r1, #12
	orr r0, r0, r1
	mov r1, #7		/* FSEL25 */
	lsl r1, r1, #15
	mvn r1, r1
	and r0, r0, r1
	mov r1, #3		/* ALT4 */
	lsl r1, r1, #15
	orr r0, r0, r1
	mov r1, #7		/* FSEL26 */
	lsl r1, r1, #18
	mvn r1, r1
	and r0, r0, r1
	mov r1, #3		/* ALT4 */
	lsl r1, r1, #18
	orr r0, r0, r1
	mov r1, #7		/* FSEL27 */
	lsl r1, r1, #21
	mvn r1, r1
	and r0, r0, r1
	mov r1, #3		/* ALT4 */
	lsl r1, r1, #21
	orr r0, r0, r1
	str r0, [r2]

	/* disable pullUD */
	ldr  r2, =0x200094		/* GPPUD */
	add  r2, r2, r3
	mov  r0, #0
	str  r0, [r2]
	mov  r1, #150			/* 150 cycle wait */
_start_disable_pullud_1:
	mov  r0, r0
	subs r1, r1, #1
	bne  _start_disable_pullud_1
	ldr  r2, =0x200098		/* GPPUDCLK0 */
	add  r2, r2, r3
	ldr  r0, [r2]
	orr  r0, r0, #0xFC00000		/* GPIO22-27 */
	str  r0, [r2]
	mov  r1, #150			/* 150 cycle wait */
_start_disable_pullud_2:
	mov  r0, r0
	subs r1, r1, #1
	bne  _start_disable_pullud_2
	mov  r0, #0
	str  r0, [r2]

loop:	b loop

#if defined(AZO_CHIP_BCM2835)
bcm283x_per: .long 0x20000000
#elif defined(AZO_CHIP_BCM2836) || (AZO_CHIP_BCM2837)
bcm283x_per: .long 0x3F000000
#else
#error define chip type.
#endif

