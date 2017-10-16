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

CFLAGS=-nostdlib -nostdinc -fno-builtin -I include -I lib/jstdclib/include
CPPFLAGS=-nostdlib -nostdinc -fno-builtin -I include
LDFLAGS=-Tlink.ld
ASFLAGS=-felf

CRTI_OBJ:=crt/crti.o
CRTBEGIN_OBJ:=$(shell $(CC) $(CFLAGS) $(LDFLAGS) -print-file-name=crtbegin.o)
CRTEND_OBJ:=$(shell $(CC) $(CFLAGS) $(LDFLAGS) -print-file-name=crtend.o)
CRTN_OBJ:=crt/crtn.o

include lib/jstdclib/make.config

SOURCES= $(CRTI_OBJ) $(CRTBEGIN_OBJ) hal/start.o hal/ports.o hal/modes.o hal/hal.o hal/vga.o kernel/jslk.o \
hal/descriptor_tables.o hal/interrupt.o hal/isr.o hal/gdt.o hal/pit.o kernel/timer.o hal/pmm.o hal/atomical.o $(JSTDC_SOURCES) \
crt/lsv.o crt/pvf.o hal/paging.o $(CRTN_OBJ) $(CRTEND_OBJ)

all: $(SOURCES) link

link:
	$(CC) -v
	i386-elf-ld $(LDFLAGS) -o jslk.kernel $(SOURCES)

travis: $(SOURCES) linktrav

linktrav:
	$(HOME)/opt/cross/bin/i386-elf-gcc -v
	$(HOME)/opt/cross/bin/i386-elf-ld $(LDFLAGS) -o jslk.kernel $(SOURCES)
	
.s.o:
	nasm $(ASFLAGS) $<

.S.o:
	$(CC) -MD -c $< -o $@ $(CFLAGS)

.cc.o:
	$(CC) -c $< -o $@ $(CFLAGS) $(CPPFLAGS)

floppy:
	echo Building floppy image...
	hdiutil attach system.img
	cp jslk.kernel /Volumes/NO\ NAME
	hdiutil detach /Volumes/NO\ NAME
	echo system.img floppy image has been updated.

clean:
	rm -rf *.o hal/*.o  hal/*.d kernel/*.o *.kernel lib/*.o crt/*.o crt/*.d
	clear

run:
	qemu-system-i386 -fda system.img
