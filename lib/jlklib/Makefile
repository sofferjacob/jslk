CFLAGS?=-O2 -g -I include -Wall
CPPFLAGS?=
LDFLAGS?=
LIBS?=

export CC=$(HOME)/opt/cross/bin/i386-elf-gcc
export AR=$(HOME)/opt/cross/bin/i386-elf-ar
export AS=$(HOME)/opt/cross/bin/i386-elf-as
 
DESTDIR?=
PREFIX?=/usr/local
EXEC_PREFIX?=$(PREFIX)
INCLUDEDIR?=$(PREFIX)/include
LIBDIR?=$(EXEC_PREFIX)/lib
 
CFLAGS:=$(CFLAGS) -ffreestanding -Wall -Wextra
CPPFLAGS:=$(CPPFLAGS) -D__is_libc -Iinclude
LIBK_CFLAGS:=$(CFLAGS)
LIBK_CPPFLAGS:=$(CPPFLAGS) -D__is_libk
 
CFLAGS:=$(CFLAGS)
CPPFLAGS:=$(CPPFLAGS)
LIBK_CFLAGS:=$(LIBK_CFLAGS)
LIBK_CPPFLAGS:=$(LIBK_CPPFLAGS)
 
FREEOBJS=\
string.o
 
HOSTEDOBJS=\
$(ARCH_HOSTEDOBJS) \
 
OBJS=\
$(FREEOBJS) \
$(HOSTEDOBJS) \
 
 
#BINARIES=libc.a libk.a # Not ready for libc yet.
BINARIES=jlklib.a
 
.PHONY: all clean install install-headers install-libs
.SUFFIXES: .o .jlklib.o .c .S
 
all: $(BINARIES)
 
libc.a: $(OBJS)
	$(AR) rcs $@ $(OBJS)
 
jlklib.a: $(LIBK_OBJS)
	$(AR) rcs $@ $(FREEOBJS)
 
.c.o:
	$(CC) -MD -c $< -o $@ -std=gnu11 $(CFLAGS) $(CPPFLAGS)
 
.c.S:
	$(CC) -MD -c $< -o $@ $(CFLAGS) $(CPPFLAGS)
 
.c.libk.o:
	$(CC) -MD -c $< -o $@ -std=gnu11 $(LIBK_CFLAGS) $(LIBK_CPPFLAGS)
 
.S.libk.o:
	$(CC) -MD -c $< -o $@ $(LIBK_CFLAGS) $(LIBK_CPPFLAGS)
 
clean:
	rm -f $(BINARIES) *.a
	rm -f $(OBJS) $(LIBK_OBJS) *.o */*.o */*/*.o
	rm -f $(OBJS:.o=.d) $(LIBK_OBJS:.o=.d) *.d */*.d */*/*.d
 
install: install-headers install-libs
 
install-headers:
	mkdir -p $(DESTDIR)$(INCLUDEDIR)
	cp -R --preserve=timestamps include/. $(DESTDIR)$(INCLUDEDIR)/.
 
install-libs: $(BINARIES)
	mkdir -p $(DESTDIR)$(LIBDIR)
	cp $(BINARIES) $(DESTDIR)$(LIBDIR)
 
-include $(OBJS:.o=.d)
-include $(LIBK_OBJS:.o=.d)
