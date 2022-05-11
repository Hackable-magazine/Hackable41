CROSS=aarch64-none-elf-
CC=$(CROSS)gcc
CFLAGS=-Wall -Wextra -O3 -g -ffreestanding -march=armv8-a+crc -mcpu=cortex-a53
LDSCRIPT=rpi3.ld

OBJS=crt0.o armutils.o delay.o uart.o util.o mbox.o clock.o fb.o hello.o

all: hello.bin

hello.elf: $(OBJS) $(LDSCRIPT)
	$(CROSS)ld -o $@ $(OBJS) -T$(LDSCRIPT) -Map hello.map

hello.bin: hello.elf
	$(CROSS)objcopy -O binary $< $@

clean:
	rm -f $(OBJS) *.bin *.elf *.map *.lst

mrproper:
	rm -f $(OBJS) *.bin *.elf *.map *.lst *~

debug:
	$(CROSS)gdb hello.elf -ex "target extended-remote :3333" -ex "load"
