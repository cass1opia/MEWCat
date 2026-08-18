#ifndef mewcat_solver_h
#define mewcat_solver_h

#include <vector>

#include "weighted_graph.h"

struct MewcpResult {
    std::vector<int> vertices;   // 0-based vertex ids of the maximum edge-weighted clique
    int weight;                  // total edge weight of the clique
    unsigned long branch_count;  // number of explored branch-and-bound nodes
    double elapsed_time_sec;     // search time (without PLS)
    bool timed_out;              // true if the search was interrupted by the time limit
};

MewcpResult solve_mewcp(weighted_graph *graph, int pls_iterations = 10,
                        bool verbose = false, double time_limit_sec = -1.0);

#endif
