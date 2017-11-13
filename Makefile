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

CFLAGS=-nostdlib -nostdinc -fno-builtin -I include -I lib/jstdclib/include $(TARGET_CFLAGS)
CPPFLAGS=-nostdlib -nostdinc -fno-builtin -I include
LDFLAGS=-Tlink.ld
ASFLAGS=-felf

include lib/jstdclib/make.config

SOURCES= $(CRTI_OBJ) $(CRTBEGIN_OBJ) $(TARGET_SOURCES) kernel/jslk.o \
 kernel/timer.o  $(JSTDC_SOURCES) \
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

floppy:
	echo Building floppy image...
	hdiutil attach system.img
	cp jslk.kernel /Volumes/NO\ NAME
	hdiutil detach /Volumes/NO\ NAME
	echo system.img floppy image has been updated.

clean:
	rm -rf hal/x86/*.o hal/x86_64/*.d hal/x86_64/*.o kernel/*.o crt/*.o crt/*.d lib/jstdclib/*.o \
	hal/x86/drivers/*.o hal/x86/memory/*.o hal/x86/io/*.o hal/x86/interrupts/*.o hal/x86/tables/*.o
	clear

run:
	qemu-system-i386 -fda system.img

image:
	echo Updating image...
	sudo /sbin/losetup /dev/loop0 floppy.img
	sudo mount /dev/loop0 /mnt2
	sudo cp sierra.kernel /mnt2/kernel
	sudo umount /dev/loop0
	sudo /sbin/losetup -d /dev/loop0
	echo Image updated.