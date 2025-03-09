#!/bin/bash

echo -e "=================== GENERATING TEST FILE ==================="
mkdir -p autograderdata

for i in 36; do
    if [ ! -f "autograderdata/inputard_${i}.txt" ]; then
        python3 ard_gen.py $i
    fi
    echo " "

    if [ ! -f "autograderdata/inputm2_${i}.txt" ]; then
        python3 m2m_gen.py $i
    fi
    echo " "
done

exit 1 # Stop here for now
