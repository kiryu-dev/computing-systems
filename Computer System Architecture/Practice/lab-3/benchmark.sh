#!/bin/bash

# initial values
OUTFILE="res.csv"
MEMORYTYPE="RAM"
BLOCKSIZE="1024"
ELEMENTTYPE="double"
COUNT="10"

echo "g++ -Wall -o app lab-3.cpp"
g++ -Wall -o app lab-3.cpp

echo "./app -n ${COUNT} -b ${BLOCKSIZE} -m ${MEMORYTYPE} -e ${ELEMENTTYPE} -f ${OUTFILE}
EXEC="./app -n $COUNT -b $BLOCKSIZE -m $MEMORYTYPE -e $ELEMENTTYPE -f $OUTFILE
$EXEC