# JSLK
Simple kernel for x86 I am writing with the objective of learning a bit more on how OS's work and to also improve my coding skills.

### C++ Branch
The purpose of this branch is to improve compatibility and integration with C++.

## General Objectives
I hope to at least get the kernel to do the following:
* File System and VFS implementation.
* Multitasking.
* Loading an ELF program.

I am not making this kernel to be portable nor to replace Linux or become something, I am just doing it with the objective of learning.

## A note on portability
Although the kernel is being designed with portability in mind, about now the kernel is not as portable as it would be with a 100% portable design. Still, porting it should be easy.

## Language Compatibility
The kernel is compatible with the System V ABI and is mostly programmed in C to keep the main codebase simple. Still the kernel supports being programmed in C++ as well with some limitations:
* Placement `new` and `delete` cannot be used yet.
* Exceptions are not supported yet.
Everything else should work fine.

> This kernel is released under the terms of the GNU GPL3, a copy of the original documentation should be provided with every derivatives or redistributions of this work.
