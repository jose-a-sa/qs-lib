#ifndef QS_TRAITS_ITERATOR_H
#define QS_TRAITS_ITERATOR_H

#include <qs/config.h>
#include <qs/meta.h>
#include <qs/traits/base.h>

#include <array>
#include <iterator>
#include <string>
#include <type_traits>
#include <vector>

#if defined(__cpp_lib_string_view)
#include <string_view>
#endif


QS_NAMESPACE_BEGIN

// -----------------------------------------------------------------------------
// iterator traits
// -----------------------------------------------------------------------------

template<class Iter, class = void>
struct iter_traits_getter : type_identity<remove_cvref_t<Iter>>
{};
template<class Iter>
struct iter_traits_getter<Iter, void_t<std::iterator_traits<remove_cvref_t<Iter>>>>
    : type_identity<std::iterator_traits<remove_cvref_t<Iter>>>
{};
template<class Iter>
using iter_traits_getter_t = typename iter_traits_getter<Iter>::type;


template<class Iter>
using iter_reference_t = decltype(*std::declval<Iter&>());
template<class Iter>
using iter_rvalue_reference_t = decltype(*std::declval<std::move_iterator<Iter>&>());
template<class Iter>
using iter_value_t = typename iter_traits_getter_t<Iter>::value_type;
template<class Iter>
using iter_difference_t = typename iter_traits_getter_t<Iter>::difference_type;
template<class Iter>
using iter_category_t = typename iter_traits_getter_t<Iter>::iterator_category;


template<class T, class = void>
struct can_reference : std::false_type
{};
template<class T>
struct can_reference<T, void_t<T&>> : std::true_type
{};
template<class T>
struct is_dereferencable : meta::test::op_star<T&, meta::fn<can_reference>>
{};


// -----------------------------------------------------------------------------
// to_address (ignore std::pointer_traits<Ptr>::to_address)
// -----------------------------------------------------------------------------

// base case of the to_address overload
template<class T>
QS_CONSTEXPR11 T* to_address(T* p) noexcept
{
    static_assert(!std::is_function<T>::value, "T is a function type");
    return p;
}

// to_address overload for fancy pointers which must have operator->() overload
template<class Ptr, enable_if_t<std::is_class<Ptr>::value && meta::test::op_arrow<Ptr const&>::value, int> = 0>
QS_CONSTEXPR11 auto to_address(Ptr const& p) noexcept -> decay_t<decltype(qs::to_address(p.operator->()))>
{
    return qs::to_address(p.operator->());
}

// -----------------------------------------------------------------------------
// check category tag helper
// -----------------------------------------------------------------------------

template<class Iter, class CategoryTag>
class is_tagged_with_category
{
    template<class I, class T, class TagRes = iter_category_t<I>>
    static constexpr auto test(int) -> is_derived_from<TagRes, CategoryTag>;
    template<class I>
    static constexpr auto test(...) -> std::false_type;

public:
    static constexpr bool value = decltype(test<Iter, CategoryTag>(0))::value;
};

// -----------------------------------------------------------------------------
// [iterator.concept.readable]
// -----------------------------------------------------------------------------

template<class Iter>
class is_indirectly_readable
{
    template<class I, class Value = iter_value_t<I>, class LRef = iter_reference_t<I>,
             class RRef = iter_rvalue_reference_t<I>, class Deref = decltype(*std::declval<I>())>
    static constexpr auto test(int)
        -> conjunction<is_same_as<Deref, LRef>, is_common_reference_with<LRef&&, Value&>,
                       is_common_reference_with<LRef&&, RRef&&>, is_common_reference_with<RRef&&, Value const&>>;
    template<class I>
    static constexpr auto test(...) -> std::false_type;

public:
    static constexpr bool value = decltype(test<remove_cvref_t<Iter>>(0))::value;
};

template<class Iter>
using iter_common_reference_t = common_reference_t<iter_reference_t<Iter>, iter_value_t<Iter>&>;


// -----------------------------------------------------------------------------
// [iterator.concept.writable]
// -----------------------------------------------------------------------------

template<class Iter, class Value>
class is_indirectly_writable
{
    template<class O, class T>
    static auto test_write(int)
        -> decltype(*std::declval<O&>() = std::declval<T&&>(), *std::declval<O&&>() = std::declval<T&&>(),
                    const_cast<const iter_reference_t<O>&&>(*std::declval<O&>())  = std::declval<T&&>(),
                    const_cast<const iter_reference_t<O>&&>(*std::declval<O&&>()) = std::declval<T&&>(),
                    std::true_type{});
    template<class O, class T>
    static auto test_write(...) -> std::false_type;

public:
    static constexpr bool value = decltype(test_write<Iter, Value>(0))::value;
};


