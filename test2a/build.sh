#!/bin/bash

set -e

ESDK=${EPIPHANY_HOME}
ELIBS=${ESDK}/tools/host/lib
EINCS=${ESDK}/tools/host/include
ELDF=${ESDK}/bsps/current/internal.ldf

# Build HOST side application
gcc math_test_a.c -o math_test_a.elf -I ${EINCS} -L ${ELIBS} -le-hal -lm

# Build DEVICE side program
e-gcc -O3 -T ${ELDF} e_math_test_a.c -o e_math_test_a.elf -mfp-mode=round-nearest -le-lib -lm

# Convert ebinary to SREC file
e-objcopy --srec-forceS3 --output-target srec e_math_test_a.elf e_math_test_a.srec


