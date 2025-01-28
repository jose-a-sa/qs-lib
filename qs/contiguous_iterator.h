
#include <qs/base.h>

#include <iterator>

QS_NAMESPACE_BEGIN

template<class T>
class contiguous_iterator
{
public:
    using iterator_type     = T;
    using value_type        = typename std::iterator_traits<iterator_type>::value_type;
    using difference_type   = typename std::iterator_traits<iterator_type>::difference_type;
    using pointer           = typename std::iterator_traits<iterator_type>::pointer;
    using reference         = typename std::iterator_traits<iterator_type>::reference;
    using iterator_category = typename std::iterator_traits<iterator_type>::iterator_category;

    QS_CONSTEXPR11 contiguous_iterator() QS_NOEXCEPT = default;

    QS_CONSTEXPR11 explicit contiguous_iterator(iterator_type i) QS_NOEXCEPT : i_{i} {}

    template<class U, class = std::enable_if_t<std::is_convertible<U, iterator_type>::value>>
    QS_CONSTEXPR11 explicit contiguous_iterator(contiguous_iterator<U> const& u) QS_NOEXCEPT : i_{u.base()}
    {}

    QS_CONSTEXPR11 iterator_type base() const QS_NOEXCEPT { return i_; }

    // clang-format off
    QS_CONSTEXPR11 reference operator*()                   const QS_NOEXCEPT { return *i_; }
    QS_CONSTEXPR11 pointer   operator->()                  const QS_NOEXCEPT { return qs::to_address(i_); }
    QS_CONSTEXPR11 reference operator[](difference_type n) const QS_NOEXCEPT { return i_[n]; }

    QS_CONSTEXPR11 contiguous_iterator& operator++()    QS_NOEXCEPT { ++i_; return *this; }
    QS_CONSTEXPR11 contiguous_iterator  operator++(int) QS_NOEXCEPT { contiguous_iterator tmp(*this); ++(*this); return tmp; }
    QS_CONSTEXPR11 contiguous_iterator& operator--()    QS_NOEXCEPT { --i_; return *this; }
    QS_CONSTEXPR11 contiguous_iterator  operator--(int) QS_NOEXCEPT { contiguous_iterator tmp(*this); --(*this); return tmp; }

    QS_CONSTEXPR11 contiguous_iterator& operator+=(difference_type n) QS_NOEXCEPT       { i_ += n;  return *this; }
    QS_CONSTEXPR11 contiguous_iterator  operator+(difference_type n)  const QS_NOEXCEPT { contiguous_iterator w(*this); w += n; return w; }
    QS_CONSTEXPR11 contiguous_iterator& operator-=(difference_type n) QS_NOEXCEPT       { return *this += -n; }
    QS_CONSTEXPR11 contiguous_iterator  operator-(difference_type n)  const QS_NOEXCEPT { contiguous_iterator w(*this); w += -n; return w; }
    // clang-format on

private:
    iterator_type i_{};

    // QS_CONSTEXPR11 explicit contiguous_iterator(iterator_type i) QS_NOEXCEPT : i_{i} {}

    // // allow contiguous_iterator to instanciate itself
    // template<class U>
    // friend class contiguous_iterator;
    // // allow vector to instanciate contiguous_iterator
    // template<class U, class Alloc>
    // friend class vector;
};


template<class U>
QS_CONSTEXPR11 bool operator==(contiguous_iterator<U> const& x, contiguous_iterator<U> const& y) QS_NOEXCEPT
{
    return x.base() == y.base();
}

template<class U, class V>
QS_CONSTEXPR11 bool operator==(contiguous_iterator<U> const& x, contiguous_iterator<V> const& y) QS_NOEXCEPT
{
    return x.base() == y.base();
}

template<class U>
QS_CONSTEXPR11 bool operator<(contiguous_iterator<U> const& x, contiguous_iterator<U> const& y) QS_NOEXCEPT
{
    return x.base() < y.base();
}

template<class U, class V>
QS_CONSTEXPR11 bool operator<(contiguous_iterator<U> const& x, contiguous_iterator<V> const& y) QS_NOEXCEPT
{
    return x.base() < y.base();
}

template<class U>
QS_CONSTEXPR11 bool operator!=(contiguous_iterator<U> const& x, contiguous_iterator<U> const& y) QS_NOEXCEPT
{
    return !(x == y);
}

template<class U, class V>
QS_CONSTEXPR11 bool operator!=(contiguous_iterator<U> const& x, contiguous_iterator<V> const& y) QS_NOEXCEPT
{
    return !(x == y);
}

template<class U>
QS_CONSTEXPR11 bool operator>(contiguous_iterator<U> const& x, contiguous_iterator<U> const& y) QS_NOEXCEPT
{
    return y < x;
}

template<class U, class V>
QS_CONSTEXPR11 bool operator>(contiguous_iterator<U> const& x, contiguous_iterator<V> const& y) QS_NOEXCEPT
{
    return y < x;
}

template<class U>
QS_CONSTEXPR11 bool operator>=(contiguous_iterator<U> const& x, contiguous_iterator<U> const& y) QS_NOEXCEPT
{
    return !(x < y);
}

template<class U, class V>
QS_CONSTEXPR11 bool operator>=(contiguous_iterator<U> const& x, contiguous_iterator<V> const& y) QS_NOEXCEPT
{
    return !(x < y);
}

template<class U>
QS_CONSTEXPR11 bool operator<=(contiguous_iterator<U> const& x, contiguous_iterator<U> const& y) QS_NOEXCEPT
{
    return !(y < x);
}

template<class U, class V>
QS_CONSTEXPR11 bool operator<=(contiguous_iterator<U> const& x, contiguous_iterator<V> const& y) QS_NOEXCEPT
{
    return !(y < x);
}

template<class U, class V>
QS_CONSTEXPR11 auto operator-(contiguous_iterator<U> const& x,
                              contiguous_iterator<V> const& y) QS_NOEXCEPT->decltype(x.base() - y.base())
{
    return x.base() - y.base();
}

template<class U>
QS_CONSTEXPR11 contiguous_iterator<U> operator+(typename contiguous_iterator<U>::difference_type n,
                                                contiguous_iterator<U>                           x) QS_NOEXCEPT
{
    x += n;
    return x;
}


QS_NAMESPACE_END
