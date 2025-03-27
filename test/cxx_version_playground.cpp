#include <qs/config.h>
#include <qs/span.h>

#include <array>

template<class T, size_t N, size_t SN>
int check_span()
{
    using qs::intl::do_not_optimize;

    std::array<T, N == qs::dynamic_extent ? 100 : N> arr{};
    qs::span<T, N>                                   s1{arr};
    // [span.obs]
    do_not_optimize(s1.size());
    do_not_optimize(s1.size_bytes());
    do_not_optimize(s1.empty());
    // [span.elem]
    do_not_optimize(s1[0]);
    do_not_optimize(s1.at(0));
    do_not_optimize(s1.front());
    do_not_optimize(s1.back());
    // [span.iter]
    do_not_optimize(s1.begin());
    do_not_optimize(s1.end());
    do_not_optimize(s1.rbegin());
    do_not_optimize(s1.rend());
    do_not_optimize(s1.cbegin());
    do_not_optimize(s1.cend());
    do_not_optimize(s1.crbegin());
    do_not_optimize(s1.crend());
    // [span.sub]
    do_not_optimize(s1.template first<SN>());
    do_not_optimize(s1.first(SN));
    do_not_optimize(s1.template last<SN>());
    do_not_optimize(s1.last(SN));
    do_not_optimize(s1.template subspan<0, SN>());
    do_not_optimize(s1.subspan(0, SN));
    do_not_optimize(s1.template subspan<SN>());
    do_not_optimize(s1.subspan(SN));
    // [span.objectrep]
    do_not_optimize(qs::as_bytes(s1));
    do_not_optimize(qs::as_writable_bytes(s1));
    return 0;
}


int main(int, char**)
{
    check_span<int, qs::dynamic_extent, 5>();
    check_span<int, 10, 5>();
    return 0;
}
