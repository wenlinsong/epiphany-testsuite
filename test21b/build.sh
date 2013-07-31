#!/bin/bash

set -e

ESDK=${EPIPHANY_HOME}
ELIBS=${ESDK}/tools/host/lib
EINCS=${ESDK}/tools/host/include
ELDF=${ESDK}/bsps/current/fast.ldf

# Build HOST side application
gcc mesh_bi.c -o mesh_bi.elf -I ${EINCS} -L ${ELIBS} -le-hal

# Build DEVICE side program
e-gcc -O0 -T ${ELDF} e_mesh_bi00.c -o e_mesh_bi00.elf -le-lib 
e-gcc -O0 -T ${ELDF} e_mesh_bi.c -o e_mesh_bi.elf -le-lib
e-gcc -O0 -T ${ELDF} e_mesh_bi1.c -o e_mesh_bi1.elf -le-lib
e-gcc -O0 -T ${ELDF} e_mesh_rec.c -o e_mesh_rec.elf -le-lib  
e-gcc -O0 -T ${ELDF} e_mesh_rec1.c -o e_mesh_rec1.elf -le-lib
    
# Convert ebinary to SREC file
e-objcopy --srec-forceS3 --output-target srec e_mesh_bi00.elf e_mesh_bi00.srec
e-objcopy --srec-forceS3 --output-target srec e_mesh_bi.elf e_mesh_bi.srec
e-objcopy --srec-forceS3 --output-target srec e_mesh_bi1.elf e_mesh_bi1.srec
e-objcopy --srec-forceS3 --output-target srec e_mesh_rec.elf e_mesh_rec.srec
e-objcopy --srec-forceS3 --output-target srec e_mesh_rec1.elf e_mesh_rec1.srec

