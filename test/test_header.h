#ifndef QS_TEST_HEADER_H
#define QS_TEST_HEADER_H

#include <qs/config.h>

#ifdef QS_TESTING

#define QS_FORWARD_TEST(test_case_name, test_name)                                                                     \
    namespace qs::tests                                                                                                \
    {                                                                                                                  \
        class test_case_name##_##test_name##_Test;                                                                     \
    }

#define QS_FRIEND_TEST(test_case_name, test_name) friend class ::qs::tests::test_case_name##_##test_name##_Test;

#include <gmock/gmock.h>

#define EXPECT_STDOUT_EQ(expression, expected)                                                                         \
    do                                                                                                                 \
    {                                                                                                                  \
        testing::internal::CaptureStdout();                                                                            \
        do                                                                                                             \
        {                                                                                                              \
            expression;                                                                                                \
        }                                                                                                              \
        while(0);                                                                                                      \
        auto const output = testing::internal::GetCapturedStdout();                                                    \
        EXPECT_EQ(output, expected);                                                                                   \
    }                                                                                                                  \
    while(0);

#else

#define QS_FRIEND_TEST(...)
#define QS_FORWARD_TEST(...)

#endif

#endif // QS_TEST_HEADER_H
