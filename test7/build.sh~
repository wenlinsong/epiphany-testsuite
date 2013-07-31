#!/bin/bash

set -e

ESDK=${EPIPHANY_HOME}
ELIBS=${ESDK}/tools/host/lib
EINCS=${ESDK}/tools/host/include
ELDF=${ESDK}/bsps/current/fast.ldf

# Build HOST side application
gcc mutex_test.c -o mutex_test.elf -I ${EINCS} -L ${ELIBS} -le-hal

# Build DEVICE side program
e-gcc -O3 -T ${ELDF} e_mutex_test0.c -o e_mutex_test0.elf -le-lib 
e-gcc -O3 -T ${ELDF} e_mutex_test.c -o e_mutex_test.elf -le-lib 

# Convert ebinary to SREC file
e-objcopy --srec-forceS3 --output-target srec e_mutex_test0.elf e_mutex_test0.srec
e-objcopy --srec-forceS3 --output-target srec e_mutex_test.elf e_mutex_test.srec


