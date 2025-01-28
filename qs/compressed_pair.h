#ifndef QS_COMPRESSED_PAIR_H_
#define QS_COMPRESSED_PAIR_H_

#include <qs/base.h>

#include <utility>
#include <tuple>


QS_NAMESPACE_BEGIN

namespace detail
{
    // Tag used to default initialize one or both of the pair's elements.
    struct default_init_tag
    {};
    struct value_init_tag
    {};

    template<class T, std::size_t I, bool CanBeEmptyBase = std::is_empty<T>::value && !std::is_final<T>::value>
    struct compressed_pair_elem
    {
        using value_type      = T;
        using reference       = T&;
        using const_reference = T const&;

        QS_CONSTEXPR11 explicit compressed_pair_elem(default_init_tag) {}
        QS_CONSTEXPR11 explicit compressed_pair_elem(value_init_tag)
            : value_()
        {}

        template<class U, class = std::enable_if_t<!std::is_same<compressed_pair_elem, std::decay_t<U>>::value>>
        QS_CONSTEXPR11 explicit compressed_pair_elem(U&& u)
            : value_(std::forward<U>(u))
        {}

        template<class... Args, std::size_t... Is>
        QS_CONSTEXPR11 explicit compressed_pair_elem(std::piecewise_construct_t, std::tuple<Args...> args,
                                                     std::index_sequence<Is...>)
            : value_(std::forward<Args>(std::get<Is>(args))...)
        {}

        QS_CONSTEXPR14 reference       get() QS_NOEXCEPT { return value_; }
        QS_CONSTEXPR11 const_reference get() const QS_NOEXCEPT { return value_; }

    private:
        T value_;
    };

    template<class T, std::size_t I>
    struct compressed_pair_elem<T, I, true> : private T
    {
        using value_type      = T;
        using reference       = T&;
        using const_reference = T const&;

        QS_CONSTEXPR11 explicit compressed_pair_elem() = default;
        //            constexpr explicit compressed_pair_elem(default_init_tag) {}
        QS_CONSTEXPR11 explicit compressed_pair_elem(value_init_tag)
            : value_type()
        {}

        template<class U, class = std::enable_if_t<!std::is_same<compressed_pair_elem, std::decay_t<U>>::value>>
        QS_CONSTEXPR11 explicit compressed_pair_elem(U&& u)
            : value_type(std::forward<U>(u))
        {}

        template<class... Args, std::size_t... Is>
        QS_CONSTEXPR11 compressed_pair_elem(std::piecewise_construct_t, std::tuple<Args...> args,
                                            std::index_sequence<Is...>)
            : value_type(std::forward<Args>(std::get<Is>(args))...)
        {}

        QS_CONSTEXPR14 reference       get() QS_NOEXCEPT { return *this; }
        QS_CONSTEXPR11 const_reference get() const QS_NOEXCEPT { return *this; }
    };

}; // namespace detail


template<class T1, class T2>
class compressed_pair : private detail::compressed_pair_elem<T1, 0>, private detail::compressed_pair_elem<T2, 1>
{
public:
    static_assert((!std::is_same<T1, T2>::value),
                  "compressed_pair cannot be instantiated when T1 and T2 are the same type; "
                  "The current implementation is NOT ABI-compatible with the previous implementation for this "
                  "configuration");

    using Base1 = detail::compressed_pair_elem<T1, 0>;
    using Base2 = detail::compressed_pair_elem<T2, 1>;

    template<class = std::enable_if_t<std::is_default_constructible<T1>::value && std::is_default_constructible<T2>::value>>
    QS_CONSTEXPR11 explicit compressed_pair()
        : Base1(detail::value_init_tag{}),
          Base2(detail::value_init_tag{})
    {}

    template<class U1, class U2>
    QS_CONSTEXPR11 explicit compressed_pair(U1&& u1, U2&& u2)
        : Base1(std::forward<U1>(u1)),
          Base2(std::forward<U2>(u2))
    {}

    template<class... Args1, class... Args2>
    QS_CONSTEXPR11 explicit compressed_pair(std::piecewise_construct_t pc, std::tuple<Args1...> args1,
                                            std::tuple<Args2...> args2)
        : Base1(pc, std::move(args1), std::make_index_sequence<sizeof...(Args1)>{}),
          Base2(pc, std::move(args2), std::make_index_sequence<sizeof...(Args2)>{})
    {}

    QS_CONSTEXPR14 typename Base1::reference       first() noexcept { return static_cast<Base1&>(*this).get(); }
    QS_CONSTEXPR11 typename Base1::const_reference first() const noexcept
    {
        return static_cast<Base1 const&>(*this).get();
    }
    QS_CONSTEXPR14 typename Base2::reference       second() noexcept { return static_cast<Base2&>(*this).get(); }
    QS_CONSTEXPR11 typename Base2::const_reference second() const noexcept
    {
        return static_cast<Base2 const&>(*this).get();
    }

    QS_CONSTEXPR14 void swap(compressed_pair& x)
        QS_NOEXCEPT_B(std::is_nothrow_swappable<T1>::value && std::is_nothrow_swappable<T2>::value)
    {
        std::swap(first(), x.first());
        std::swap(second(), x.second());
    }
};


QS_NAMESPACE_END


template<class T1, class T2>
inline QS_CONSTEXPR14 void swap(qs::compressed_pair<T1, T2>& x, qs::compressed_pair<T1, T2>& y)
    QS_NOEXCEPT_B(std::is_nothrow_swappable<T1>::value&& std::is_nothrow_swappable<T2>::value)
{
    x.swap(y);
}


#endif // QS_COMPRESSED_PAIR_H_
