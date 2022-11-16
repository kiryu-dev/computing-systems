#!/bin/bash

# initial values
OUTFILE="res.csv"
# MODE="openmp"
# SIZE="4"

echo "g++ -fopenmp -Wall -o app lab-5.cpp"
g++ -fopenmp -Wall -o app lab-5.cpp

echo "mode,size,threads,time" > $OUTFILE

MODE="openmp"
for (( SIZE=1000; SIZE<=5000; SIZE+=1000 ))
do
 for (( THREADS=2; THREADS<=8; THREADS+=2 ))
 do
  echo "./app -s ${SIZE} -t ${THREADS} -f ${OUTFILE} -m ${MODE}"
  EXEC="./app -s $SIZE -t $THREADS -f $OUTFILE -m $MODE"
  $EXEC
 done
done

MODE="posix"
for (( SIZE=1000; SIZE<=5000; SIZE+=1000 ))
do
 for (( THREADS=2; THREADS<=8; THREADS+=2 ))
 do
  echo "./app -s ${SIZE} -t ${THREADS} -f ${OUTFILE} -m ${MODE}"
  EXEC="./app -s $SIZE -t $THREADS -f $OUTFILE -m $MODE"
  $EXEC
 done
done

MODE="default"
THREADS="1"
for (( SIZE=1000; SIZE<=5000; SIZE+=1000 ))
do
 echo "./app -s ${SIZE} -t ${THREADS} -f ${OUTFILE} -m ${MODE}"
 EXEC="./app -s $SIZE -t $THREADS -f $OUTFILE -m $MODE"
 $EXEC
done