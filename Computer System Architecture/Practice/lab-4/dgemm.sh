#!/bin/bash

# initial values
OUTFILE="res.csv"

echo "g++ -Wall -o app lab-4.cpp"
g++ -Wall -o app lab-4.cpp

echo "dgemm version,size,block size,time" > $OUTFILE


BLOCK_SIZE="0"
for (( SIZE=1024; SIZE<=5120; SIZE+=1024 ))
do
 echo "./app -s ${SIZE} -b ${BLOCK_SIZE} -f ${OUTFILE}"
 EXEC="./app -s $SIZE -b $BLOCK_SIZE -f $OUTFILE"
 $EXEC
done


for (( BLOCK_SIZE=1; BLOCK_SIZE<=1024; BLOCK_SIZE*=2 ))
do
 for (( SIZE=1024; SIZE<=5120; SIZE+=1024 ))
 do
  echo "./app -s ${SIZE} -b ${BLOCK_SIZE} -f ${OUTFILE}"
  EXEC="./app -s $SIZE -b $BLOCK_SIZE -f $OUTFILE"
  $EXEC
 done
done