// -----------------------------------------------------------------------------
// [iterator.concept.winc]
// -----------------------------------------------------------------------------

template<class Iter>
class is_weakly_incrementable
{
    template<class I, class Diff = iter_difference_t<I>>
    static auto test_difference_t(int) -> is_signed_integral<Diff>;
    template<class I>
    static auto test_difference_t(...) -> std::false_type;

public:
    static constexpr bool value = !is_same_as<Iter, bool>::value && decltype(test_difference_t<Iter>(0))::value &&
                                  meta::test::op_preincrement<Iter&, meta::fn<is_same_as, Iter&>>::value &&
                                  meta::test::op_postincrement<Iter&>::value;
};


// -----------------------------------------------------------------------------
// [iterator.concept.inc]
// -----------------------------------------------------------------------------

template<class Iter>
struct is_incrementable
    : conjunction<is_copyable<Iter>, is_default_initializable<Iter>, is_equality_comparable<Iter>,
                  is_weakly_incrementable<Iter>, meta::test::op_postincrement<Iter&, meta::fn<is_same_as, Iter>>>
{};


// -----------------------------------------------------------------------------
// [iterator.concept.iterator]
// -----------------------------------------------------------------------------

template<class Iter>
struct is_input_or_output_iterator : conjunction<is_weakly_incrementable<Iter>, is_dereferencable<Iter>>
{};


// -----------------------------------------------------------------------------
// [iterator.concept.sentinel]
// -----------------------------------------------------------------------------

template<class Sentinel, class Iter>
struct is_sentinel_for
    : conjunction<is_copyable<Sentinel>, is_default_initializable<Sentinel>, is_input_or_output_iterator<Iter>,
                  is_weakly_equality_comparable_with<Sentinel, Iter>>
{};


// user override to disable is_sized_sentinel_for<Sentinel, Iter>
template<class Sentinel, class Iter>
struct disable_is_sized_sentinel_for : std::false_type
{};


template<class Sentinel, class Iter, class = void>
struct is_sized_sentinel_for : std::false_type
{};

template<class Sentinel, class Iter>
struct is_sized_sentinel_for<Sentinel, Iter, void_t<iter_difference_t<Iter>>>
    : conjunction<is_sentinel_for<Sentinel, Iter>,
                  negation<disable_is_sized_sentinel_for<remove_cvref_t<Sentinel>, remove_cvref_t<Iter>>>,
                  meta::test::op_minus<Sentinel const&, Iter const&, meta::fn<is_same_as, iter_difference_t<Iter>>>,
                  meta::test::op_minus<Iter const&, Sentinel const&, meta::fn<is_same_as, iter_difference_t<Iter>>>>
{};


// -----------------------------------------------------------------------------
// [iterator.concept.input]
// -----------------------------------------------------------------------------

template<class Iter>
struct is_input_iterator : conjunction<is_input_or_output_iterator<Iter>, is_indirectly_readable<Iter>>
{};

template<class Iter>
struct is_input_iterator_tagged
    : conjunction<is_input_iterator<Iter>, is_tagged_with_category<Iter, std::input_iterator_tag>>
{};

// -----------------------------------------------------------------------------
// [iterator.concept.output]
// -----------------------------------------------------------------------------

template<class Iter, class Value>
class is_output_iterator
{
    template<class I, class T, class Res = decltype(*(std::declval<I&>()++) = std::forward<T>(std::declval<T&&>()))>
    static auto test_write(int) -> std::true_type;
    template<class I, class T>
    static auto test_write(...) -> std::false_type;

public:
    static constexpr bool value = is_input_or_output_iterator<Iter>::value &&
                                  is_indirectly_writable<Iter, Value>::value &&
                                  decltype(test_write<Iter, Value>(0))::value;
};

// -----------------------------------------------------------------------------
// [iterator.concept.forward]
// -----------------------------------------------------------------------------

template<class Iter>
struct is_forward_iterator : conjunction<is_input_iterator<Iter>, is_incrementable<Iter>, is_sentinel_for<Iter, Iter>>
{};

template<class Iter>
struct is_forward_iterator_tagged
    : conjunction<is_forward_iterator<Iter>, is_tagged_with_category<Iter, std::forward_iterator_tag>>
{};

// -----------------------------------------------------------------------------
// [iterator.concept.bidir]
// -----------------------------------------------------------------------------

