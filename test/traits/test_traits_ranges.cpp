#include <qs/config.h>
#include <test/test_header.h>

#include <qs/traits/ranges.h>

#include <array>
#include <deque>
#include <forward_list>
#include <list>
#include <map>
#include <set>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>


QS_NAMESPACE_BEGIN


template<class T, class V = remove_reference_t<T>, class Ref = add_lvalue_reference_t<V>,
         class RRef = add_rvalue_reference_t<V>, class CV = V const, class CRef = add_lvalue_reference_t<CV>,
         class CRRef = add_rvalue_reference_t<CV>>
bool is_range_check()
{
    return is_range<V>::value && is_range<Ref>::value && is_range<RRef>::value && is_range<CV>::value &&
           is_range<CRef>::value && is_range<CRRef>::value;
}

TEST(Traits_Ranges, IsRange)
{
    struct MyInt
    {
        int    a;
        double b;
    };
    using X = std::string;
    using Y = MyInt;

    EXPECT_TRUE((is_range_check<std::array<X, 10>>()));
    EXPECT_TRUE((is_range_check<std::deque<X>>()));
    EXPECT_TRUE((is_range_check<std::forward_list<X>>()));
    EXPECT_TRUE((is_range_check<std::list<X>>()));
    EXPECT_TRUE((is_range_check<std::map<X, Y>>()));
    EXPECT_TRUE((is_range_check<std::set<X>>()));
    EXPECT_TRUE((is_range_check<std::string>()));
    EXPECT_TRUE((is_range_check<std::unordered_map<X, Y>>()));
    EXPECT_TRUE((is_range_check<std::unordered_set<X>>()));
    EXPECT_TRUE((is_range_check<std::vector<X>>()));
}

template<class T, class V = remove_reference_t<T>, class Ref = add_lvalue_reference_t<V>,
         class RRef = add_rvalue_reference_t<V>, class CV = V const, class CRef = add_lvalue_reference_t<CV>,
         class CRRef = add_rvalue_reference_t<CV>>
bool is_input_range_check()
{
    return is_input_range<T>::value && is_input_range<Ref>::value && is_input_range<RRef>::value &&
           is_input_range<CV>::value && is_input_range<CRef>::value && is_input_range<CRRef>::value;
}

TEST(Traits_Ranges, IsInputRange)
{
    struct MyInt
    {
        int    a;
        double b;
    };
    using X = std::string;
    using Y = MyInt;

    EXPECT_TRUE((is_input_range_check<std::array<X, 10>>()));
    EXPECT_TRUE((is_input_range_check<std::deque<X>>()));
    EXPECT_TRUE((is_input_range_check<std::forward_list<X>>()));
    EXPECT_TRUE((is_input_range_check<std::list<X>>()));
    EXPECT_TRUE((is_input_range_check<std::map<X, Y>>()));
    EXPECT_TRUE((is_input_range_check<std::set<X>>()));
    EXPECT_TRUE((is_input_range_check<std::string>()));
    EXPECT_TRUE((is_input_range_check<std::unordered_map<X, Y>>()));
    EXPECT_TRUE((is_input_range_check<std::unordered_set<X>>()));
    EXPECT_TRUE((is_input_range_check<std::vector<X>>()));
}

template<class T, class V = remove_reference_t<T>, class Ref = add_lvalue_reference_t<V>,
         class RRef = add_rvalue_reference_t<V>, class CV = V const, class CRef = add_lvalue_reference_t<CV>,
         class CRRef = add_rvalue_reference_t<CV>>
bool is_borrowed_range_check()
{
    return !is_borrowed_range<V>::value && is_borrowed_range<Ref>::value && !is_borrowed_range<RRef>::value &&
           !is_borrowed_range<CV>::value && is_borrowed_range<CRef>::value && !is_borrowed_range<CRRef>::value;
}

TEST(Traits_Ranges, IsBorrowedRange)
{
    struct MyInt
    {
        int    a;
        double b;
    };
    using X = std::string;
    using Y = MyInt;

    EXPECT_TRUE((is_borrowed_range_check<std::array<X, 10>>()));
    EXPECT_TRUE((is_borrowed_range_check<std::deque<X>>()));
    EXPECT_TRUE((is_borrowed_range_check<std::forward_list<X>>()));
    EXPECT_TRUE((is_borrowed_range_check<std::list<X>>()));
    EXPECT_TRUE((is_borrowed_range_check<std::map<X, Y>>()));
    EXPECT_TRUE((is_borrowed_range_check<std::set<X>>()));
    EXPECT_TRUE((is_borrowed_range_check<std::string>()));
    EXPECT_TRUE((is_borrowed_range_check<std::unordered_map<X, Y>>()));
    EXPECT_TRUE((is_borrowed_range_check<std::unordered_set<X>>()));
    EXPECT_TRUE((is_borrowed_range_check<std::vector<X>>()));
}

