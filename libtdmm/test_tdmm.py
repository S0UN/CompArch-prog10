import ctypes
import time
import os
import numpy as np
import matplotlib.pyplot as plt
from subprocess import call
import random

# Compile the C code into a shared library
if not os.path.exists("libtdmm.so"):
    call(["gcc", "-shared", "-fPIC", "tdmm.c", "-o", "libtdmm.so"])

# Load the shared library
lib = ctypes.CDLL("./libtdmm.so")

# Define function signatures
lib.t_init.argtypes = [ctypes.c_int]
lib.t_init.restype = None
lib.t_malloc.argtypes = [ctypes.c_size_t]
lib.t_malloc.restype = ctypes.c_void_p
lib.t_free.argtypes = [ctypes.c_void_p]
lib.t_free.restype = None

# Allocation strategies
FIRST_FIT, BEST_FIT, WORST_FIT = 0, 1, 2
STRATEGIES = {"First Fit": FIRST_FIT, "Best Fit": BEST_FIT, "Worst Fit": WORST_FIT}

# Test parameters
METADATA = 16  # Size of Block struct
REPEATS = 1
NUM_TEST_CASES = 1000
TOTAL_OPS = 1000
NUM_MALLOCS = 550
NUM_FREES = 450

def run_test(strategy, sizes):
    lib.t_init(strategy)
    pointers = []
    alloc_times = []
    free_times = []
    utilization = []
    total_allocated = 0
    total_requested = 4096 * 4
    peak_overhead = 0

    operations = ['malloc'] * NUM_MALLOCS + ['free'] * NUM_FREES
    np.random.shuffle(operations)

    start_time = time.perf_counter()

    for op in operations:
        current_overhead = (len(pointers) * METADATA) / total_requested * 100
        peak_overhead = max(peak_overhead, current_overhead)

        if op == 'malloc':
            size = np.random.choice(sizes)
            t0 = time.perf_counter_ns()
            ptr = lib.t_malloc(size)
            t1 = time.perf_counter_ns()
            alloc_times.append((size, (t1 - t0) / 1e9))
            if ptr:
                pointers.append((ptr, size))
                total_allocated += size + METADATA
                if total_allocated > total_requested:
                    new_chunk = max(4096 * 4, size + METADATA)
                    total_requested += new_chunk
        elif op == 'free' and pointers:
            idx = np.random.randint(len(pointers))
            ptr, size = pointers.pop(idx)
            t0 = time.perf_counter_ns()
            lib.t_free(ptr)
            t1 = time.perf_counter_ns()
            free_times.append((size, (t1 - t0) / 1e9))
            total_allocated -= (size + METADATA)

        utilization.append((time.perf_counter() - start_time, total_allocated / total_requested))

    total_time = time.perf_counter() - start_time
    for ptr, _ in pointers:
        lib.t_free(ptr)

    return {
        "avg_utilization": np.mean([u[1] for u in utilization]) * 100,
        "utilization_over_time": utilization,
        "total_time": total_time,
        "alloc_times": alloc_times,
        "free_times": free_times,
        "overhead": peak_overhead
    }

# Generate test cases
TEST_CASES = {
    "ExtremeLargeAllocations": {
        "sizes": [2**i for i in range(20, 28)],  # 1MB to 256MB
        "num_ops": 200,
        "free_prob": 0.3,
    },
    "HighConcurrencySmall": {
        "sizes": [i for i in range(16, 513)],  # 16–512 bytes
        "num_ops": 20000,
        "free_prob": 0.1,
    },
    "FragmentationNightmare": {
        "sizes": [16, 256, 4096, 65536],
        "num_ops": 20000,
        "free_prob": 0.5,
    },
}

size_options = [
    [i for i in range(1, 257)],         
    [2**i for i in range(20, 24)],        
    [2**i for i in range(0, 24, 2)],    
    [16, 1048576],                        
    [2**i for i in range(0, 24)]       
]
for i in range(NUM_TEST_CASES):
    size_range = random.choice(size_options)
    TEST_CASES[f"Test_{i:04d}"] = {"sizes": size_range}

