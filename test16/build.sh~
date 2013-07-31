#!/bin/bash

set -e

ESDK=${EPIPHANY_HOME}
ELIBS=${ESDK}/tools/host/lib
EINCS=${ESDK}/tools/host/include
ELDF=${ESDK}/bsps/current/fast.ldf

# Build HOST side application
gcc dma_message_test.c -o dma_message_test.elf -I ${EINCS} -L ${ELIBS} -le-hal

# Build DEVICE side program
e-gcc -O3 -T ${ELDF} e_dma_message_test.c -o e_dma_message_test.elf -le-lib 
e-gcc -O3 -T ${ELDF} e_dma_message_slave_test.c -o e_dma_message_slave_test.elf -le-lib 

# Convert ebinary to SREC file
e-objcopy --srec-forceS3 --output-target srec e_dma_message_test.elf e_dma_message_test.srec
e-objcopy --srec-forceS3 --output-target srec e_dma_message_slave_test.elf e_dma_message_slave_test.srec


