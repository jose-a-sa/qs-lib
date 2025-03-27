#include <test/test_header.h>


#include <array>
#include <string>
#include <vector>


#include <qs/utils/lifecycle_tracker.h>


#define IGNORE_STDOUT(...)                                                                                             \
    testing::internal::CaptureStdout();                                                                                \
    __VA_ARGS__;                                                                                                       \
    testing::internal::GetCapturedStdout();


QS_NAMESPACE_BEGIN


struct MyInt
{
    MyInt() = default;
    MyInt(int v)
        : v{v} {};
    int v{};

    constexpr bool operator==(MyInt const& rhs) const { return v == rhs.v; }
};


template<size_t Uuid>
struct lifecycle_logger<MyInt, Uuid> : lifecycle_default_logger<MyInt, Uuid>
{
    template<lifecycle_event Cnt>
    QS_CONSTEXPR17 void log_event(MyInt const& self, std::string const& type_name) const
    {
        fmt::println(event_fmt_map_[static_cast<size_t>(Cnt)], type_name, self.v);
    }

private:
    static constexpr std::array<char const*, 6> event_fmt_map_{"{0}(...) -> [v: {1}]",   "{0}({0} const&) -> [v: {1}]",
                                                               "{0}({0}&&) -> [v: {1}]", "=({0} const&) -> [v: {1}]",
                                                               "=({0}&&) -> [v: {1}]",   "~{0}() -> [v: {1}]"};
};


std::ostream& operator<<(std::ostream& out, lifecycle_counters const& cnts)
{
    return out << "{" << cnts.constructor << ", " << cnts.copy_constructor << ", " << cnts.move_constructor << ", "
               << cnts.copy_assignment << ", " << cnts.move_assignment << ", " << cnts.destructor << "}";
}


