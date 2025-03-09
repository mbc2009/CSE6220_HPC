#!/bin/bash

#SBATCH --job-name=mpi_results
#SBATCH --output=mpi_results.out
#SBATCH --error=mpi_results.err
#SBATCH --nodes=1
#SBATCH --ntasks-per-node=24
#SBATCH --time=00:05:30

mkdir -p autograderout

MAX_EXECUTION_TIME=$((300 * 5))

# Run the tests within the time limit
timeout $MAX_EXECUTION_TIME bash -c '

lscpu
echo "-------------------------------------"

module load openmpi

make clean
make many2many
make allreduce

echo -e "=================== CHECKING TEST FILE ==================="

for i in 4 8 15 16 24 48; do

    if [ ! -f "autograderdata/inputard_${i}.txt" ]; then
        echo "No AllReduce ${i} Input"
        exit 1
    fi

    if [ ! -f "autograderdata/inputm2_${i}.txt" ]; then
        echo "No M2M ${i} Input"
        exit 1
    fi

    if [ ! -f "autograderdata/outputard_${i}.txt" ]; then
        echo "No AllReduce ${i} Output"
        exit 1
    fi

    if [ ! -f "autograderdata/outputm2_${i}.txt" ]; then
        echo "No M2M ${i} Output"
        exit 1
    fi

    if [ ! -f "autograderdata/outputard_${i}.txt" ]; then
        echo "No AllReduce ${i} Output"
        exit 1
    fi

    if [ ! -f "autograderdata/outputm2_${i}.txt" ]; then
        echo "No M2M ${i} Output"
        exit 1
    fi
done


echo " "
echo "=================== CORRECTNESS TESTS ==================="
echo " "

for i in 4 8 16; do
    echo "##AllReduce $i"
    output=$(srun -n $i ./ard autograderdata/inputard_${i}.txt autograderout/myoutputard_${i}.txt)
    echo "$output"
    if diff autograderdata/outputard_${i}.txt autograderout/myoutputard_${i}.txt > /dev/null; then
        echo "AllReduce P=$i Correctness passed!"
    else
        echo "AllReduce P=$i Failed"
    fi
    echo " "
done


echo -e "=================================="

for i in 4 8 16; do
    echo "##Many2Many $i"
    output=$(srun -n $i ./m2m autograderdata/inputm2_${i}.txt autograderdata/outputm2_${i}.txt)
    echo "$output"
done


echo -e "==================== RUNTIME TESTS ====================="
for i in 16 24; do
    echo "##AllReduce, P=$i"
    output=$(srun -n $i ./ard autograderdata/inputard_${i}.txt)
    custom_time=$(echo "$output" | sed -n "s/.*Time taken by custom function: \([0-9.]*\).*/\1/p")
    echo "Custom function time: $custom_time"

    mpi_time=40
    multiplier=1
    if (( $(echo "$custom_time < $multiplier * $mpi_time" | bc -l) )); then
        echo "Runtime test passed!"
    else
        echo "Runtime test failed"
    fi

    mpi_time=30
    multiplier=1
    if (( $(echo "$custom_time < $multiplier * $mpi_time" | bc -l) )); then
        echo "Runtime test passed!"
    else
        echo "Runtime test failed"
    fi

    echo " "
    echo "=================================="
    echo " "
done


echo " "
echo "=================================="
echo " "

for i in 16 24; do
    echo "##Many2Many, p=$i"
    output=$(srun -n $i ./m2m autograderdata/inputm2_${i}.txt)
    custom_time=$(echo "$output" | sed -n "s/.*Time taken by custom function: \([0-9.]*\).*/\1/p")
    echo "Custom function time: $custom_time"

    mpi_time=50
    multiplier=1
    if (( $(echo "$custom_time < $multiplier * $mpi_time" | bc -l) )); then
        echo "Runtime test passed!"
    else
        echo "Runtime test failed"
    fi

    mpi_time=40
    multiplier=1
    if (( $(echo "$custom_time < $multiplier * $mpi_time" | bc -l) )); then
        echo "Runtime test passed!"
    else
        echo "Runtime test failed"
    fi
done

'

rm -rf autograderout/


if [[ $? -eq 124 ]]; then
    echo "Error: Script execution exceeded $MAX_EXECUTION_TIME seconds."
    exit 1
fi
