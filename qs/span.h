#ifndef QS_SPAN_H
#define QS_SPAN_H

#include <qs/config.h>
#include <qs/memory.h>
#include <qs/traits/iterator.h>
#include <qs/traits/ranges.h>

#include <stdexcept>
#include <type_traits>


QS_NAMESPACE_BEGIN


QS_INLINE_VAR constexpr size_t dynamic_extent = std::numeric_limits<size_t>::max();


template<class T, size_t Extent = dynamic_extent>
class span;


namespace intl
{
    template<class>
    struct is_std_array : std::false_type
    {};
    template<class T, size_t S>
    struct is_std_array<std::array<T, S>> : std::true_type
    {};


    template<class>
    struct is_span : std::false_type
    {};
    template<class T, size_t S>
    struct is_span<span<T, S>> : std::true_type
    {};


    template<class FromType, class ToType, size_t FromExtent = dynamic_extent, size_t ToExtent = dynamic_extent>
    struct is_span_convertible
        : std::integral_constant<bool, (ToExtent == dynamic_extent || FromExtent == ToExtent) &&
                                           std::is_convertible<FromType (*)[], ToType (*)[]>::value>
    {};

    template<class It, class T, class = void>
    struct is_span_compatible_iterator : std::false_type
    {};
    template<class It, class T>
    struct is_span_compatible_iterator<It, T, void_t<iter_reference_t<It>>>
        : conjunction<is_contiguous_iterator<It>, is_span_convertible<remove_reference_t<iter_reference_t<It>>, T>>
    {};


    template<class Sentinel, class It>
    struct is_span_compatible_sentinel : std::integral_constant<bool, is_sized_sentinel_for<Sentinel, It>::value &&
                                                                          !std::is_convertible<Sentinel, size_t>::value>
    {};

    template<class Rng, class Elem, class = void>
    struct is_span_compatible_range : std::false_type
    {};

    template<class Rng, class Elem>
    struct is_span_compatible_range<Rng, Elem, void_t<range_reference_t<Rng>>>
        : std::integral_constant<bool,
                                 is_contiguous_range<Rng>::value && is_sized_range<Rng>::value &&
                                     (is_borrowed_range<Rng>::value || std::is_const<Rng>::value) &&
                                     !(is_span<Rng>::value || is_std_array<Rng>::value || std::is_array<Rng>::value) &&
                                     is_span_convertible<remove_reference_t<range_reference_t<Rng>>, Elem>::value>
    {};
} // namespace intl


// span implementation for fixed-size

template<class T, size_t Extent>
class span
{
public:
    using element_type           = T;
    using value_type             = remove_cv_t<T>;
    using size_type              = size_t;
    using difference_type        = ptrdiff_t;
    using pointer                = element_type*;
    using const_pointer          = element_type const*;
    using reference              = element_type&;
    using const_reference        = element_type const&;
    using iterator               = pointer; // implementation-defined (simplifying to pointer for now)
    using const_iterator         = const_pointer; //(since C++23)	std::const_iterator<iterator>
    using reverse_iterator       = std::reverse_iterator<iterator>;
    using const_reverse_iterator = std::reverse_iterator<const_iterator>;

    static constexpr size_t extent = Extent;


    // [span.cons], span constructors, copy, assignment, and destructor
    QS_CONSTEXPR11 span() noexcept
        : data_{nullptr} {};

    // NOTE: in optimized builds, temporary initializer_list leaves the span with dangling pointer (undefined state)
    template<class U = element_type, enable_if_t<std::is_const<U>::value, int> = 0>
    QS_CONSTEXPR11 explicit span(std::initializer_list<value_type> il)
        : data_{il.begin()}
    {
        QS_VERIFY(extent == il.size(), "Size mismatch in span's constructor Extent != il.size().");
    }

    QS_CONSTEXPR11       span(const span&) noexcept      = default;
    QS_CONSTEXPR11 span& operator=(const span&) noexcept = default;

