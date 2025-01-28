# LOG
```cpp
/*

rand() Pseudorandom number generator is too slow, thus been replaced
test-2 time: 5.7 >> 5.5 sec

srand(time(0) + rank * 100); // Seed random number generator uniquely for each rank

for (int i = 0; i < total; i++) {

vectors[i] = static_cast<float>(rand()) / RAND_MAX; // Generate normalized random values

}

*/
```

# OUTPUT
```plain
-------------------------------------
Correctness test
n = 1000000
# of processors: 1 and 16
PASSED: Estimated Pi within range [3.13, 3.15]
-------------------------------------
RunTime test
n = 1000000000
# of processors: 8
PASSED: Time (3.01166 seconds) within 5.5 seconds
-------------------------------------
Scalability test
n = 1000000000
# of processors: 16
PASSED: Speedup (15.80918844204567893108) is at least 15.5 for 16 processors
---------------------------------------
```