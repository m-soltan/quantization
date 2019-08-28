#!/usr/bin/env bash

program=$1
directory=$2
testsErr=0
testsOk=0

for i in $(echo ${directory}/*.in)
do
    fName=${i%%.in}
    valgrind  -q --log-file="valgrind_report" --leak-check=full --show-leak-kinds=all  --errors-for-leak-kinds=all --error-exitcode=42 ./${program} <${fName}.in 1>${fName%%.in}.Xout 2>${fName%%.in}.Xerr

    if ! cmp ${fName}.out ${fName}.Xout >/dev/null
    then {
        ((++testsErr))
        echo "wrong answer in ${fName}"
    }
    else
    {
        ((++testsOk))
        echo "${fName} ok"
    }
    fi;

    rm ${fName}.Xerr
    rm ${fName}.Xout
done

echo "*** FINAL RESULT ***"
echo "correct: ${testsOk}"
echo "errors:  ${testsErr}"
