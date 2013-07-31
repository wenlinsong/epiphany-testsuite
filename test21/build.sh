#!/bin/bash

set -e

ESDK=${EPIPHANY_HOME}
ELIBS=${ESDK}/tools/host/lib
EINCS=${ESDK}/tools/host/include
ELDF=${ESDK}/bsps/current/fast.ldf

# Build HOST side application
gcc mesh_bandwidth.c -o mesh_bandwidth.elf -I ${EINCS} -L ${ELIBS} -le-hal


# Build DEVICE side program
e-gcc -O3 -T ${ELDF} e_mesh_bandwidth_near.c -o e_mesh_bandwidth_near.elf -le-lib 


# Convert ebinary to SREC file
e-objcopy --srec-forceS3 --output-target srec e_mesh_bandwidth_near.elf e_mesh_bandwidth_near.srec



