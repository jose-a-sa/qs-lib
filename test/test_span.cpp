#include "test/test_header.h"

#include <initializer_list>
#include <qs/config.h>
#include <qs/span.h>
#include "gmock/gmock.h"

#include <vector>


QS_NAMESPACE_BEGIN

namespace test
{
    TEST(Span, CTor_IterAndSize)
    {
        std::vector<int> vec{1, 2, 3};

        auto matcher = testing::HasSubstr("size mismatch in span's constructor (iterator, len)");
        EXPECT_DEBUG_DEATH(({ span<int, 3>{(int*)nullptr, 1}; }), matcher);
        EXPECT_DEBUG_DEATH(({ span<int, 3>{vec.data(), 1}; }), matcher);

        // EXPECT_THROW((span<int, 3>((int*)nullptr, 1)), std::logic_error);
        // EXPECT_THROW((span<int, 3>{vec.data(), 1}), std::logic_error);
        
        span<int, 3> s{vec};
        EXPECT_THAT(s, testing::Pointwise(testing::Eq(), {1, 2, 3}));
    }

    TEST(Span, CTor_IterAndSentinel)
    {
        std::vector<int> vec{1, 2, 3};

        auto matcher =
            testing::HasSubstr("invalid range in span's constructor (iterator, sentinel): last - first != extent");

        EXPECT_DEBUG_DEATH(({ span<int, 3>{(int*)nullptr, (int*)nullptr}; }), matcher);
        EXPECT_DEBUG_DEATH(({ span<int, 3>{vec.begin(), vec.end() + 1}; }), matcher);

        // EXPECT_THROW((span<int, 3>{(int*)nullptr, (int*)nullptr}), std::logic_error);
        // EXPECT_THROW((span<int, 3>{vec.data(), vec.data() + 1}), std::logic_error);

        span s{vec};
        EXPECT_THAT(s, testing::Pointwise(testing::Eq(), {1, 2, 3}));
    }

    TEST(Span, First)
    {
        auto s = span<int const, 3>({1, 2, 3});

        auto matcher1 = testing::HasSubstr("span<T, N>::first(count): count out of range");
        EXPECT_DEBUG_DEATH(({ s.first(-1); }), matcher1);
        EXPECT_DEBUG_DEATH(({ s.first(4); }), matcher1);

        std::vector<int> vec{1, 2, 3};
        auto             s2 = span(vec);

        EXPECT_DEBUG_DEATH(({ s2.first<4>(); }), testing::HasSubstr("span<T>::first<Count>(): Count out of range"));
        auto matcher2 = testing::HasSubstr("span<T>::first(count): count out of range");
        EXPECT_DEBUG_DEATH(({ s2.first(-1); }), matcher2);
        EXPECT_DEBUG_DEATH(({ s2.first(4); }), matcher2);

        // EXPECT_THROW((s.first<4>()), std::logic_error);
        // EXPECT_THROW((s.first(-1)), std::logic_error);
        // EXPECT_THROW((s.first(4)), std::logic_error);
    }

    TEST(Span, Last)
    {
        auto s = span<int const, 3>({1, 2, 3});

        auto matcher1 = testing::HasSubstr("span<T, N>::last(count): count out of range");
        EXPECT_DEBUG_DEATH(({ s.last(-1); }), matcher1);
        EXPECT_DEBUG_DEATH(({ s.last(4); }), matcher1);

        std::vector<int> vec{1, 2, 3};
        auto             s2 = span(vec);

        EXPECT_DEBUG_DEATH(({ s2.last<4>(); }), testing::HasSubstr("span<T>::last<Count>(): Count out of range"));
        auto matcher2 = testing::HasSubstr("span<T>::last(count): count out of range");
        EXPECT_DEBUG_DEATH(({ s2.last(-1); }), matcher2);
        EXPECT_DEBUG_DEATH(({ s2.last(4); }), matcher2);

        // EXPECT_THROW((s.last<4>()), std::logic_error);
        // EXPECT_THROW((s.last(-1)), std::logic_error);
        // EXPECT_THROW((s.last(4)), std::logic_error);
    }

