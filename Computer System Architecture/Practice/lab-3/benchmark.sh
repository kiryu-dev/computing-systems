#!/bin/bash

# initial values
OUTFILE="res.csv"
MEMORYTYPE="RAM"
# BLOCKSIZE="131072"
COUNT="10"

echo "MemoryType,BlockSize,ElementType,BufferSize,LaunchNum,Timer,WriteTime,AverageWriteTime,WriteBandwidth,AbsError(write),RelError(write),ReadTime,AverageReadTime,ReadBandwidth,AbsError(read),RelError(read)" > $OUTFILE

echo "g++ -Wall -O3 -o app lab-3.cpp"
g++ -Wall -O3 -o app lab-3.cpp

BLOCKSIZE="131072"
echo "./app -n ${COUNT} -b ${BLOCKSIZE} -m ${MEMORYTYPE} -f ${OUTFILE}"
EXEC="./app -n $COUNT -b $BLOCKSIZE -m $MEMORYTYPE -f $OUTFILE"
$EXEC

BLOCKSIZE="1048576"
echo "./app -n ${COUNT} -b ${BLOCKSIZE} -m ${MEMORYTYPE} -f ${OUTFILE}"
EXEC="./app -n $COUNT -b $BLOCKSIZE -m $MEMORYTYPE -f $OUTFILE"
$EXEC

BLOCKSIZE="6291456"
echo "./app -n ${COUNT} -b ${BLOCKSIZE} -m ${MEMORYTYPE} -f ${OUTFILE}"
EXEC="./app -n $COUNT -b $BLOCKSIZE -m $MEMORYTYPE -f $OUTFILE"
$EXEC

BLOCKSIZE="16777216"
echo "./app -n ${COUNT} -b ${BLOCKSIZE} -m ${MEMORYTYPE} -f ${OUTFILE}"
EXEC="./app -n $COUNT -b $BLOCKSIZE -m $MEMORYTYPE -f $OUTFILE"
$EXEC

# MEMORYTYPE="SSD"
# BLOCKSIZE="4194304"
# COUNT="20"
# echo "./app -n ${COUNT} -b ${BLOCKSIZE} -m ${MEMORYTYPE} -f ${OUTFILE}"
# EXEC="./app -n $COUNT -b $BLOCKSIZE -m $MEMORYTYPE -f $OUTFILE"
# $EXEC