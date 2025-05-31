ISO_NAME=CharlotOS.iso
CC=gcc
CFLAGS=-ffreestanding -m32 -Wall -Wextra -O2

# Add new filesystem source files to OBJS
OBJS=boot/boot.o kernel/kernel.o kernel/tty.o kernel/input.o kernel/fs/fs.o kernel/fs/fat12.o

all: iso/$(ISO_NAME)

# Compile kernel source files
kernel/kernel.o: kernel/kernel.c
	$(CC) $(CFLAGS) -c $< -o $@

kernel/tty.o: kernel/tty.c
	$(CC) $(CFLAGS) -c $< -o $@

kernel/input.o: kernel/input.c
	$(CC) $(CFLAGS) -c $< -o $@

kernel/fs/fs.o: kernel/fs/fs.c
	$(CC) $(CFLAGS) -c $< -o $@

kernel/fs/fat12.o: kernel/fs/fat12.c kernel/fs/fat12.h
	$(CC) $(CFLAGS) -c $< -o $@

# Assemble boot file
boot/boot.o: boot/boot.asm
	nasm -f elf32 $< -o $@

# Link all objects into kernel.bin
kernel.bin: $(OBJS) linker.ld
	ld -m elf_i386 -T linker.ld -o $@ $(OBJS)

# Build ISO with GRUB
iso/$(ISO_NAME): kernel.bin boot/grub.cfg
	mkdir -p iso/boot/grub
	cp kernel.bin iso/boot/
	cp boot/grub.cfg iso/boot/grub/
	grub-mkrescue -o $@ iso/

clean:
	rm -rf *.o *.bin iso kernel.bin kernel/*.o boot/*.o
