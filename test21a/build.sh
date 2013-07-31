#!/bin/bash

set -e

ESDK=${EPIPHANY_HOME}
ELIBS=${ESDK}/tools/host/lib
EINCS=${ESDK}/tools/host/include
ELDF=${ESDK}/bsps/current/fast.ldf

# Build HOST side application
gcc mesh_one.c -o mesh_one.elf -I ${EINCS} -L ${ELIBS} -le-hal

# Build DEVICE side program
e-gcc -O0 -T ${ELDF} e_mesh_one.c -o e_mesh_one.elf -le-lib 
e-gcc -O0 -T ${ELDF} e_mesh_one1.c -o e_mesh_one1.elf -le-lib

    
# Convert ebinary to SREC file
e-objcopy --srec-forceS3 --output-target srec e_mesh_one.elf e_mesh_one.srec
e-objcopy --srec-forceS3 --output-target srec e_mesh_one1.elf e_mesh_one1.srec