TEST(LifetimeTracker, TrivialType)
{
    {
        std::vector<lifecycle_tracker<MyInt>> vec;
        vec.reserve(100);

        EXPECT_STDOUT_EQ(({ vec.emplace_back(10); }), "qs::MyInt(...) -> [v: 10]\n"
                                                      "qs::MyInt(qs::MyInt const&) -> [v: 10]\n"
                                                      "~qs::MyInt() -> [v: 10]\n");
        EXPECT_EQ((lifecycle_tracker<MyInt>::get_counters()), (lifecycle_counters{1, 1, 0, 0, 0, 1}));

        EXPECT_STDOUT_EQ(({ vec.emplace_back(17); }), "qs::MyInt(...) -> [v: 17]\n"
                                                      "qs::MyInt(qs::MyInt const&) -> [v: 17]\n"
                                                      "~qs::MyInt() -> [v: 17]\n");
        EXPECT_EQ((lifecycle_tracker<MyInt>::get_counters()), (lifecycle_counters{2, 2, 0, 0, 0, 2}));

        EXPECT_STDOUT_EQ(({ vec.insert(vec.cend(), {22, 23, 24, 25}); }), "qs::MyInt(...) -> [v: 22]\n"
                                                                          "qs::MyInt(...) -> [v: 23]\n"
                                                                          "qs::MyInt(...) -> [v: 24]\n"
                                                                          "qs::MyInt(...) -> [v: 25]\n"
                                                                          "qs::MyInt(qs::MyInt const&) -> [v: 22]\n"
                                                                          "qs::MyInt(qs::MyInt const&) -> [v: 23]\n"
                                                                          "qs::MyInt(qs::MyInt const&) -> [v: 24]\n"
                                                                          "qs::MyInt(qs::MyInt const&) -> [v: 25]\n"
                                                                          "~qs::MyInt() -> [v: 25]\n"
                                                                          "~qs::MyInt() -> [v: 24]\n"
                                                                          "~qs::MyInt() -> [v: 23]\n"
                                                                          "~qs::MyInt() -> [v: 22]\n");
        EXPECT_EQ((lifecycle_tracker<MyInt>::get_counters()), (lifecycle_counters{6, 6, 0, 0, 0, 6}));

        EXPECT_STDOUT_EQ(({ auto vec_c = vec; }), "qs::MyInt(qs::MyInt const&) -> [v: 10]\n"
                                                  "qs::MyInt(qs::MyInt const&) -> [v: 17]\n"
                                                  "qs::MyInt(qs::MyInt const&) -> [v: 22]\n"
                                                  "qs::MyInt(qs::MyInt const&) -> [v: 23]\n"
                                                  "qs::MyInt(qs::MyInt const&) -> [v: 24]\n"
                                                  "qs::MyInt(qs::MyInt const&) -> [v: 25]\n"
                                                  "~qs::MyInt() -> [v: 25]\n"
                                                  "~qs::MyInt() -> [v: 24]\n"
                                                  "~qs::MyInt() -> [v: 23]\n"
                                                  "~qs::MyInt() -> [v: 22]\n"
                                                  "~qs::MyInt() -> [v: 17]\n"
                                                  "~qs::MyInt() -> [v: 10]\n");
        EXPECT_EQ((lifecycle_tracker<MyInt>::get_counters()), (lifecycle_counters{6, 12, 0, 0, 0, 12}));


        EXPECT_STDOUT_EQ(({ vec.erase(vec.begin() + 4, vec.end()); }), "~qs::MyInt() -> [v: 25]\n"
                                                                       "~qs::MyInt() -> [v: 24]\n");
        EXPECT_EQ((lifecycle_tracker<MyInt>::get_counters()), (lifecycle_counters{6, 12, 0, 0, 0, 14}));


        IGNORE_STDOUT(auto vec_c = vec);
        EXPECT_EQ((lifecycle_tracker<MyInt>::get_counters()), (lifecycle_counters{6, 16, 0, 0, 0, 14}));


        EXPECT_STDOUT_EQ(({ vec_c.assign({111, 112, 113}); }), "qs::MyInt(...) -> [v: 111]\n"
                                                               "qs::MyInt(...) -> [v: 112]\n"
                                                               "qs::MyInt(...) -> [v: 113]\n"
                                                               "=(qs::MyInt const&) -> [v: 111]\n"
                                                               "=(qs::MyInt const&) -> [v: 112]\n"
                                                               "=(qs::MyInt const&) -> [v: 113]\n"
                                                               "~qs::MyInt() -> [v: 23]\n"
                                                               "~qs::MyInt() -> [v: 113]\n"
                                                               "~qs::MyInt() -> [v: 112]\n"
                                                               "~qs::MyInt() -> [v: 111]\n");
        EXPECT_EQ((lifecycle_tracker<MyInt>::get_counters()), (lifecycle_counters{9, 16, 0, 3, 0, 18}));


        EXPECT_STDOUT_EQ(({ vec = std::move(vec_c); }), "~qs::MyInt() -> [v: 23]\n"
                                                        "~qs::MyInt() -> [v: 22]\n"
                                                        "~qs::MyInt() -> [v: 17]\n"
                                                        "~qs::MyInt() -> [v: 10]\n"
                                                        "qs::MyInt(qs::MyInt const&) -> [v: 111]\n"
                                                        "qs::MyInt(qs::MyInt const&) -> [v: 112]\n"
                                                        "qs::MyInt(qs::MyInt const&) -> [v: 113]\n"
                                                        "~qs::MyInt() -> [v: 113]\n"
                                                        "~qs::MyInt() -> [v: 112]\n"
                                                        "~qs::MyInt() -> [v: 111]\n");
        EXPECT_EQ((lifecycle_tracker<MyInt>::get_counters()), (lifecycle_counters{9, 19, 0, 3, 0, 25}));


        testing::internal::CaptureStdout();
    }
    auto output = testing::internal::GetCapturedStdout();
    EXPECT_EQ(output, "~qs::MyInt() -> [v: 113]\n"
                      "~qs::MyInt() -> [v: 112]\n"
                      "~qs::MyInt() -> [v: 111]\n");
    EXPECT_EQ((lifecycle_tracker<MyInt>::get_counters()), (lifecycle_counters{9, 19, 0, 3, 0, 28}));


    lifecycle_tracker<MyInt>::reset_counters();
    EXPECT_EQ((lifecycle_tracker<MyInt>::get_counters()), (lifecycle_counters{}));
}

