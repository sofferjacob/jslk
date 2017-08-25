# Makefile for Edison Kernel
# The C and C++ rules are already setup by default.
# The only one that needs changing is the assembler
# rule, as we use nasm instead of GNU as.
DESTDIR?=
INCLUDEDIR?=include
PREFIX?=/usr/local
EXEC_PREFIX?=$(PREFIX)
BOOTDIR?=$(EXEC_PREFIX)/boot

export AR=i386-elf-ar
export AS=i386-elf-as
export CC=i386-elf-gcc
export C++=i386-elf-g++
export GDB=i386-elf-gdb
export SYSROOT=$(pwd)/sysroot

CFLAGS=-nostdlib -nostdinc -fno-builtin -I include -I lib/jstdclib/include
CPPFLAGS=-nostdlib -nostdinc -fno-builtin -I include --sysroot=$SYSROOT
LDFLAGS=-Tlink.ld
ASFLAGS=-felf

include lib/jstdclib/make.config # So we can use it in kernel mode.

SOURCES=$(JSTDC_SOURCES) hal/start.o hal/ports.o hal/modes.o hal/hal.o hal/vga.o kernel/jlk.o hal/dprint.o \
hal/idt.o hal/isr.o hal/idt_s.o hal/isr_s.o

all: $(SOURCES) link

link:
	$(CC) -v
	/Users/development/opt/cross/i386-elf/bin/ld $(LDFLAGS) -o jlk.kernel $(SOURCES)

.s.o:
	nasm $(ASFLAGS) $<

floppy:
	echo Building floppy image...
	hdiutil attach system.img
	cp jlk.kernel /Volumes/NO\ NAME
	hdiutil detach /Volumes/NO\ NAME
	echo system.img floppy image has been updated.

clean:
	rm -rf *.o hal/*.o kernel/*.o *.kernel lib/*.o
	clear

run:
	qemu-system-i386 -fda system.img