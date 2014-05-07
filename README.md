esi
===

Code and support files for ESI MLCC jam detection circuit

SETUP:

Designed for use on Linux, tested on

Linux 3.14.2-1-ARCH #1 SMP PREEMPT Sun Apr 27 11:28:44 CEST 2014 x86_64 GNU/Linux

Required packages (ARCH Linux, other distributions may have different names)

community/avr-binutils
community-testing/avr-gcc
community/avr-libc
core/make

Recommended packages:

community/avr-gdb

USE:

All interaction is done through the Makefile using the GNU make utility.

To build code ONLY:

$ make

To flash and rebuild code if necessary:

$ make program
