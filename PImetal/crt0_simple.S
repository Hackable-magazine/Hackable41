	.section .text.boot
	.global __start
	.type	__start, %function

__start:
	#  Initilize MPID/MPIDR registers for all Cores
	mrs x0, midr_el1
	mrs x1, mpidr_el1
	msr vpidr_el2, x0
	msr vmpidr_el2, x1

	# Disable coprocessor traps for all Cores
	mov x0, #0x33ff
	msr cptr_el2, x0      // Disable coprocessor traps to EL2
	msr hstr_el2, xzr      // Disable coprocessor traps to EL2
	mov x0, #3 << 20
	msr cpacr_el1, x0      // Enable FP/SIMD at EL1

/*
	# Initialize SCTLR_EL2 for all Cores
	#   RES1 bits (29,28,23,22,20,11) to 1
	#   RES0 bits (31,30,27,21,17,13,10,6) +
	#   UCI,EE,EOE,WXN,nTWE,nTWI,UCT,DZE,I,UMA,SED,ITD,
	#   CP15BEN,SA0,SA,C,A,M to 0
	mov x0, #0x0800
	movk x0, #0x30d0, lsl #16
	orr    x0, x0, #(0x1 << 2)            // The C bit on (data cache).
	orr    x0, x0, #(0x1 << 12)           // The I bit on (instruction cache)
	msr sctlr_el2, x0
*/

/*
	# Initialize HCR_EL2 so EL1 is 64 bits for all Cores
	mov x0, #(1 << 31)      // 64bit EL1
	msr hcr_el2, x0

	# Initialize SCTLR_EL1 for all Cores
	#   RES1 bits (29,28,23,22,20,11) to 1
	#   RES0 bits (31,30,27,21,17,13,10,6) +
	#   UCI,EE,EOE,WXN,nTWE,nTWI,UCT,DZE,I,UMA,SED,ITD,
	#   CP15BEN,SA0,SA,C,A,M to 0
	mov x0, #0x0800
	movk x0, #0x30d0, lsl #16
	orr    x0, x0, #(0x1 << 2)            // The C bit on (data cache).
	orr    x0, x0, #(0x1 << 12)           // The I bit on (instruction cache)
	msr sctlr_el1, x0

	# Set up exception handlers
	ldr x0, =_vectors
	msr vbar_el1, x0

	#  Return to the EL1_SP1 mode from EL2 for all Cores
	mov x0, #0x3c5       // EL1_SP1 | D | A | I | F
	msr spsr_el2, x0      // Set spsr_el2 with settings
	adr x0, exit_el1      // Address to exit EL2
	msr elr_el2, x0       // Set elevated return register
	eret         // Call elevated return
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


