# Makefile for Edison Kernel
# The C and C++ rules are already setup by default.
# The only one that needs changing is the assembler
# rule, as we use nasm instead of GNU as.
DESTDIR?=
INCLUDEDIR?=include
PREFIX?=/usr/local
EXEC_PREFIX?=$(PREFIX)
BOOTDIR?=$(EXEC_PREFIX)/boot

include target/$(TARGET)/make.config

CRTBEGIN_OBJ:=$(shell $(CC) $(CFLAGS) $(LDFLAGS) -print-file-name=crtbegin.o)
CRTEND_OBJ:=$(shell $(CC) $(CFLAGS) $(LDFLAGS) -print-file-name=crtend.o)

CFLAGS=-nostdlib -nostdinc -Wno-write-strings -fno-builtin -I include -I lib/jstdclib/include $(TARGET_CFLAGS)
CPPFLAGS=-nostdlib -nostdinc -fno-builtin -I include
LDFLAGS=-Tlink.ld
ASFLAGS=-felf

include lib/jstdclib/make.config

SOURCES= $(CRTI_OBJ) $(CRTBEGIN_OBJ) $(TARGET_SOURCES) kernel/jslk.o \
 kernel/timer.o kernel/heap.o kernel/orderedArray.o kernel/initrd.o kernel/vfs.o kernel/kern_debug/breakpoints.o $(JSTDC_SOURCES) \
crt/lsv.o crt/pvf.o  $(CRTN_OBJ) $(CRTEND_OBJ)

all: $(SOURCES) link

link:
	$(CC) -v
	$(LD) $(LDFLAGS) -o jslk.kernel $(SOURCES)

.s.o:
	nasm $(ASFLAGS) $<

.S.o:
	$(CC) -MD -c $< -o $@ $(CFLAGS)

.cc.o:
	$(CC) -c $< -o $@ $(CFLAGS) $(CPPFLAGS)

clean:
	rm -rf hal/x86/*.o hal/x86_64/*.d hal/x86_64/*.o kernel/*.o crt/*.o crt/*.d lib/jstdclib/*.o \
	hal/x86/drivers/*.o hal/x86/memory/*.o hal/x86/io/*.o hal/x86/interrupts/*.o hal/x86/tables/*.o \
	*.img *.iso
	clear

image:
	cp jslk.kernel iso/boot
	cp initrd.img iso/mods
	grub-mkrescue -o jslk.iso iso

inird:
	./make_initrd initrd/test.txt initrd/test.txt initrd/test2.txt initrd/test2.txt initrd/hpotter.txt initrd/hpotter.txt initrd/log.txt initrd/log.txt

run:
	qemu-system-i386 -cdrom jslk.iso

build:
	make clean all inird image run

# ===================================== #
#    Floppy Images are now obsolete,    #
#        Use ISO images instead         #
# ===================================== #

#run:
#	qemu-system-i386 -fda system.img

#floppy:
#	echo Building floppy image...
#	hdiutil attach system.img
#	cp jslk.kernel /Volumes/NO\ NAME
#	cp initrd.img /Volumes/NO\ NAME
#	hdiutil detach /Volumes/NO\ NAME
#	echo system.img floppy image has been updated.

#image:
#	echo Updating image...
#	sudo /sbin/losetup /dev/loop0 floppy.img
#	sudo mount /dev/loop0 /mnt2
#	sudo cp sierra.kernel /mnt2/kernel
#	sudo umount /dev/loop0
#	sudo /sbin/losetup -d /dev/loop0
#	echo Image updated.