# Run tests
results = {}
total_test_cases = 0
for test_name, params in TEST_CASES.items():
    results[test_name] = {}
    for strat_name, strat in STRATEGIES.items():
        print(f"Running {test_name} with {strat_name}...")
        avg_results = {"avg_utilization": 0, "total_time": 0, "alloc_times": [], "free_times": [], "overhead": 0}
        util_time = []
        for _ in range(REPEATS):
            res = run_test(strat, params["sizes"])
            avg_results["avg_utilization"] += res["avg_utilization"] / REPEATS
            avg_results["total_time"] += res["total_time"] / REPEATS
            avg_results["alloc_times"].extend(res["alloc_times"])
            avg_results["free_times"].extend(res["free_times"])
            avg_results["overhead"] += res["overhead"] / REPEATS
            if not util_time:
                util_time = res["utilization_over_time"]
            total_test_cases += 1
        results[test_name][strat_name] = avg_results
        results[test_name][strat_name]["utilization_over_time"] = util_time

# Aggregate results
agg_results = {strat: {"avg_utilization": [], "total_time": [], "alloc_times": [], "free_times": [], "overhead": []}
               for strat in STRATEGIES}
for test_name in TEST_CASES:
    for strat_name in STRATEGIES:
        res = results[test_name][strat_name]
        agg_results[strat_name]["avg_utilization"].append(res["avg_utilization"])
        agg_results[strat_name]["total_time"].append(res["total_time"])
        agg_results[strat_name]["alloc_times"].extend(res["alloc_times"])
        agg_results[strat_name]["free_times"].extend(res["free_times"])
        agg_results[strat_name]["overhead"].append(res["overhead"])

# Plotting
plt.figure(figsize=(15, 10))
plt.suptitle(f"Aggregated Results Across {NUM_TEST_CASES} Test Cases (550 Mallocs, 450 Frees)")

# 1. %age memory utilization on average
plt.subplot(2, 3, 1)
plt.boxplot([agg_results[strat]["avg_utilization"] for strat in STRATEGIES], labels=STRATEGIES.keys())
plt.title("Avg Memory Utilization (%)")
plt.ylabel("%")

# 2. %age memory utilization as a function of time (sample)
plt.subplot(2, 3, 2)
test_name = "Test_0000"
for name in STRATEGIES:
    times, utils = zip(*results[test_name][name]["utilization_over_time"])
    plt.plot(times, [u * 100 for u in utils], label=name)
plt.title(f"Utilization Over Time (Sample: {test_name})")
plt.xlabel("Time (s)")
plt.ylabel("%")
plt.legend()

# 3. Overall speed
plt.subplot(2, 3, 3)
plt.boxplot([agg_results[strat]["total_time"] for strat in STRATEGIES], labels=STRATEGIES.keys())
plt.title("Overall Speed")
plt.ylabel("Time (s)")

# 4. Speeds of t_malloc() and t_free() vs size
plt.subplot(2, 3, 4)
for name in STRATEGIES:
    sizes, times = zip(*agg_results[name]["alloc_times"])
    plt.loglog(sizes, times, '.', label=name, alpha=0.5)
plt.title("t_malloc Speed vs Size")
plt.xlabel("Size (bytes)")
plt.ylabel("Time (s)")
plt.legend()

plt.subplot(2, 3, 5)
for name in STRATEGIES:
    sizes, times = zip(*agg_results[name]["free_times"])
    plt.loglog(sizes, times, '.', label=name, alpha=0.5)
plt.title("t_free Speed vs Size")
plt.xlabel("Size (bytes)")
plt.ylabel("Time (s)")
plt.legend()

# 5. Overhead
plt.subplot(2, 3, 6)
plt.boxplot([agg_results[strat]["overhead"] for strat in STRATEGIES], labels=STRATEGIES.keys())
plt.title("Peak Overhead (%)")
plt.ylabel("%")

plt.tight_layout(rect=[0, 0, 1, 0.95])
plt.savefig("aggregated_results.png")
plt.close()

# Print results
print(f"\nTotal Test Cases Run: {total_test_cases}")
for strat_name in STRATEGIES:
    print(f"\n=== Aggregated Results for {strat_name} ===")
    print(f"  Avg Utilization: {np.mean(agg_results[strat_name]['avg_utilization']):.2f}% "
          f"(± {np.std(agg_results[strat_name]['avg_utilization']):.2f}%)")
    print(f"  Total Time: {np.mean(agg_results[strat_name]['total_time']):.2f}s "
          f"(± {np.std(agg_results[strat_name]['total_time']):.2f}s)")
    print(f"  Avg t_malloc Time: {np.mean([t for _, t in agg_results[strat_name]['alloc_times']]):.6f}s")
    print(f"  Avg t_free Time: {np.mean([t for _, t in agg_results[strat_name]['free_times']]):.6f}s")
    print(f"  Overhead: {np.mean(agg_results[strat_name]['overhead']):.2f}% "
          f"(± {np.std(agg_results[strat_name]['overhead']):.2f}%)")