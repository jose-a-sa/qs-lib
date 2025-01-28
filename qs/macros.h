#ifndef QS_MACROS_H_
#define QS_MACROS_H_

// Stringify
#define QS_STRINGIFY(v) QS_STRINGIFY_(v)
#define QS_STRINGIFY_(v) #v

// Concatenation)
#define QS_CONCAT(A, B) QS_CONCAT_(A, B)
#define QS_CONCAT_(A, B) A##B

#define QS_EXPAND(...) __VA_ARGS__

#define QS_NARGS(...) _QS_NARGS_IMPL(__VA_ARGS__, 32, 31, 30, 29, 28, 27, 26, 25, 24, 23, 22, 21, 20, 19, 18, 17, 16, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0)
#define _QS_NARGS_IMPL(_32, _31, _30, _29, _28, _27, _26, _25, _24, _23, _22, _21, _20, _19, _18, _17, _16, _15, _14, _13, _12, _11, _10, _9, _8, _7, _6, _5, _4, _3, _2, _1, N, ...) N

// count arguments minus 1
#define QS_NARGS_MINUS_ONE(...) _QS_NARGS_MINUS_ONE_IMPL(__VA_ARGS__, 31, 30, 29, 28, 27, 26, 25, 24, 23, 22, 21, 20, 19, 18, 17, 16, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0)
#define _QS_NARGS_MINUS_ONE_IMPL(_32, _31, _30, _29, _28, _27, _26, _25, _24, _23, _22, _21, _20, _19, _18, _17, _16, _15, _14, _13, _12, _11, _10, _9, _8, _7, _6, _5, _4, _3, _2, _1, N, ...) N

#define QS_MAKE_INDEX_SEQUENCE(N) QS_CONCAT(_QS_INDEX_SEQUENCE_IMPL_, N)
#define _QS_INDEX_SEQUENCE_IMPL_0
#define _QS_INDEX_SEQUENCE_IMPL_1 0
#define _QS_INDEX_SEQUENCE_IMPL_2 _QS_INDEX_SEQUENCE_IMPL_1, 1
#define _QS_INDEX_SEQUENCE_IMPL_3 _QS_INDEX_SEQUENCE_IMPL_2, 2
#define _QS_INDEX_SEQUENCE_IMPL_4 _QS_INDEX_SEQUENCE_IMPL_3, 3
#define _QS_INDEX_SEQUENCE_IMPL_5 _QS_INDEX_SEQUENCE_IMPL_4, 4
#define _QS_INDEX_SEQUENCE_IMPL_6 _QS_INDEX_SEQUENCE_IMPL_5, 5
#define _QS_INDEX_SEQUENCE_IMPL_7 _QS_INDEX_SEQUENCE_IMPL_6, 6
#define _QS_INDEX_SEQUENCE_IMPL_8 _QS_INDEX_SEQUENCE_IMPL_7, 7
#define _QS_INDEX_SEQUENCE_IMPL_9 _QS_INDEX_SEQUENCE_IMPL_8, 8
#define _QS_INDEX_SEQUENCE_IMPL_10 _QS_INDEX_SEQUENCE_IMPL_9, 9
#define _QS_INDEX_SEQUENCE_IMPL_11 _QS_INDEX_SEQUENCE_IMPL_10, 10
#define _QS_INDEX_SEQUENCE_IMPL_12 _QS_INDEX_SEQUENCE_IMPL_11, 11
#define _QS_INDEX_SEQUENCE_IMPL_13 _QS_INDEX_SEQUENCE_IMPL_12, 12
#define _QS_INDEX_SEQUENCE_IMPL_14 _QS_INDEX_SEQUENCE_IMPL_13, 13
#define _QS_INDEX_SEQUENCE_IMPL_15 _QS_INDEX_SEQUENCE_IMPL_14, 14
#define _QS_INDEX_SEQUENCE_IMPL_16 _QS_INDEX_SEQUENCE_IMPL_15, 15
#define _QS_INDEX_SEQUENCE_IMPL_17 _QS_INDEX_SEQUENCE_IMPL_16, 16
#define _QS_INDEX_SEQUENCE_IMPL_18 _QS_INDEX_SEQUENCE_IMPL_17, 17
#define _QS_INDEX_SEQUENCE_IMPL_19 _QS_INDEX_SEQUENCE_IMPL_18, 18
#define _QS_INDEX_SEQUENCE_IMPL_20 _QS_INDEX_SEQUENCE_IMPL_19, 19
#define _QS_INDEX_SEQUENCE_IMPL_21 _QS_INDEX_SEQUENCE_IMPL_20, 20
#define _QS_INDEX_SEQUENCE_IMPL_22 _QS_INDEX_SEQUENCE_IMPL_21, 21
#define _QS_INDEX_SEQUENCE_IMPL_23 _QS_INDEX_SEQUENCE_IMPL_22, 22
#define _QS_INDEX_SEQUENCE_IMPL_24 _QS_INDEX_SEQUENCE_IMPL_23, 23
#define _QS_INDEX_SEQUENCE_IMPL_25 _QS_INDEX_SEQUENCE_IMPL_24, 24
#define _QS_INDEX_SEQUENCE_IMPL_26 _QS_INDEX_SEQUENCE_IMPL_25, 25
#define _QS_INDEX_SEQUENCE_IMPL_27 _QS_INDEX_SEQUENCE_IMPL_26, 26
#define _QS_INDEX_SEQUENCE_IMPL_28 _QS_INDEX_SEQUENCE_IMPL_27, 27
#define _QS_INDEX_SEQUENCE_IMPL_29 _QS_INDEX_SEQUENCE_IMPL_28, 28
#define _QS_INDEX_SEQUENCE_IMPL_30 _QS_INDEX_SEQUENCE_IMPL_29, 29
#define _QS_INDEX_SEQUENCE_IMPL_31 _QS_INDEX_SEQUENCE_IMPL_30, 30
#define _QS_INDEX_SEQUENCE_IMPL_32 _QS_INDEX_SEQUENCE_IMPL_31, 31