template<class T, class V = remove_reference_t<T>, class Ref = add_lvalue_reference_t<V>,
         class RRef = add_rvalue_reference_t<V>, class CV = V const, class CRef = add_lvalue_reference_t<CV>,
         class CRRef = add_rvalue_reference_t<CV>>
bool is_sized_range_check()
{
    return is_sized_range<V>::value && is_sized_range<Ref>::value && is_sized_range<RRef>::value &&
           is_sized_range<CV>::value && is_sized_range<CRef>::value && is_sized_range<CRRef>::value;
}

TEST(Traits_Ranges, IsSizedRange)
{
    struct MyInt
    {
        int    a;
        double b;
    };
    using X = std::string;
    using Y = MyInt;

    EXPECT_TRUE((is_sized_range_check<std::array<X, 10>>()));
    EXPECT_TRUE((is_sized_range_check<std::deque<X>>()));
    EXPECT_FALSE((is_sized_range_check<std::forward_list<X>>()));
    EXPECT_TRUE((is_sized_range_check<std::list<X>>()));
    EXPECT_TRUE((is_sized_range_check<std::map<X, Y>>()));
    EXPECT_TRUE((is_sized_range_check<std::set<X>>()));
    EXPECT_TRUE((is_sized_range_check<std::string>()));
    EXPECT_TRUE((is_sized_range_check<std::unordered_map<X, Y>>()));
    EXPECT_TRUE((is_sized_range_check<std::unordered_set<X>>()));
    EXPECT_TRUE((is_sized_range_check<std::vector<X>>()));
}


template<class T, class E, class V = remove_reference_t<T>, class Ref = add_lvalue_reference_t<V>,
         class RRef = add_rvalue_reference_t<V>, class CV = V const, class CRef = add_lvalue_reference_t<CV>,
         class CRRef = add_rvalue_reference_t<CV>>
bool is_output_range_check()
{
    return is_output_range<V, E>::value && is_output_range<Ref, E>::value && is_output_range<RRef, E>::value &&
           !is_output_range<CV, E>::value && !is_output_range<CRef, E>::value && !is_output_range<CRRef, E>::value;
}

TEST(Traits_Ranges, IsOutputRange)
{
    struct MyInt
    {
        int    a;
        double b;
    };
    using X = std::string;
    using Y = MyInt;

    EXPECT_TRUE((is_output_range_check<std::array<X, 10>, X>()));
    EXPECT_TRUE((is_output_range_check<std::deque<X>, X>()));
    EXPECT_TRUE((is_output_range_check<std::forward_list<X>, X>()));
    EXPECT_TRUE((is_output_range_check<std::list<X>, X>()));
    EXPECT_FALSE((is_output_range_check<std::map<X, Y>, std::pair<X const, Y>>()));
    EXPECT_FALSE((is_output_range_check<std::set<X>, X>()));
    EXPECT_TRUE((is_output_range_check<std::string, char>()));
    EXPECT_FALSE((is_output_range_check<std::unordered_map<X, Y>, std::pair<X const, Y>>()));
    EXPECT_FALSE((is_output_range_check<std::unordered_set<X>, X>()));
    EXPECT_TRUE((is_output_range_check<std::vector<X>, X>()));
}

template<class T, class V = remove_reference_t<T>, class Ref = add_lvalue_reference_t<V>,
         class RRef = add_rvalue_reference_t<V>, class CV = V const, class CRef = add_lvalue_reference_t<CV>,
         class CRRef = add_rvalue_reference_t<CV>>
bool is_forward_range_check()
{
    return is_forward_range<V>::value && is_forward_range<Ref>::value && is_forward_range<RRef>::value &&
           is_forward_range<CV>::value && is_forward_range<CRef>::value && is_forward_range<CRRef>::value;
}

TEST(Traits_Ranges, IsForwardRange)
{
    struct MyInt
    {
        int    a;
        double b;
    };
    using X = std::string;
    using Y = MyInt;

    EXPECT_TRUE((is_forward_range_check<std::array<X, 10>>()));
    EXPECT_TRUE((is_forward_range_check<std::deque<X>>()));
    EXPECT_TRUE((is_forward_range_check<std::forward_list<X>>()));
    EXPECT_TRUE((is_forward_range_check<std::list<X>>()));
    EXPECT_TRUE((is_forward_range_check<std::map<X, Y>>()));
    EXPECT_TRUE((is_forward_range_check<std::set<X>>()));
    EXPECT_TRUE((is_forward_range_check<std::string>()));
    EXPECT_TRUE((is_forward_range_check<std::unordered_map<X, Y>>()));
    EXPECT_TRUE((is_forward_range_check<std::unordered_set<X>>()));
    EXPECT_TRUE((is_forward_range_check<std::vector<X>>()));
}

