#include "os_dependent.h"

#include <sched.h>

int sqaod::getDefaultNumThreads() {
#ifdef _OPENMP
    cpu_set_t cpuset;
    CPU_ZERO(&cpuset);
    sched_getaffinity(0, sizeof(cpuset), &cpuset);
    return CPU_COUNT(&cpuset);
#else
    return 1;
#endif
}

#include <stdlib.h>

void *sqaod::aligned_alloc(int alignment, size_t size) {
    return ::aligned_alloc(alignment, size);
}

void sqaod::aligned_free(void *pv) {
    ::free(pv);
}