    template<class Iterator, enable_if_t<intl::is_span_compatible_iterator<Iterator, element_type>::value, int> = 0>
    QS_CONSTEXPR11 explicit span(Iterator first, size_type count)
        : data_{to_address(first)}
    {
        QS_VERIFY(Extent == dynamic_extent || Extent == count, "size mismatch in span's constructor (iterator, len)");
    }

    template<class Iterator, class Sentinel,
             enable_if_t<intl::is_span_compatible_iterator<Iterator, element_type>::value &&
                             intl::is_span_compatible_sentinel<Sentinel, Iterator>::value,
                         int> = 0>
    QS_CONSTEXPR11 explicit span(Iterator first, Sentinel last)
        : data_(qs::to_address(first))
    {
        // [span.cons]/10
        // Throws: When and what last - first throws.
        QS_MAYBE_UNUSED auto dist = last - first;
        QS_VERIFY(dist >= 0, "invalid range in span's constructor (iterator, sentinel)");
        QS_VERIFY(Extent == dynamic_extent || Extent == dist,
                  "invalid range in span's constructor (iterator, sentinel): last - first != extent");
    }

    QS_CONSTEXPR11 span(type_identity_t<T> (&arr)[Extent]) noexcept
        : data_{arr}
    {}

    template<class V, enable_if_t<intl::is_span_convertible<V, element_type>::value, int> = 0>
    QS_CONSTEXPR11 span(std::array<V, Extent>& arr) noexcept
        : data_{arr.data()}
    {}

    template<class V, enable_if_t<intl::is_span_convertible<V const, element_type>::value, int> = 0>
    QS_CONSTEXPR11 span(std::array<V, Extent> const& arr) noexcept
        : data_{arr.data()}
    {}

    template<class V, enable_if_t<intl::is_span_convertible<V, element_type>::value, int> = 0>
    QS_CONSTEXPR11 span(span<V, Extent> const& other)
        : data_{other.data()}
    {}

    template<class V, enable_if_t<intl::is_span_convertible<V, element_type>::value, int> = 0>
    QS_CONSTEXPR11 explicit span(span<V, dynamic_extent> const& other) noexcept
        : data_{other.data()}
    {
        QS_VERIFY(Extent == other.size(), "size mismatch in span's constructor (other span)");
    }

    template<class Rng, enable_if_t<intl::is_span_compatible_range<Rng, T>::value, int> = 0>
    QS_CONSTEXPR11 explicit span(Rng&& r)
        : data_{qs::data(r)}
    {
        QS_VERIFY(Extent == dynamic_extent || Extent == qs::size(r), "size mismatch in span's constructor (range)");
    }

    // [span.obs], span observers
    QS_CONSTEXPR11 size_type size() const noexcept { return Extent; }
    QS_CONSTEXPR11 size_type size_bytes() const noexcept { return Extent * sizeof(element_type); }
    QS_CONSTEXPR11 bool      empty() const noexcept { return size() == 0; }

    // [span.elem], span element access //
    QS_CONSTEXPR11 reference operator[](size_type index) const noexcept(is_nothrow_contract_violation)
    {
        return QS_VERIFY(index < size(), "span<T, N>::operator[](index): index out of range"), this->data_[index];
    };
    QS_CONSTEXPR11 reference at(size_type index) const noexcept(is_nothrow_contract_violation)
    {
        return (index >= size()) ? (throw_out_of_range_("span"), this->data_[0]) : this->data_[index];
    }
    QS_CONSTEXPR11 reference front() const noexcept(is_nothrow_contract_violation)
    {
        return QS_VERIFY(!empty(), "span<T, N>::front() called on empty span"), this->data_[0];
    }
    QS_CONSTEXPR11 reference back() const noexcept(is_nothrow_contract_violation)
    {
        return QS_VERIFY(!empty(), "span<T, N>::back() called on empty span"), this->data_[size() - 1];
    }
    QS_CONSTEXPR11 pointer data() const noexcept { return this->data_; }

