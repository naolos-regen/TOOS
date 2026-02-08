GPPARAMS = -m32 -nostdlib -fno-builtin -fno-exceptions -fno-leading-underscore -fno-stack-protector
ASPARAMS = --32
LDPARAMS = -melf_i386

objects = obj/loader.o obj/kernel.o obj/gdt.o obj/sd.o

obj/%.o: src/%.c
	gcc $(GPPARAMS) -o $@ -c $<

obj/%.o: src/%.s
	as $(ASPARAMS) -o $@ -c $<

boot/mykernel.bin: src/linker.ld $(objects)
	ld $(LDPARAMS) -T $< -o $@ $(objects)

qemu/mykernel.iso: boot/mykernel.bin
	mkdir -p iso/boot/grub
	cp $< iso/boot/
	echo 'set timeout=0' 							>  iso/boot/grub/grub.cfg
	echo 'set default=0' 	  						>> iso/boot/grub/grub.cfg
	echo '' 								>> iso/boot/grub/grub.cfg
	echo 'menuentry "DummyOS" {' 						>> iso/boot/grub/grub.cfg
	echo ' multiboot /boot/mykernel.bin' 					>> iso/boot/grub/grub.cfg
	echo ' boot' 								>> iso/boot/grub/grub.cfg
	echo '}' 								>> iso/boot/grub/grub.cfg
	grub-mkrescue --output=$@ iso
	rm -rf iso

install: boot/mykernel.bin
	sudo cp $< /boot/mykernel.bin

run: qemu/mykernel.iso
	qemu-system-i386 -cdrom $< -boot d
