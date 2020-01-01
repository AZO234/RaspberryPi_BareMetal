.text
	.globl _start
_start:
	mrs  x0, S3_3_c0_c0_5	/* core0 only */
	ands x0, x0, #0x3
	bne  loop

	mov x3, #0
	ldr w3, =bcm283x_per
	ldr w3, [x3]
	ldr x2, =0x200008	/* GPFSEL2 */
	add w2, w2, w3
	ldr w0, [x2]
	mov w1, #7		/* FSEL22 */
	lsl w1, w1, #6
	mvn w1, w1
	and w0, w0, w1
	mov w1, #3		/* ALT4 */
	lsl w1, w1, #6
	orr w0, w0, w1
	mov w1, #7		/* FSEL23 */
	lsl w1, w1, #9
	mvn w1, w1
	and w0, w0, w1
	mov w1, #3		/* ALT4 */
	lsl w1, w1, #9
	orr w0, w0, w1
	mov w1, #7		/* FSEL24 */
	lsl w1, w1, #12
	mvn w1, w1
	and w0, w0, w1
	mov w1, #3		/* ALT4 */
	lsl w1, w1, #12
	orr w0, w0, w1
	mov w1, #7		/* FSEL25 */
	lsl w1, w1, #15
	mvn w1, w1
	and w0, w0, w1
	mov w1, #3		/* ALT4 */
	lsl w1, w1, #15
	orr w0, w0, w1
	mov w1, #7		/* FSEL26 */
	lsl w1, w1, #18
	mvn w1, w1
	and w0, w0, w1
	mov w1, #3		/* ALT4 */
	lsl w1, w1, #18
	orr w0, w0, w1
	mov w1, #7		/* FSEL27 */
	lsl w1, w1, #21
	mvn w1, w1
	and w0, w0, w1
	mov w1, #3		/* ALT4 */
	lsl w1, w1, #21
	orr w0, w0, w1
	str w0, [x2]

	/* disable pullUD */
	ldr  x2, =0x200094		/* GPPUD */
	add  w2, w2, w3
	mov  w0, #0
	str  w0, [x2]
	mov  w1, #150			/* 150 cycle wait */
_start_disable_pullud_1:
	mov  w0, w0
	subs w1, w1, #1
	bne  _start_disable_pullud_1
	ldr  x2, =0x200098		/* GPPUDCLK0 */
	add  w2, w2, w3
	ldr  w0, [x2]
	orr  w0, w0, #0xFC00000		/* GPIO22-27 */
	str  w0, [x2]
	mov  w1, #150			/* 150 cycle wait */
_start_disable_pullud_2:
	mov  w0, w0
	subs w1, w1, #1
	bne  _start_disable_pullud_2
	mov  w0, #0
	str  w0, [x2]

loop:	b loop

bcm283x_per: .long 0x3F000000

