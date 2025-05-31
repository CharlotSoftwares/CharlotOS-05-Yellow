ISO_NAME=CharlotOS.iso

all: iso/$(ISO_NAME)

kernel/kernel.o: kernel/kernel.c
	mkdir -p kernel
	gcc -ffreestanding -m32 -c $< -o $@

boot/boot.o: boot/boot.asm
	nasm -f elf32 $< -o $@

kernel.bin: boot/boot.o kernel/kernel.o linker.ld
	ld -m elf_i386 -T linker.ld -o kernel.bin boot/boot.o kernel/kernel.o

iso/$(ISO_NAME): kernel.bin boot/grub.cfg
	mkdir -p iso/boot/grub
	cp kernel.bin iso/boot/
	cp boot/grub.cfg iso/boot/grub/
	grub-mkrescue -o iso/$(ISO_NAME) iso/

clean:
	rm -rf *.o *.bin iso kernel.bin kernel/*.o boot/*.o
