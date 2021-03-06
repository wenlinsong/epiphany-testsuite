#!/bin/bash

set -e

ESDK=${EPIPHANY_HOME}
ELIBS=${ESDK}/tools/host/lib
EINCS=${ESDK}/tools/host/include
ELDF=${ESDK}/bsps/current/fast.ldf

# Build HOST side application
gcc math_test.c -o math_test.elf -I ${EINCS} -L ${ELIBS} -le-hal -lm

# Build DEVICE side program
e-gcc -O3 -T ${ELDF} e_math_test.c -o e_math_test.elf -mfp-mode=round-nearest -le-lib -lm
e-gcc -O3 -T ${ELDF} e_math_test1.c -o e_math_test1.elf -mfp-mode=round-nearest -le-lib -lm

# Convert ebinary to SREC file
e-objcopy --srec-forceS3 --output-target srec e_math_test.elf e_math_test.srec
e-objcopy --srec-forceS3 --output-target srec e_math_test1.elf e_math_test1.srec