    // [span.iter], span iterator support
    QS_CONSTEXPR11 iterator               begin() const noexcept { return data(); };
    QS_CONSTEXPR11 iterator               end() const noexcept { return data() + size(); };
    QS_CONSTEXPR11 const_iterator         cbegin() const noexcept { return data(); };
    QS_CONSTEXPR11 const_iterator         cend() const noexcept { return data() + size(); };
    QS_CONSTEXPR11 reverse_iterator       rbegin() const noexcept { return reverse_iterator(end()); };
    QS_CONSTEXPR11 reverse_iterator       rend() const noexcept { return reverse_iterator(begin()); };
    QS_CONSTEXPR11 const_reverse_iterator crbegin() const noexcept { return const_reverse_iterator(end()); };
    QS_CONSTEXPR11 const_reverse_iterator crend() const noexcept { return const_reverse_iterator(begin()); }

    // [span.sub], span subviews
    template<size_t Count>
    QS_CONSTEXPR11 span<T, Count> first() const noexcept
    {
        static_assert(Count <= extent, "span<T, N>::first<Count>(): Count out of range");
        return span<T, Count>{data(), Count};
    }
    QS_CONSTEXPR11 span<T, dynamic_extent> first(size_type count) const noexcept(is_nothrow_contract_violation)
    {
        return QS_VERIFY(count <= size(), "span<T, N>::first(count): count out of range"),
               span<T, dynamic_extent>{data(), count};
    }

    template<size_t Count>
    QS_CONSTEXPR11 span<T, Count> last() const noexcept
    {
        static_assert(Count <= extent, "span<T, N>::last<Count>(): Count out of range");
        return span<T, Count>{data() + size() - Count, Count};
    }
    QS_CONSTEXPR11 span<T, dynamic_extent> last(size_type count) const noexcept(is_nothrow_contract_violation)
    {
        return QS_VERIFY(count <= size(), "span<T, N>::last(count): count out of range"),
               span<T, dynamic_extent>{data() + size() - count, count};
    }

    template<size_t Offset, size_t Count = dynamic_extent>
    QS_CONSTEXPR11 auto subspan() const noexcept -> span<T, Count != dynamic_extent ? Count : extent - Offset>
    {
        static_assert(Offset <= extent, "span<T>::subspan<Offset, Count>(): Offset out of range");
        static_assert(Count == dynamic_extent || Count <= extent - Offset,
                      "span<T>::subspan<Offset, Count>(): Offset + Count out of range");

        using return_t = span<T, Count != dynamic_extent ? Count : extent - Offset>;
        return return_t{data() + Offset, Count != dynamic_extent ? Count : extent - Offset};
    }

    QS_CONSTEXPR11 auto subspan(size_type offset, size_type count = dynamic_extent) const
        noexcept(is_nothrow_contract_violation) -> span<T, dynamic_extent>
    {
        return QS_VERIFY(offset <= size(), "span<T, N>::subspan(offset, count): offset out of range"),
               (count == dynamic_extent
                    ? span<T, dynamic_extent>{data() + offset, size() - offset}
                    : (QS_VERIFY(count <= size() - offset, "span<T, N>::subspan(offset, count): count out of range"),
                       span<T, dynamic_extent>{data() + offset, count}));
    }

    // // [span.objectrep], views of object representation
    // QS_CONSTEXPR11 span<byte const, extent * sizeof(element_type)> as_bytes() const noexcept
    // {
    //     return {reinterpret_cast<byte const*>(data()), size_bytes()};
    // }

    // QS_CONSTEXPR11 span<byte, extent * sizeof(element_type)> as_writable_bytes() const noexcept
    // {
    //     return {reinterpret_cast<byte*>(data()), size_bytes()};
    // }

private:
    pointer data_;

