#ifndef QS_SPAN_H_
#define QS_SPAN_H_

#include <qs/base.h>

#include <iterator>
#include <type_traits>


QS_NAMESPACE_BEGIN

template<class Bool, class T>
struct switch_case;

namespace detail
{
    template<class... Pairs>
    struct switch_type_impl;

    template<class Bool, class T, class... Rest>
    struct switch_type_impl<switch_case<Bool, T>, Rest...> : 
        std::conditional<Bool::value, T, typename switch_type_impl<Rest...>::type> {};
    
    template<class V>
    struct switch_type_impl<V> { using type = V; };  
}

template<class... TArgs>
using switch_t = typename detail::switch_type_impl<TArgs...>::type; 


template<class Iterator, class IteratorTag, class Element = void>
struct is_std_iterator : std::integral_constant<bool,
    is_std_iterator<Iterator, IteratorTag, void>::value &&
    std::is_constructible<Element, typename std::iterator_traits<Iterator>::reference>::value
> {};

template<class Iterator, class IteratorTag>
struct is_std_iterator<Iterator, IteratorTag, void> : 
    std::is_convertible<typename std::iterator_traits<Iterator>::iterator_category, IteratorTag> {};



template<class Iterator, class IteratorTag, class Element = void>
struct is_std_iterator_exact : 
    std::integral_constant<bool, 
        is_std_iterator_exact<Iterator, IteratorTag, void>::value && 
        std::is_constructible<Element, typename std::iterator_traits<Iterator>::reference>::value> {};

template<class Iterator>
struct is_std_iterator_exact<Iterator, std::input_iterator_tag, void> : 
    std::integral_constant<bool,
        std::is_convertible<typename std::iterator_traits<Iterator>::iterator_category, std::input_iterator_tag>::value &&
        !std::is_convertible<typename std::iterator_traits<Iterator>::iterator_category, std::forward_iterator_tag>::value> {};

template<class Iterator>
struct is_std_iterator_exact<Iterator, std::forward_iterator_tag, void> : 
    std::integral_constant<bool,
        std::is_convertible<typename std::iterator_traits<Iterator>::iterator_category, std::forward_iterator_tag>::value &&
        !std::is_convertible<typename std::iterator_traits<Iterator>::iterator_category, std::bidirectional_iterator_tag>::value> {};

template<class Iterator>
struct is_std_iterator_exact<Iterator, std::bidirectional_iterator_tag, void> : 
    std::integral_constant<bool,
        std::is_convertible<typename std::iterator_traits<Iterator>::iterator_category, std::bidirectional_iterator_tag>::value &&
        !std::is_convertible<typename std::iterator_traits<Iterator>::iterator_category, std::random_access_iterator_tag>::value> {};

template<class Iterator>
struct is_std_iterator_exact<Iterator, std::random_access_iterator_tag, void> : 
    std::is_convertible<typename std::iterator_traits<Iterator>::iterator_category, std::random_access_iterator_tag> {};



#if defined(__cpp_deduction_guides)
template<class Iterator, class IteratorTag, class Element = void>
inline constexpr bool is_std_iterator_v = is_std_iterator<Iterator, IteratorTag, Element>::value;
#endif // __cpp_deduction_guides



QS_NAMESPACE_END


#endif // QS_SPAN_H_