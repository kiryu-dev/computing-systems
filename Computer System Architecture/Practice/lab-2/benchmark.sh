#!/bin/bash

# initial values
OUTFILE="res.csv"
echo "PModel,Task,OpType,Opt,InsCount,Timer,Time,LNum,AvTime,AbsError,RelError,TaskPerf" > $OUTFILE
MODEL=$(lscpu | grep -i 'Имя модели' | cut -f 2 -d ":" | awk '{$1=$1}1' | sed "s/\s/_/g")
COUNT="10"

OPTIMISATION="O0"
echo "g++ -Wall -${OPTIMISATION} -o app lab-2.cpp"
if [[ "$OPTIMISATION" == "None" ]]; then
    g++ -Wall -o app lab-2.cpp
else
    g++ -Wall -"$OPTIMISATION" -o app lab-2.cpp
fi

TASK="sin"
OPERANDTYPE="double"
echo "./app -n ${COUNT} -t ${TASK} -o ${OPERANDTYPE} --opt ${OPTIMISATION} -f ${OUTFILE} -m ${MODEL}"
EXEC="./app -n $COUNT -t $TASK -o $OPERANDTYPE --opt $OPTIMISATION -f $OUTFILE -m $MODEL"
$EXEC

OPERANDTYPE="int"
echo "./app -n ${COUNT} -t ${TASK} -o ${OPERANDTYPE} --opt ${OPTIMISATION} -f ${OUTFILE} -m ${MODEL}"
EXEC="./app -n $COUNT -t $TASK -o $OPERANDTYPE --opt $OPTIMISATION -f $OUTFILE -m $MODEL"
$EXEC

TASK="cos"
OPERANDTYPE="double"
echo "./app -n ${COUNT} -t ${TASK} -o ${OPERANDTYPE} --opt ${OPTIMISATION} -f ${OUTFILE} -m ${MODEL}"
EXEC="./app -n $COUNT -t $TASK -o $OPERANDTYPE --opt $OPTIMISATION -f $OUTFILE -m $MODEL"
$EXEC

OPERANDTYPE="int"
echo "./app -n ${COUNT} -t ${TASK} -o ${OPERANDTYPE} --opt ${OPTIMISATION} -f ${OUTFILE} -m ${MODEL}"
EXEC="./app -n $COUNT -t $TASK -o $OPERANDTYPE --opt $OPTIMISATION -f $OUTFILE -m $MODEL"
$EXEC

TASK="log"
OPERANDTYPE="double"
echo "./app -n ${COUNT} -t ${TASK} -o ${OPERANDTYPE} --opt ${OPTIMISATION} -f ${OUTFILE} -m ${MODEL}"
EXEC="./app -n $COUNT -t $TASK -o $OPERANDTYPE --opt $OPTIMISATION -f $OUTFILE -m $MODEL"
$EXEC

OPERANDTYPE="int"
echo "./app -n ${COUNT} -t ${TASK} -o ${OPERANDTYPE} --opt ${OPTIMISATION} -f ${OUTFILE} -m ${MODEL}"
EXEC="./app -n $COUNT -t $TASK -o $OPERANDTYPE --opt $OPTIMISATION -f $OUTFILE -m $MODEL"
$EXEC

OPTIMISATION="O1"
echo "g++ -Wall -${OPTIMISATION} -o app lab-2.cpp"
if [[ "$OPTIMISATION" == "None" ]]; then
    g++ -Wall -o app lab-2.cpp
else
    g++ -Wall -"$OPTIMISATION" -o app lab-2.cpp
fi

TASK="sin"
OPERANDTYPE="double"
echo "./app -n ${COUNT} -t ${TASK} -o ${OPERANDTYPE} --opt ${OPTIMISATION} -f ${OUTFILE} -m ${MODEL}"
EXEC="./app -n $COUNT -t $TASK -o $OPERANDTYPE --opt $OPTIMISATION -f $OUTFILE -m $MODEL"
$EXEC

OPERANDTYPE="int"
echo "./app -n ${COUNT} -t ${TASK} -o ${OPERANDTYPE} --opt ${OPTIMISATION} -f ${OUTFILE} -m ${MODEL}"
EXEC="./app -n $COUNT -t $TASK -o $OPERANDTYPE --opt $OPTIMISATION -f $OUTFILE -m $MODEL"
$EXEC

TASK="cos"
OPERANDTYPE="double"
echo "./app -n ${COUNT} -t ${TASK} -o ${OPERANDTYPE} --opt ${OPTIMISATION} -f ${OUTFILE} -m ${MODEL}"
EXEC="./app -n $COUNT -t $TASK -o $OPERANDTYPE --opt $OPTIMISATION -f $OUTFILE -m $MODEL"
$EXEC

