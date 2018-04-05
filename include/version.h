#ifndef _jlk_version_h
#define _jlk_version_h

// Release type
#define PRE_ALPHA 0x0
#define ALPHA     0x1
#define BETA      0x02
#define RELEASE   0x03

/* 
Release Channels
================
This indicate where the release is posted and it's purpose.
The canary release is a build from the current working tree,
with the latest fetatures that might not be fully supported, 
never make it to the final release or not work at all. Releases
form this channel can work perfectly, be buggy or not work at all.
It's use is just recommended for testing. 
The dev channel contains working builds that may still be buggy, it is
inteded for developers who want to test the latest features and use the
latest API to start upgrading their app/service/product. 
The test channel contains builds that are stable, contain the latest features
but that are still being tested and not ready for release. They might still be
buggy, and are intended for users who want to test or use the latest features.
Right now the only channels that are being used are: canary, test and release
(although test will not be used that often). I will start releasing builds
in the dev channel when a more complex API is built with a kind of usable userland.
*/
#define CANARY 0
#define DEV    1
#define TEST   2
#define REL    4

#define KERNEL_NAME "JSLK"
#define KERNEL_VERSION_MAJOR 0
#define KERNEL_VERSION_MINOR 0
#define KERNEL_VERSION_RELEASE 9
#define REL_TYPE 0x0
#define REL_CHANNEL 0
// #define KERNEL_VERSION_EXTRA 0
#define KERNEL_RELEASE_TYPE "pre-alpha/debug/canary"
#define KERNEL_VERSION_STRING "0.0.9 pa/debug/canary"
#define KERNEL_NAME_FULL "JSLK Kernel"
#define LICENSE "Copyright (c) 2018 Jacobo Soffer. Released under GNU GPL3"
#define KERNEL_BUILD_DATE __DATE__
#define KERNEL_BUILD_NUM  2018040400
#define JSLK_API_VERSION "0011"  // For 0.0.9 pre-alpha
#define JSLK_API_NUM 0011

#endif
