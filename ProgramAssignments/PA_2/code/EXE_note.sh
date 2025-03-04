clear

# Resources request
module load openmpi
salloc -A gts-yw6 -N1 --ntasks-per-node=24 --ntasks-per-core=1 --mem=0 -t00:20:00 --constraint=gold6226

# compile
make   allreduce 
make   many2many

# self test
srun    -n 5 ./m2m ./small_example/m2m_smallinput.txt
srun    -n 6 ./ard ./small_example/ard_smallinput.txt

# grading
bash   autograder_gendata.sh # generate massive (2.5 GB) data for autograder, loc: ./autograderdata (files inside deleted)
bash  autograder.sh


<< COMMENT
mpicxx -Wall -O3 -std=c++2a -o ard allreduce_main.cpp custom_collectives.cpp
mpicxx -Wall -O3 -std=c++2a -o m2m manytomany_main.cpp custom_collectives.cpp
Architecture:             x86_64
  CPU op-mode(s):         32-bit, 64-bit
  Address sizes:          46 bits physical, 48 bits virtual
  Byte Order:             Little Endian
CPU(s):                   24
  On-line CPU(s) list:    0-23
Vendor ID:                GenuineIntel
  Model name:             Intel(R) Xeon(R) Gold 6226 CPU @ 2.70GHz
    CPU family:           6
    Model:                85
    Thread(s) per core:   1
    Core(s) per socket:   12
    Socket(s):            2
    Stepping:             7
    CPU(s) scaling MHz:   73%
    CPU max MHz:          3700.0000
    CPU min MHz:          1200.0000
    BogoMIPS:             5400.00
    Flags:                fpu vme de pse tsc msr pae mce cx8 apic sep mtrr pge mca cmov pat pse36 clflush dts acpi mmx fxsr sse sse2 ss ht tm p
                          be syscall nx pdpe1gb rdtscp lm constant_tsc art arch_perfmon pebs bts rep_good nopl xtopology nonstop_tsc cpuid aper
                          fmperf pni pclmulqdq dtes64 monitor ds_cpl vmx smx est tm2 ssse3 sdbg fma cx16 xtpr pdcm pcid dca sse4_1 sse4_2 x2api
                          c movbe popcnt tsc_deadline_timer aes xsave avx f16c rdrand lahf_lm abm 3dnowprefetch cpuid_fault epb cat_l3 cdp_l3 i
                          ntel_ppin ssbd mba ibrs ibpb stibp ibrs_enhanced tpr_shadow flexpriority ept vpid ept_ad fsgsbase tsc_adjust bmi1 avx
                          2 smep bmi2 erms invpcid cqm mpx rdt_a avx512f avx512dq rdseed adx smap clflushopt clwb intel_pt avx512cd avx512bw av
                          x512vl xsaveopt xsavec xgetbv1 xsaves cqm_llc cqm_occup_llc cqm_mbm_total cqm_mbm_local dtherm ida arat pln pts hwp h
                          wp_act_window hwp_epp hwp_pkg_req vnmi pku ospke avx512_vnni md_clear flush_l1d arch_capabilities
Virtualization features:  
  Virtualization:         VT-x
Caches (sum of all):      
  L1d:                    768 KiB (24 instances)
  L1i:                    768 KiB (24 instances)
  L2:                     24 MiB (24 instances)
  L3:                     38.5 MiB (2 instances)
NUMA:                     
  NUMA node(s):           2
  NUMA node0 CPU(s):      0-11
  NUMA node1 CPU(s):      12-23
Vulnerabilities:          
  Gather data sampling:   Vulnerable
  Itlb multihit:          KVM: Mitigation: VMX disabled
  L1tf:                   Not affected
  Mds:                    Not affected
  Meltdown:               Not affected
  Mmio stale data:        Vulnerable
  Reg file data sampling: Not affected
  Retbleed:               Vulnerable
  Spec rstack overflow:   Not affected
  Spec store bypass:      Vulnerable
  Spectre v1:             Vulnerable: __user pointer sanitization and usercopy barriers only; no swapgs barriers
  Spectre v2:             Vulnerable; IBPB: disabled; STIBP: disabled; PBRSB-eIBRS: Vulnerable; BHI: Vulnerable
  Srbds:                  Not affected
  Tsx async abort:        Mitigation; TSX disabled
-------------------------------------
rm -f m2m ard
mpicxx -Wall -O3 -std=c++2a -o m2m manytomany_main.cpp custom_collectives.cpp
mpicxx -Wall -O3 -std=c++2a -o ard allreduce_main.cpp custom_collectives.cpp
 
=================== CHECKING TEST FILE ===================
 
 
=================== CORRECTNESS TESTS ===================
 
##AllReduce 15
Time taken by custom function: 12.765098
AllReduce P=15 Correctness passed!
 
##AllReduce 24
Time taken by custom function: 22.296647
AllReduce P=24 Correctness passed!
 
 
==================================
 
##Many2Many 15
size_in_file: 15, size: 15
Time taken by custom function: 11.418711
Correct Output
 
##Many2Many 24
size_in_file: 24, size: 24
Time taken by custom function: 31.630223
Correct Output
==================== RUNTIME TESTS =====================
 
##AllReduce
Custom function time: 22.021015
Runtime test passed!
Runtime test passed!
 
==================================
 
##Many2Many
Custom function time: 31.538533
Runtime test passed!
Runtime test passed!
(base) [hliu686@atl1-1-02-010-9-1 pa2]$ 