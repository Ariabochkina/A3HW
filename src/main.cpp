#include "hyperloglog.hpp"
#include "hyperloglog_improved.hpp"
#include "hash_func_gen.hpp"
#include "random_stream_gen.hpp"
#include <algorithm>
#include <cmath>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>

int main(int argc, char* argv[]) {
    std::vector<std::size_t> sizes = {10000, 50000, 100000};
    int B = 10;
    double step = 5.0;
    int runs = 20;
    std::string out_path = "data/experiment_results.csv";
    bool improved = false;

    if (argc >= 2) out_path = argv[1];
    if (argc >= 3) B = std::atoi(argv[2]);
    if (argc >= 4) runs = std::max(1, std::atoi(argv[3]));
    if (argc >= 5 && std::string(argv[4]) == "improved") {
        improved = true;
        if (out_path == "data/experiment_results.csv")
            out_path = "data/experiment_results_improved.csv";
    }

    FNV1aHash32 hash_fn;
    std::ofstream fout(out_path);
    if (!fout) {
        std::cerr << "Не удалось открыть " << out_path << "\n";
        return 1;
    }
    fout << "run,stream_size,B,step_pct,step_idx,prefix_size,F_t0,N_t\n";

    RandomStreamGen gen;
    for (std::size_t sz : sizes) {
        for (int r = 0; r < runs; ++r) {
            gen.set_seed(static_cast<std::uint64_t>(r * 12345 + sz));
            gen.generate(sz);
            std::vector<double> steps;
            for (double p = step; p <= 100.0; p += step)
                steps.push_back(p / 100.0);

            for (double p : steps) {
                auto [beg, end] = gen.prefix(p);
                std::size_t n_prefix = static_cast<std::size_t>(end - beg);
                std::size_t F_t0 = exact_count(beg, end);
                double N_t;
                if (improved) {
                    HyperLogLogImproved hll(B, &hash_fn);
                    for (auto it = beg; it != end; ++it) hll.add(*it);
                    N_t = hll.estimate();
                } else {
                    HyperLogLog hll(B, &hash_fn);
                    for (auto it = beg; it != end; ++it) hll.add(*it);
                    N_t = hll.estimate();
                }
                int idx = static_cast<int>(std::round(p * 100.0 / step));
                fout << r << "," << sz << "," << B << "," << (p * 100.0) << ","
                    << idx << "," << n_prefix << "," << F_t0 << ","
                    << static_cast<std::size_t>(std::round(N_t)) << "\n";
            }
        }
    }
    std::cout << "Записано: " << out_path << (improved ? " (улучш.)" : "") << "\n";
    return 0;
}
