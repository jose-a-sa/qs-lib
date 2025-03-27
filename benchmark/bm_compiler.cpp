#include <benchmark/benchmark.h>

#include <cstdint>
#include <qs/config.h>

#include <algorithm>
#include <random>
#include <vector>

#include <fmt/core.h>


// Benchmark function to measure cache size effects
static void BM_CacheSize(benchmark::State& s)
{
    using value_t = int64_t;

    // Calculate the number of bytes and elements
    auto const bytes = [&](int64_t n)
    {
        auto const sz = 1 << (n / 2);
        return sz + (n % 2) * (sz / 2);
    }(s.range(0));

    auto const count = bytes / sizeof(value_t);

    // Initialize random number generator
    std::mt19937_64                        eng(std::random_device{}());
    std::uniform_int_distribution<int64_t> dist{};

    std::vector<value_t> data(count);
    std::generate(data.begin(), data.end(), [&] { return dist(eng); });

    eng.seed(std::random_device{}());

    // Create and shuffle an index vector
    std::vector<size_t> indx(count);
    std::iota(indx.begin(), indx.end(), 0ull);
    std::shuffle(indx.begin(), indx.end(), eng);

    value_t sum{};
    benchmark::DoNotOptimize(sum);

    for(auto i: indx)
        sum += data[i];

    sum = {};
    for(auto i: indx)
        sum += data[i];

    // Benchmark loop
    for(auto _: s)
    {
        value_t sum = 0;
        for(auto i: indx)
            sum += data[i];
        benchmark::DoNotOptimize(sum);
    }

    // Set benchmark counters
    s.counters["size"] = benchmark::Counter(static_cast<double>(bytes), benchmark::Counter::kDefaults,
                                            benchmark::Counter::OneK::kIs1024);
    s.counters["rate"] = benchmark::Counter(static_cast<double>(s.iterations() * bytes), benchmark::Counter::kIsRate,
                                            benchmark::Counter::OneK::kIs1024);
}
// Register the benchmark with a range of sizes
BENCHMARK(BM_CacheSize)->DenseRange(6, 60)->ReportAggregatesOnly(true);


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


// Main function to run the benchmarks
BENCHMARK_MAIN();