template<size_t Uuid>
struct lifecycle_logger<std::string, Uuid> : lifecycle_default_logger<std::string, Uuid>
{
    template<lifecycle_event Cnt>
    QS_CONSTEXPR17 void log_event(std::string const& self, std::string const& type_name) const
    {
        fmt::println(event_fmt_map_[static_cast<size_t>(Cnt)], type_name, self);
    }

private:
    static constexpr std::array<char const*, 6> event_fmt_map_{"{0}(...) -> {1:?}",   "{0}({0} const&) -> {1:?}",
                                                               "{0}({0}&&) -> {1:?}", "=({0} const&) -> {1:?}",
                                                               "=({0}&&) -> {1:?}",   "~{0}() -> {1:?}"};
};

TEST(LifetimeTracker, StdString)
{
    lifecycle_tracker_mt<std::string>::set_type_name("std::string");

    {
        std::vector<lifecycle_tracker_mt<std::string>> vec;
        vec.reserve(100);

        EXPECT_STDOUT_EQ(({ vec.emplace_back("abc"); }), "std::string(...) -> \"abc\"\n"
                                                         "std::string(std::string const&) -> \"abc\"\n"
                                                         "~std::string() -> \"abc\"\n");
        EXPECT_EQ((lifecycle_tracker_mt<std::string>::get_counters()), (lifecycle_counters{1, 1, 0, 0, 0, 1}));


        EXPECT_STDOUT_EQ(({ vec.emplace_back("def"); }), "std::string(...) -> \"def\"\n"
                                                         "std::string(std::string const&) -> \"def\"\n"
                                                         "~std::string() -> \"def\"\n");
        EXPECT_EQ((lifecycle_tracker_mt<std::string>::get_counters()), (lifecycle_counters{2, 2, 0, 0, 0, 2}));


        EXPECT_STDOUT_EQ(({ vec.insert(vec.cend(), {"22", "23", "24", "25"}); }),
                         "std::string(...) -> \"22\"\n"
                         "std::string(...) -> \"23\"\n"
                         "std::string(...) -> \"24\"\n"
                         "std::string(...) -> \"25\"\n"
                         "std::string(std::string const&) -> \"22\"\n"
                         "std::string(std::string const&) -> \"23\"\n"
                         "std::string(std::string const&) -> \"24\"\n"
                         "std::string(std::string const&) -> \"25\"\n"
                         "~std::string() -> \"25\"\n"
                         "~std::string() -> \"24\"\n"
                         "~std::string() -> \"23\"\n"
                         "~std::string() -> \"22\"\n");
        EXPECT_EQ((lifecycle_tracker_mt<std::string>::get_counters()), (lifecycle_counters{6, 6, 0, 0, 0, 6}));


        EXPECT_STDOUT_EQ(({ auto vec_c = vec; }), "std::string(std::string const&) -> \"abc\"\n"
                                                  "std::string(std::string const&) -> \"def\"\n"
                                                  "std::string(std::string const&) -> \"22\"\n"
                                                  "std::string(std::string const&) -> \"23\"\n"
                                                  "std::string(std::string const&) -> \"24\"\n"
                                                  "std::string(std::string const&) -> \"25\"\n"
                                                  "~std::string() -> \"25\"\n"
                                                  "~std::string() -> \"24\"\n"
                                                  "~std::string() -> \"23\"\n"
                                                  "~std::string() -> \"22\"\n"
                                                  "~std::string() -> \"def\"\n"
                                                  "~std::string() -> \"abc\"\n");
        EXPECT_EQ((lifecycle_tracker_mt<std::string>::get_counters()), (lifecycle_counters{6, 12, 0, 0, 0, 12}));


        IGNORE_STDOUT(auto vec_c = vec);
        EXPECT_EQ((lifecycle_tracker_mt<std::string>::get_counters()), (lifecycle_counters{6, 18, 0, 0, 0, 12}));


        EXPECT_STDOUT_EQ(({ vec_c.erase(vec_c.begin() + 4, vec_c.end()); }), "~std::string() -> \"25\"\n"
                                                                             "~std::string() -> \"24\"\n");
        EXPECT_EQ((lifecycle_tracker_mt<std::string>::get_counters()), (lifecycle_counters{6, 18, 0, 0, 0, 14}));


        EXPECT_STDOUT_EQ(
            ({ vec_c.assign({"aaaaaaaaaaaaaaaaaaaaaaa", "bbbbbbbbbbbbbbbbbbbbbbb", "ccccccccccccccccccccccc"}); }),
            "std::string(...) -> \"aaaaaaaaaaaaaaaaaaaaaaa\"\n"
            "std::string(...) -> \"bbbbbbbbbbbbbbbbbbbbbbb\"\n"
            "std::string(...) -> \"ccccccccccccccccccccccc\"\n"
            "=(std::string const&) -> \"aaaaaaaaaaaaaaaaaaaaaaa\"\n"
            "=(std::string const&) -> \"bbbbbbbbbbbbbbbbbbbbbbb\"\n"
            "=(std::string const&) -> \"ccccccccccccccccccccccc\"\n"
            "~std::string() -> \"23\"\n"
            "~std::string() -> \"ccccccccccccccccccccccc\"\n"
            "~std::string() -> \"bbbbbbbbbbbbbbbbbbbbbbb\"\n"
            "~std::string() -> \"aaaaaaaaaaaaaaaaaaaaaaa\"\n");
        EXPECT_EQ((lifecycle_tracker_mt<std::string>::get_counters()), (lifecycle_counters{9, 18, 0, 3, 0, 18}));


        EXPECT_STDOUT_EQ(({ vec = std::move(vec_c); }),
                         "~std::string() -> \"25\"\n"
                         "~std::string() -> \"24\"\n"
                         "~std::string() -> \"23\"\n"
                         "~std::string() -> \"22\"\n"
                         "~std::string() -> \"def\"\n"
                         "~std::string() -> \"abc\"\n"
                         "std::string(std::string const&) -> \"aaaaaaaaaaaaaaaaaaaaaaa\"\n"
                         "std::string(std::string const&) -> \"bbbbbbbbbbbbbbbbbbbbbbb\"\n"
                         "std::string(std::string const&) -> \"ccccccccccccccccccccccc\"\n"
                         "~std::string() -> \"ccccccccccccccccccccccc\"\n"
                         "~std::string() -> \"bbbbbbbbbbbbbbbbbbbbbbb\"\n"
                         "~std::string() -> \"aaaaaaaaaaaaaaaaaaaaaaa\"\n");
        EXPECT_EQ((lifecycle_tracker_mt<std::string>::get_counters()), (lifecycle_counters{9, 21, 0, 3, 0, 27}));


        testing::internal::CaptureStdout();
    }
    auto output = testing::internal::GetCapturedStdout();
    EXPECT_EQ(output, "~std::string() -> \"ccccccccccccccccccccccc\"\n"
                      "~std::string() -> \"bbbbbbbbbbbbbbbbbbbbbbb\"\n"
                      "~std::string() -> \"aaaaaaaaaaaaaaaaaaaaaaa\"\n");
    EXPECT_EQ((lifecycle_tracker_mt<std::string>::get_counters()), (lifecycle_counters{9, 21, 0, 3, 0, 30}));


    lifecycle_tracker_mt<std::string>::reset_counters();
    EXPECT_EQ((lifecycle_tracker_mt<std::string>::get_counters()), (lifecycle_counters{}));
}


QS_NAMESPACE_END
