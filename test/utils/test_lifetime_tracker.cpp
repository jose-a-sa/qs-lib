#include "test/test_header.h"

#include <qs/utils/lifecycle_tracker.h>
#include <string>
#include <vector>

QS_NAMESPACE_BEGIN

template<class T, size_t Uuid = 0>
struct lifecycle_tracker_testing_log;


struct MyInt
{
    MyInt() = default;
    MyInt(int v)
        : v{v} {};
    int v{};

    constexpr bool operator==(MyInt const& rhs) const { return v == rhs.v; }
};

template<size_t Uuid>
struct lifecycle_tracker_testing_log<MyInt, Uuid> : lifecycle_tracker_default_logger<MyInt, Uuid>
{
    static void expect_values_eq(std::vector<MyInt> const& expected)
    {
        EXPECT_EQ(values, expected);
        values.clear();
    }

    static void expect_counters_eq(lifecycle_counters const& cnts)
    {
        lifecycle_tracker<MyInt, Uuid>::print_counters();
        EXPECT_EQ((lifecycle_tracker<MyInt, Uuid>::get_counters()), cnts);
    }

    template<class... Args>
    static constexpr void log_value(Args&&... args)
    {
        values.emplace_back(std::forward<Args>(args)...);
    }

    template<lifecycle_event Cnt>
    constexpr void log_event(MyInt const&           self, lifecycle_counters const& /*cnts*/,
                             std::string_view const type_name) const
    {
        log_value(self);
        std::printf(lifecycle_tracker_default_logger<MyInt, Uuid>::template log_event_format<Cnt>(),
                    static_cast<int>(type_name.size()), type_name.data(), static_cast<int>(type_name.size()),
                    type_name.data());
        std::printf(" -> {v: %d} \n", self.v);
    }

private:
    inline static std::vector<MyInt> values{};
};

template<size_t Uuid>
struct lifecycle_tracker_logger<MyInt, Uuid> : lifecycle_tracker_testing_log<MyInt, Uuid>
{};


TEST(LifetimeTracker, CopyTrivialType)
{
    {
        std::vector<lifecycle_tracker<MyInt>> vec;
        vec.reserve(100);

        vec.emplace_back(10);
        vec.emplace_back(17);
        lifecycle_tracker_testing_log<MyInt>::expect_counters_eq({2, 0, 0, 0, 0, 0});
        lifecycle_tracker_testing_log<MyInt>::expect_values_eq({10, 17});

        vec.insert(vec.cend(), {22, 23, 24, 25});
        lifecycle_tracker_testing_log<MyInt>::expect_counters_eq({6, 4, 0, 0, 0, 4});
        lifecycle_tracker_testing_log<MyInt>::expect_values_eq({22, 23, 24, 25, 22, 23, 24, 25, 25, 24, 23, 22});

        auto vec_c = vec; // copy
        lifecycle_tracker_testing_log<MyInt>::expect_counters_eq({6, 10, 0, 0, 0, 4});
        lifecycle_tracker_testing_log<MyInt>::expect_values_eq({10, 17, 22, 23, 24, 25});

        vec.erase(vec.begin() + 4, vec.end());
        lifecycle_tracker_testing_log<MyInt>::expect_counters_eq({6, 10, 0, 0, 0, 6});
        lifecycle_tracker_testing_log<MyInt>::expect_values_eq({25, 24});

        vec.assign({111, 112, 113});
        lifecycle_tracker_testing_log<MyInt>::expect_counters_eq({9, 10, 0, 3, 0, 10});
        lifecycle_tracker_testing_log<MyInt>::expect_values_eq({111, 112, 113, 111, 112, 113, 23, 113, 112, 111});

        vec = std::move(vec_c);
        lifecycle_tracker_testing_log<MyInt>::expect_counters_eq({9, 10, 0, 3, 0, 13});
        lifecycle_tracker_testing_log<MyInt>::expect_values_eq({113, 112, 111});
    }

    lifecycle_tracker_testing_log<MyInt>::expect_counters_eq({9, 10, 0, 3, 0, 19});
    lifecycle_tracker_testing_log<MyInt>::expect_values_eq({25, 24, 23, 22, 17, 10});
}


