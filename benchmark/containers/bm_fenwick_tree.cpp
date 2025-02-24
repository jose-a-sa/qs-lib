#include <benchmark/benchmark.h>

#include "qs/config.h"
#include "qs/containers/fenwick_tree.h"

#include <vector>

QS_NAMESPACE_BEGIN

namespace bench
{
    static void BM_FenwickTree_query(benchmark::State& state)
    {
        auto const N = state.range(0);
        std::vector<int> test(N);
        for(int i = 0; i < N; ++i)
            test[i] = i + 1;

        FenwickTree<int> const tree(test.begin(), test.end());
        auto val = tree.query(N - 1);

        for(auto _: state)
        {
            // worst case scenario 2^k - 1
            val = tree.query(N - 1);
        }

        benchmark::DoNotOptimize(val);
        state.SetComplexityN(N);
    }
    BENCHMARK(BM_FenwickTree_query)
        ->RangeMultiplier(2)
        ->Range(16, 16 << 16)
        ->Complexity()
        ->DisplayAggregatesOnly();

    static void BM_FenwickTree_update(benchmark::State& state)
    {
        auto const N = state.range(0);
        std::vector<int> test(N);
        for(int i = 0; i < N; ++i)
            test[i] = i + 1;

        FenwickTree<int> tree(test.begin(), test.end());

        for(auto _: state)
        {
            tree.update(1, 1);
        }

        benchmark::DoNotOptimize(tree);
        state.SetComplexityN(N);
    }
    BENCHMARK(BM_FenwickTree_update)
        ->RangeMultiplier(2)
        ->Range(16, 16 << 16)
        ->Complexity()
        ->DisplayAggregatesOnly();

    static void BM_FenwickTree_vectorIteratorConstruction(benchmark::State& state)
    {
        auto const N = state.range(0);
        std::vector<int> test(N);
        for(int i = 0; i < N; ++i)
            test[i] = i + 1;

        for(auto _: state)
        {
            FenwickTree<int> tree(test.begin(), test.end());
        }

        state.SetComplexityN(N);
    }
    BENCHMARK(BM_FenwickTree_vectorIteratorConstruction)
        ->RangeMultiplier(2)
        ->Range(16, 16 << 16)
        ->Complexity()
        ->DisplayAggregatesOnly();

    static void BM_FenwickTree_resize(benchmark::State& state)
    {
        auto const N = state.range(0);

        for(auto _: state)
        {
            FenwickTree<int> tree(0);
            tree.resize(N);
            // benchmark::DoNotOptimize(tree);
        }

        state.SetComplexityN(N);
    }
    BENCHMARK(BM_FenwickTree_resize)
        ->RangeMultiplier(2)
        ->Range(16, 16 << 16)
        ->Complexity()
        ->DisplayAggregatesOnly();

    static void BM_FenwickTree_ResizeUpdate(benchmark::State& state)
    {
        auto const N = state.range(0);
        FenwickTree<int> tree(0);

        for(auto _: state)
        {
            tree.resize(0);
            for(int i = 1; i <= N; i++)
            {
                tree.resize(i + 1);
                tree.update(i, i);
            }
        }

        benchmark::DoNotOptimize(tree);
        state.SetComplexityN(N);
    }
    BENCHMARK(BM_FenwickTree_ResizeUpdate)
        ->RangeMultiplier(2)
        ->Range(16, 16 << 16)
        ->Complexity()
        ->DisplayAggregatesOnly();

} // namespace bench

QS_NAMESPACE_END

BENCHMARK_MAIN();
