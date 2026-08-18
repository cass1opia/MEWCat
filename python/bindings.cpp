#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

#include <tuple>
#include <vector>

#include "mewcat_solver.h"
#include "weighted_graph.h"

namespace py = pybind11;

static py::dict solve(int n,
                      const std::vector<std::tuple<int, int, int>> &edges,
                      int pls_iterations, bool verbose, double time_limit_sec) {
    if (n < 0) {
        throw std::invalid_argument("n must be non-negative");
    }

    MewcpResult result;
    {
        py::gil_scoped_release release;
        weighted_graph graph(n, edges);
        result = solve_mewcp(&graph, pls_iterations, verbose, time_limit_sec);
    }

    py::dict out;
    out["vertices"] = result.vertices;
    out["weight"] = result.weight;
    out["branch_count"] = result.branch_count;
    out["elapsed_time_sec"] = result.elapsed_time_sec;
    out["timed_out"] = result.timed_out;
    return out;
}

PYBIND11_MODULE(mewcat, m) {
    m.doc() =
        "MEWCat: a fast exact solver for the Maximum Edge-Weighted Clique "
        "Problem (Liu, Xiao, Zhou, AAAI-24).";

    m.def("solve", &solve, py::arg("n"), py::arg("edges"),
          py::arg("pls_iterations") = 10, py::arg("verbose") = false,
          py::arg("time_limit_sec") = -1.0,
          R"doc(Solve the Maximum Edge-Weighted Clique Problem.

Parameters
----------
n : int
    Number of vertices. Vertices are 0-based (0 .. n-1).
edges : list[tuple[int, int, int]]
    Undirected edges as (u, v, weight) with positive integer weights.
    Self-loops and out-of-range vertices are ignored; duplicate edges keep
    the last given weight.
pls_iterations : int, optional
    Iterations of the Phased Local Search heuristic used to seed a lower
    bound (default 10). Set to 0 to disable.
verbose : bool, optional
    Print progress/statistics to stdout (default False).
time_limit_sec : float, optional
    Wall-clock time limit in seconds (default -1.0 = no limit). If the
    limit is reached, the best solution found so far is returned and
    "timed_out" is set to True in the result dict.

Returns
-------
dict
    {
        "vertices": list[int],       # 0-based vertices of the optimal clique
        "weight": int,               # total edge weight of the clique
        "branch_count": int,         # explored branch-and-bound nodes
        "elapsed_time_sec": float,   # search time (excluding PLS)
        "timed_out": bool,           # True if time limit was reached
    }
)doc");
}