#define QS_CONCAT_IF_ONE_ARG(A, B, ...) QS_CONCAT(_QS_CONCAT_IF_ONE_ARG_, QS_NARGS(__VA_ARGS__))(A, B, __VA_ARGS__)
#define _QS_CONCAT_IF_ONE_ARG_1(A, B, ...) QS_CONCAT(A, B)
#define _QS_CONCAT_IF_ONE_ARG_2(A, ...) A
#define _QS_CONCAT_IF_ONE_ARG_3(A, ...) A
#define _QS_CONCAT_IF_ONE_ARG_4(A, ...) A
#define _QS_CONCAT_IF_ONE_ARG_5(A, ...) A
#define _QS_CONCAT_IF_ONE_ARG_6(A, ...) A
#define _QS_CONCAT_IF_ONE_ARG_7(A, ...) A
#define _QS_CONCAT_IF_ONE_ARG_8(A, ...) A
#define _QS_CONCAT_IF_ONE_ARG_9(A, ...) A
#define _QS_CONCAT_IF_ONE_ARG_10(A, ...) A
#define _QS_CONCAT_IF_ONE_ARG_11(A, ...) A
#define _QS_CONCAT_IF_ONE_ARG_12(A, ...) A
#define _QS_CONCAT_IF_ONE_ARG_13(A, ...) A
#define _QS_CONCAT_IF_ONE_ARG_14(A, ...) A
#define _QS_CONCAT_IF_ONE_ARG_15(A, ...) A
#define _QS_CONCAT_IF_ONE_ARG_16(A, ...) A
#define _QS_CONCAT_IF_ONE_ARG_17(A, ...) A
#define _QS_CONCAT_IF_ONE_ARG_18(A, ...) A
#define _QS_CONCAT_IF_ONE_ARG_19(A, ...) A
#define _QS_CONCAT_IF_ONE_ARG_20(A, ...) A
#define _QS_CONCAT_IF_ONE_ARG_21(A, ...) A
#define _QS_CONCAT_IF_ONE_ARG_22(A, ...) A
#define _QS_CONCAT_IF_ONE_ARG_23(A, ...) A
#define _QS_CONCAT_IF_ONE_ARG_24(A, ...) A
#define _QS_CONCAT_IF_ONE_ARG_25(A, ...) A
#define _QS_CONCAT_IF_ONE_ARG_26(A, ...) A
#define _QS_CONCAT_IF_ONE_ARG_27(A, ...) A
#define _QS_CONCAT_IF_ONE_ARG_28(A, ...) A
#define _QS_CONCAT_IF_ONE_ARG_29(A, ...) A
#define _QS_CONCAT_IF_ONE_ARG_30(A, ...) A
#define _QS_CONCAT_IF_ONE_ARG_31(A, ...) A
#define _QS_CONCAT_IF_ONE_ARG_32(A, ...) A