template<class Iter>
struct is_bidirectional_iterator
    : conjunction<is_forward_iterator<Iter>, meta::test::op_predecrement<Iter&, meta::fn<is_same_as, Iter&>>,
                  meta::test::op_postdecrement<Iter&, meta::fn<is_same_as, Iter>>>
{};

template<class Iter>
struct is_bidirectional_iterator_tagged
    : conjunction<is_bidirectional_iterator<Iter>, is_tagged_with_category<Iter, std::bidirectional_iterator_tag>>
{};


// -----------------------------------------------------------------------------
// [iterator.concept.random]
// -----------------------------------------------------------------------------

template<class Iter, class = void>
struct is_random_access_iterator : std::false_type
{};

template<class Iter>
struct is_random_access_iterator<Iter, void_t<iter_difference_t<Iter>, iter_reference_t<Iter>>>
    : conjunction<is_bidirectional_iterator<Iter>, is_totally_ordered<Iter>, is_sized_sentinel_for<Iter, Iter>,
                  meta::test::op_plus_assign<Iter&, iter_difference_t<Iter> const&, meta::fn<is_same_as, Iter&>>,
                  meta::test::op_plus<Iter const&, iter_difference_t<Iter> const&, meta::fn<is_same_as, Iter>>,
                  meta::test::op_plus<iter_difference_t<Iter> const&, Iter const&, meta::fn<is_same_as, Iter>>,
                  meta::test::op_minus_assign<Iter&, iter_difference_t<Iter> const&, meta::fn<is_same_as, Iter&>>,
                  meta::test::op_minus<Iter const&, iter_difference_t<Iter> const&, meta::fn<is_same_as, Iter>>,
                  meta::test::op_subscript<Iter const&, iter_difference_t<Iter> const&,
                                           meta::fn<is_same_as, iter_reference_t<Iter>>>>
{};


template<class Iter>
struct is_random_access_iterator_tagged
    : conjunction<is_random_access_iterator<Iter>, is_tagged_with_category<Iter, std::random_access_iterator_tag>>
{};


// -----------------------------------------------------------------------------
// [iterator.concept.contiguous]
// -----------------------------------------------------------------------------


namespace intl
{
    // HACK: workaround for deque iterators, which are flagged as contiguous iterators by this trait, however they are
    //  random access iterators. to_address() also resolves, but does not obey the property
    //  to_address(it) + n == to_address(it + n), if n is larger than the deques block size.
    //  This fix checks if Iter can be converted to deque<iter_value_t<Iter>>::iterator or variants.
    //  The std::reverse_iterator also fails the contiguous iterator test as supposed.

    template<class Iter>
    class is_std_contiguous_iterator
    {
        template<class I, class V = iter_value_t<I>, class C = std::vector<V>>
        static auto test_vector(int) -> is_convertible_to<I, typename C::const_iterator>;
        template<class>
        static auto test_vector(...) -> std::false_type;

        template<class I, class V = iter_value_t<I>, class C = std::array<V, 1>>
        static auto test_array(int) -> is_convertible_to<I, typename C::const_iterator>;
        template<class>
        static auto test_array(...) -> std::false_type;

        template<class I, class V = iter_value_t<I>,
                 class Enable = enable_if_t<!std::is_array<V>::value && std::is_standard_layout<V>::value &&
                                            std::is_trivial<V>::value>,
                 class C      = std::basic_string<V>>
        static auto test_string(int) -> is_convertible_to<I, typename C::const_iterator>;
        template<class>
        static auto test_string(...) -> std::false_type;

#if defined(__cpp_lib_string_view)

        template<class I, class V = iter_value_t<I>,
                 class Enable = enable_if_t<!std::is_array<V>::value && std::is_standard_layout<V>::value &&
                                            std::is_trivial<V>::value>,
                 class C      = std::basic_string<V>>
        static auto test_string_view(int) -> is_convertible_to<I, typename C::const_iterator>;
        template<class>
        static auto test_string_view(...) -> std::false_type;

    public:
        static constexpr bool value =
            disjunction<decltype(test_vector<Iter>(0)), decltype(test_array<Iter>(0)), decltype(test_string<Iter>(0)),
                        decltype(test_string_view<Iter>(0))>::value;
#else
    public:
        static constexpr bool value = disjunction<decltype(test_vector<Iter>(0)), decltype(test_array<Iter>(0)),
                                                  decltype(test_string<Iter>(0))>::value;
#endif
    };

} // namespace intl


template<class Iter>
struct is_contiguous_iterator : intl::is_std_contiguous_iterator<Iter>
{};

template<class T>
struct is_contiguous_iterator<T*> : std::true_type
{};


QS_NAMESPACE_END


#endif // QS_TRAITS_ITERATOR_H
