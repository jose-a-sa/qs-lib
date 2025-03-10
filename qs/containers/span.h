#ifndef QS_SPAN_H_
#define QS_SPAN_H_

#include <cstddef>
#include <iterator>

#include <qs/config.h>
#include <qs/memory.h>
#include <qs/traits/iterator.h>


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
        : conjugation<is_contiguous_iterator<It>, is_span_convertible<remove_reference_t<iter_reference_t<It>>, T>>
    {};

    template<class Sentinel, class It>
    struct is_span_compatible_sentinel : std::integral_constant<bool, is_sized_sentinel_for<Sentinel, It>::value &&
                                                                          !std::is_convertible<Sentinel, size_t>::value>
    {};

    // TODO: implement the following concepts
    // [ ] is_contiguous_range<R>
    // [ ] is_sized_range<R>
    // [ ] is_borrowed_range<R> (or is_const_v<ElementType>)
    // [ ] range_reference_t<R>
    template<class Range, class ElementType>
    struct is_span_compatible_range
        : std::integral_constant<bool, //
                                 ranges::contiguous_range<Range> && //
                                     ranges::sized_range<Range> && //
                                     (ranges::borrowed_range<Range> || std::is_const<ElementType>::value) && //
                                     !is_span<remove_cvref_t<Range>>::value && //
                                     !is_std_array<remove_cvref_t<Range>>::value && //
                                     !std::is_array<remove_cvref_t<Range>>::value && //
                                     is_span_convertible<remove_reference_t<range_reference_t<Range>>, ElementType>>
    {};
} // namespace intl

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

    template<class U = element_type, enable_if_t<std::is_const<U>::value, int> = 0>
    QS_CONSTEXPR11 explicit span(std::initializer_list<value_type> il)
        : data_{il.begin()}
    {
        QS_VERIFY(extent == il.size(), "Size mismatch in span's constructor Extent != il.size().");
    }

    QS_CONSTEXPR11       span(const span&) noexcept      = default;
    QS_CONSTEXPR11 span& operator=(const span&) noexcept = default;

    // requires __span_compatible_iterator<element_type> It
    QS_CONSTEXPR11 explicit span(pointer first, size_type count)
        : data_{first}
    {
        QS_VERIFY(extent == count, "size mismatch in span's constructor (iterator, len)");
    }

    template<class Iterator, enable_if_t<intl::is_span_compatible_iterator<Iterator, element_type>::value, int> = 0>
    QS_CONSTEXPR11 explicit span(Iterator first, size_type count)
        : storage_(std::to_address(first), count)
    {
        QS_VERIFY(Extent == dynamic_extent || Extent == count, "size mismatch in span's constructor (iterator, len)");
    }

    template<class Iterator, class Sentinel,
             enable_if_t<intl::is_span_compatible_iterator<Iterator, element_type>::value &&
                             intl::is_span_compatible_sentinel<Sentinel, Iterator>::value,
                         int> = 0>
    QS_CONSTEXPR11 explicit span(Iterator first, Sentinel last)
        : storage_(std::to_address(first), last - first)
    {
        // [span.cons]/10
        // Throws: When and what last - first throws.
        [[maybe_unused]] auto dist = last - first;
        QS_VERIFY(dist >= 0, "invalid range in span's constructor (iterator, sentinel)");
        QS_VERIFY(Extent == dynamic_extent || Extent == dist,
                  "invalid range in span's constructor (iterator, sentinel): last - first != extent");
    }

    QS_CONSTEXPR11 span(type_identity_t<element_type> (&arr)[extent]) noexcept
        : data_{arr}
    {}

    template<class V, enable_if_t<intl::is_span_convertible<V, element_type>::value, int> = 0>
    QS_CONSTEXPR11 span(std::array<V, extent>& arr) noexcept
        : data_{arr.data()}
    {}

    template<class V, enable_if_t<intl::is_span_convertible<V const, element_type>::value, int> = 0>
    QS_CONSTEXPR11 span(std::array<V, extent> const& arr) noexcept
        : data_{arr.data()}
    {}

    template<class V, enable_if_t<intl::is_span_convertible<V, element_type>::value, int> = 0>
    QS_CONSTEXPR11 span(span<V, extent> const& other)
        : data_{other.data()}
    {}

    template<class V, enable_if_t<intl::is_span_convertible<V, element_type>::value, int> = 0>
    QS_CONSTEXPR11 explicit span(span<V, dynamic_extent> const& other) noexcept
        : data_{other.data()}
    {
        QS_VERIFY(extent == other.size(), "size mismatch in span's constructor (other span)");
    }

    // TODO: implement range constructors of fixed extent
    // template<class Range>
    // // requires __span_compatible_range<element_type> _Range
    // QS_CONSTEXPR11 explicit span(Range&& r)
    //     : storage_(r.data(), r.size())
    // {
    //     QS_VERIFY(extent == dynamic_extent || extent == r.size(),
    //               "size mismatch in span's constructor (range)");
    // }


    // [span.obs], span observers
    QS_CONSTEXPR11 size_type size() const noexcept { return extent; }
    QS_CONSTEXPR11 size_type size_bytes() const noexcept { return extent * sizeof(element_type); }
    QS_CONSTEXPR11 bool      empty() const noexcept { return size() == 0; }

    // [span.elem], span element access //
    QS_CONSTEXPR11 reference operator[](size_type index) const
    {
        return QS_VERIFY(index < size(), "span<T, N>::operator[](index): index out of range"), this->data_[index];
    };
    QS_CONSTEXPR11 reference at(size_type index) const
    {
        return (index >= size()) ? (throw_out_of_range_("span"), this->data_[0]) : this->data_[index];
    }
    QS_CONSTEXPR11 reference front() const
    {
        return QS_VERIFY(!empty(), "span<T, N>::front() called on empty span"), this->data_[0];
    }
    QS_CONSTEXPR11 reference back() const
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
    QS_CONSTEXPR11 span<element_type, Count> first() const noexcept
    {
        static_assert(Count <= extent, "span<T, N>::first<Count>(): Count out of range");
        return span<element_type, Count>{data(), Count};
    }
    QS_CONSTEXPR11 span<element_type, dynamic_extent> first(size_type count) const noexcept
    {
        return QS_VERIFY(count <= size(), "span<T, N>::first(count): count out of range"),
               span<element_type, dynamic_extent>{data(), count};
    }

    template<size_t Count>
    QS_CONSTEXPR11 span<element_type, Count> last() const noexcept
    {
        static_assert(Count <= extent, "span<T, N>::last<Count>(): Count out of range");
        return span<element_type, Count>{data() + size() - Count, Count};
    }
    QS_CONSTEXPR11 span<element_type, dynamic_extent> last(size_type count) const noexcept
    {
        return QS_VERIFY(count <= size(), "span<T, N>::last(count): count out of range"),
               span<element_type, dynamic_extent>{data() + size() - count, count};
    }

    template<size_t Offset, size_t Count = dynamic_extent>
    QS_CONSTEXPR11 auto subspan() const noexcept
        -> span<element_type, Count != dynamic_extent ? Count : extent - Offset>
    {
        static_assert(Offset <= extent, "span<T>::subspan<Offset, Count>(): Offset out of range");
        static_assert(Count == dynamic_extent || Count <= extent - Offset,
                      "span<T>::subspan<Offset, Count>(): Offset + Count out of range");

        using return_t = span<element_type, Count != dynamic_extent ? Count : extent - Offset>;
        return return_t{data() + Offset, Count != dynamic_extent ? Count : extent - Offset};
    }

    QS_CONSTEXPR11 auto subspan(size_type offset, size_type count = dynamic_extent) const noexcept
        -> span<element_type, dynamic_extent>
    {
        return QS_VERIFY(offset <= size(), "span<T, N>::subspan(offset, count): offset out of range"),
               (count == dynamic_extent
                    ? span<element_type, dynamic_extent>{data() + offset, size() - offset}
                    : (QS_VERIFY(count <= size() - offset, "span<T, N>::subspan(offset, count): count out of range"),
                       span<element_type, dynamic_extent>{data() + offset, count}));
    }

    // [span.objectrep], views of object representation
    QS_CONSTEXPR11 span<byte const, extent * sizeof(element_type)> as_bytes() const noexcept
    {
        return {reinterpret_cast<byte const*>(data()), size_bytes()};
    }

    QS_CONSTEXPR11 span<byte, extent * sizeof(element_type)> as_writable_bytes() const noexcept
    {
        return {reinterpret_cast<byte*>(data()), size_bytes()};
    }

