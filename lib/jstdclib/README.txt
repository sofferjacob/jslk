jstdc
======
Minimal implementation of a C Standard Library (work in progress, will add stuff as I need it).

How to use in Kernel mode
--------------------------
1. Add the library include directory path to the kernel Makefile.
2. Add the following lines to the Makefile before the 'SOURCES=' section.
```
include lib/jstdclib/make.config
SOURCES=$(JSTDC_SOURCES) ... ...
````

How to use as a Library
------------------------
1. cd to this directory in a terminal emulator.
2. Run 'make'.
3. It will produce the file "libk.a". Add it to your sysroot/ filesystem / or wherever you want and also
copy the headers to a standard include directory (in the sysroot or wherever you want to).