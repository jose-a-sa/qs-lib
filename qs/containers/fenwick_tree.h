#ifndef QS_CONTAINERS_FENWICKTREE_H_
#define QS_CONTAINERS_FENWICKTREE_H_

#include <qs/config.h>

#include <vector>
#include <stdexcept>


QS_NAMESPACE_BEGIN

template<class T, class Allocator = std::allocator<T>>
class FenwickTree
{
public:
    using value_type      = remove_cvref_t<T>;
    using allocator_type  = Allocator;
    using reference       = value_type&;
    using const_reference = value_type const&;
    using size_type       = typename std::allocator_traits<allocator_type>::size_type;
    using difference_type = typename std::allocator_traits<allocator_type>::difference_type;
    using pointer         = typename std::allocator_traits<allocator_type>::pointer;
    using const_pointer   = typename std::allocator_traits<allocator_type>::const_pointer;
    using ssize_type      = typename std::common_type<std::ptrdiff_t, typename std::make_signed<size_type>::type>::type;

    QS_CONSTEXPR14 explicit FenwickTree(size_type);
    QS_CONSTEXPR14 explicit FenwickTree(allocator_type const&);
    QS_CONSTEXPR14 FenwickTree(size_type, allocator_type const&); // C++14
    QS_CONSTEXPR14 FenwickTree(size_type, value_type const&, allocator_type const& = allocator_type());

    template<class InputIterator>
    QS_CONSTEXPR14 FenwickTree(InputIterator, InputIterator, allocator_type const& = allocator_type());

    QS_CONSTEXPR11 size_type     size() const;
    QS_CONSTEXPR11 ssize_type    ssize() const { return static_cast<ssize_type>(size()); };
    QS_CONSTEXPR11 const_pointer data() const;

    QS_CONSTEXPR14 void update(size_type, const_reference);

    QS_CONSTEXPR14 value_type query(size_type) const;

    QS_CONSTEXPR14 void resize(size_type);

private:
    std::vector<value_type, allocator_type> tree_;

    template<class Index>
    QS_CONSTEXPR11 Index increment_binary_index(Index) const noexcept;

    template<class Index>
    QS_CONSTEXPR11 Index decrement_binary_index(Index) const noexcept;

    QS_CONSTEXPR14 size_type recommend_size(size_type) const;

    QS_CONSTEXPR14 void build_prefix_tree(size_type = 0, size_type = std::numeric_limits<size_type>::max());
};


template<class T, class Allocator>
QS_CONSTEXPR14 FenwickTree<T, Allocator>::FenwickTree(size_type n)
    : tree_(n + 1)
{}

template<class T, class Allocator>
QS_CONSTEXPR14 FenwickTree<T, Allocator>::FenwickTree(allocator_type const& a)
    : tree_(a)
{}

template<class T, class Allocator>
QS_CONSTEXPR14 FenwickTree<T, Allocator>::FenwickTree(size_type n, allocator_type const& a)
    : tree_(n + 1, a)
{}

template<class T, class Allocator>
QS_CONSTEXPR14 FenwickTree<T, Allocator>::FenwickTree(size_type n, value_type const& x, allocator_type const& a)
    : tree_(n + 1, x, a)
{
    build_prefix_tree();
}

template<class T, class Allocator>
template<class InputIterator>
QS_CONSTEXPR14 FenwickTree<T, Allocator>::FenwickTree(InputIterator first, InputIterator last, allocator_type const& a)
    : tree_(1, a)
{
    size_type const n = std::distance(first, last);
    tree_.resize(n + 1);
    //tree_.insert(tree_.end(), first, last);
    size_t i = 0;
    while(first != last)
        update(i++, *first++);
    build_prefix_tree();
}

template<class T, class Allocator>
QS_CONSTEXPR11 typename FenwickTree<T, Allocator>::size_type FenwickTree<T, Allocator>::size() const
{
    return tree_.empty() ? 0 : tree_.size() - 1;
}

template<class T, class Allocator>
QS_CONSTEXPR11 typename FenwickTree<T, Allocator>::const_pointer FenwickTree<T, Allocator>::data() const
{
    return tree_.data();
}

template<class T, class Allocator>
QS_CONSTEXPR14 void FenwickTree<T, Allocator>::update(size_type index, const_reference increment)
{
    QS_ASSERT(index <= tree_.size(), "FenwickTree index out of bounds");
    for(size_type idx = index + 1; idx < tree_.size(); idx = increment_binary_index(idx))
        tree_[idx] += increment;
}

template<class T, class Allocator>
QS_CONSTEXPR14 typename FenwickTree<T, Allocator>::value_type FenwickTree<T, Allocator>::query(size_type end) const
{
    QS_ASSERT(end < tree_.size(), "FenwickTree index out of bounds");
    value_type result{};
    for(size_type idx = end; idx > 0; idx = decrement_binary_index(idx))
        result += tree_[idx];
    return result;
}

template<class T, class Allocator>
QS_CONSTEXPR14 void FenwickTree<T, Allocator>::resize(size_type new_size)
{
    size_t const old_size = tree_.size();
    tree_.resize(new_size + 1);
    build_prefix_tree(old_size);
}

template<class T, class Allocator>
template<class Index>
QS_CONSTEXPR11 Index FenwickTree<T, Allocator>::increment_binary_index(Index index) const noexcept
{
    return index + (index & (-index));
}

template<class T, class Allocator>
template<class Index>
QS_CONSTEXPR11 Index FenwickTree<T, Allocator>::decrement_binary_index(Index index) const noexcept
{
    return index - (index & (-index));
}

template<class T, class Allocator>
QS_CONSTEXPR14 typename FenwickTree<T, Allocator>::size_type
FenwickTree<T, Allocator>::recommend_size(size_type new_size) const
{
    size_type const ms = tree_.max_size();
    if(new_size > ms)
        throw std::length_error("qs::FenwickTree");
    size_type const cap = tree_.capacity();
    if(cap >= ms / 2)
        return ms;
    return std::max(2 * cap, new_size);
}

template<class T, class Allocator>
QS_CONSTEXPR14 void FenwickTree<T, Allocator>::build_prefix_tree(size_type first, size_type last)
{
    if(!(first < last))
        return;

    size_type const start = 1;
    size_type const end   = (last < tree_.size()) ? last : tree_.size();

    for(size_type i = start; i < end; ++i)
    {
        auto const parent = increment_binary_index(i);
        if(first <= parent && parent < end)
            tree_[parent] += tree_[i];
    }
}


#if defined(__cpp_deduction_guides)

template<class InputIterator>
FenwickTree(InputIterator, InputIterator) -> FenwickTree<typename std::iterator_traits<InputIterator>::value_type>;

#endif


QS_NAMESPACE_END

#endif // QS_CONTAINERS_FENWICKTREE_H_


// template<typename It, typename Val>
// void add_fenwick(It beg, size_t n, size_t i, Val val)
// {
//     for(; i < n; i |= i + 1)
//         beg[i] += val;
// }
// template<typename It, typename Val>
// Val sum_fenwick(It beg, size_t i, Val sum)
// {
//     for(; i > 0; i = i & (i - 1))
//         sum += beg[i - 1];
//     return sum;
// }