    QS_NORETURN QS_INLINE QS_CONSTEXPR11 void throw_out_of_range_(char const* const msg) const
    {
        throw std::out_of_range(msg);
    }
};


// span implementation for dynamic-size

template<class T>
class span<T, dynamic_extent>
{
public:
    using element_type           = T;
    using value_type             = remove_cv_t<T>;
    using size_type              = size_t;
    using difference_type        = ptrdiff_t;
    using pointer                = element_type*;
    using const_pointer          = element_type const*;
    using reference              = element_type&;
    using const_reference        = element_type const&;
    using iterator               = pointer; // implementation-defined (simplifying to pointer for now)
    using const_iterator         = const_pointer; //(since C++23)	std::const_iterator<iterator>
    using reverse_iterator       = std::reverse_iterator<iterator>;
    using const_reverse_iterator = std::reverse_iterator<const_iterator>;


    // [span.cons], span constructors, copy, assignment, and destructor
    QS_CONSTEXPR11 span() noexcept
        : data_{nullptr},
          size_{0} {};

    // NOTE: in optimized builds, temporary initializer_list leaves the span with dangling pointer (undefined state)
    template<class U = element_type, enable_if_t<std::is_const<U>::value, int> = 0>
    QS_CONSTEXPR11 explicit span(std::initializer_list<value_type> il)
        : data_{il.begin()},
          size_{il.size()}
    {}

    QS_CONSTEXPR11       span(const span&) noexcept      = default;
    QS_CONSTEXPR11 span& operator=(const span&) noexcept = default;

    template<class Iterator, enable_if_t<intl::is_span_compatible_iterator<Iterator, element_type>::value, int> = 0>
    QS_CONSTEXPR11 explicit span(Iterator first, size_type count)
        : data_{to_address(first)},
          size_{count}
    {}

    template<class Iterator, class Sentinel,
             enable_if_t<intl::is_span_compatible_iterator<Iterator, element_type>::value &&
                             intl::is_span_compatible_sentinel<Sentinel, Iterator>::value,
                         int> = 0>
    QS_CONSTEXPR11 explicit span(Iterator first, Sentinel last)
        : data_(to_address(first)),
          size_(last - first)
    {
        QS_VERIFY(last - first >= 0, "invalid range in span's constructor (iterator, sentinel)");
    }


    template<size_t N>
    QS_CONSTEXPR11 span(type_identity_t<element_type> (&arr)[N]) noexcept
        : data_{arr},
          size_{N}
    {}

    template<class V, size_t N, enable_if_t<intl::is_span_convertible<V, element_type>::value, int> = 0>
    QS_CONSTEXPR11 span(std::array<V, N>& arr) noexcept
        : data_{arr.data()},
          size_{N}
    {}

    template<class V, size_t N, enable_if_t<intl::is_span_convertible<V const, element_type>::value, int> = 0>
    QS_CONSTEXPR11 span(std::array<V, N> const& arr) noexcept
        : data_{arr.data()},
          size_{N}
    {}

    template<class V, size_t E, enable_if_t<intl::is_span_convertible<V, element_type>::value, int> = 0>
    QS_CONSTEXPR11 explicit span(span<V, E> const& other) noexcept
        : data_{other.data()},
          size_{other.size()}
    {}

    template<class Rng, enable_if_t<intl::is_span_compatible_range<Rng, element_type>::value, int> = 0>
    QS_CONSTEXPR11 explicit span(Rng&& r)
        : data_(qs::data(r)),
          size_{qs::size(r)}
    {}


    // [span.obs], span observers
    QS_CONSTEXPR11 size_type size() const noexcept { return size_; }
    QS_CONSTEXPR11 size_type size_bytes() const noexcept { return size_ * sizeof(element_type); }
    QS_CONSTEXPR11 bool      empty() const noexcept { return size() == 0; }

