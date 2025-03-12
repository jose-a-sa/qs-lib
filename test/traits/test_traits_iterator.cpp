#include <qs/config.h>
#include <string_view>
#include <test/test_header.h>

#include <qs/traits/iterator.h>

#include <array>
#include <deque>
#include <forward_list>
#include <list>
#include <map>
#include <set>
#include <string>
#include <type_traits>
#include <unordered_map>
#include <unordered_set>
#include <vector>


QS_NAMESPACE_BEGIN

namespace test
{
    template<class C>
    constexpr bool is_input_ouput_iterator_check() noexcept
    {
        return is_input_or_output_iterator<typename C::iterator>::value &&
               is_input_or_output_iterator<typename C::const_iterator>::value;
    }

    template<class C>
    constexpr bool is_input_ouput_rev_iterator_check() noexcept
    {
        return is_input_or_output_iterator<typename C::reverse_iterator>::value &&
               is_input_or_output_iterator<typename C::const_reverse_iterator>::value;
    }

    TEST(Traits_Iterator, IsInputOrOutputIterator)
    {
        struct MyInt
        {};
        using X = std::string;
        using Y = MyInt;

        EXPECT_TRUE((is_input_ouput_iterator_check<std::array<X, 10>>()));
        EXPECT_TRUE((is_input_ouput_iterator_check<std::deque<X>>()));
        EXPECT_TRUE((is_input_ouput_iterator_check<std::forward_list<X>>()));
        EXPECT_TRUE((is_input_ouput_iterator_check<std::list<X>>()));
        EXPECT_TRUE((is_input_ouput_iterator_check<std::map<X, Y>>()));
        EXPECT_TRUE((is_input_ouput_iterator_check<std::set<X>>()));
        EXPECT_TRUE((is_input_ouput_iterator_check<std::string>()));
        EXPECT_TRUE((is_input_ouput_iterator_check<std::unordered_map<X, Y>>()));
        EXPECT_TRUE((is_input_ouput_iterator_check<std::unordered_set<X>>()));
        EXPECT_TRUE((is_input_ouput_iterator_check<std::vector<X>>()));

        EXPECT_TRUE((is_input_ouput_rev_iterator_check<std::array<X, 10>>()));
        EXPECT_TRUE((is_input_ouput_rev_iterator_check<std::deque<X>>()));
        EXPECT_TRUE((is_input_ouput_rev_iterator_check<std::list<X>>()));
        EXPECT_TRUE((is_input_ouput_rev_iterator_check<std::map<X, Y>>()));
        EXPECT_TRUE((is_input_ouput_rev_iterator_check<std::set<X>>()));
        EXPECT_TRUE((is_input_ouput_rev_iterator_check<std::string>()));
        EXPECT_TRUE((is_input_ouput_rev_iterator_check<std::vector<X>>()));
    }

    template<class C>
    constexpr bool is_input_iterator_check() noexcept
    {
        return is_input_iterator<typename C::iterator>::value && is_input_iterator<typename C::const_iterator>::value;
    }

    template<class C>
    constexpr bool is_input_rev_iterator_check() noexcept
    {
        return is_input_iterator<typename C::reverse_iterator>::value &&
               is_input_iterator<typename C::const_reverse_iterator>::value;
    }

    TEST(Traits_Iterator, IsInputtIterator)
    {
        struct MyInt
        {
            int    a;
            double b;
        };
        using X = std::string;
        using Y = MyInt;

        EXPECT_TRUE((is_input_iterator_check<std::array<X, 10>>()));
        EXPECT_TRUE((is_input_iterator_check<std::deque<X>>()));
        EXPECT_TRUE((is_input_iterator_check<std::forward_list<X>>()));
        EXPECT_TRUE((is_input_iterator_check<std::list<X>>()));
        EXPECT_TRUE((is_input_iterator_check<std::map<X, Y>>()));
        EXPECT_TRUE((is_input_iterator_check<std::set<X>>()));
        EXPECT_TRUE((is_input_iterator_check<std::string>()));
        EXPECT_TRUE((is_input_iterator_check<std::unordered_map<X, Y>>()));
        EXPECT_TRUE((is_input_iterator_check<std::unordered_set<X>>()));
        EXPECT_TRUE((is_input_iterator_check<std::vector<X>>()));

        EXPECT_TRUE((is_input_rev_iterator_check<std::array<X, 10>>()));
        EXPECT_TRUE((is_input_rev_iterator_check<std::deque<X>>()));
        EXPECT_TRUE((is_input_rev_iterator_check<std::list<X>>()));
        EXPECT_TRUE((is_input_rev_iterator_check<std::map<X, Y>>()));
        EXPECT_TRUE((is_input_rev_iterator_check<std::set<X>>()));
        EXPECT_TRUE((is_input_rev_iterator_check<std::string>()));
        EXPECT_TRUE((is_input_rev_iterator_check<std::vector<X>>()));
    }

