# JSLK Kernel

JSLK is a simple 32-bit hobby kernel designed to run on the x86 architecture (although portability has been taken into consideration). It was started in August 2017 with the objective of learning how a computer operating system works and to improve the coding skills of the main developer. The kernel is mostly written in C with some bits of assembly and has partial compatibility with C++ (a runtime is provided). Some of its features are: extensive HAL API, System V ABI compatibility, memory management, interrupt handling, VGA driver, timers and delays, some synchronization primitives, a growing home-made C library, etc. You can test the current kernel online [here.](https://sofferjacob.github.io/jslk.html)

[![Build Status](https://travis-ci.org/sofferjacob/jslk.svg?branch=master)](https://travis-ci.org/sofferjacob/jslk)
[![GitHub license](https://img.shields.io/badge/license-GPL3-yellow.svg)](https://github.com/sofferjacob/jslk/blob/master/LICENSE)
[![Project Status](https://img.shields.io/badge/project-active_development-brightgreen.svg)](https://github.com/sofferjacob/jslk/blob/x64/docs/status.md)
[![Kernel Version](https://img.shields.io/badge/kernel-v0.0.8pa-blue.svg)](https://github.com/sofferjacob/jslk/releases)

## Directory Structure
`crt`: C/C++ Runtime.

`docs`: Some documentation (writing in progress, thus incomplete).

`iso`: Filesystem used to build ISO image.

`initrd`: Files used to build the initrd.

`hal`: Hardware Abstraction Layer and API.

`include`: Public kernel headers.

`kernel`: Main kernel.

`lib`: Kernel Libraries.

`target`: Target build recipes.

## Supported Architectures
* x86 (`x86common`).
* x86_64 (`x64_common`).
> Note: JSLK is not fully ported to x86_64 architecture yet, if you want to check out the latest features build for x86 instead.
* Raspberry Pi 1/2/zero (coming soon) (`bcm28common`).

## Building for x86_64

These instructions will help you build JSLK Kernel for the x86 platform.

### Prerequisites

JSLK can be built on any UNIX like system, such as any Linux distribution, FreeBSD or macOS. Windows can also be used with the WSL enabled. To build the kernel you first need to install some dependencies and the toolchain.

On Linux:
```
$ sudo apt-get install nasm wget git qemu xorriso  # Change apt-get install for whatever package manager you are using.
$ wget https://github.com/sofferjacob/edison-toolchain/releases/download/1.0.0/sierra_toolchain_linux_x86_64_v.1.0.0.tar.gz
```

On macOS:
```
$ /usr/bin/ruby -e "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/master/install)"
$ brew install nasm
$ brew install wget
$ brew install qemu
$ brew install xorriso
$ wget https://github.com/sofferjacob/edison-toolchain/releases/download/1.0.0/sierra_toolchain_mac_v.1.0.0.zip
```

On other operating systems the packages:
* git
* nasm
* qemu
* i386-elf toolchain (can be built using the scripts [here](https://github.com/sofferjacob/edison-toolchain))

Can be built from source or installed from  other sources.

> Note: GRUB must be re-compiled with the i386-elf architecture as a target on any OS. You can find instructions on how to do so [here.](http://wiki.osdev.org/GRUB_2#Installing_GRUB_2_on_OS_X)

### Building

To build JSLK from source copy and paste the following commands in a terminal prompt:
```
$ git clone https://github.com/sofferjacob/jslk.git
$ cd jslk
$ export TARGET=x64common  # You can change this for whichever target you want
$ make
```
> Note: For this to work, your toolchain must be in your path (you can change this in the Makefile).

To clean the source tree:
```
$ make clean
```

And to build an ISO image:
```
$ make floppy
```

You can also pass the target as an argument to make instead of defining it as a variable:
```
$ make TARGET=x64common clean all floppy run
```

To build the initird:
```
$ ./make_initrd file1 file1 file2 file2 ... file5 file5
```
**Note:** The included utility only works in macOS. Recompile the utility for your host OS using:
```
$ gcc initrd/make_initrd.c -o make_initrd
````

## Testing

Although JSLK can be tested on real hardware, it is not recommended, since a bug in the kernel can brick
your system. Besides installation to a hard drive is not currently supported, thus testing on a Virtual
Machine is recommended.

### Testing on a VM

Build the kernel and a floppy image, then:

```
$ make run
```

## Contributing

Check [`CONTRIBUTING.md`](CONTRIBUTING.md) for guidelines on how to contribute to the project, create issues and pull requests. We also encourage you to read our [code of conduct](CODE_OF_CONDUCT.md).

## Documentation

You may find the documentation for the project in the [Wiki](https://github.com/sofferjacob/jslk/wiki) or in the `docs` folder. Right now you the documentation is different in this places, but I will try to make it the same in the future. Also the documentation is in the writing and may be incomplete.

## License

JSLK is licensed under the GNU General Public License v3.0 - see the [LICENSE](LICENSE) file for details. An original copy of the documentation of this product should be included with every derivative work or redistributions of this project.

## Acknowledgments

See `docs/attrib.txt`.