// get n-th argument
#define QS_GET_ARG(N, ...) QS_CONCAT(_QS_GET_ARG_, N)(__VA_ARGS__)
#define _QS_GET_ARG_0(_0, ...) _0
#define _QS_GET_ARG_1(_0, _1, ...) _1
#define _QS_GET_ARG_2(_0, _1, _2, ...) _2
#define _QS_GET_ARG_3(_0, _1, _2, _3, ...) _3
#define _QS_GET_ARG_4(_0, _1, _2, _3, _4, ...) _4
#define _QS_GET_ARG_5(_0, _1, _2, _3, _4, _5, ...) _5
#define _QS_GET_ARG_6(_0, _1, _2, _3, _4, _5, _6, ...) _6
#define _QS_GET_ARG_7(_0, _1, _2, _3, _4, _5, _6, _7, ...) _7
#define _QS_GET_ARG_8(_0, _1, _2, _3, _4, _5, _6, _7, _8, ...) _8
#define _QS_GET_ARG_9(_0, _1, _2, _3, _4, _5, _6, _7, _8, _9, ...) _9
#define _QS_GET_ARG_10(_0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, ...) _10
#define _QS_GET_ARG_11(_0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, ...) _11
#define _QS_GET_ARG_12(_0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, ...) _12
#define _QS_GET_ARG_13(_0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, ...) _13
#define _QS_GET_ARG_14(_0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, ...) _14
#define _QS_GET_ARG_15(_0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, ...) _15
#define _QS_GET_ARG_16(_0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, ...) _16
#define _QS_GET_ARG_17(_0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, _17, ...) _17
#define _QS_GET_ARG_18(_0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, _17, _18, ...) _18
#define _QS_GET_ARG_19(_0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, _17, _18, _19, ...) _19
#define _QS_GET_ARG_20(_0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, _17, _18, _19, _20, ...) _20
#define _QS_GET_ARG_21(_0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, _17, _18, _19, _20, _21, ...) _21
#define _QS_GET_ARG_22(_0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, _17, _18, _19, _20, _21, _22, ...) _22
#define _QS_GET_ARG_23(_0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, _17, _18, _19, _20, _21, _22, _23, ...) _23
#define _QS_GET_ARG_24(_0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, _17, _18, _19, _20, _21, _22, _23, _24, ...) _24
#define _QS_GET_ARG_25(_0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, _17, _18, _19, _20, _21, _22, _23, _24, _25, ...) _25
#define _QS_GET_ARG_26(_0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, _17, _18, _19, _20, _21, _22, _23, _24, _25, _26, ...) _26
#define _QS_GET_ARG_27(_0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, _17, _18, _19, _20, _21, _22, _23, _24, _25, _26, _27, ...) _27
#define _QS_GET_ARG_28(_0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, _17, _18, _19, _20, _21, _22, _23, _24, _25, _26, _27, _28, ...) _28
#define _QS_GET_ARG_29(_0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, _17, _18, _19, _20, _21, _22, _23, _24, _25, _26, _27, _28, _29, ...) _29
#define _QS_GET_ARG_30(_0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, _17, _18, _19, _20, _21, _22, _23, _24, _25, _26, _27, _28, _29, _30, ...) _30
#define _QS_GET_ARG_31(_0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, _17, _18, _19, _20, _21, _22, _23, _24, _25, _26, _27, _28, _29, _30, _31, ...) _31
#define _QS_GET_ARG_32(_0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, _17, _18, _19, _20, _21, _22, _23, _24, _25, _26, _27, _28, _29, _30, _31, _32, ...) _32

// Get last argument - placeholder decrements by one
#define QS_GET_ARGS_LAST(...) QS_GET_ARG(QS_NARGS(__VA_ARGS__), _, __VA_ARGS__ ,,,,,,,,,,,)