    template<class C, class T>
    constexpr bool is_output_iterator_check() noexcept
    {
        return is_output_iterator<typename C::iterator, T>::value &&
               !is_output_iterator<typename C::const_iterator, T>::value; // not for const iterator
    }

    template<class C, class T>
    constexpr bool is_output_rev_iterator_check() noexcept
    {
        return is_output_iterator<typename C::reverse_iterator, T>::value &&
               !is_output_iterator<typename C::const_reverse_iterator, T>::value; // not for const iterator
    }

    TEST(Traits_Iterator, IsOutputIterator)
    {
        struct MyInt
        {
            int    a;
            double b;
        };
        using X = std::string;
        using Y = MyInt;

        EXPECT_TRUE((is_output_iterator_check<std::array<X, 10>, X>()));
        EXPECT_TRUE((is_output_iterator_check<std::deque<X>, X>()));
        EXPECT_TRUE((is_output_iterator_check<std::forward_list<X>, X>()));
        EXPECT_TRUE((is_output_iterator_check<std::list<X>, X>()));
        EXPECT_FALSE((is_output_iterator_check<std::map<X, Y>, std::pair<X const, Y>>()));
        EXPECT_FALSE((is_output_iterator_check<std::set<X>, std::pair<X const, Y>>()));
        EXPECT_TRUE((is_output_iterator_check<std::string, char>()));
        EXPECT_FALSE((is_output_iterator_check<std::unordered_map<X, Y>, std::pair<X const, Y>>()));
        EXPECT_FALSE((is_output_iterator_check<std::unordered_set<X>, X>()));
        EXPECT_TRUE((is_output_iterator_check<std::vector<X>, X>()));

        EXPECT_TRUE((is_output_rev_iterator_check<std::array<X, 10>, X>()));
        EXPECT_TRUE((is_output_rev_iterator_check<std::deque<X>, X>()));
        EXPECT_TRUE((is_output_rev_iterator_check<std::list<X>, X>()));
        EXPECT_FALSE((is_output_rev_iterator_check<std::map<X, Y>, std::pair<X const, Y>>()));
        EXPECT_FALSE((is_output_rev_iterator_check<std::set<X>, std::pair<X const, Y>>()));
        EXPECT_TRUE((is_output_rev_iterator_check<std::string, char>()));
        EXPECT_TRUE((is_output_rev_iterator_check<std::vector<X>, X>()));
    }

    template<class C>
    constexpr bool is_forward_iterator_check() noexcept
    {
        return is_forward_iterator<typename C::iterator>::value &&
               is_forward_iterator<typename C::const_iterator>::value;
    }

    template<class C>
    constexpr bool is_forward_rev_iterator_check() noexcept
    {
        return is_forward_iterator<typename C::reverse_iterator>::value &&
               is_forward_iterator<typename C::const_reverse_iterator>::value;
    }