template<size_t Uuid>
struct lifecycle_tracker_testing_log<std::string, Uuid> : lifecycle_tracker_default_logger<std::string, Uuid>
{
    static void expect_values_eq(std::vector<std::string> const& expected)
    {
        EXPECT_EQ(values, expected);
        values.clear();
    }

    static void expect_counters_eq(lifecycle_counters const& cnts)
    {
        lifecycle_tracker<std::string, Uuid>::print_counters();
        EXPECT_EQ((lifecycle_tracker<std::string, Uuid>::get_counters()), cnts);
    }

    template<class... Args>
    static constexpr void log_value(Args&&... args)
    {
        values.emplace_back(std::forward<Args>(args)...);
    }

    using base = lifecycle_tracker_default_logger<std::string, Uuid>;

    template<lifecycle_event Cnt>
    constexpr void log_event(std::string const&     self, lifecycle_counters const& /*cnts*/,
                             std::string_view const type_name) const
    {
        log_value(self);
        std::printf(base::template log_event_format<Cnt>(), static_cast<int>(type_name.size()), type_name.data(),
                    static_cast<int>(type_name.size()), type_name.data());
        std::printf(" -> \"%.*s\" \n", static_cast<int>(self.size()), self.data());
    }

private:
    inline static std::vector<std::string> values{};
};

template<size_t Uuid>
struct lifecycle_tracker_logger<std::string, Uuid> : lifecycle_tracker_testing_log<std::string, Uuid>
{};


TEST(LifetimeTracker, StdString)
{
    lifecycle_tracker<std::string>::set_type_name("std::string");

    {
        std::vector<lifecycle_tracker<std::string>> vec;
        vec.reserve(100);

        vec.emplace_back("abc");
        vec.emplace_back("def");
        lifecycle_tracker_testing_log<std::string>::expect_counters_eq({2, 0, 0, 0, 0, 0});
        lifecycle_tracker_testing_log<std::string>::expect_values_eq({"abc", "def"});

        vec.insert(vec.cend(), {"22", "23", "24", "25"});
        lifecycle_tracker_testing_log<std::string>::expect_counters_eq({6, 4, 0, 0, 0, 4});
        lifecycle_tracker_testing_log<std::string>::expect_values_eq(
            {"22", "23", "24", "25", "22", "23", "24", "25", "25", "24", "23", "22"});

        auto vec_c = vec; // copy
        lifecycle_tracker_testing_log<std::string>::expect_counters_eq({6, 10, 0, 0, 0, 4});
        lifecycle_tracker_testing_log<std::string>::expect_values_eq({"abc", "def", "22", "23", "24", "25"});

        vec.erase(vec.begin() + 4, vec.end());
        lifecycle_tracker_testing_log<std::string>::expect_counters_eq({6, 10, 0, 0, 0, 6});
        lifecycle_tracker_testing_log<std::string>::expect_values_eq({"25", "24"});

        vec.assign({"aaaaaaaaaaaaaaaaaaaaaaa", "bbbbbbbbbbbbbbbbbbbbbbb", "ccccccccccccccccccccccc"});
        lifecycle_tracker_testing_log<std::string>::expect_counters_eq({9, 10, 0, 3, 0, 10});
        lifecycle_tracker_testing_log<std::string>::expect_values_eq(
            {"aaaaaaaaaaaaaaaaaaaaaaa", "bbbbbbbbbbbbbbbbbbbbbbb", "ccccccccccccccccccccccc", "aaaaaaaaaaaaaaaaaaaaaaa",
             "bbbbbbbbbbbbbbbbbbbbbbb", "ccccccccccccccccccccccc", "23", "ccccccccccccccccccccccc",
             "bbbbbbbbbbbbbbbbbbbbbbb", "aaaaaaaaaaaaaaaaaaaaaaa"});

        vec = std::move(vec_c);
        lifecycle_tracker_testing_log<std::string>::expect_counters_eq({9, 10, 0, 3, 0, 13});
        lifecycle_tracker_testing_log<std::string>::expect_values_eq(
            {"ccccccccccccccccccccccc", "bbbbbbbbbbbbbbbbbbbbbbb", "aaaaaaaaaaaaaaaaaaaaaaa"});
    }
}


QS_NAMESPACE_END