OPERANDTYPE="int"
echo "./app -n ${COUNT} -t ${TASK} -o ${OPERANDTYPE} --opt ${OPTIMISATION} -f ${OUTFILE} -m ${MODEL}"
EXEC="./app -n $COUNT -t $TASK -o $OPERANDTYPE --opt $OPTIMISATION -f $OUTFILE -m $MODEL"
$EXEC

TASK="log"
OPERANDTYPE="double"
echo "./app -n ${COUNT} -t ${TASK} -o ${OPERANDTYPE} --opt ${OPTIMISATION} -f ${OUTFILE} -m ${MODEL}"
EXEC="./app -n $COUNT -t $TASK -o $OPERANDTYPE --opt $OPTIMISATION -f $OUTFILE -m $MODEL"
$EXEC

OPERANDTYPE="int"
echo "./app -n ${COUNT} -t ${TASK} -o ${OPERANDTYPE} --opt ${OPTIMISATION} -f ${OUTFILE} -m ${MODEL}"
EXEC="./app -n $COUNT -t $TASK -o $OPERANDTYPE --opt $OPTIMISATION -f $OUTFILE -m $MODEL"
$EXEC

OPTIMISATION="O2"
echo "g++ -Wall -${OPTIMISATION} -o app lab-2.cpp"
if [[ "$OPTIMISATION" == "None" ]]; then
    g++ -Wall -o app lab-2.cpp
else
    g++ -Wall -"$OPTIMISATION" -o app lab-2.cpp
fi

TASK="sin"
OPERANDTYPE="double"
echo "./app -n ${COUNT} -t ${TASK} -o ${OPERANDTYPE} --opt ${OPTIMISATION} -f ${OUTFILE} -m ${MODEL}"
EXEC="./app -n $COUNT -t $TASK -o $OPERANDTYPE --opt $OPTIMISATION -f $OUTFILE -m $MODEL"
$EXEC

OPERANDTYPE="int"
echo "./app -n ${COUNT} -t ${TASK} -o ${OPERANDTYPE} --opt ${OPTIMISATION} -f ${OUTFILE} -m ${MODEL}"
EXEC="./app -n $COUNT -t $TASK -o $OPERANDTYPE --opt $OPTIMISATION -f $OUTFILE -m $MODEL"
$EXEC

TASK="cos"
OPERANDTYPE="double"
echo "./app -n ${COUNT} -t ${TASK} -o ${OPERANDTYPE} --opt ${OPTIMISATION} -f ${OUTFILE} -m ${MODEL}"
EXEC="./app -n $COUNT -t $TASK -o $OPERANDTYPE --opt $OPTIMISATION -f $OUTFILE -m $MODEL"
$EXEC

OPERANDTYPE="int"
echo "./app -n ${COUNT} -t ${TASK} -o ${OPERANDTYPE} --opt ${OPTIMISATION} -f ${OUTFILE} -m ${MODEL}"
EXEC="./app -n $COUNT -t $TASK -o $OPERANDTYPE --opt $OPTIMISATION -f $OUTFILE -m $MODEL"
$EXEC

TASK="log"
OPERANDTYPE="double"
echo "./app -n ${COUNT} -t ${TASK} -o ${OPERANDTYPE} --opt ${OPTIMISATION} -f ${OUTFILE} -m ${MODEL}"
EXEC="./app -n $COUNT -t $TASK -o $OPERANDTYPE --opt $OPTIMISATION -f $OUTFILE -m $MODEL"
$EXEC

OPERANDTYPE="int"
echo "./app -n ${COUNT} -t ${TASK} -o ${OPERANDTYPE} --opt ${OPTIMISATION} -f ${OUTFILE} -m ${MODEL}"
EXEC="./app -n $COUNT -t $TASK -o $OPERANDTYPE --opt $OPTIMISATION -f $OUTFILE -m $MODEL"
$EXEC

OPTIMISATION="O3"
echo "g++ -Wall -${OPTIMISATION} -o app lab-2.cpp"
if [[ "$OPTIMISATION" == "None" ]]; then
    g++ -Wall -o app lab-2.cpp
else
    g++ -Wall -"$OPTIMISATION" -o app lab-2.cpp
fi

TASK="sin"
OPERANDTYPE="double"
echo "./app -n ${COUNT} -t ${TASK} -o ${OPERANDTYPE} --opt ${OPTIMISATION} -f ${OUTFILE} -m ${MODEL}"
EXEC="./app -n $COUNT -t $TASK -o $OPERANDTYPE --opt $OPTIMISATION -f $OUTFILE -m $MODEL"
$EXEC