    TEST(Traits_Iterator, IsForwardIterator)
    {
        struct MyInt
        {
            int    a;
            double b;
        };
        using X = std::string;
        using Y = MyInt;

        EXPECT_TRUE((is_forward_iterator_check<std::array<X, 10>>()));
        EXPECT_TRUE((is_forward_iterator_check<std::deque<X>>()));
        EXPECT_TRUE((is_forward_iterator_check<std::forward_list<X>>()));
        EXPECT_TRUE((is_forward_iterator_check<std::list<X>>()));
        EXPECT_TRUE((is_forward_iterator_check<std::map<X, Y>>()));
        EXPECT_TRUE((is_forward_iterator_check<std::string>()));
        EXPECT_TRUE((is_forward_iterator_check<std::unordered_map<X, Y>>()));
        EXPECT_TRUE((is_forward_iterator_check<std::unordered_set<X>>()));
        EXPECT_TRUE((is_forward_iterator_check<std::vector<X>>()));

        EXPECT_TRUE((is_forward_rev_iterator_check<std::array<X, 10>>()));
        EXPECT_TRUE((is_forward_rev_iterator_check<std::deque<X>>()));
        EXPECT_TRUE((is_forward_rev_iterator_check<std::list<X>>()));
        EXPECT_TRUE((is_forward_rev_iterator_check<std::map<X, Y>>()));
        EXPECT_TRUE((is_forward_rev_iterator_check<std::set<X>>()));
        EXPECT_TRUE((is_forward_rev_iterator_check<std::string>()));
        EXPECT_TRUE((is_forward_rev_iterator_check<std::vector<X>>()));
    }

    template<class C>
    constexpr bool is_bidirectional_iterator_check() noexcept
    {
        return is_bidirectional_iterator<typename C::iterator>::value &&
               is_bidirectional_iterator<typename C::const_iterator>::value;
    }

    template<class C>
    constexpr bool is_bidirectional_rev_iterator_check() noexcept
    {
        return is_bidirectional_iterator<typename C::reverse_iterator>::value &&
               is_bidirectional_iterator<typename C::const_reverse_iterator>::value;
    }

    TEST(Traits_Iterator, IsBidirectionalIterator)
    {
        struct MyInt
        {
            int    a;
            double b;
        };
        using X = std::string;
        using Y = MyInt;

        EXPECT_TRUE((is_bidirectional_iterator_check<std::array<X, 10>>()));
        EXPECT_TRUE((is_bidirectional_iterator_check<std::deque<X>>()));
        EXPECT_FALSE((is_bidirectional_iterator_check<std::forward_list<X>>()));
        EXPECT_TRUE((is_bidirectional_iterator_check<std::list<X>>()));
        EXPECT_TRUE((is_bidirectional_iterator_check<std::map<X, Y>>()));
        EXPECT_TRUE((is_bidirectional_iterator_check<std::string>()));
        EXPECT_FALSE((is_bidirectional_iterator_check<std::unordered_map<X, Y>>()));
        EXPECT_FALSE((is_bidirectional_iterator_check<std::unordered_set<X>>()));
        EXPECT_TRUE((is_bidirectional_iterator_check<std::vector<X>>()));

        EXPECT_TRUE((is_bidirectional_rev_iterator_check<std::array<X, 10>>()));
        EXPECT_TRUE((is_bidirectional_rev_iterator_check<std::deque<X>>()));
        EXPECT_TRUE((is_bidirectional_rev_iterator_check<std::list<X>>()));
        EXPECT_TRUE((is_bidirectional_rev_iterator_check<std::map<X, Y>>()));
        EXPECT_TRUE((is_bidirectional_rev_iterator_check<std::string>()));
        EXPECT_TRUE((is_bidirectional_rev_iterator_check<std::vector<X>>()));
    }

    template<class C>
    constexpr bool is_random_access_iterator_check() noexcept
    {
        return is_random_access_iterator<typename C::iterator>::value &&
               is_random_access_iterator<typename C::const_iterator>::value;
    }

    template<class C>
    constexpr bool is_random_access_rev_iterator_check() noexcept
    {
        return is_random_access_iterator<typename C::reverse_iterator>::value &&
               is_random_access_iterator<typename C::const_reverse_iterator>::value;
    }