    // [span.elem], span element access //
    QS_CONSTEXPR11 reference operator[](size_type index) const noexcept(is_nothrow_contract_violation)
    {
        return QS_VERIFY(index < size(), "span<T>::operator[](index): index out of range"), this->data_[index];
    };
    QS_CONSTEXPR11 reference at(size_type index) const noexcept(is_nothrow_contract_violation)
    {
        return index >= size() ? (throw_out_of_range_("span"), this->data_[0]) : this->data_[index];
    }
    QS_CONSTEXPR11 reference front() const noexcept(is_nothrow_contract_violation)
    {
        return QS_VERIFY(!empty(), "span<T>::front() called on empty span"), this->data_[0];
    }
    QS_CONSTEXPR11 reference back() const noexcept(is_nothrow_contract_violation)
    {
        return QS_VERIFY(!empty(), "span<T>::back() called on empty span"), this->data_[size() - 1];
    }
    QS_CONSTEXPR11 pointer data() const noexcept { return this->data_; }

    // [span.iter], span iterator support
    QS_CONSTEXPR11 iterator               begin() const noexcept { return data(); };
    QS_CONSTEXPR11 iterator               end() const noexcept { return data() + size(); };
    QS_CONSTEXPR11 const_iterator         cbegin() const noexcept { return data(); };
    QS_CONSTEXPR11 const_iterator         cend() const noexcept { return data() + size(); };
    QS_CONSTEXPR11 reverse_iterator       rbegin() const noexcept { return reverse_iterator(end()); };
    QS_CONSTEXPR11 reverse_iterator       rend() const noexcept { return reverse_iterator(begin()); };
    QS_CONSTEXPR11 const_reverse_iterator crbegin() const noexcept { return const_reverse_iterator(cend()); };
    QS_CONSTEXPR11 const_reverse_iterator crend() const noexcept { return const_reverse_iterator(cbegin()); }


    // [span.sub], span subviews
    template<size_t Count>
    QS_CONSTEXPR11 span<T, Count> first() const noexcept(is_nothrow_contract_violation)
    {
        return QS_VERIFY(Count <= size(), "span<T>::first<Count>(): Count out of range"), span<T, Count>{data(), Count};
    }
    QS_CONSTEXPR11 span<T, dynamic_extent> first(size_type count) const noexcept(is_nothrow_contract_violation)
    {
        return QS_VERIFY(count <= size(), "span<T>::first(count): count out of range"),
               span<T, dynamic_extent>{data(), count};
    }

    template<size_t Count>
    QS_CONSTEXPR11 span<T, Count> last() const noexcept(is_nothrow_contract_violation)
    {
        return QS_VERIFY(Count <= size(), "span<T>::last<Count>(): Count out of range"),
               span<T, Count>{data() + size() - Count, Count};
    }
    QS_CONSTEXPR11 span<T, dynamic_extent> last(size_type count) const noexcept(is_nothrow_contract_violation)
    {
        return QS_VERIFY(count <= size(), "span<T>::last(count): count out of range"),
               span<T, dynamic_extent>{data() + size() - count, count};
    }

    template<size_t Offset, size_t Count = dynamic_extent>
    QS_CONSTEXPR11 auto subspan() const noexcept(is_nothrow_contract_violation) -> span<T, Count>
    {
        return QS_VERIFY(Offset <= size(), "span<T>::subspan<Offset, Count>(): Offset out of range"),
               QS_VERIFY(Count == dynamic_extent || Count <= size() - Offset,
                         "span<T>::subspan<Offset, Count>(): Offset + Count out of range"),
               span<T, Count>{data() + Offset, Count == dynamic_extent ? size() - Offset : Count};
    }

