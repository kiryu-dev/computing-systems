#!/bin/bash

# initial values
OUTFILE="res.csv"

echo "g++ -Wall -o app lab-4.cpp"
g++ -Wall -o app lab-4.cpp

echo "dgemm version,size,block size,time" > $OUTFILE


BLOCK_SIZE="0"
for (( SIZE=1000; SIZE<=5000; SIZE+=1000 ))
do
 echo "./app -s ${SIZE} -b ${BLOCK_SIZE} -f ${OUTFILE}"
 EXEC="./app -s $SIZE -b $BLOCK_SIZE -f $OUTFILE"
 $EXEC
done


for (( BLOCK_SIZE=1; BLOCK_SIZE<=1024; BLOCK_SIZE*=2 ))
do
 for (( SIZE=1000; SIZE<=5000; SIZE+=1000 ))
 do
  echo "./app -s ${SIZE} -b ${BLOCK_SIZE} -f ${OUTFILE}"
  EXEC="./app -s $SIZE -b $BLOCK_SIZE -f $OUTFILE"
  $EXEC
 done
done