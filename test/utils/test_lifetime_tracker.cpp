#include "test/test_header.h"

#include "gmock/gmock.h"
#include <qs/utils/lifecycle_tracker.h>
#include <string>
#include <vector>

QS_NAMESPACE_BEGIN

template<class T, size_t Uuid = 0>
struct lc_logger_tester;

template<class T, size_t Uuid>
struct lc_logger_tester_base : lifecycle_tracker_default_logger<T, Uuid>
{
    using base = lifecycle_tracker_default_logger<T, Uuid>;
    using base::log_event;
    using base::print_counters;

    static void expect_values_eq(std::vector<T> const& expected)
    {
        EXPECT_THAT(values, ::testing::Pointwise(::testing::Eq(), expected));
        values.clear();
    }

    template<class... Args>
    static constexpr void log_value(Args&&... args)
    {
        values.emplace_back(std::forward<Args>(args)...);
    }

protected:
    using base::log_counters_format;
    using base::log_event_format;

private:
    inline static std::vector<T> values{};
};

template<class T, bool MultiThreaded = false, size_t Uuid = 0>
struct lc_tracker_tester : std::conditional_t<MultiThreaded, lifecycle_tracker_mt<T, Uuid>, lifecycle_tracker<T, Uuid>>
{
    using base = std::conditional_t<MultiThreaded, lifecycle_tracker_mt<T, Uuid>, lifecycle_tracker<T, Uuid>>;
    using base::base;
    using base::get_counters;
    using base::get_type_name;
    using base::print_counters;
    using base::reset_counters;
    using base::set_type_name;

    using counter_ref_or_value_t = std::conditional_t<MultiThreaded, lifecycle_counters, lifecycle_counters const&>;

    static constexpr counter_ref_or_value_t compare_print_counters(lifecycle_counters const& expected)
    {
        auto&& cnts = print_counters();
        EXPECT_EQ(cnts, expected);
        return cnts;
    }

    static constexpr counter_ref_or_value_t compare_get_counters(lifecycle_counters const& expected)
    {
        auto&& cnts = get_counters();
        EXPECT_EQ(cnts, expected);
        return cnts;
    }
};


struct MyInt
{
    MyInt() = default;
    MyInt(int v)
        : v{v} {};
    int v{};

    constexpr bool operator==(MyInt const& rhs) const { return v == rhs.v; }
};

template<size_t Uuid>
struct lc_logger_tester<MyInt, Uuid> : lc_logger_tester_base<MyInt, Uuid>
{
    using base   = lc_logger_tester_base<MyInt, Uuid>;

    template<lifecycle_event Cnt>
    constexpr void log_event(MyInt const& self, std::string_view const type_name) const
    {
        base::log_value(self);
        std::printf(base::template log_event_format<Cnt>(), static_cast<int>(type_name.size()), type_name.data(),
                    static_cast<int>(type_name.size()), type_name.data());
        std::printf(" -> {v: %d} \n", self.v);
    }
};

template<size_t Uuid>
struct lifecycle_tracker_logger<MyInt, Uuid> : lc_logger_tester<MyInt, Uuid>
{};

TEST(LifetimeTracker, CopyTrivialType)
{
    {
        std::vector<lc_tracker_tester<MyInt>> vec;
        vec.reserve(100);

        vec.emplace_back(10);
        vec.emplace_back(17);
        lc_tracker_tester<MyInt>::compare_print_counters({2, 0, 0, 0, 0, 0});
        lc_logger_tester<MyInt>::expect_values_eq({10, 17});

        vec.insert(vec.cend(), {22, 23, 24, 25});
        lc_tracker_tester<MyInt>::compare_print_counters({6, 4, 0, 0, 0, 4});
        lc_logger_tester<MyInt>::expect_values_eq({22, 23, 24, 25, 22, 23, 24, 25, 25, 24, 23, 22});

        auto vec_c = vec; // copy
        lc_tracker_tester<MyInt>::compare_print_counters({6, 10, 0, 0, 0, 4});
        lc_logger_tester<MyInt>::expect_values_eq({10, 17, 22, 23, 24, 25});

        vec.erase(vec.begin() + 4, vec.end());
        lc_tracker_tester<MyInt>::compare_print_counters({6, 10, 0, 0, 0, 6});
        lc_logger_tester<MyInt>::expect_values_eq({25, 24});

        vec.assign({111, 112, 113});
        lc_tracker_tester<MyInt>::compare_print_counters({9, 10, 0, 3, 0, 10});
        lc_logger_tester<MyInt>::expect_values_eq({111, 112, 113, 111, 112, 113, 23, 113, 112, 111});

        vec = std::move(vec_c);
        lc_tracker_tester<MyInt>::compare_print_counters({9, 10, 0, 3, 0, 13});
        lc_logger_tester<MyInt>::expect_values_eq({113, 112, 111});
    }

    lc_tracker_tester<MyInt>::compare_print_counters({9, 10, 0, 3, 0, 19});
    lc_logger_tester<MyInt>::expect_values_eq({25, 24, 23, 22, 17, 10});
}