    TEST(Traits_Iterator, IsRandomAccessIterator)
    {
        struct MyInt
        {
            int    a;
            double b;
        };
        using X = std::string;
        using Y = MyInt;

        EXPECT_TRUE((is_random_access_iterator_check<std::array<X, 10>>()));
        EXPECT_TRUE((is_random_access_iterator_check<std::deque<X>>()));
        EXPECT_FALSE((is_random_access_iterator_check<std::forward_list<X>>()));
        EXPECT_FALSE((is_random_access_iterator_check<std::list<X>>()));
        EXPECT_FALSE((is_random_access_iterator_check<std::map<X, Y>>()));
        EXPECT_TRUE((is_random_access_iterator_check<std::string>()));
        EXPECT_FALSE((is_random_access_iterator_check<std::unordered_map<X, Y>>()));
        EXPECT_FALSE((is_random_access_iterator_check<std::unordered_set<X>>()));
        EXPECT_TRUE((is_random_access_iterator_check<std::vector<X>>()));

        EXPECT_TRUE((is_random_access_rev_iterator_check<std::array<X, 10>>()));
        EXPECT_TRUE((is_random_access_rev_iterator_check<std::deque<X>>()));
        EXPECT_FALSE((is_random_access_rev_iterator_check<std::list<X>>()));
        EXPECT_FALSE((is_random_access_rev_iterator_check<std::map<X, Y>>()));
        EXPECT_TRUE((is_random_access_rev_iterator_check<std::string>()));
        EXPECT_TRUE((is_random_access_rev_iterator_check<std::vector<X>>()));
    }

    template<class C>
    constexpr bool is_contiguous_iterator_check() noexcept
    {
        return is_contiguous_iterator<typename C::iterator>::value &&
               is_contiguous_iterator<typename C::const_iterator>::value;
    }

    template<class C>
    constexpr bool is_contiguous_rev_iterator_check() noexcept
    {
        return is_contiguous_iterator<typename C::reverse_iterator>::value &&
               is_contiguous_iterator<typename C::const_reverse_iterator>::value;
    }

    TEST(Traits_Iterator, IsContiguousIterator)
    {
        struct MyInt
        {
            int    a;
            double b;
        };
        using X = std::string;
        using Y = MyInt;

        EXPECT_TRUE((is_contiguous_iterator_check<std::array<X, 10>>()));
        EXPECT_FALSE((is_contiguous_iterator_check<std::deque<X>>()));
        EXPECT_FALSE((is_contiguous_iterator_check<std::forward_list<X>>()));
        EXPECT_FALSE((is_contiguous_iterator_check<std::list<X>>()));
        EXPECT_FALSE((is_contiguous_iterator_check<std::map<X, Y>>()));
        EXPECT_TRUE((is_contiguous_iterator_check<std::string>()));
        EXPECT_FALSE((is_contiguous_iterator_check<std::unordered_map<X, Y>>()));
        EXPECT_FALSE((is_contiguous_iterator_check<std::unordered_set<X>>()));
        EXPECT_TRUE((is_contiguous_iterator_check<std::vector<X>>()));

        EXPECT_FALSE((is_contiguous_rev_iterator_check<std::array<X, 10>>()));
        EXPECT_FALSE((is_contiguous_rev_iterator_check<std::deque<X>>()));
        EXPECT_FALSE((is_contiguous_rev_iterator_check<std::list<X>>()));
        EXPECT_FALSE((is_contiguous_rev_iterator_check<std::map<X, Y>>()));
        EXPECT_FALSE((is_contiguous_rev_iterator_check<std::string>()));
        EXPECT_FALSE((is_contiguous_rev_iterator_check<std::vector<X>>()));
    }

    TEST(Traits_Iterator, ToAddress)
    {
        struct MyInt
        {
            int    a;
            double b;
        };
        using X = MyInt;

        EXPECT_TRUE((std::is_same<decltype(qs::to_address(std::array<X, 10>::iterator{})), X*>::value));
        EXPECT_TRUE((std::is_same<decltype(qs::to_address(std::deque<X>::iterator{})), X*>::value));
        EXPECT_TRUE((std::is_same<decltype(qs::to_address(std::string::iterator{})), char*>::value));
        EXPECT_TRUE((std::is_same<decltype(qs::to_address(std::vector<X>::iterator{})), X*>::value));

        EXPECT_TRUE((std::is_same<decltype(qs::to_address(std::array<X, 10>::const_iterator{})), X const*>::value));
        EXPECT_TRUE((std::is_same<decltype(qs::to_address(std::deque<X>::const_iterator{})), X const*>::value));
        EXPECT_TRUE((std::is_same<decltype(qs::to_address(std::string::const_iterator{})), char const*>::value));
        EXPECT_TRUE((std::is_same<decltype(qs::to_address(std::vector<X>::const_iterator{})), X const*>::value));
    }

} // namespace test

QS_NAMESPACE_END
