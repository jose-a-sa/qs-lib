#ifndef QS_EXTENDED_GCD_H_
#define QS_EXTENDED_GCD_H_

#include <qs/config.h>
#include <tuple>
#include <type_traits>
#include <utility>

QS_NAMESPACE_BEGIN

namespace intl
{
    // Helper struct to determine the return type of the extended GCD implementation.
    template<class... Ts>
    struct extended_gcd_impl_return
    {
        using common_t = std::common_type_t<Ts...>;
        using tuple_t =
            std::tuple<std::remove_reference_t<decltype(std::declval<Ts>(), std::declval<common_t>())>...>;
        using type = std::pair<common_t, tuple_t>;
    };

    template<class... Ts>
    using extended_gcd_impl_return_t = typename extended_gcd_impl_return<Ts...>::type;

    // Base case for the extended GCD implementation.
    // This function handles the case where there is only one integer.
    template<class T>
    inline constexpr std::enable_if_t<std::is_integral<T>::value, extended_gcd_impl_return_t<T>>
    extended_gcd_impl(T a)
    {
        return std::make_pair(a, std::make_tuple<T>(1));
    }

    // Extended GCD implementation for two integers.
    // This function computes the GCD of two integers and also finds the coefficients x and y
    // such that ax + by = gcd(a, b).
    // The algorithm is based on the iterative version of the Extended Euclidean Algorithm.
    // It works by repeatedly applying the Euclidean algorithm and updating the coefficients
    // until the remainder becomes zero.
    // The final values of x and y are the coefficients that satisfy the equation.
    template<class T, class U>
    inline constexpr std::enable_if_t<std::conjunction<std::is_integral<T>, std::is_integral<U>>::value,
                                      extended_gcd_impl_return_t<T, U>>
    extended_gcd_impl(T a, U b)
    {
        using common_t = std::common_type_t<T, U>;
        common_t x = 1, y = 0;
        common_t x1 = 0, y1 = 1;
        common_t a1 = a, b1 = b;

        common_t temp_x = 0, temp_y = 0, temp_a1 = 0, q = 0;
        while(b1)
        {
            q = a1 / b1;
            // Save temporary variables
            temp_x  = x1;
            temp_y  = y1;
            temp_a1 = b1;
            // Perform the Euclidean algorithm step
            x1 = x - q * x1;
            y1 = y - q * y1;
            b1 = a1 - q * b1;
            // Update x, y, and a1
            x  = temp_x;
            y  = temp_y;
            a1 = temp_a1;
        }
        return std::make_pair(a1, std::make_tuple(x, y));
    }

    // Combines the results of two extended GCD computations.
    // This function takes the results of two extended GCD computations and combines them
    // to produce the final result.
    // The combination is done by recursively applying the Extended Euclidean Algorithm
    // to the GCDs of the two results and updating the coefficients accordingly.
    template<class T1, class Tup1, class T2, class Tup2>
    inline constexpr auto extended_gcd_impl_combine(std::pair<T1, Tup1> left, std::pair<T2, Tup2> right)
    {
        using common_t           = std::common_type_t<T1, T2>;
        auto     combined_result = extended_gcd_impl(left.first, right.first);
        common_t combined_gcd    = combined_result.first;
        common_t x               = std::get<0>(combined_result.second);
        common_t y               = std::get<1>(combined_result.second);

        auto combined_coefficients = std::tuple_cat(
            std::apply([&](auto... cs) { return std::make_tuple((cs * x)...); }, std::move(left.second)),
            std::apply([&](auto... cs) { return std::make_tuple((cs * y)...); }, std::move(right.second)));

        return std::make_pair(combined_gcd, std::move(combined_coefficients));
    }

    // Extended GCD implementation for multiple integers.
    // This function recursively computes the GCD of multiple integers and finds the coefficients
    // such that a1*x1 + a2*x2 + ... + an*xn = gcd(a1, a2, ..., an).
    // It works by combining the results of the extended GCD computations for pairs of integers
    // until all integers are processed.
    template<class First, class Second, class... Rest>
    inline constexpr std::enable_if_t<
        std::conjunction<std::is_integral<First>, std::is_integral<Second>, std::is_integral<Rest>...>::value,
        extended_gcd_impl_return_t<First, Second, Rest...>>
    extended_gcd_impl(First a, Second b, Rest... rest)
    {
        return extended_gcd_impl_combine(extended_gcd_impl(a, b), extended_gcd_impl(rest...));
    }
} // namespace intl

/**
 * @brief Computes the extended GCD of the given integers.
 *
 * @details The extended GCD algorithm not only computes the greatest common divisor (GCD) of two or more
 * integers, but also finds the coefficients (often called Bézout coefficients) that express the GCD as a
 * linear combination of the given integers. Specifically, for integers \(a\) and \(b\), it finds integers
 * \(x\) and \(y\) such that:
 *
 * \[
 *     a*x + b*y = \gcd(a, b)
 * \]
 *
 * For more than two integers, the algorithm finds coefficients \(x_1, x_2, \ldots, x_n\) such that:
 *
 * \[
 *     a_1 x_1 + a_2 x_2 + \ldots + a_n x_n = \gcd(a_1, a_2, \ldots, a_n)
 * \]
 *
 * @tparam Ts The types of the integers. All types must be integral.
 * @param args The integers.
 * @return A pair containing the GCD and a tuple with the coefficients.
 */
template<class... Ts>
constexpr auto extended_gcd(Ts... args)
{
    return intl::extended_gcd_impl(args...);
}


namespace intl
{
    template<class T, class U>
    constexpr std::common_type_t<T, U> binomial_mod2_impl(T n, U k)
    {
        using common_t           = std::common_type_t<T, U>;
        constexpr auto full_ones = ~common_t(0);
        if(n < k)
            return 0;
        common_t const mask = n | ~(n | k);
        return (mask & full_ones) == full_ones;
    }
} // namespace intl


QS_NAMESPACE_END

#endif // QS_EXTENDED_GCD_H_
