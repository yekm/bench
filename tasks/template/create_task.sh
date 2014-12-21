#!/bin/bash -ex

[ -z "$1" ] && echo "specify task name" && exit 1
tn=$1
ltn=${tn,,}
utn=${tn^^}

mkdir ../$ltn
for f in *.t; do
    dst="../$ltn/${ltn}_${f%.t}"
    cp $f $dst
    sed -i "s/%TN%/$tn/g; s/%UTN%/$utn/g; s/%LTN%/$ltn/g;" $dst
done

sed -i "s,set(TASKS_SRC,set(TASKS_SRC\n    $ltn/${ltn}_task.cpp," ../CMakeLists.txt
