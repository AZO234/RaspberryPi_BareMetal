;@-------------------------------------------------------------------------
;@ Copyright (c) 2016 AZO typesylph@gmail.com
;@   referred to David Welch https://github.com/dwelch67
;@
;@ Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files
;@ (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge,
;@ publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so,
;@ subject to the following conditions:
;@
;@ The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
;@
;@ THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
;@ MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE
;@ FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
;@ WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
;@-------------------------------------------------------------------------

#define AZO_ASSEMBLY
#include "rp_baremetal.h"

;@-------------------------------------------------------------------------
.text
	.globl _start
_start:
	ldr pc, reset_handler
	ldr pc, undefined_handler
	ldr pc, swi_handler
	ldr pc, prefetch_handler
	ldr pc, data_handler
	ldr pc, unused_handler
	ldr pc, irq_handler
	ldr pc, fiq_handler
reset_handler:      .word reset
undefined_handler:  .word hang
swi_handler:        .word hang
prefetch_handler:   .word hang
data_handler:       .word hang
unused_handler:     .word hang
irq_handler:        .word irq
fiq_handler:        .word hang

;@-------------------------------------------------------------------------
/*
By bootloader,
RP2,3's core0,1,2,3 start here async in SVC mode.
RP2,3's core0 start here. othre core are stop. in HYP mode.
*/

reset:
#if !defined(AZO_CHIP_BCM2835) && defined(AZO_CPU_SINGLE)
	mrc  p15, 0, r0, c0, c0, 5
	ands r0, r0, #0x3
	beq  reset_single_c0
reset_single_cn:
	wfi
	b    reset_single_cn
reset_single_c0:
#endif

#if defined(AZO_CHIP_BCM2836) || defined(AZO_CHIP_BCM2837) || defined(AZO_CHIP_BCM2711)
	// non-secure
	mrc p15, 0, r0, c1, c1, 0
	orr r0, r0, #0x1
	mcr p15, 0, r0, c1, c1, 0
#endif

#if defined(AZO_CHIP_BCM2837) || defined(AZO_CHIP_BCM2711)
	// stop caching
	mov r12, #0
	mcr p15, 0, r12, c7, c10, 1
	dsb
	mov r12, #0
	mcr p15, 0, r12, c7, c5, 0
	mov r12, #0
	mcr p15, 0, r12, c7, c5, 6
	dsb
	isb
#endif

	// enable L1 Cache
	mrc p15, 0, r0, c1, c0, 0	;@ r0 = System Control Register
	orr r0, #0x0004			;@ Data Cache (Bit 2)
	orr r0, #0x0800			;@ Branch Prediction (Bit 11)
	orr r0, #0x1000			;@ Instruction Caches (Bit 12)
	mcr p15, 0, r0, c1, c0, 0	;@ System Control Register = R0

#if !defined(AZO_CHIP_BCM2835) && !defined(AZO_CPU_SINGLE)
	// core branch
	mrc p15, 0, r0, c0, c0, 5
	and r0, r0, #0x3
	cmp r0, #1
	beq reset_c1
	cmp r0, #2
	beq reset_c2
	cmp r0, #3
	beq reset_c3
#endif

#if defined(AZO_CHIP_BCM2835) || defined(AZO_CHIP_HYP)
	// copy vector
	mov r0,#0x8000
	mov r1,#0x0000
	ldmia r0!,{r2-r9}
	stmia r1!,{r2-r9}
	ldmia r0!,{r2-r9}
	stmia r1!,{r2-r9}
#endif

#ifdef AZO_CHIP_HYP
	mov r1, #0	// start address
	// Core 1 wakeup (WPI sleeping -> interrupt)
	ldr r0, =0x4000009C	// Core 1 Mailbox 3 write-set
	str r1, [r0]
	// Core 2 wakeup (WPI sleeping -> interrupt)
	ldr r0, =0x400000AC	// Core 2 Mailbox 3 write-set
	str r1, [r0]
	// Core 3 wakeup (WPI sleeping -> interrupt)
	ldr r0, =0x400000BC	// Core 3 Mailbox 3 write-set
	str r1, [r0]
#endif

	// set IRQ stack
	mov r0, #0xD2
	msr cpsr_c, r0	;@ (PSR_IRQ_MODE|PSR_FIQ_DIS|PSR_IRQ_DIS)
	mov sp, #0x8000
	// set FIQ stack
	mov r0, #0xD1
	msr cpsr_c, r0	;@ (PSR_FIQ_MODE|PSR_FIQ_DIS|PSR_IRQ_DIS)
	mov sp, #0x4000
	// set SVC stack
	mov r0, #0xD3
	msr cpsr_c, r0	;@ (PSR_SVC_MODE|PSR_FIQ_DIS|PSR_IRQ_DIS)
	ldr r0, =0x8000000
	mov sp, r0

	bl notmain
hang:	b hang

reset_c1:
	// set IRQ stack
	mov r0, #0xD2
	msr cpsr_c, r0	;@ (PSR_IRQ_MODE|PSR_FIQ_DIS|PSR_IRQ_DIS)
	mov sp, #0x7000
	// set FIQ stack
	mov r0, #0xD1
	msr cpsr_c, r0	;@ (PSR_FIQ_MODE|PSR_FIQ_DIS|PSR_IRQ_DIS)
	mov sp, #0x3000
	// set SVC stack
	mov r0, #0xD3
	msr cpsr_c, r0	;@ (PSR_SVC_MODE|PSR_FIQ_DIS|PSR_IRQ_DIS)
	ldr r0, =0x7F80000
	mov sp, r0

	bl notmain
	b hang

reset_c2:
	// set IRQ stack
	mov r0, #0xD2
	msr cpsr_c, r0	;@ (PSR_IRQ_MODE|PSR_FIQ_DIS|PSR_IRQ_DIS)
	mov sp, #0x6000
	// set FIQ stack
	mov r0, #0xD1
	msr cpsr_c, r0	;@ (PSR_FIQ_MODE|PSR_FIQ_DIS|PSR_IRQ_DIS)
	mov sp, #0x2000
	// set SVC stack
	mov r0, #0xD3
	msr cpsr_c, r0	;@ (PSR_SVC_MODE|PSR_FIQ_DIS|PSR_IRQ_DIS)
	ldr r0, =0x7F00000
	mov sp, r0

	bl notmain
	b hang

reset_c3:
	// set IRQ stack
	mov r0, #0xD2
	msr cpsr_c, r0	;@ (PSR_IRQ_MODE|PSR_FIQ_DIS|PSR_IRQ_DIS)
	mov sp, #0x5000
	// set FIQ stack
	mov r0, #0xD1
	msr cpsr_c, r0	;@ (PSR_FIQ_MODE|PSR_FIQ_DIS|PSR_IRQ_DIS)
	mov sp, #0x1000
	// set SVC stack
	mov r0, #0xD3
	msr cpsr_c, r0	;@ (PSR_SVC_MODE|PSR_FIQ_DIS|PSR_IRQ_DIS)
	ldr r0, =0x7E80000
	mov sp, r0

	bl notmain
	b hang

;@-------------------------------------------------------------------------
irq:
	push {r0-r12, lr}
	bl c_irq_handler
	pop  {r0-r12, lr}
	subs pc, lr, #4

;@-------------------------------------------------------------------------
.text
;@ void dummy(void);
	.globl dummy
dummy:
	mov r0, r0
	bx lr

;@-------------------------------------------------------------------------
.text
;@ uint32_t arm_get_coreid(void);
	.globl arm_get_coreid
arm_get_coreid:
#ifdef AZO_CHIP_BCM2835
	mov r0, #0
#else
	mrc p15, 0, r0, c0, c0, 5
	and r0, #0x3
#endif	// AZO_CHIP_BCM2835
    bx lr

;@-------------------------------------------------------------------------
.text
;@ void arm_irq_enable(void);
	.globl arm_irq_enable
arm_irq_enable:
	mrs r0, cpsr
	bic r0, r0, #0x80
	msr cpsr_c, r0
	bx lr

;@-------------------------------------------------------------------------
.text
;@ void arm_irq_disable(void);
	.globl arm_irq_disable
arm_irq_disable:
	mrs r0, cpsr
	orr r0, r0, #0x80
	msr cpsr_c, r0
	bx lr

;@-------------------------------------------------------------------------
.text
;@ void arm_bp_enable(void);
	.globl arm_bp_enable
arm_bp_enable:
	push {lr}
	bl arm_bp_invalidate
	pop {lr}
	mrc p15, 0, r0, c1, c0, 0
	orr r0, r0, #0x800	;@ Z: bp
	mcr p15, 0, r0, c1, c0, 0
#ifdef AZO_CHIP_BCM2835
	mcr p15, 0, r0, c7, c9, 4
	mcr p15, 0, r0, c7, c10, 4
#else
	isb
	dsb
#endif	// AZO_CHIP_BCM2835
arm_bp_enable_end:
	bx lr

;@-------------------------------------------------------------------------
.text
;@ void arm_bp_disable(void);
	.globl arm_bp_disable
arm_bp_disable:
	push {lr}
	bl arm_bp_invalidate
	pop {lr}
	mrc p15, 0, r0, c1, c0, 0
	bic r0, r0, #0x800	;@ Z: bp
	mcr p15, 0, r0, c1, c0, 0
#ifdef AZO_CHIP_BCM2835
	mcr p15, 0, r0, c7, c9, 4
	mcr p15, 0, r0, c7, c10, 4
#else
	isb
	dsb
#endif	// AZO_CHIP_BCM2835
arm_bp_disable_end:
	bx lr

;@-------------------------------------------------------------------------
.text
;@ void arm_icache_enable(void);
	.globl arm_icache_enable
arm_icache_enable:
	push {lr}
	bl arm_icache_invalidate
	pop {lr}
	mrc p15, 0, r0, c1, c0, 0
	orr r0, r0, #0x1000	;@ I: instruction
	mcr p15, 0, r0, c1, c0, 0
#ifdef AZO_CHIP_BCM2835
	mcr p15, 0, r0, c7, c9, 4
#else
	isb
#endif	// AZO_CHIP_BCM2835
	bx lr

;@-------------------------------------------------------------------------
.text
;@ void arm_icache_disable(void);
	.globl arm_icache_disable
arm_icache_disable:
	push {lr}
	bl arm_icache_invalidate
	pop {lr}
	mrc p15, 0, r0, c1, c0, 0
	bic r0, r0, #0x1000	;@ I: instruction
	mcr p15, 0, r0, c1, c0, 0
#ifdef AZO_CHIP_BCM2835
	mcr p15, 0, r0, c7, c9, 4
#else
	isb
#endif	// AZO_CHIP_BCM2835
	bx lr

;@-------------------------------------------------------------------------
.text
;@ void arm_dcache_enable(void);
	.globl arm_dcache_enable
arm_dcache_enable:
	push {lr}
	bl arm_dcache_clean_invalidate
	pop {lr}
	mrc p15, 0, r0, c1, c0, 0
	orr r0, r0, #0x4	;@ C: data
	mcr p15, 0, r0, c1, c0, 0
#ifdef AZO_CHIP_BCM2835
	mcr p15, 0, r0, c7, c10, 4
#else
	dsb
#endif	// AZO_CHIP_BCM2835
	bx lr

;@-------------------------------------------------------------------------
.text
;@ void arm_dcache_disable(void);
	.globl arm_dcache_disable
arm_dcache_disable:
	push {lr}
	bl arm_dcache_clean_invalidate
	pop {lr}
	mrc p15, 0, r0, c1, c0, 0
	bic r0, r0, #0x4	;@ C: data
	mcr p15, 0, r0, c1, c0, 0
#ifdef AZO_CHIP_BCM2835
	mcr p15, 0, r0, c7, c10, 4
#else
	dsb
#endif	// AZO_CHIP_BCM2835
	bx lr

;@-------------------------------------------------------------------------
.text
;@ void arm_acquire_lock(volatile void* lock);
	.globl arm_acquire_lock
arm_acquire_lock:
	mov r1, #1
#ifdef AZO_CHIP_BCM2835
	mov r0, #0
	mcr p15, 0, r0, c7, c10, 5
	mcr p15, 0, r0, c7, c10, 4
#else
	dmb
	dsb
#endif	// AZO_CHIP_BCM2835
	ldrex r2, [r0]		;@ set exclusive tag
	cmp r2, #0
	bne arm_acquire_lock
	strex r2, r1, [r0]	;@ get exclusive access
	cmp r2, #0
	bne arm_acquire_lock
#ifdef AZO_CHIP_BCM2835
	mcr p15, 0, r0, c7, c10, 5
	mcr p15, 0, r0, c7, c10, 4
#else
	dmb
	dsb
#endif	// AZO_CHIP_BCM2835
	bx lr

;@-------------------------------------------------------------------------
.text
;@ void arm_release_lock(volatile void* lock);
	.globl arm_release_lock
arm_release_lock:
	mov r1, #0
	str r1, [r0]
#ifdef AZO_CHIP_BCM2835
	mcr p15, 0, r0, c7, c10, 5
	mcr p15, 0, r0, c7, c10, 4
#else
	dmb
	dsb
#endif	// AZO_CHIP_BCM2835
	bx lr

;@-------------------------------------------------------------------------
.text
;@ void arm_smp_enable(void);
	.globl arm_smp_enable
arm_smp_enable:
	mrc p15, 0, r0, c1, c0, 1	;@ set SMP bit in ACTLR
	orr r0, r0, #0x40
	mcr p15, 0, r0, c1, c0, 1
	bx lr

;@-------------------------------------------------------------------------
.text
;@ void arm_smp_disable(void);
	.globl arm_smp_disable
arm_smp_disable:
	mrc p15, 0, r0, c1, c0, 1	;@ clear SMP bit in ACTLR
	bic r0, r0, #0x40
	mcr p15, 0, r0, c1, c0, 1
	bx lr

;@-------------------------------------------------------------------------
.text
;@ void arm_icache_invalidate(void);
	.globl arm_icache_invalidate
arm_icache_invalidate:
	mrc p15, 0, r0, c1, c0, 0	;@ SCTLR
	tst r0, #0x1000
	beq arm_icache_invalidate_end
	mov r0, #0
	mcr p15, 0, r0, c7, c5, 0	;@ invalidate instruction caches
#ifdef AZO_CHIP_BCM2835
	mcr p15, 0, r0, c7, c9, 4
#else
	isb
#endif	// AZO_CHIP_BCM2835
arm_icache_invalidate_end:
	bx lr

;@-------------------------------------------------------------------------
.text
;@ void arm_dcache_invalidate(void);
	.globl arm_dcache_invalidate
arm_dcache_invalidate:
	mrc p15, 0, r0, c1, c0, 0	;@ SCTLR
	tst r0, #0x4
	beq arm_dcache_invalidate_end
	mov r0, #0
	mcr p15, 0, r0, c7, c6, 0	;@ invalidate data caches
#ifdef AZO_CHIP_BCM2835
	mcr p15, 0, r0, c7, c10, 4
#else
	dsb
#endif	// AZO_CHIP_BCM2835
arm_dcache_invalidate_end:
	bx lr

;@-------------------------------------------------------------------------
.text
;@ void arm_dcache_clean(void);
	.globl arm_dcache_clean
arm_dcache_clean:
	mrc p15, 0, r0, c1, c0, 0	;@ SCTLR
	tst r0, #0x4
	beq arm_dcache_clean_end
	mov r0, #0
	mcr p15, 0, r0, c7, c10, 0	;@ clean data caches
#ifdef AZO_CHIP_BCM2835
	mcr p15, 0, r0, c7, c10, 4
	mcr p15, 0, r0, c7, c10, 5
#else
	dsb
	dmb
#endif	// AZO_CHIP_BCM2835
arm_dcache_clean_end:
	bx lr

;@-------------------------------------------------------------------------
.text
;@ void arm_dcache_clean_invalidate(void);
	.globl arm_dcache_clean_invalidate
arm_dcache_clean_invalidate:
	mrc p15, 0, r0, c1, c0, 0	;@ SCTLR
	tst r0, #0x4
	beq arm_dcache_clean_invalidate_end
	mov r0, #0
	mcr p15, 0, r0, c7, c14, 0	;@ clean and invalidate data caches
#ifdef AZO_CHIP_BCM2835
	mcr p15, 0, r0, c7, c10, 4
	mcr p15, 0, r0, c7, c10, 5
#else
	dsb
	dmb
#endif	// AZO_CHIP_BCM2835
arm_dcache_clean_invalidate_end:
	bx lr

;@-------------------------------------------------------------------------
.text
;@ void arm_cache_invalidate(void);
	.globl arm_cache_invalidate
arm_cache_invalidate:
	mrc p15, 0, r0, c1, c0, 0	;@ SCTLR
	tst r0, #0x4
	beq arm_cache_invalidate_end
	mov r0, #0
	mcr p15, 0, r0, c7, c7, 0	;@ invalidate unified caches
#ifdef AZO_CHIP_BCM2835
	mcr p15, 0, r0, c7, c10, 4
	mcr p15, 0, r0, c7, c9, 4
#else
	dsb
	isb
#endif	// AZO_CHIP_BCM2835
arm_cache_invalidate_end:
	bx lr

;@-------------------------------------------------------------------------
.text
;@ void arm_bp_invalidate(void);
	.globl arm_bp_invalidate
arm_bp_invalidate:
	mrc p15, 0, r0, c1, c0, 0	;@ SCTLR
	tst r0, #0x800
	beq arm_bp_invalidate_end
	mov r0, #0
	mcr p15, 0, r0, c7, c5, 6	;@ invalidate branch prediction
#ifdef AZO_CHIP_BCM2835
	mcr p15, 0, r0, c7, c9, 4
	mcr p15, 0, r0, c7, c10, 4
#else
	isb
	dsb
#endif	// AZO_CHIP_BCM2835
arm_bp_invalidate_end:
	bx lr

;@-------------------------------------------------------------------------
.text
;@ void arm_tlb_invalidate(void);
	.globl arm_tlb_invalidate
arm_tlb_invalidate:
	mov r0, #0
	mcr p15, 0, r0, c8, c7, 0	;@ invalidate tlb
#ifdef AZO_CHIP_BCM2835
	mov r0, #0
	mcr p15, 0, r0, c7, c9, 4
#else
	isb
#endif	// AZO_CHIP_BCM2835
	bx lr

;@-------------------------------------------------------------------------
.text
;@ void arm_alignment_enable(void);
	.globl arm_alignment_enable
arm_alignment_enable:
	mrc p15, 0, r0, c8, c7, 0	;@ SCTLR
	orr r0, #0x2
	mcr p15, 0, r0, c8, c7, 0
#ifdef AZO_CHIP_BCM2835
	mcr p15, 0, r0, c7, c9, 4
#else
	isb
#endif	// AZO_CHIP_BCM2835
	bx lr

;@-------------------------------------------------------------------------
.text
;@ void arm_alignment_disable(void);
	.globl arm_alignment_disable
arm_alignment_disable:
	mrc p15, 0, r0, c8, c7, 0	;@ SCTLR
	bic r0, #0x2
	mcr p15, 0, r0, c8, c7, 0
#ifdef AZO_CHIP_BCM2835
	mcr p15, 0, r0, c7, c9, 4
#else
	isb
#endif	// AZO_CHIP_BCM2835
	bx lr

;@-------------------------------------------------------------------------
.text
;@ void arm_mmu_reset(void);
	.globl arm_mmu_reset
arm_mmu_reset:
	push {lr}
	bl arm_dcache_clean_invalidate
	bl arm_icache_invalidate
	bl arm_bp_invalidate
	pop {lr}
	ldr r0, =0x00C50878
	mcr p15, 0, r0, c8, c7, 0
#ifdef AZO_CHIP_BCM2835
	mcr p15, 0, r0, c7, c9, 4
	mcr p15, 0, r0, c7, c10, 4
	mcr p15, 0, r0, c7, c10, 5
#else
	isb
	dsb
	dmb
#endif	// AZO_CHIP_BCM2835
	bx lr

;@-------------------------------------------------------------------------
.text
;@ void arm_mmu_init(volatile uint32_t* tlb);
	.globl arm_mmu_init
arm_mmu_init:
	push {r0,lr}
	bl arm_dcache_clean_invalidate
	bl arm_icache_invalidate
	bl arm_bp_invalidate
	pop {r0,lr}
	ldr r1, =0x55555555
	mcr p15, 0, r1, c3, c0, 0	;@ setup domains (CP15 c3)
					;@ Write Domain Access Control Register
					;@ use access permissions from TLB entry
	push {r0,lr}
	bl arm_tlb_invalidate
	pop {r0,lr}
	mov r1, #0
	mcr p15, 0, r1, c2, c0, 2	;@ always use TTBR0
	orr r0, r0, #0x4A
	mcr p15, 0, r0, c2, c0, 0	;@ set TTBR0 (page table walk inner and outer write-back,
					;@ write-allocate, cacheable, shareable memory)
#ifdef AZO_CHIP_BCM2835
	mcr p15, 0, r0, c7, c9, 4
#else
	isb
#endif	// AZO_CHIP_BCM2835
	mrc p15, 0, r0, c1, c0, 0	;@ enable MMU in SCTLR
	ldr r1, =0x73027807
	and r0, r0, r1
	ldr r1, =0x20000001
	orr r0, r0, r1
	mcr p15, 0, r0, c1, c0, 0
	bx lr

;@-------------------------------------------------------------------------
.text
;@ void arm_data_memorybarrier(void);
	.globl arm_data_memorybarrier
arm_data_memorybarrier:
#ifdef AZO_CHIP_BCM2835
	mcr p15, 0, r0, c7, c9, 4
	mcr p15, 0, r0, c7, c10, 4
	mcr p15, 0, r0, c7, c10, 5
#else
	isb
	dsb
	dmb
#endif	// AZO_CHIP_BCM2835
	bx lr

;@-------------------------------------------------------------------------
.text
;@ void arm_instruction_syncbarrier(void);
	.globl arm_instruction_syncbarrier
arm_instruction_syncbarrier:
#ifdef AZO_CHIP_BCM2835
	mcr p15, 0, r0, c7, c9, 4
#else
	isb
#endif	// AZO_CHIP_BCM2835
	bx lr

;@-------------------------------------------------------------------------
.text
;@ void arm_drain_write_buffer(void);
	.globl arm_drain_write_buffer
arm_drain_write_buffer:
	mov r0, #0
	mcr p15, 0, r0, c7, c10, 4
	bx lr

;@-------------------------------------------------------------------------
.text
;@ memcpy
;@ void azo_memcpy(void* dst, void* src, uint32_t len);
	.globl azo_memcpy
azo_memcpy:
	cmp r2, #0
	bxeq lr
	add r3, r1, r2
	bxlo lr
	sub r3, r0, r1
	tst r3, #3
	bne azo_memcpy_5
	tst r0, #3
	beq azo_memcpy_2
azo_memcpy_1:
	ldr r3, [r1], #1
	str r3, [r0], #1
	tst r0, #3
	bne azo_memcpy_1
azo_memcpy_2:
	and r3, r2, #3
	sub r2, r2, r3
	cmp r2, #4
	blo azo_memcpy_3
	push {r0-r3, lr}
	bl azo_memcpy_a4
	pop {r0-r3, lr}
	cmp r3, #0
	beq azo_memcpy_12
azo_memcpy_3:
	add r3, r2, r3
	add r0, r0, r2
	add r1, r1, r2
azo_memcpy_4:
	ldrb r2, [r1], #1
	strb r2, [r0], #1
	cmp r1, r3
	blo azo_memcpy_4
	b azo_memcpy_12
azo_memcpy_5:
	tst r0, #3
	beq azo_memcpy_7
azo_memcpy_6:
	ldrb r3, [r1], #1
	strb r3, [r0], #1
	sub r2, r2, #1
	cmp r2, #0
	beq azo_memcpy_12
	tst r0, #3
	bne azo_memcpy_6
azo_memcpy_7:
	add r3, r1, r2
	cmp r2, #4
	blo azo_memcpy_11
	sub r3, r1, #4
	push {r4-r11, lr}
azo_memcpy_8:
	mov r4, #0
	mov r5, #24
	mov r6, #0
azo_memcpy_9:
	ldrb r4, [r1], #1
	lsl r4, r4, #24
	lsr r4, r4, r5
	orr r6, r4, r6
	cmp r6, #0
	streq r5, [r0], #4
	cmp r1, r3
	bhs azo_memcpy_10
	cmp r5, #0
	beq azo_memcpy_8
	sub r5, r5, #8
	b azo_memcpy_9
azo_memcpy_10:
	pop {r4-r11, lr}
	add r3, r3, #4
azo_memcpy_11:
	ldrb r2, [r1], #1
	strb r2, [r0], #1
	cmp r1, r3
	blo azo_memcpy_11
azo_memcpy_12:
	bx lr

;@-------------------------------------------------------------------------
.text
;@ memcpy (align by 4 only)
;@ void azo_memcpy_a4(void* dst_a4, void* src_a4, uint32_t len_a4);
	.globl azo_memcpy_a4
azo_memcpy_a4:
	cmp r2, #0
	bxeq lr
	tst r0, #3
	bxne lr
	tst r1, #3
	bxne lr
	tst r2, #3
	bxne lr
	add r3, r1, r2
	bxlo lr
azo_memcpy_a4_1:
#ifdef AZO_MEM_VFP
	cmp r2, #0xB0
#else
	cmp r2, #0x30
#endif	// AZO_MEM_VFP
	blo azo_memcpy_a4_3
	push {r4-r11, lr}
	add r3, r0, r2
	ldr r12, =azo_memcpy_sp
	mrc p15, 0, r3, c0, c0, 5
	and r3, r3, #3
	lsl r3, r3, #3
	add r12, r3, r12
#ifdef AZO_MEM_VFP
	sub r2, r3, #0xB0
	mrc p15, 0, r3, c1, c0, 2
	mov r4, #0xF
	lsl r4, r4, #20
	mcr p15, 0, r4, c1, c0, 2
	vmrs r4, fpexc
	and r4, r4, #0x40000000
	orr r3, r3, r4
	str r3, [r12], #4
	orr r4, r4, #0x40000000
	vmsr fpexc, r4
	vpush {d8-d15}
#else
	sub r2, r3, #0x30
#endif	// AZO_MEM_VFP
	str sp, [r12]
azo_memcpy_a4_2:
	ldmia r1!, {r3-r14}
	stmia r0!, {r3-r14}
#ifdef AZO_MEM_VFP
	vldmia r1!, {d0-d15}
	vstmia r0!, {d0-d15}
#endif	// AZO_MEM_VFP
	cmp r1, r2
	blo azo_memcpy_a4_2
	ldr r12, =azo_memcpy_sp
	mrc p15, 0, r3, c0, c0, 5
	and r3, r3, #3
	lsl r3, r3, #3
	add r12, r3, r12
	ldr sp, [r12]
#ifdef AZO_MEM_VFP
	vpop {d8-d15}
	ldr r3, [r12, #4]
	and r4, r3, #0x40000000
	vmsr fpexc, r4
	bic r3, r3, #0x40000000
	mcr p15, 0, r3, c1, c0, 2
#endif	// AZO_MEM_VFP
	pop {r4-r11, lr}
azo_memcpy_a4_3:
	ldr r3, [r1], #4
	str r3, [r0], #4
	cmp r1, r2
	blo azo_memcpy_a4_3
	bx lr

.bss
azo_memcpy_sp:
	.long 0, 0, 0, 0, 0, 0, 0, 0

;@-------------------------------------------------------------------------
.text
;@ memset
;@ void azo_memset(void* dst, uint8_t val, uint32_t len);
	.globl azo_memset
azo_memset:
	cmp r2, #0
	bxeq lr
	add r3, r0, r2
	bxlo lr
azo_memset_1:
	tst r0, #3
	beq azo_memset_2
	strb r1, [r0], #1
	sub r2, r2, #1
	beq azo_memset_6
	b azo_memset_1
azo_memset_2:
	and r1, r1, #0xFF
	mov r3, r1
	lsl r3, r3, #8
	orr r1, r1, r3
	mov r3, r1
	lsl r3, r3, #16
	orr r1, r1, r3
#ifdef AZO_MEM_VFP
	cmp r2, #0xB4
#else
	cmp r2, #0x34
#endif	// AZO_MEM_VFP
	blo azo_memset_4
	push {r4-r11, lr}
	add r3, r0, r2
	ldr r12, =azo_memset_sp
	mrc p15, 0, r3, c0, c0, 5
	and r3, r3, #3
	lsl r3, r3, #3
	add r12, r3, r12
#ifdef AZO_MEM_VFP
	sub r2, r3, #0xB4
	mrc p15, 0, r3, c1, c0, 2
	mov r4, #0xF
	lsl r4, r4, #20
	mcr p15, 0, r4, c1, c0, 2
	vmrs r4, fpexc
	and r4, r4, #0x40000000
	orr r3, r3, r4
	str r3, [r12, #4]
	orr r4, r4, #0x40000000
	vmsr fpexc, r4
	vpush {d8-d15}
#else
	sub r2, r3, #0x34
#endif	// AZO_MEM_VFP
	str sp, [r12]
	mov r3, r1
	mov r4, r1
	mov r5, r3
	mov r6, r1
	mov r7, r3
	mov r8, r4
	mov r9, r5
	mov r10, r1
	mov r11, r3
	mov r12, r4
	mov r13, r5
	mov r14, r6
#ifdef AZO_MEM_VFP
	vmov d0, r7, r8
	vmov.f64 d1, d0
	vmov.f64 d2, d0
	vmov.f64 d3, d1
	vmov.f64 d4, d0
	vmov.f64 d5, d1
	vmov.f64 d6, d2
	vmov.f64 d7, d3
	vmov.f64 d8, d0
	vmov.f64 d9, d1
	vmov.f64 d10, d2
	vmov.f64 d11, d3
	vmov.f64 d12, d4
	vmov.f64 d13, d5
	vmov.f64 d14, d6
	vmov.f64 d15, d7
#endif	// AZO_MEM_VFP
azo_memset_3:
	stmia r0!, {r1, r3-r14}
#ifdef AZO_MEM_VFP
	vstmia r0!, {d0-d15}
#endif	// AZO_MEM_VFP
	cmp r0, r2
	blo azo_memset_3
	cmp r0, r2
	bne azo_memset_4
	stmia r0!, {r1, r3-r14}
#ifdef AZO_MEM_VFP
	vstmia r0!, {d0-d15}
#endif	// AZO_MEM_VFP
azo_memset_4:
	ldr r12, =azo_memset_sp
	mrc p15, 0, r3, c0, c0, 5
	and r3, r3, #3
	lsl r3, r3, #3
	add r12, r3, r12
	ldr sp, [r12]
#ifdef AZO_MEM_VFP
	vpop {d8-d15}
	ldr r3, [r12, #4]
	and r4, r3, #0x40000000
	vmsr fpexc, r4
	bic r3, r3, #0x40000000
	mcr p15, 0, r3, c1, c0, 2
#endif	// AZO_MEM_VFP
	pop {r4-r11, lr}
#ifdef AZO_MEM_VFP
	add r2, r2, #0xB4
#else
	add r2, r2, #0x34
#endif	// AZO_MEM_VFP
	cmp r0, r2
	bhs azo_memset_5
	sub r3, r2, r0
	cmp r3, #4
	blo azo_memset_6
	sub r2, r2, #4
azo_memset_5:
	str r1, [r0], #4
	cmp r0, r2
	blo azo_memset_5
	add r2, r2, #4
	cmp r0, r2
	beq azo_memset_7
azo_memset_6:
	strb r1, [r0], #1
	cmp r0, r2
	blo azo_memset_6
azo_memset_7:
	bx lr

.bss
azo_memset_sp:
	.long 0, 0, 0, 0, 0, 0, 0, 0

;@-------------------------------------------------------------------------
.text
;@ get cpuclock
;@ uint32_t rp_get_cpuclock(void);
	.globl rp_get_cpuclock
rp_get_cpuclock:
	push {r4-r11, lr}
	mrs r8, cpsr
	mov r1, r8
	bic r1, r1, #0xC
	msr cpsr_c, r1			;@ lock
	mrc p15, 0, r12, c15, c12, 0	;@ performance monitor control
	tst r12, #1
	bne rp_get_cpuclock_1
	mov r0, #1
	mcr p15, 0, r0, c15, c12, 0	;@ performance monitor counter enable
rp_get_cpuclock_1:
	ldr r11, =RP_STIMER_CS	;@ system timer(1us)
	ldr r10, [r11]
	tst r10, #2
	bne rp_get_cpuclock_2
	mov r0, #2
	str r0, [r11]			;@ system timer enable
rp_get_cpuclock_2:
	ldr r9, =RP_STIMER_CLO	;@ system timer counter
	ldr r0, [r9]
rp_get_cpuclock_3:
	ldr r1, [r9]
	cmp r0, r1
	beq rp_get_cpuclock_3	;@ wait for start
	mrc p15, 0, r2, c15, c12, 1	;@ start value
rp_get_cpuclock_4:
	ldr r0, [r9]
	cmp r0, r1
	beq rp_get_cpuclock_4	;@ wait for end
	mrc p15, 0, r3, c15, c12, 1	;@ end value
	mov r0, #0
	tst r10, #2
	strne r0, [r11]			;@ system timer disable
	tst r12, #1
	mcrne p15, 0, r1, c15, c12, 0	;@ performance monitor counter disable
	mov r1, #0xFF
	and r1, r1, r8
	msr cpsr_c, r1			;@ unlock
	sub r0, r3, r2			;@ end value - start value
	tst r12, #8
	lslne r0, r0, #6		;@ x64
	pop {r4-r11, lr}
	bx lr

;@-------------------------------------------------------------------------
.text
;@ uint32 div
;@ uint32_t azo_uint32_div(uint32_t val, uint32_t div);
	.globl azo_uint32_div
azo_uint32_div:
	cmp r1, #0
	moveq r0, #0
	bxeq lr
	cmp r1, #1
	moveq r1, #0
	bxeq lr
	cmp r0, r1
	movlo r0, #0
	bxlo lr
	cmp r1, #2
	andeq r1, r0, #1
	lsreq r0, r0, #1
	bxeq lr
	mov r12, #0
	mov r2, #1
azo_uint32_div_1:
	lsl r2, r2, #1
	tst r2, #0x80000000
	bne azo_uint32_div_2
	mul r3, r1, r2
	tst r3, #0x80000000
	bne azo_uint32_div_2
	cmp r3, r0
	blo azo_uint32_div_1
	lsr r2, r2, #1
	mul r3, r1, r2
azo_uint32_div_2:
	cmp r0, r3
	blo azo_uint32_div_3
	sub r0, r0, r3
	add r12, r12, r2
azo_uint32_div_3:
	lsrs r2, r2, #1
	beq azo_uint32_div_4
	mul r3, r1, r2
	b azo_uint32_div_2
azo_uint32_div_4:
	mov r1, r0		;@ mod
	mov r0, r12		;@ div
	bx lr

;@-------------------------------------------------------------------------
.text
;@ uint32 mod
;@ uint32_t azo_uint32_mod(uint32_t val, uint32_t div);
	.globl azo_uint32_mod
azo_uint32_mod:
	cmp r1, #0
	moveq r0, #0
	bxeq lr
	cmp r0, r1
	bxlo lr
	push {lr}
	bl azo_uint32_div
	pop {lr}
	mov r0, r1
	bx lr

;@-------------------------------------------------------------------------
#if defined(AZO_CHIP_BCM2835)
#define BCM_CLOCK 700
#elif defined(AZO_CHIP_BCM2836) || defined(AZO_CHIP_BCM2837) || defined(AZO_CHIP_BCM2711)
#define BCM_CLOCK 600
#endif

.text
;@ nanosec wait
;@ just this function call, 1us + a little more.
;@ granularity is about 4count at 700MHz = about 2.8ns.
;@ void rp_wait_nsec(uint32_t nsec);
	.globl rp_wait_nsec
rp_wait_nsec:
	cmp r0, #0
	bxeq lr
	push {r4-r11, lr}
	mrs r11, cpsr
	mov r1, r11
	bic r1, r1, #0xC
	msr cpsr_c, r1			;@ lock
#if defined(AZO_CHIP_BCM2835)
	mrc p15, 0, r12, c15, c12, 0
#elif defined(AZO_CHIP_BCM2836) || defined(AZO_CHIP_BCM2837) || defined(AZO_CHIP_BCM2711)
	mrc p15, 0, r12, c9, c12, 0
#endif
	tst r12, #1
	bne rp_wait_nsec_1
	mov r1, #1
#if defined(AZO_CHIP_BCM2835)
	mcr p15, 0, r1, c15, c12, 0	;@ performance monitor counter enable
#elif defined(AZO_CHIP_BCM2836) || defined(AZO_CHIP_BCM2837) || defined(AZO_CHIP_BCM2711)
	mcr p15, 0, r1, c9, c12, 0	;@ performance monitor counter enable
#endif
rp_wait_nsec_1:
#if defined(AZO_CHIP_BCM2835)
	mrc p15, 0, r2, c15, c12, 1	;@ init start value
#elif defined(AZO_CHIP_BCM2836) || defined(AZO_CHIP_BCM2837) || defined(AZO_CHIP_BCM2711)
	mrc p15, 0, r2, c9, c13, 0	;@ init start value
#endif
	ldr r1, =BCM_CLOCK
	mul r0, r1, r0
	ldr r1, =1000
	bl azo_uint32_div
#if defined(AZO_CHIP_BCM2835)
	mrc p15, 0, r3, c15, c12, 1	;@ init end value
#elif defined(AZO_CHIP_BCM2836) || defined(AZO_CHIP_BCM2837) || defined(AZO_CHIP_BCM2711)
	mrc p15, 0, r3, c9, c13, 0	;@ init end value
#endif
	sub r2, r3, r2
	tst r12, #8
	lslne r0, r0, #6		;@ x64
	cmp r0, r2			;@ wait end value
	blo rp_wait_nsec_4		;@ init time >= wait end : finish
rp_wait_nsec_2:
#if defined(AZO_CHIP_BCM2835)
	mrc p15, 0, r1, c15, c12, 1	;@ wait start value
#elif defined(AZO_CHIP_BCM2836) || defined(AZO_CHIP_BCM2837) || defined(AZO_CHIP_BCM2711)
	mrc p15, 0, r1, c9, c13, 0	;@ wait start value
#endif
	mvn r2, r1
	add r2, r2, #1			;@ wait start complement
rp_wait_nsec_3:
#if defined(AZO_CHIP_BCM2835)
	mrc p15, 0, r1, c15, c12, 1	;@ current value
#elif defined(AZO_CHIP_BCM2836) || defined(AZO_CHIP_BCM2837) || defined(AZO_CHIP_BCM2711)
	mrc p15, 0, r1, c9, c13, 0	;@ current value
#endif
	tst r12, #8
	lslne r1, r1, #6		;@ x64
	add r1, r1, r2
	cmp r1, r0
	blo rp_wait_nsec_3
rp_wait_nsec_4:
	tst r12, #1
	bne rp_wait_nsec_5
	mov r1, #0
#if defined(AZO_CHIP_BCM2835)
	mcr p15, 0, r1, c15, c12, 0	;@ performance monitor counter disable
#elif defined(AZO_CHIP_BCM2836) || defined(AZO_CHIP_BCM2837) || defined(AZO_CHIP_BCM2711)
	mcr p15, 0, r1, c9, c12, 0	;@ performance monitor counter disable
#endif
rp_wait_nsec_5:
	mov r0, #0xFF
	and r0, r0, r11
	msr cpsr_c, r0			;@ unlock
	pop {r4-r11, lr}
	bx lr

;@-------------------------------------------------------------------------
.text
;@ get cpuclock(median)
;@ sample count is than 3, chop min/max. (anti-hysteresis)
;@ uint32_t rp_get_cpuclock_med(uint32_t count);
	.globl rp_get_cpuclock_med
rp_get_cpuclock_med:
	cmp r0, #0
	moveq r0, #5
	mov r1, r0
	push {r4-r11, lr}
	mrs r11, cpsr
	mov r2, r11
	bic r2, r2, #0xC
	msr cpsr_c, r2			;@ lock
#if defined(AZO_CHIP_BCM2836) || defined(AZO_CHIP_BCM2837) || defined(AZO_CHIP_BCM2711)
	mrc p15, 0, r6, c9, c12, 1	;@ Performance Monitors Count Enable Set register
	tst r6, #0x80000000
	bne rp_get_cpuclock_med_1
	mov r2, r6
	orr r2, r2, #0x80000000
	mcr p15, 0, r2, c9, c12, 1	;@ Cycle counter enabled
#endif

rp_get_cpuclock_med_1:

#if defined(AZO_CHIP_BCM2835)
	mrc p15, 0, r10, c15, c12, 0	;@ performance monitor control
#elif defined(AZO_CHIP_BCM2836) || defined(AZO_CHIP_BCM2837) || defined(AZO_CHIP_BCM2711)
	mrc p15, 0, r10, c9, c12, 0	;@ Performance Monitors Control Register
#endif
	tst r10, #1
	bne rp_get_cpuclock_med_2
	mov r2, #1
#if defined(AZO_CHIP_BCM2835)
	mcr p15, 0, r2, c15, c12, 0	;@ performance monitor counter enable
#elif defined(AZO_CHIP_BCM2836) || defined(AZO_CHIP_BCM2837) || defined(AZO_CHIP_BCM2711)
	mcr p15, 0, r2, c9, c12, 0	;@ performance monitor counter enable
#endif
rp_get_cpuclock_med_2:
	ldr r9, =RP_STIMER_CS
	ldr r8, [r9]
	tst r8, #2
	bne rp_get_cpuclock_med_3
	mov r2, #2
	str r2, [r9]			;@ system timer enable
rp_get_cpuclock_med_3:
	ldr r7, =RP_STIMER_CLO
	ldr r2, [r7]
rp_get_cpuclock_med_4:
	ldr r3, [r7]
	cmp r2, r3
	beq rp_get_cpuclock_med_4	;@ wait for start
#if defined(AZO_CHIP_BCM2835)
	mrc p15, 0, r4, c15, c12, 1	;@ start value
#elif defined(AZO_CHIP_BCM2836) || defined(AZO_CHIP_BCM2837) || defined(AZO_CHIP_BCM2711)
	mrc p15, 0, r4, c9, c13, 0	;@ start value
#endif
	mov r2, r3
rp_get_cpuclock_med_5:
	ldr r3, [r7]
	cmp r2, r3
	beq rp_get_cpuclock_med_5	;@ wait for end
#if defined(AZO_CHIP_BCM2835)
	mrc p15, 0, r5, c15, c12, 1	;@ end(next start) value
#elif defined(AZO_CHIP_BCM2836) || defined(AZO_CHIP_BCM2837) || defined(AZO_CHIP_BCM2711)
	mrc p15, 0, r5, c9, c13, 0	;@ end(next start) value
#endif
rp_get_cpuclock_med_6:
	mov r2, r3
	sub r4, r5, r4
	push {r4}
	mov r4, r5
	subs r0, r0, #1
	bne rp_get_cpuclock_med_5
	pop {r0}
	cmp r1, #1
	beq rp_get_cpuclock_med_9
	pop {r2}
	cmp r1, #2
	bne rp_get_cpuclock_med_7
	add r0, r0, r2
	lsr r0, r0, #1
	b rp_get_cpuclock_med_9
rp_get_cpuclock_med_7:
	cmp r0, r2
	movlo r3, r0
	movlo r4, r2
	movhs r3, r2
	movhs r4, r0
	add r0, r0, r2
	mov r5, #2
rp_get_cpuclock_med_8:
	pop {r2}
	add r0, r0, r2
	cmp r2, r3
	movlo r3, r2
	cmp r2, r4
	movhs r4, r2
	add r5, r5, #1
	cmp r1, r5
	bne rp_get_cpuclock_med_8
	sub r0, r0, r3
	sub r0, r0, r4
	cmp r1, #3
	beq rp_get_cpuclock_med_9
	sub r1, r1, #2
	bl azo_uint32_div
rp_get_cpuclock_med_9:
	tst r8, #2
	bne rp_get_cpuclock_med_10
	mov r1, #0
	str r1, [r9]			;@ system timer disable
rp_get_cpuclock_med_10:
	tst r10, #1
	bne rp_get_cpuclock_med_11
	mov r1, #0
#if defined(AZO_CHIP_BCM2835)
	mcr p15, 0, r1, c15, c12, 0	;@ performance monitor counter disable
#elif defined(AZO_CHIP_BCM2836) || defined(AZO_CHIP_BCM2837) || defined(AZO_CHIP_BCM2711)
	mcr p15, 0, r1, c9, c12, 0	;@ performance monitor counter disable
#endif
rp_get_cpuclock_med_11:
	tst r6, #0x80000000
	bne rp_get_cpuclock_med_12
#if defined(AZO_CHIP_BCM2836) || defined(AZO_CHIP_BCM2837) || defined(AZO_CHIP_BCM2711)
	mov r6, #0x80000000
	mcr p15, 0, r6, c9, c12, 2	;@ Performance Monitors Count Enable Clear
#endif
rp_get_cpuclock_med_12:
	and r1, r11, #0xFF
	msr cpsr_c, r1			;@ unlock
	tst r10, #8
	lslne r0, r0, #6		;@ x64
	pop {r4-r11, lr}
	bx lr