OPERANDTYPE="int"
echo "./app -n ${COUNT} -t ${TASK} -o ${OPERANDTYPE} --opt ${OPTIMISATION} -f ${OUTFILE} -m ${MODEL}"
EXEC="./app -n $COUNT -t $TASK -o $OPERANDTYPE --opt $OPTIMISATION -f $OUTFILE -m $MODEL"
$EXEC

TASK="cos"
OPERANDTYPE="double"
echo "./app -n ${COUNT} -t ${TASK} -o ${OPERANDTYPE} --opt ${OPTIMISATION} -f ${OUTFILE} -m ${MODEL}"
EXEC="./app -n $COUNT -t $TASK -o $OPERANDTYPE --opt $OPTIMISATION -f $OUTFILE -m $MODEL"
$EXEC

OPERANDTYPE="int"
echo "./app -n ${COUNT} -t ${TASK} -o ${OPERANDTYPE} --opt ${OPTIMISATION} -f ${OUTFILE} -m ${MODEL}"
EXEC="./app -n $COUNT -t $TASK -o $OPERANDTYPE --opt $OPTIMISATION -f $OUTFILE -m $MODEL"
$EXEC

TASK="log"
OPERANDTYPE="double"
echo "./app -n ${COUNT} -t ${TASK} -o ${OPERANDTYPE} --opt ${OPTIMISATION} -f ${OUTFILE} -m ${MODEL}"
EXEC="./app -n $COUNT -t $TASK -o $OPERANDTYPE --opt $OPTIMISATION -f $OUTFILE -m $MODEL"
$EXEC

OPERANDTYPE="int"
echo "./app -n ${COUNT} -t ${TASK} -o ${OPERANDTYPE} --opt ${OPTIMISATION} -f ${OUTFILE} -m ${MODEL}"
EXEC="./app -n $COUNT -t $TASK -o $OPERANDTYPE --opt $OPTIMISATION -f $OUTFILE -m $MODEL"
$EXEC

OPTIMISATION="None"
echo "g++ -Wall -${OPTIMISATION} -o app lab-2.cpp"
if [[ "$OPTIMISATION" == "None" ]]; then
    g++ -Wall -o app lab-2.cpp
else
    g++ -Wall -"$OPTIMISATION" -o app lab-2.cpp
fi

TASK="sin"
OPERANDTYPE="double"
echo "./app -n ${COUNT} -t ${TASK} -o ${OPERANDTYPE} --opt ${OPTIMISATION} -f ${OUTFILE} -m ${MODEL}"
EXEC="./app -n $COUNT -t $TASK -o $OPERANDTYPE --opt $OPTIMISATION -f $OUTFILE -m $MODEL"
$EXEC

OPERANDTYPE="int"
echo "./app -n ${COUNT} -t ${TASK} -o ${OPERANDTYPE} --opt ${OPTIMISATION} -f ${OUTFILE} -m ${MODEL}"
EXEC="./app -n $COUNT -t $TASK -o $OPERANDTYPE --opt $OPTIMISATION -f $OUTFILE -m $MODEL"
$EXEC

TASK="cos"
OPERANDTYPE="double"
echo "./app -n ${COUNT} -t ${TASK} -o ${OPERANDTYPE} --opt ${OPTIMISATION} -f ${OUTFILE} -m ${MODEL}"
EXEC="./app -n $COUNT -t $TASK -o $OPERANDTYPE --opt $OPTIMISATION -f $OUTFILE -m $MODEL"
$EXEC

OPERANDTYPE="int"
echo "./app -n ${COUNT} -t ${TASK} -o ${OPERANDTYPE} --opt ${OPTIMISATION} -f ${OUTFILE} -m ${MODEL}"
EXEC="./app -n $COUNT -t $TASK -o $OPERANDTYPE --opt $OPTIMISATION -f $OUTFILE -m $MODEL"
$EXEC

TASK="log"
OPERANDTYPE="double"
echo "./app -n ${COUNT} -t ${TASK} -o ${OPERANDTYPE} --opt ${OPTIMISATION} -f ${OUTFILE} -m ${MODEL}"
EXEC="./app -n $COUNT -t $TASK -o $OPERANDTYPE --opt $OPTIMISATION -f $OUTFILE -m $MODEL"
$EXEC

OPERANDTYPE="int"
echo "./app -n ${COUNT} -t ${TASK} -o ${OPERANDTYPE} --opt ${OPTIMISATION} -f ${OUTFILE} -m ${MODEL}"
EXEC="./app -n $COUNT -t $TASK -o $OPERANDTYPE --opt $OPTIMISATION -f $OUTFILE -m $MODEL"
$EXEC