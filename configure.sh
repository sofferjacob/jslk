#!/bin/bash
# JSLK Environment configuration script
# First let's clean up, just in case
rm -rfv sysroot
mkdir sysroot
mkdir -p sysroot/usr/include
mkdir sysroot/usr/lib
# Now let's install the headers
cp lib/jstdclib/include/*.h sysroot/usr/include
cp include/*h sysroot/usr/include