private:
    pointer data_;

    QS_NORETURN QS_INLINE QS_CONSTEXPR11 void throw_out_of_range_(char const* const msg) const
    {
        throw std::out_of_range(msg);
    }
};

// TODO: implement span<T, dynamic_extent> to avoid assertions where only static_asserts should be used
template<class T>
class span<T, dynamic_extent>
{
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

private:
    pointer   data_;
    size_type size_;

    QS_NORETURN QS_INLINE QS_CONSTEXPR11 void throw_out_of_range_(char const* const msg) const
    {
        throw std::out_of_range(msg);
    }
};


#if defined(__cpp_deduction_guides)

// TODO: requires is_contiguous_iterator<It> concept and iter_reference_t<Range>
// template<class It, class EndOrSize> // contiguous_iterator<It>
// span(It, EndOrSize) -> span<remove_reference_t<iter_reference_t<It>>>;

template<class T, size_t N>
span(T (&)[N]) -> span<T, N>;

template<class T, size_t N>
span(std::array<T, N>&) -> span<T, N>;

template<class T, size_t N>
span(std::array<T, N> const&) -> span<T const, N>;

// TODO: requires is_contiguous_range<Range> concept and range_reference_t<Range>
// template<class Range>
// span(Range&&) -> span<remove_reference_t<range_reference_t<Range>>>;

#endif // __cpp_deduction_guides


// [span.comparison], span comparison operators
template<class ElementL, size_t ExtentL, class ElementR, size_t ExtentR>
QS_CONSTEXPR11 bool operator==(span<ElementL, ExtentL> l, span<ElementR, ExtentR> r);
template<class ElementL, size_t ExtentL, class ElementR, size_t ExtentR>
QS_CONSTEXPR11 bool operator!=(span<ElementL, ExtentL> l, span<ElementR, ExtentR> r);
template<class ElementL, size_t ExtentL, class ElementR, size_t ExtentR>
QS_CONSTEXPR11 bool operator<(span<ElementL, ExtentL> l, span<ElementR, ExtentR> r);
template<class ElementL, size_t ExtentL, class ElementR, size_t ExtentR>
QS_CONSTEXPR11 bool operator<=(span<ElementL, ExtentL> l, span<ElementR, ExtentR> r);
template<class ElementL, size_t ExtentL, class ElementR, size_t ExtentR>
QS_CONSTEXPR11 bool operator>(span<ElementL, ExtentL> l, span<ElementR, ExtentR> r);
template<class ElementL, size_t ExtentL, class ElementR, size_t ExtentR>
QS_CONSTEXPR11 bool operator>=(span<ElementL, ExtentL> l, span<ElementR, ExtentR> r);

QS_NAMESPACE_END


#endif // QS_SPAN_H_
