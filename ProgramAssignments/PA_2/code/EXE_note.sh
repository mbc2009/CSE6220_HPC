clear

# Resources request
module load openmpi
#salloc -A gts-yw6 -N2 --ntasks-per-node=24 --ntasks-per-core=1 --mem=0 -t00:20:00 --constraint=gold6226


# compile
make   allreduce; make   many2many

# self test
#srun     -n 5 ./m2m ./small_example/m2m_smallinput.txt
#srun     -n 6 ./ard ./small_example/ard_smallinput.txt

# grading
#bash  autograder_gendata.sh # generate massive (2.5 GB) data for autograder, loc: ./autograderdata (files inside deleted)
bash  autograder.sh