// Get at most the first N arguments
#define QS_GET_ARGS_TAKE_LEFT(N, ...) QS_CONCAT(_QS_GET_ARGS_TAKE_LEFT_, N)(__VA_ARGS__)
#define _QS_GET_ARGS_TAKE_LEFT_0(...)
#define _QS_GET_ARGS_TAKE_LEFT_1(_0, ...) _0
#define _QS_GET_ARGS_TAKE_LEFT_2(_0, _1, ...) _0, _1
#define _QS_GET_ARGS_TAKE_LEFT_3(_0, _1, _2, ...) _0, _1, _2
#define _QS_GET_ARGS_TAKE_LEFT_4(_0, _1, _2, _3, ...) _0, _1, _2, _3
#define _QS_GET_ARGS_TAKE_LEFT_5(_0, _1, _2, _3, _4, ...) _0, _1, _2, _3, _4
#define _QS_GET_ARGS_TAKE_LEFT_6(_0, _1, _2, _3, _4, _5, ...) _0, _1, _2, _3, _4, _5
#define _QS_GET_ARGS_TAKE_LEFT_7(_0, _1, _2, _3, _4, _5, _6, ...) _0, _1, _2, _3, _4, _5, _6
#define _QS_GET_ARGS_TAKE_LEFT_8(_0, _1, _2, _3, _4, _5, _6, _7, ...) _0, _1, _2, _3, _4, _5, _6, _7
#define _QS_GET_ARGS_TAKE_LEFT_9(_0, _1, _2, _3, _4, _5, _6, _7, _8, ...) _0, _1, _2, _3, _4, _5, _6, _7, _8
#define _QS_GET_ARGS_TAKE_LEFT_10(_0, _1, _2, _3, _4, _5, _6, _7, _8, _9, ...) _0, _1, _2, _3, _4, _5, _6, _7, _8, _9
#define _QS_GET_ARGS_TAKE_LEFT_11(_0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, ...) _0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10
#define _QS_GET_ARGS_TAKE_LEFT_12(_0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, ...) _0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11
#define _QS_GET_ARGS_TAKE_LEFT_13(_0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, ...) _0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12
#define _QS_GET_ARGS_TAKE_LEFT_14(_0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, ...) _0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13
#define _QS_GET_ARGS_TAKE_LEFT_15(_0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, ...) _0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14
#define _QS_GET_ARGS_TAKE_LEFT_16(_0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, ...) _0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15
#define _QS_GET_ARGS_TAKE_LEFT_17(_0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, ...) _0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16
#define _QS_GET_ARGS_TAKE_LEFT_18(_0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, _17, ...) _0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, _17
#define _QS_GET_ARGS_TAKE_LEFT_19(_0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, _17, _18, _19, ...) _0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, _17, _18
#define _QS_GET_ARGS_TAKE_LEFT_20(_0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, _17, _18, _19, _20, ...) _0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, _17, _18, _19
#define _QS_GET_ARGS_TAKE_LEFT_21(_0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, _17, _18, _19, _20, _21, ...) _0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, _17, _18, _19, _20
#define _QS_GET_ARGS_TAKE_LEFT_22(_0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, _17, _18, _19, _20, _21, _22, ...) _0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, _17, _18, _19, _20, _21
#define _QS_GET_ARGS_TAKE_LEFT_23(_0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, _17, _18, _19, _20, _21, _22, _23, ...) _0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, _17, _18, _19, _20, _21, _22
#define _QS_GET_ARGS_TAKE_LEFT_24(_0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, _17, _18, _19, _20, _21, _22, _23, _24, ...) _0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, _17, _18, _19, _20, _21, _22, _23
#define _QS_GET_ARGS_TAKE_LEFT_25(_0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, _17, _18, _19, _20, _21, _22, _23, _24, _25, ...) _0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, _17, _18, _19, _20, _21, _22, _23, _24
#define _QS_GET_ARGS_TAKE_LEFT_26(_0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, _17, _18, _19, _20, _21, _22, _23, _24, _25, _26, ...) _0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, _17, _18, _19, _20, _21, _22, _23, _24, _25
#define _QS_GET_ARGS_TAKE_LEFT_27(_0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, _17, _18, _19, _20, _21, _22, _23, _24, _25, _26, _27, ...) _0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, _17, _18, _19, _20, _21, _22, _23, _24, _25, _26
#define _QS_GET_ARGS_TAKE_LEFT_28(_0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, _17, _18, _19, _20, _21, _22, _23, _24, _25, _26, _27, _28, ...) _0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, _17, _18, _19, _20, _21, _22, _23, _24, _25, _26, _27
#define _QS_GET_ARGS_TAKE_LEFT_29(_0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, _17, _18, _19, _20, _21, _22, _23, _24, _25, _26, _27, _28, _29, ...) _0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, _17, _18, _19, _20, _21, _22, _23, _24, _25, _26, _27, _28
#define _QS_GET_ARGS_TAKE_LEFT_30(_0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, _17, _18, _19, _20, _21, _22, _23, _24, _25, _26, _27, _28, _29, _30, ...) _0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, _17, _18, _19, _20, _21, _22, _23, _24, _25, _26, _27, _28, _29
#define _QS_GET_ARGS_TAKE_LEFT_31(_0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, _17, _18, _19, _20, _21, _22, _23, _24, _25, _26, _27, _28, _29, _30, _31, ...) _0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, _17, _18, _19, _20, _21, _22, _23, _24, _25, _26, _27, _28, _29, _30
#define _QS_GET_ARGS_TAKE_LEFT_32(_0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, _17, _18, _19, _20, _21, _22, _23, _24, _25, _26, _27, _28, _29, _30, _31, _32, ...) _0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, _17, _18, _19, _20, _21, _22, _23, _24, _25, _26, _27, _28, _29, _30, _31