template<size_t Uuid>
struct lc_logger_tester<std::string, Uuid> : lc_logger_tester_base<std::string, Uuid>
{
    using base   = lc_logger_tester_base<std::string, Uuid>;

    template<lifecycle_event Cnt>
    constexpr void log_event(std::string const& self, std::string_view const type_name) const
    {
        base::log_value(self);
        std::printf(base::template log_event_format<Cnt>(), static_cast<int>(type_name.size()), type_name.data(),
                    static_cast<int>(type_name.size()), type_name.data());
        std::printf(" -> \"%.*s\" \n", static_cast<int>(self.size()), self.data());
    }
};

template<size_t Uuid>
struct lifecycle_tracker_logger<std::string, Uuid> : lc_logger_tester<std::string, Uuid>
{};

TEST(LifetimeTracker, StdString)
{
    lc_tracker_tester<std::string,true>::set_type_name("std::string");

    {
        std::vector<lc_tracker_tester<std::string,true>> vec;
        vec.reserve(100);

        vec.emplace_back("abc");
        vec.emplace_back("def");
        lc_tracker_tester<std::string,true>::compare_print_counters({2, 0, 0, 0, 0, 0});
        lc_logger_tester<std::string>::expect_values_eq({"abc", "def"});

        vec.insert(vec.cend(), {"22", "23", "24", "25"});
        lc_tracker_tester<std::string,true>::compare_print_counters({6, 4, 0, 0, 0, 4});
        lc_logger_tester<std::string>::expect_values_eq(
            {"22", "23", "24", "25", "22", "23", "24", "25", "25", "24", "23", "22"});

        auto vec_c = vec; // copy
        lc_tracker_tester<std::string,true>::compare_print_counters({6, 10, 0, 0, 0, 4});
        lc_logger_tester<std::string>::expect_values_eq({"abc", "def", "22", "23", "24", "25"});

        vec.erase(vec.begin() + 4, vec.end());
        lc_tracker_tester<std::string,true>::compare_print_counters({6, 10, 0, 0, 0, 6});
        lc_logger_tester<std::string>::expect_values_eq({"25", "24"});

        vec.assign({"aaaaaaaaaaaaaaaaaaaaaaa", "bbbbbbbbbbbbbbbbbbbbbbb", "ccccccccccccccccccccccc"});
        lc_tracker_tester<std::string,true>::compare_print_counters({9, 10, 0, 3, 0, 10});
        lc_logger_tester<std::string>::expect_values_eq(
            {"aaaaaaaaaaaaaaaaaaaaaaa", "bbbbbbbbbbbbbbbbbbbbbbb", "ccccccccccccccccccccccc", "aaaaaaaaaaaaaaaaaaaaaaa",
             "bbbbbbbbbbbbbbbbbbbbbbb", "ccccccccccccccccccccccc", "23", "ccccccccccccccccccccccc",
             "bbbbbbbbbbbbbbbbbbbbbbb", "aaaaaaaaaaaaaaaaaaaaaaa"});

        vec = std::move(vec_c);
        lc_tracker_tester<std::string,true>::compare_print_counters({9, 10, 0, 3, 0, 13});
        lc_logger_tester<std::string>::expect_values_eq(
            {"ccccccccccccccccccccccc", "bbbbbbbbbbbbbbbbbbbbbbb", "aaaaaaaaaaaaaaaaaaaaaaa"});
    }
}


QS_NAMESPACE_END
