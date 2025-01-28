#ifndef QS_ENUMERATOR_H_
#define QS_ENUMERATOR_H_

#include <qs/base.h>

QS_NAMESPACE_BEGIN


// Template class to enumerate over a container
// Note: The enumerator class needs more work to differentiate between const/non-const views.
template<class Iterator, class Index>
class enumerator
{
public:
    struct iterator
    {
        using reference = typename std::iterator_traits<Iterator>::reference;
        constexpr iterator(Iterator iter, Index start = -1)
            : current_{start},
              iter_{iter} {};

        constexpr std::tuple<Index, reference> operator*() const { return {current_, *iter_}; }
        constexpr iterator&                    operator++()
        {
            ++current_;
            ++iter_;
            return *this;
        }
        constexpr bool operator!=(iterator& other) const { return iter_ != other.iter_; }

    private:
        Index    current_{}; // Current index
        Iterator iter_{};    // Current iterator
    };

    constexpr enumerator(Iterator begin, Iterator end, Index start = 0)
        : start_{start},
          begin_{begin},
          end_{end}
    {}
    constexpr auto begin() const { return iterator(begin_, start_); }

    constexpr auto end() const { return iterator(end_); }

private:
    Iterator begin_;
    Iterator end_;
    Index    start_;
};

// Function to create an enumerator for a container
template<class Container, class Index>
constexpr auto enumerate(Container& container, Index start = 0)
{
    return enumerator(std::begin(container), std::end(container), start);
}

QS_NAMESPACE_END

#endif // QS_ENUMERATOR_H_

