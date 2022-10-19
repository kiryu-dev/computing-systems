#!/bin/bash

# initial values
OUTFILE="res.csv"
MEMORYTYPE="SSD"
BLOCKSIZE="4194304"
ELEMENTTYPE="int"
COUNT="20"

echo "g++ -Wall -o app lab-3.cpp"
g++ -Wall -o app lab-3.cpp

echo "./app -n ${COUNT} -b ${BLOCKSIZE} -m ${MEMORYTYPE} -e ${ELEMENTTYPE} -f ${OUTFILE}"
EXEC="./app -n $COUNT -b $BLOCKSIZE -m $MEMORYTYPE -e $ELEMENTTYPE -f $OUTFILE"
$EXEC