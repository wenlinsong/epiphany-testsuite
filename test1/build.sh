#!/bin/bash

set -e

ESDK=${EPIPHANY_HOME}
ELIBS=${ESDK}/tools/host/lib
EINCS=${ESDK}/tools/host/include
ELDF=${ESDK}/bsps/current/fast.ldf

# Build HOST side application
gcc register-test.c -o register-test.elf -I ${EINCS} -L ${ELIBS} -le-hal

# Build DEVICE side program
e-gcc -T ${ELDF} e-register-test2.c -o e-register-test2.elf -le-lib
e-gcc -T ${ELDF} e-register-test3.c -o e-register-test3.elf -le-lib

# Convert ebinary to SREC file
e-objcopy --srec-forceS3 --output-target srec e-register-test2.elf e-register-test2.srec
e-objcopy --srec-forceS3 --output-target srec e-register-test3.elf e-register-test3.srec