    QS_CONSTEXPR11 auto subspan(size_type offset, size_type count = dynamic_extent) const
        noexcept(is_nothrow_contract_violation) -> span<T, dynamic_extent>
    {
        return QS_VERIFY(offset <= size(), "span<T>::subspan(offset, count): offset out of range"),
               (count == dynamic_extent
                    ? span<T, dynamic_extent>{data() + offset, size() - offset}
                    : (QS_VERIFY(count <= size() - offset, "span<T>::subspan(offset, count): count out of range"),
                       span<T, dynamic_extent>{data() + offset, count}));
    }

    // [span.objectrep], views of object representation
    // QS_CONSTEXPR11 span<byte const, dynamic_extent> as_bytes() const noexcept
    // {
    //     return {reinterpret_cast<const byte*>(data()), size_bytes()};
    // }

    // QS_CONSTEXPR11 span<byte, dynamic_extent> as_writable_bytes() const noexcept
    // {
    //     return {reinterpret_cast<byte*>(data()), size_bytes()};
    // }

private:
    pointer   data_;
    size_type size_;

    QS_NORETURN QS_INLINE QS_CONSTEXPR11 void throw_out_of_range_(char const* const msg) const
    {
        throw std::out_of_range(msg);
    }
};


#if defined(__cpp_deduction_guides)

template<class It, class EndOrSize, enable_if_t<is_contiguous_iterator<It>::value, int> = 0>
span(It, EndOrSize) -> span<remove_reference_t<iter_reference_t<It>>>;

template<class T, size_t N>
span(T (&)[N]) -> span<T, N>;

template<class T, size_t N>
span(std::array<T, N>&) -> span<T, N>;

template<class T, size_t N>
span(std::array<T, N> const&) -> span<T const, N>;

template<class Rng, enable_if_t<is_contiguous_range<Rng>::value, int> = 0>
span(Rng&&) -> span<remove_reference_t<range_reference_t<Rng>>>;

#endif // __cpp_deduction_guides


template<class T, size_t N>
QS_CONSTEXPR11 auto as_bytes(span<T, N> s) noexcept -> decltype(auto)
{
    constexpr auto Count = N == dynamic_extent ? dynamic_extent : N * sizeof(T);
    return span<byte const, Count>(reinterpret_cast<byte const*>(s.data()), s.size_bytes());
}


template<class T, size_t N, enable_if_t<!std::is_const<T>::value, int> = 0>
QS_CONSTEXPR11 auto as_writable_bytes(span<T, N> s) noexcept -> decltype(auto)
{
    constexpr auto Count = N == dynamic_extent ? dynamic_extent : N * sizeof(T);
    return span<byte, Count>{reinterpret_cast<byte*>(s.data()), s.size_bytes()};
}


// [span.comparison], span comparison operators
// template<class ElementL, size_t ExtentL, class ElementR, size_t ExtentR>
// QS_CONSTEXPR11 bool operator==(span<ElementL, ExtentL> l, span<ElementR, ExtentR> r);
// template<class ElementL, size_t ExtentL, class ElementR, size_t ExtentR>
// QS_CONSTEXPR11 bool operator!=(span<ElementL, ExtentL> l, span<ElementR, ExtentR> r);
// template<class ElementL, size_t ExtentL, class ElementR, size_t ExtentR>
// QS_CONSTEXPR11 bool operator<(span<ElementL, ExtentL> l, span<ElementR, ExtentR> r);
// template<class ElementL, size_t ExtentL, class ElementR, size_t ExtentR>
// QS_CONSTEXPR11 bool operator<=(span<ElementL, ExtentL> l, span<ElementR, ExtentR> r);
// template<class ElementL, size_t ExtentL, class ElementR, size_t ExtentR>
// QS_CONSTEXPR11 bool operator>(span<ElementL, ExtentL> l, span<ElementR, ExtentR> r);
// template<class ElementL, size_t ExtentL, class ElementR, size_t ExtentR>
// QS_CONSTEXPR11 bool operator>=(span<ElementL, ExtentL> l, span<ElementR, ExtentR> r);

QS_NAMESPACE_END


#endif //  QS_SPAN_H
