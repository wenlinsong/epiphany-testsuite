#!/bin/bash

set -e

ESDK=${EPIPHANY_HOME}
ELIBS=${ESDK}/tools/host/lib
EINCS=${ESDK}/tools/host/include
ELDF=${ESDK}/bsps/current/fast.ldf

# Build HOST side application
gcc mesh_test.c -o mesh_test.elf -I ${EINCS} -L ${ELIBS} -le-hal

# Build DEVICE side program
e-gcc -O0 -T ${ELDF} e_mesh_test.c -o e_mesh_test.elf -le-lib 

# Convert ebinary to SREC file
e-objcopy --srec-forceS3 --output-target srec e_mesh_test.elf e_mesh_test.srec



