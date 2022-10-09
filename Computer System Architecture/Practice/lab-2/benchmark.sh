#!/bin/bash

# initial values
OUTFILE="res.csv"
OPTIMISATION="O2"
TASK="sin"
OPERANDTYPE="double"
COUNT="10"

MODEL=$(lscpu | grep -i 'Имя модели' | cut -f 2 -d ":" | awk '{$1=$1}1' | sed "s/\s/_/g")

echo "g++ -Wall -${OPTIMISATION} -o app lab-2.cpp"
if [[ "$OPTIMISATION" == "None" ]]; then
    g++ -Wall -o app lab-2.cpp
else
    g++ -Wall -"$OPTIMISATION" -o app lab-2.cpp
fi

echo "./app -n ${COUNT} -t ${TASK} -o ${OPERANDTYPE} --opt ${OPTIMISATION} -f ${OUTFILE} -m ${MODEL}"
EXEC="./app -n $COUNT -t $TASK -o $OPERANDTYPE --opt $OPTIMISATION -f $OUTFILE -m $MODEL"
$EXEC