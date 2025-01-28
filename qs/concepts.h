#ifndef QS_SPAN_H_
#define QS_SPAN_H_

#include <qs/base.h>

#include <iterator>


QS_NAMESPACE_BEGIN


template<class Iterator, class IteratorTag, class Element = void>
struct is_std_iterator : std::integral_constant<bool,
    std::is_convertible<typename std::iterator_traits<Iterator>::iterator_category, IteratorTag>::value &&
    std::is_constructible<Element, typename std::iterator_traits<Iterator>::reference>::value
> {};

template<class Iterator, class IteratorTag>
struct is_std_iterator<Iterator, IteratorTag, void> : std::is_convertible<typename std::iterator_traits<Iterator>::iterator_category, IteratorTag> {};


#if QS_HAS(VARIABLE_TEMPLATES)
template<class Iterator, class IteratorTag, class Element = void>
QS_INLV_CONSTEXPR11 bool is_std_iterator_v = is_std_iterator<Iterator, IteratorTag, Element>::value;
#endif // QS_HAS(VARIABLE_TEMPLATES)


QS_NAMESPACE_END


#endif // QS_SPAN_H_