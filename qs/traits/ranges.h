#ifndef QS_TRAITS_RANGES_H
#define QS_TRAITS_RANGES_H

#include <qs/config.h>
#include <qs/meta.h>
#include <qs/traits/iterator.h>
#include <type_traits>
#include "qs/traits/base.h"


QS_NAMESPACE_BEGIN


template<class Rng>
using iterator_t = decltype(std::begin(std::declval<Rng&>()));
template<class Rng>
using sentinel_t = decltype(std::end(std::declval<Rng&>()));

template<class Rng>
using range_difference_t = iter_difference_t<iterator_t<Rng>>;
template<class Rng>
using range_value_t = iter_value_t<iterator_t<Rng>>;
template<class Rng>
using range_reference_t = iter_reference_t<iterator_t<Rng>>;
template<class Rng>
using range_rvalue_reference_t = iter_rvalue_reference_t<iterator_t<Rng>>;
template<class Rng>
using range_common_reference_t = iter_common_reference_t<iterator_t<Rng>>;

template<class Rng>
using range_size_t = decltype(qs::size(std::declval<Rng&>()));


// [range.range]

template<class Rng, class = void>
struct is_range : std::false_type
{};
template<class Rng>
struct is_range<Rng, void_t<iterator_t<Rng>, sentinel_t<Rng>>> : std::true_type
{};


template<class Rng, class = void>
struct is_input_range : std::false_type
{};
template<class Rng>
struct is_input_range<Rng, void_t<iterator_t<Rng>>> : conjunction<is_range<Rng>, is_input_iterator<iterator_t<Rng>>>
{};


template<class Rng>
struct is_borrowed_range : conjunction<is_range<Rng>, std::is_lvalue_reference<Rng>>
{};


// [range.sized]

template<class Rng, class = void>
struct is_sized_range : std::false_type
{};
template<class Rng>
struct is_sized_range<Rng, void_t<range_size_t<Rng>>> : is_range<Rng>
{};

// [range.refinements], other range refinements

template<class Rng, class T, class = void>
struct is_output_range : std::false_type
{};
template<class Rng, class T>
struct is_output_range<Rng, T, void_t<iterator_t<Rng>>>
    : conjunction<is_range<Rng>, is_output_iterator<iterator_t<Rng>, T>>
{};


template<class Rng, class = void>
struct is_forward_range : std::false_type
{};
template<class Rng>
struct is_forward_range<Rng, void_t<iterator_t<Rng>>>
    : conjunction<is_input_range<Rng>, is_forward_iterator<iterator_t<Rng>>>
{};


template<class Rng, class = void>
struct is_bidirectional_range : std::false_type
{};
template<class Rng>
struct is_bidirectional_range<Rng, void_t<iterator_t<Rng>>>
    : conjunction<is_forward_range<Rng>, is_bidirectional_iterator<iterator_t<Rng>>>
{};


template<class Rng, class = void>
struct is_random_access_range : std::false_type
{};
template<class Rng>
struct is_random_access_range<Rng, void_t<iterator_t<Rng>>>
    : conjunction<is_bidirectional_range<Rng>, is_random_access_iterator<iterator_t<Rng>>>
{};


template<class Rng>
class is_contiguous_range : std::false_type
{
    template<class R, class It = iterator_t<R>, class Ref = range_reference_t<R>,
             class D = decltype(qs::data(std::declval<R&>()))>
    static constexpr auto test(int) -> conjunction<is_same_as<D, add_pointer_t<Ref>>, is_contiguous_iterator<It>>;
    template<class>
    static constexpr auto test(...) -> std::false_type;

public:
    static constexpr bool value = is_random_access_range<Rng>::value && decltype(test<Rng>(0))::value;
};


QS_NAMESPACE_END

#endif // QS_TRAITS_RANGES_H
