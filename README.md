# Cache Coherence and NUMA Memory Performance Analysis

## 1. Objective

To analyze MSI, MESI and MOESI cache coherence protocols, investigate false sharing in parallel programs, and evaluate NUMA local and remote memory access performance.

## 2. Technologies Used

- Ubuntu 22.04.5 LTS
- GCC 11.4.0
- C programming
- Python 3.10
- OpenMP
- numactl
- VMware Workstation

## 3. Experiment Overview

### Part A: Cache Coherence Simulation

Implemented a two-core snooping cache simulator to compare:

- MSI: Modified, Shared, Invalid
- MESI: Modified, Exclusive, Shared, Invalid
- MOESI: Modified, Owned, Exclusive, Shared, Invalid

Two memory layouts were simulated:

- Adjacent: Both counters share one cache line.
- Padded: Counters occupy separate cache lines.

The simulator records bus transactions, invalidations, writebacks and cache-to-cache transfers.

### Part B: False Sharing

Implemented an OpenMP program to measure two threads incrementing independent counters.

The experiment compares adjacent and padded counter layouts.

### Part C: NUMA Memory Sweep

Implemented a 512 MB memory sweep with four passes and a 64-byte stride.

Local and remote memory placement were tested using numactl.

### Part D: Combined Comparison

The comparison program evaluates:

- Adjacent and padded layouts with 1, 2 and 4 threads.
- Memory sweep performance with 1 and 2 threads.
- Default, local and remote NUMA placement.

## 4. Project Structure

```text
Cache-Coherence-NUMA/
├── programs/
│   ├── coherence_sim.py
│   ├── false_sharing.c
│   ├── numa_sweep.c
│   └── compare_numa.c
├── results/
│   └── experiment_results.txt
├── screenshots/
│   ├── 1_VM_NUMA_Setup.png
│   ├── 2_Coherence_Simulator.png
│   ├── 3_False_Sharing_Results.png
│   ├── 4_NUMA_Results.png
│   └── 5_Comparison_Program.png
└── README.md

```

5. Compilation
Navigate to the programs directory:
cd programs


Compile the C programs:
gcc -O2 false_sharing.c -fopenmp -o false_sharing
gcc -O2 numa_sweep.c -fopenmp -o numa_sweep
gcc -O2 compare_numa.c -fopenmp -o compare_numa


6. Execution
Cache Coherence Simulator
python3 coherence_sim.py 1000


False Sharing
OMP_NUM_THREADS=2 ./false_sharing adjacent
OMP_NUM_THREADS=2 ./false_sharing padded


NUMA Memory Sweep
OMP_NUM_THREADS=1 numactl --cpunodebind=0 --membind=0 ./numa_sweep
OMP_NUM_THREADS=1 numactl --cpunodebind=0 --membind=1 ./numa_sweep


Combined Comparison
./compare_numa
numactl --cpunodebind=0 --membind=0 ./compare_numa
numactl --cpunodebind=0 --membind=1 ./compare_numa


7. Key Results
Cache Coherence Simulation
Layout	Protocol	Bus Transactions	Writebacks	C2C
Adjacent	MSI	4000	1999	0
Adjacent	MESI	3999	1999	0
Adjacent	MOESI	3999	0	1999
Padded	MSI	4	0	0
Padded	MESI	2	0	0
Padded	MOESI	2	0	0

False Sharing — Standalone Benchmark
Median of three runs:
Layout	Median Time
Adjacent	0.033223 s
Padded	0.026023 s

Observed adjacent/padded ratio: 1.277.
NUMA Memory Sweep — Combined Program
Median of three runs:
Placement	1 Thread	2 Threads
Local	0.0978 s	0.0733 s
Remote	0.0988 s	0.0771 s

All times are in seconds.
8. Observations
1. The simulator demonstrated differences in bus traffic between MSI, MESI and MOESI.
2. Padding reduced simulated cache-line sharing.
3. MOESI used cache-to-cache transfers instead of forced writebacks in the adjacent workload.
4. The combined benchmark showed better padded-counter performance at 2 and 4 threads.
5. Two-thread memory sweeps were faster than one-thread sweeps in the recorded runs.
6. Remote memory access was slightly slower in the combined NUMA measurements.
9. Limitations
The experiment was executed inside a VMware virtual machine exposing two virtual NUMA nodes.

The host system may not provide physically independent memory nodes. Therefore, the NUMA measurements represent virtual placement behavior and should not be interpreted as measurements from a physical multi-socket NUMA system.

Execution times can also vary with VM scheduling and system load.
10. Conclusion
The experiment demonstrated cache coherence behavior using MSI, MESI and MOESI simulation, investigated false sharing through adjacent and padded counters, and evaluated local and remote NUMA memory placement.

The results highlight the importance of cache-line-aware data layout and memory placement in parallel computing.
