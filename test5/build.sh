#!/bin/bash

set -e

ESDK=${EPIPHANY_HOME}
ELIBS=${ESDK}/tools/host/lib
EINCS=${ESDK}/tools/host/include
ELDF=${ESDK}/bsps/current/fast.ldf

# Build HOST side application
gcc interrupt_test.c -o interrupt_test.elf -I ${EINCS} -L ${ELIBS} -le-hal

# Build DEVICE side program
e-gcc -O3 -T ${ELDF} e_interrupt_test.c -o e_interrupt_test.elf -mfp-mode=round-nearest -le-lib 


# Convert ebinary to SREC file
e-objcopy --srec-forceS3 --output-target srec e_interrupt_test.elf e_interrupt_test.srec