    TEST(Span, Subspan_Offset)
    {
        auto il = {1, 2, 3, 4, 5};
        auto s = span<int const, 5>(il);

        auto matcher1 = testing::HasSubstr("span<T, N>::subspan(offset, count): offset out of range");
        EXPECT_DEBUG_DEATH(({ s.subspan(-1); }), matcher1);
        EXPECT_DEBUG_DEATH(({ s.subspan(6); }), matcher1);

        std::vector<int> vec{1, 2, 3, 4, 5};
        auto             s2 = span(vec);

        EXPECT_DEBUG_DEATH(({ s2.subspan<6>(); }), testing::HasSubstr("span<T>::subspan<Offset, Count>(): Offset out of range"));
        auto matcher2 = testing::HasSubstr("span<T>::subspan(offset, count): offset out of range");
        EXPECT_DEBUG_DEATH(({ s2.subspan(-1); }), matcher2);
        EXPECT_DEBUG_DEATH(({ s2.subspan(6); }), matcher2);

        // EXPECT_THROW((s.subspan<6>()), std::logic_error);
        // EXPECT_THROW((s.subspan(-1)), std::logic_error);
        // EXPECT_THROW((s.subspan(6)), std::logic_error);
    }

    TEST(Span, Subspan_OffsetAndCount)
    {
        auto il = {1, 2, 3, 4, 5};
        auto s = span<int const, 5>(il);

        auto matcher1 = testing::HasSubstr("span<T, N>::subspan(offset, count): count out of range");
        EXPECT_DEBUG_DEATH(({ s.subspan(0,-2); }), matcher1);
        EXPECT_DEBUG_DEATH(({ s.subspan(0, 6); }), matcher1);

        EXPECT_THAT(s.subspan(1,3), testing::Pointwise(testing::Eq(), {2, 3, 4}));
        EXPECT_THAT(s.subspan(2,3), testing::Pointwise(testing::Eq(), {3, 4, 5}));

        std::vector<int> vec{1, 2, 3, 4, 5};
        auto             s2 = span(vec);

        EXPECT_DEBUG_DEATH(({ s2.subspan<0, 6>(); }), testing::HasSubstr("span<T>::subspan<Offset, Count>(): Offset + Count out of range"));
        auto matcher2 = testing::HasSubstr("span<T>::subspan(offset, count): count out of range");
        EXPECT_DEBUG_DEATH(({ s2.subspan(0,-2); }), matcher2);
        EXPECT_DEBUG_DEATH(({ s2.subspan(0, 6); }), matcher2);

        EXPECT_THAT(s2.subspan(1,3), testing::Pointwise(testing::Eq(), {2, 3, 4}));
        EXPECT_THAT(s2.subspan(2,3), testing::Pointwise(testing::Eq(), {3, 4, 5}));

        // EXPECT_THROW((s.subspan<0, 6>()), std::logic_error);
        // EXPECT_THROW((s.subspan(0, -2)), std::logic_error);
        // EXPECT_THROW((s.subspan(0, 6)), std::logic_error);
    }

    TEST(Span, OperatorSubscript)
    {
        auto il = {1, 2, 3};
        auto s = span<int const, 3>(il);

        auto matcher1 = testing::HasSubstr("span<T, N>::operator[](index): index out of range");
        EXPECT_DEBUG_DEATH(({ s[-2]; }), matcher1);
        EXPECT_DEBUG_DEATH(({ s[42]; }), matcher1);

        EXPECT_EQ(s[0], 1);
        EXPECT_EQ(s[1], 2);
        EXPECT_EQ(s[2], 3);

        std::vector<int> vec{1, 2, 3};
        auto             s2 = span(vec);

        EXPECT_EQ(s2[0], 1);
        EXPECT_EQ(s2[1], 2);
        EXPECT_EQ(s2[2], 3);

        auto matcher2 = testing::HasSubstr("span<T>::operator[](index): index out of range");
        EXPECT_DEBUG_DEATH(({ s2[-2]; }), matcher2);
        EXPECT_DEBUG_DEATH(({ s2[42]; }), matcher2);
        
        // EXPECT_THROW((s[-2]), std::logic_error);
        // EXPECT_THROW((s[42]), std::logic_error);
    }

    TEST(Span, FrontAndBack)
    {
        span<int, 0> const s{};

        EXPECT_DEBUG_DEATH(({ s.front(); }), testing::HasSubstr("span<T, N>::front() called on empty span"));
        EXPECT_DEBUG_DEATH(({ s.back(); }), testing::HasSubstr("span<T, N>::back() called on empty span"));

        span<int> const s2{};

        EXPECT_DEBUG_DEATH(({ s2.front(); }), testing::HasSubstr("span<T>::front() called on empty span"));
        EXPECT_DEBUG_DEATH(({ s2.back(); }), testing::HasSubstr("span<T>::back() called on empty span"));

        // EXPECT_THROW((s.front()), std::logic_error);
        // EXPECT_THROW((s.back()), std::logic_error);
    }


} // namespace test

QS_NAMESPACE_END