#define QS_GET_ARGS_MOST(...) QS_GET_ARGS_TAKE_LEFT(QS_NARGS_MINUS_ONE(__VA_ARGS__), __VA_ARGS__)

#define QS_APPLY_WITH_INDEX(F, INDEX, X) F(X, INDEX)
// Iterative macros to handle variadic arguments
#define QS_FOR_EACH(F, ...) _QS_FOR_EACH_IMPL(F, __VA_ARGS__) 
#define _QS_FOR_EACH_IMPL_SENTINEL(F, X) QS_APPLY_WITH_INDEX(F, 0, X)
#define _QS_FOR_EACH_IMPL(F, ...) \
    QS_CONCAT_IF_ONE_ARG(_QS_FOR_EACH_IMPL, _SENTINEL, QS_GET_ARGS_MOST(__VA_ARGS__))(F, QS_GET_ARGS_MOST(__VA_ARGS__)) \
    QS_APPLY_WITH_INDEX(F, QS_NARGS_MINUS_ONE(__VA_ARGS__), QS_GET_ARGS_LAST(__VA_ARGS__))
// Iterative macros to handle variadic arguments with a separator
#define QS_FOR_EACH_WITH_COMMA(F, ...) _QS_FOR_EACH_WITH_COMMA_IMPL(F, __VA_ARGS__) 
#define _QS_FOR_EACH_WITH_COMMA_IMPL_SENTINEL(F, X) F(0, X)
#define _QS_FOR_EACH_WITH_COMMA_IMPL(F, ...) \
    QS_CONCAT_IF_ONE_ARG(_QS_FOR_EACH_WITH_COMMA_IMPL, _SENTINEL, QS_GET_ARGS_MOST(__VA_ARGS__))(F, QS_GET_ARGS_MOST(__VA_ARGS__))\
    , QS_APPLY_WITH_INDEX(F, QS_NARGS_MINUS_ONE(__VA_ARGS__), QS_GET_ARGS_LAST(__VA_ARGS__))
// Iterative macros to handle variadic arguments with a separator
#define QS_FOR_EACH_WITH_SEP(F, SEP, ...) _QS_FOR_EACH_WITH_SEP_IMPL(F, SEP, __VA_ARGS__) 
#define _QS_FOR_EACH_WITH_SEP_IMPL_SENTINEL(F, SEP, X) F(0, X)
#define _QS_FOR_EACH_WITH_SEP_IMPL(F, SEP, ...) \
    QS_CONCAT_IF_ONE_ARG(_QS_FOR_EACH_WITH_SEP_IMPL, _SENTINEL, QS_GET_ARGS_MOST(__VA_ARGS__))(F, SEP, QS_GET_ARGS_MOST(__VA_ARGS__)) \
    SEP QS_APPLY_WITH_INDEX(F, QS_NARGS_MINUS_ONE(__VA_ARGS__), QS_GET_ARGS_LAST(__VA_ARGS__))

