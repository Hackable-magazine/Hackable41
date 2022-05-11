	.section .text.boot
	.global __start
	.type	__start, %function

__start:
	//Are we allready in EL1 ?
	mrs x0, CurrentEL	// Current Exception Level register
	mov x0, #0x0004
	lsr x0, x0, #2		// put CurrentEL>>2 to x0 (EL, bits [3:2])
	cmp x0, #1		// x0 == 01b ?
	cbz x0, exit_el1	// yes, skip EL conf

confel1:
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

// Save all corruptible registers (x29,x30 are assumed done before call)
register_save:
	stp x27, x28, [sp, #-16]!
	stp x25, x26, [sp, #-16]!
	stp x23, x24, [sp, #-16]!
	stp x21, x22, [sp, #-16]!
	stp x19, x20, [sp, #-16]!
	stp x17, x18, [sp, #-16]!
	stp x15, x16, [sp, #-16]!
	stp x13, x14, [sp, #-16]!
	stp x11, x12, [sp, #-16]!
	stp x9, x10, [sp, #-16]!
	stp x7, x8, [sp, #-16]!
	stp x5, x6, [sp, #-16]!
	stp x3, x4, [sp, #-16]!
	stp x1, x2, [sp, #-16]!
	str x0, [sp, #-16]!
	ret

// Restore all corruptible registers
register_restore:
	ldr x0, [sp], #16
	ldp x1, x2, [sp], #16
	ldp x3, x4, [sp], #16
	ldp x5, x6, [sp], #16
	ldp x7, x8, [sp], #16
	ldp x9, x10, [sp], #16
	ldp x11, x12, [sp], #16
	ldp x13, x14, [sp], #16
	ldp x15, x16, [sp], #16
	ldp x17, x18, [sp], #16
	ldp x19, x20, [sp], #16
	ldp x21, x22, [sp], #16
	ldp x23, x24, [sp], #16
	ldp x25, x26, [sp], #16
	ldp x27, x28, [sp], #16
	ret

	// important, code has to be properly aligned
	.balign 0x800
_vectors:
	// from current EL with sp_el0
	// synchronous
	.balign 0x80
	b 0b

	// IRQ
	.balign 0x80
	b 0b

	// FIQ
	.balign 0x80
	b 0b

	// SError
	.balign 0x80
	b 0b

	// from current EL with sp_elx, x != 0
	// synchronous
	.balign 0x80
	stp x29, x30, [sp, #-16]!  // Save x30 link register and x29 just so we dont waste space
	bl register_save   // Save corruptible registers .. it assumes x29,x30 saved
	mov     x0, #0
	bl register_restore // restore corruptible registers .. does all but x29,x30
	ldp x29, x30, [sp], #16  // restore x29,x30 pulling stack back up 16
	eret

	// IRQ
	.balign  0x80
	stp x29, x30, [sp, #-16]!  // Save x30 link register and x29
	bl register_save
	bl register_restore // restore corruptible registers .. does all but x29,x30
	ldp x29, x30, [sp], #16  // restore x29,x30 pulling stack back up 16
	eret

	// FIQ
	.balign  0x80
	stp x29, x30, [sp, #-16]!  // Save x30 link register and x29 just so we dont waste space
	bl register_save   // Save corruptible registers .. it assumes x29,x30 saved
	mov     x0, #2
	bl register_restore // restore corruptible registers .. does all but x29,x30
	ldp x29, x30, [sp], #16  // restore x29,x30 pulling stack back up 16
	eret

	// SError
	.balign  0x80
	stp x29, x30, [sp, #-16]!  // Save x30 link register and x29 just so we dont waste space
	bl register_save   // Save corruptible registers .. it assumes x29,x30 saved
	mov     x0, #3
	bl register_restore // restore corruptible registers .. does all but x29,x30
	ldp x29, x30, [sp], #16  // restore x29,x30 pulling stack back up 16
	eret

	// from lower EL, target minus 1 is AARCH64
	// synchronous
	.balign 0x80
	b 0b

	// IRQ
	.balign 0x80
	b 0b

	// FIQ
	.balign 0x80
	b 0b

	// SError
	.balign 0x80
	b 0b

	// from lower EL, target minus 1 is AARCH32
	// synchronous
	.balign 0x80
	b 0b

	// IRQ
	.balign 0x80
	b 0b

	// FIQ
	.balign 0x80
	b 0b

	// SError
	.balign 0x80
	b 0b
