OUTPUT_ARCH(aarch64)
ENTRY(__start)

MEMORY
{
  RAM (rwx) : ORIGIN = 0x80000, LENGTH = 0x10000000
}

SECTIONS
{
  .text :
  {
    . = ALIGN(4);
    KEEP(*(.text.boot))
    *(.text .text.*)
    . = ALIGN(4);
  } > RAM

  .rodata :
  {
    . = ALIGN(4);
    *(.rodata .rodata.*)
    . = ALIGN(4);
  }

  /*
  .ARM.extab   : { *(.ARM.extab*) }
  PROVIDE_HIDDEN (__exidx_start = .);
  .ARM.exidx   : { *(.ARM.exidx*) }
  PROVIDE_HIDDEN (__exidx_end = .);
  */

  .data :
  {
    . = ALIGN(4);
    *(.data .data.*)
    . = ALIGN(4);
  }

  .bss (NOLOAD):
  {
    . = ALIGN(4);
    __bss_start = ALIGN(0x10);
    *(.bss .bss.*)
    __bss_end = ALIGN(0x10);
    . = ALIGN(4);
  }
  __end = .;

  . = ALIGN(0x10);
  . += 0x1000;
  __stack_start = .;

  /DISCARD/ :
  {
   *(.comment) *(.gnu*)
   *(.note*) *(.eh_frame*)
  }
}

__bss_size = (__bss_end - __bss_start) >> 3;

