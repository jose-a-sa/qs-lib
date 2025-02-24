#include <benchmark/benchmark.h>
#include "qs/config.h"

#include <algorithm>
#include <cmath>
#include <random>
#include <vector>

#include <fmt/core.h>

// This code is still in development

QS_NAMESPACE_BEGIN

namespace bench
{
    // Benchmark function to measure cache size effects
    static void BM_CacheSize(benchmark::State& s)
    {
        using int_type = int64_t;

        // Calculate the number of bytes and elements
        auto const bytes = 1 << s.range(0);
        auto const count = bytes / sizeof(int_type);

        // Initialize random number generator
        std::mt19937_64                         eng(std::random_device{}());
        std::uniform_int_distribution<int_type> rand_unif(int_type{}, std::numeric_limits<int_type>::max() - 1);

        // Create and fill a vector with random values
        std::vector<int> v(count);
        std::generate(v.begin(), v.end(), [&]{ return rand_unif(eng); });

        // Reseed the random number generator
        eng.seed(std::random_device{}());

        // Create and shuffle an index vector
        std::vector<size_t> indx(count);
        std::iota(indx.begin(), indx.end(), 0ull);
        std::shuffle(indx.begin(), indx.end(), eng);

        // Benchmark loop
        for(auto _: s)
        {
            double sum = 0.;
            for(auto i: indx)
                sum += v[i];
            benchmark::DoNotOptimize(sum);
        }

        // Set benchmark counters
        s.counters["size"] = benchmark::Counter(
            static_cast<double>(bytes), benchmark::Counter::kDefaults, benchmark::Counter::OneK::kIs1024);
        s.counters["rate"] = benchmark::Counter(static_cast<double>(s.iterations() * bytes),
                                                benchmark::Counter::kIsRate,
                                                benchmark::Counter::OneK::kIs1024);
    }
    // Register the benchmark with a range of sizes
    BENCHMARK(BM_CacheSize)->DenseRange(4, 28)->ReportAggregatesOnly(true);


    // Benchmark function to measure small string optimization
    static void BM_SmallStringOptimization(benchmark::State& s)
    {
        int const string_len = s.range(0);

        // Benchmark loop
        for(auto _: s)
        {
            std::string str(string_len, 'a');
            benchmark::DoNotOptimize(str);
        }
    }
    // Register the benchmark with a range of string lengths
    BENCHMARK(BM_SmallStringOptimization)->DenseRange(1, 32);

} // namespace bench

QS_NAMESPACE_END

// Main function to run the benchmarks
BENCHMARK_MAIN();