#define QS_ARGS_REPEAT(N, ARG) QS_CONCAT(_QS_ARGS_REPEAT_IMPL_, N)(ARG)
#define _QS_ARGS_REPEAT_IMPL_0(ARG)
#define _QS_ARGS_REPEAT_IMPL_1(ARG) ARG
#define _QS_ARGS_REPEAT_IMPL_2(ARG) _QS_ARGS_REPEAT_IMPL_1(ARG), ARG
#define _QS_ARGS_REPEAT_IMPL_3(ARG) _QS_ARGS_REPEAT_IMPL_2(ARG), ARG
#define _QS_ARGS_REPEAT_IMPL_4(ARG) _QS_ARGS_REPEAT_IMPL_3(ARG), ARG
#define _QS_ARGS_REPEAT_IMPL_5(ARG) _QS_ARGS_REPEAT_IMPL_4(ARG), ARG
#define _QS_ARGS_REPEAT_IMPL_6(ARG) _QS_ARGS_REPEAT_IMPL_5(ARG), ARG
#define _QS_ARGS_REPEAT_IMPL_7(ARG) _QS_ARGS_REPEAT_IMPL_6(ARG), ARG
#define _QS_ARGS_REPEAT_IMPL_8(ARG) _QS_ARGS_REPEAT_IMPL_7(ARG), ARG
#define _QS_ARGS_REPEAT_IMPL_9(ARG) _QS_ARGS_REPEAT_IMPL_8(ARG), ARG
#define _QS_ARGS_REPEAT_IMPL_10(ARG) _QS_ARGS_REPEAT_IMPL_9(ARG), ARG
#define _QS_ARGS_REPEAT_IMPL_11(ARG) _QS_ARGS_REPEAT_IMPL_10(ARG), ARG
#define _QS_ARGS_REPEAT_IMPL_12(ARG) _QS_ARGS_REPEAT_IMPL_11(ARG), ARG
#define _QS_ARGS_REPEAT_IMPL_13(ARG) _QS_ARGS_REPEAT_IMPL_12(ARG), ARG
#define _QS_ARGS_REPEAT_IMPL_14(ARG) _QS_ARGS_REPEAT_IMPL_13(ARG), ARG
#define _QS_ARGS_REPEAT_IMPL_15(ARG) _QS_ARGS_REPEAT_IMPL_14(ARG), ARG
#define _QS_ARGS_REPEAT_IMPL_16(ARG) _QS_ARGS_REPEAT_IMPL_15(ARG), ARG
#define _QS_ARGS_REPEAT_IMPL_17(ARG) _QS_ARGS_REPEAT_IMPL_16(ARG), ARG
#define _QS_ARGS_REPEAT_IMPL_18(ARG) _QS_ARGS_REPEAT_IMPL_17(ARG), ARG
#define _QS_ARGS_REPEAT_IMPL_19(ARG) _QS_ARGS_REPEAT_IMPL_18(ARG), ARG
#define _QS_ARGS_REPEAT_IMPL_20(ARG) _QS_ARGS_REPEAT_IMPL_19(ARG), ARG
#define _QS_ARGS_REPEAT_IMPL_21(ARG) _QS_ARGS_REPEAT_IMPL_20(ARG), ARG
#define _QS_ARGS_REPEAT_IMPL_22(ARG) _QS_ARGS_REPEAT_IMPL_21(ARG), ARG
#define _QS_ARGS_REPEAT_IMPL_23(ARG) _QS_ARGS_REPEAT_IMPL_22(ARG), ARG
#define _QS_ARGS_REPEAT_IMPL_24(ARG) _QS_ARGS_REPEAT_IMPL_23(ARG), ARG
#define _QS_ARGS_REPEAT_IMPL_25(ARG) _QS_ARGS_REPEAT_IMPL_24(ARG), ARG
#define _QS_ARGS_REPEAT_IMPL_26(ARG) _QS_ARGS_REPEAT_IMPL_25(ARG), ARG
#define _QS_ARGS_REPEAT_IMPL_27(ARG) _QS_ARGS_REPEAT_IMPL_26(ARG), ARG
#define _QS_ARGS_REPEAT_IMPL_28(ARG) _QS_ARGS_REPEAT_IMPL_27(ARG), ARG
#define _QS_ARGS_REPEAT_IMPL_29(ARG) _QS_ARGS_REPEAT_IMPL_28(ARG), ARG
#define _QS_ARGS_REPEAT_IMPL_30(ARG) _QS_ARGS_REPEAT_IMPL_29(ARG), ARG
#define _QS_ARGS_REPEAT_IMPL_31(ARG) _QS_ARGS_REPEAT_IMPL_30(ARG), ARG
#define _QS_ARGS_REPEAT_IMPL_32(ARG) _QS_ARGS_REPEAT_IMPL_31(ARG), ARG



#endif //QS_MACROS_H_