template<class T, class V = remove_reference_t<T>, class Ref = add_lvalue_reference_t<V>,
         class RRef = add_rvalue_reference_t<V>, class CV = V const, class CRef = add_lvalue_reference_t<CV>,
         class CRRef = add_rvalue_reference_t<CV>>
bool is_bidirectional_range_check()
{
    return is_bidirectional_range<V>::value && is_bidirectional_range<Ref>::value &&
           is_bidirectional_range<RRef>::value && is_bidirectional_range<CV>::value &&
           is_bidirectional_range<CRef>::value && is_bidirectional_range<CRRef>::value;
}

TEST(Traits_Ranges, IsBidirectionalRange)
{
    struct MyInt
    {
        int    a;
        double b;
    };
    using X = std::string;
    using Y = MyInt;

    EXPECT_TRUE((is_bidirectional_range_check<std::array<X, 10>>()));
    EXPECT_TRUE((is_bidirectional_range_check<std::deque<X>>()));
    EXPECT_FALSE((is_bidirectional_range_check<std::forward_list<X>>()));
    EXPECT_TRUE((is_bidirectional_range_check<std::list<X>>()));
    EXPECT_TRUE((is_bidirectional_range_check<std::map<X, Y>>()));
    EXPECT_TRUE((is_bidirectional_range_check<std::set<X>>()));
    EXPECT_TRUE((is_bidirectional_range_check<std::string>()));
    EXPECT_FALSE((is_bidirectional_range_check<std::unordered_map<X, Y>>()));
    EXPECT_FALSE((is_bidirectional_range_check<std::unordered_set<X>>()));
    EXPECT_TRUE((is_bidirectional_range_check<std::vector<X>>()));
}

template<class T, class V = remove_reference_t<T>, class Ref = add_lvalue_reference_t<V>,
         class RRef = add_rvalue_reference_t<V>, class CV = V const, class CRef = add_lvalue_reference_t<CV>,
         class CRRef = add_rvalue_reference_t<CV>>
bool is_random_access_range_check()
{
    return is_random_access_range<V>::value && is_random_access_range<Ref>::value &&
           is_random_access_range<RRef>::value && is_random_access_range<CV>::value &&
           is_random_access_range<CRef>::value && is_random_access_range<CRRef>::value;
}

TEST(Traits_Ranges, IsRandomAccessRange)
{
    struct MyInt
    {
        int    a;
        double b;
    };
    using X = std::string;
    using Y = MyInt;

    EXPECT_TRUE((is_random_access_range_check<std::array<X, 10>>()));
    EXPECT_TRUE((is_random_access_range_check<std::deque<X>>()));
    EXPECT_FALSE((is_random_access_range_check<std::forward_list<X>>()));
    EXPECT_FALSE((is_random_access_range_check<std::list<X>>()));
    EXPECT_FALSE((is_random_access_range_check<std::map<X, Y>>()));
    EXPECT_FALSE((is_random_access_range_check<std::set<X>>()));
    EXPECT_TRUE((is_random_access_range_check<std::string>()));
    EXPECT_FALSE((is_random_access_range_check<std::unordered_map<X, Y>>()));
    EXPECT_FALSE((is_random_access_range_check<std::unordered_set<X>>()));
    EXPECT_TRUE((is_random_access_range_check<std::vector<X>>()));
}


template<class T, class V = remove_reference_t<T>, class Ref = add_lvalue_reference_t<V>,
         class RRef = add_rvalue_reference_t<V>, class CV = V const, class CRef = add_lvalue_reference_t<CV>,
         class CRRef = add_rvalue_reference_t<CV>>
bool is_contiguous_range_check()
{
    return is_contiguous_range<V>::value && is_contiguous_range<Ref>::value && is_contiguous_range<RRef>::value &&
           is_contiguous_range<CV>::value && is_contiguous_range<CRef>::value && is_contiguous_range<CRRef>::value;
}

TEST(Traits_Ranges, IsContiguousRange)
{
    struct MyInt
    {
        int    a;
        double b;
    };
    using X = std::string;
    using Y = MyInt;

    EXPECT_TRUE((is_contiguous_range_check<std::array<X, 10>>()));
    EXPECT_FALSE((is_contiguous_range_check<std::deque<X>>()));
    EXPECT_FALSE((is_contiguous_range_check<std::forward_list<X>>()));
    EXPECT_FALSE((is_contiguous_range_check<std::list<X>>()));
    EXPECT_FALSE((is_contiguous_range_check<std::map<X, Y>>()));
    EXPECT_FALSE((is_contiguous_range_check<std::set<X>>()));
    EXPECT_TRUE((is_contiguous_range_check<std::string>()));
    EXPECT_FALSE((is_contiguous_range_check<std::unordered_map<X, Y>>()));
    EXPECT_FALSE((is_contiguous_range_check<std::unordered_set<X>>()));
    EXPECT_TRUE((is_contiguous_range_check<std::vector<X>>()));
}


QS_NAMESPACE_END
