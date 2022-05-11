	.section .text.boot
	.global __start
	.type	__start, %function

.equ   SCTLR_ENABLE_DATA_CACHE,        0x4
.equ   SCTLR_ENABLE_BRANCH_PREDICTION, 0x800
.equ   SCTLR_ENABLE_INSTRUCTION_CACHE, 0x1000

__start:
	#  Initilize MPID/MPIDR registers for all Cores
	mrs x0, midr_el1
	mrs x1, mpidr_el1
	msr vpidr_el2, x0
	msr vmpidr_el2, x1

// https://stackoverflow.com/questions/32374599/mcr-and-mrc-does-not-exist-on-aarch64
/*
	// R0 = System Control Register
	mrc p15,0,r0,c1,c0,0
	// Enable caches and branch prediction
	orr r0,#SCTLR_ENABLE_BRANCH_PREDICTION
	orr r0,#SCTLR_ENABLE_DATA_CACHE
	orr r0,#SCTLR_ENABLE_INSTRUCTION_CACHE
	// System Control Register = R0
	mcr p15,0,r0,c1,c0,0
*/

exit_el1:
	# get cpuid, move slaves to loop
	mrs	x7, mpidr_el1
	and	x7, x7, #3
	cbz	x7, __start_master
0:	wfe
	b	0b  // jump to 0 backward

__start_master:
	# Load stack pointer
	ldr	x2, =__stack_start
	mov	sp, x2

	# Clear BSS
	ldr	w0, =__bss_start
	ldr	w1, =__bss_size
1:	cbz	x1, 2f  // jump 1 forward
	str	xzr, [x0], #8
	sub	x1, x1, #1
	cbnz	x1, 1b
2:
	bl	kernelmain	// Call main
	b	0b	// loop forever


