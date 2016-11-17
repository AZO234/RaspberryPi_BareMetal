.text
.globl _start
_start:
	mrs  x0, S3_3_c0_c0_5	/* core0 only */
	ands x0, x0, #0x3
	bne  loop

	ldr x3, =bcm283x_per
	ldr x3, [x3]
	ldr x2, =0x200008	/* GPFSEL2 */
	add x2, x2, x3
	ldr x0, [x2]
	mov x1, #7		/* FSEL22 */
	lsl x1, x1, #6
	mvn x1, x1
	and x0, x0, x1
	mov x1, #3		/* ALT4 */
	lsl x1, x1, #6
	orr x0, x0, x1
	mov x1, #7		/* FSEL23 */
	lsl x1, x1, #9
	mvn x1, x1
	and x0, x0, x1
	mov x1, #3		/* ALT4 */
	lsl x1, x1, #9
	orr x0, x0, x1
	mov x1, #7		/* FSEL24 */
	lsl x1, x1, #12
	mvn x1, x1
	and x0, x0, x1
	mov x1, #3		/* ALT4 */
	lsl x1, x1, #12
	orr x0, x0, x1
	mov x1, #7		/* FSEL25 */
	lsl x1, x1, #15
	mvn x1, x1
	and x0, x0, x1
	mov x1, #3		/* ALT4 */
	lsl x1, x1, #15
	orr x0, x0, x1
	mov x1, #7		/* FSEL26 */
	lsl x1, x1, #18
	mvn x1, x1
	and x0, x0, x1
	mov x1, #3		/* ALT4 */
	lsl x1, x1, #18
	orr x0, x0, x1
	mov x1, #7		/* FSEL27 */
	lsl x1, x1, #21
	mvn x1, x1
	and x0, x0, x1
	mov x1, #3		/* ALT4 */
	lsl x1, x1, #21
	orr x0, x0, x1
	str x0, [x2]

	/* disable pullUD */
	ldr  x2, =0x200094		/* GPPUD */
	add  x2, x2, x3
	mov  x0, #0
	str  x0, [x2]
	mov  x1, #150			/* 150 cycle wait */
_start_disable_pullud_1:
	mov  x0, x0
	subs x1, x1, #1
	bne  _start_disable_pullud_1
	ldr  x2, =0x200098		/* GPPUDCLK0 */
	add  x2, x2, x3
	ldr  x0, [x2]
	orr  x0, x0, #0xFC00000		/* GPIO22-27 */
	str  x0, [x2]
	mov  x1, #150			/* 150 cycle wait */
_start_disable_pullud_2:
	mov  x0, x0
	subs x1, x1, #1
	bne  _start_disable_pullud_2
	mov  x0, #0
	str  x0, [x2]

loop:	b loop

bcm283x_per: .word 0x3F000000

