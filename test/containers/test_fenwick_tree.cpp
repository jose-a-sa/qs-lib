#include "test/test_header.h"

#include <vector>
#include "qs/containers/fenwick_tree.h"


QS_NAMESPACE_BEGIN

namespace test
{

    static const auto lst1 = {-45,87,63,75,-37,-32,-29,-66,64,-84,-53,11,37,-81,-43,-87,-99,5,-3,-23,44,98,64,-42,38,-70,-46,83,17,-88,-28,18,38,0,-18,31,-21,41,-57,85,-75,-95,-34,97,0,-41,-24,52,-22,-53,50,-64,100,27,74,43,84,-22,74,19,38,29,-17,70,-40,-94,-6,23,-17,-80,-41,31,-99,-61,-18,80,-90,-53,-97,-15,-13,98,-46,43,76,88,-40,54,-9,-50,-31,-86,-12,-75,24,-23,73,3,54,58,67,56,-13,3,-3,100,85,83,-24,61,-100,72,15,73,81,2,-42,64,-34,-46,-93,56,8,50,-77,31,-71,70,84,76,18,63,-91,71,52,94,90,-49,-13,-6,-44,42,-54,38,18,-79,85,-29,59,76,1,39,33,14,93,63,28,-67,-92,52,18,48,75,-87,-11,68,42,-89,11,5,-10,32,-99,-8,89,42,-85,-73,22,91,87,89,96,-57,87,-33,93,69,-15,-34,-66,66,32,-6,83,46,24,5,35,-77};
    static const auto pre1 = {0,-45,42,105,180,143,111,82,16,80,-4,-57,-46,-9,-90,-133,-220,-319,-314,-317,-340,-296,-198,-134,-176,-138,-208,-254,-171,-154,-242,-270,-252,-214,-214,-232,-201,-222,-181,-238,-153,-228,-323,-357,-260,-260,-301,-325,-273,-295,-348,-298,-362,-262,-235,-161,-118,-34,-56,18,37,75,104,87,157,117,23,17,40,23,-57,-98,-67,-166,-227,-245,-165,-255,-308,-405,-420,-433,-335,-381,-338,-262,-174,-214,-160,-169,-219,-250,-336,-348,-423,-399,-422,-349,-346,-292,-234,-167,-111,-124,-121,-124,-24,61,144,120,181,81,153,168,241,322,324,282,346,312,266,173,229,237,287,210,241,170,240,324,400,418,481,390,461,513,607,697,648,635,629,585,627,573,611,629,550,635,606,665,741,742,781,814,828,921,984,1012,945,853,905,923,971,1046,959,948,1016,1058,969,980,985,975,1007,908,900,989,1031,946,873,895,986,1073,1162,1258,1201,1288,1255,1348,1417,1402,1368,1302,1368,1400,1394,1477,1523,1547,1552,1587,1510};

    TEST(FenwickTree, UpdateAndQuery)
    {
        for(size_t sz = 1; sz <= lst1.size(); ++sz)
        {
            std::vector<int> const vec(lst1.begin(), lst1.begin() + sz);
            std::vector<int> const prefix(pre1.begin(), pre1.begin() + sz + 1);

            FenwickTree<int> tree(vec.size());
            EXPECT_EQ(tree.size(), vec.size());

            for(size_t i = 0; i < vec.size(); ++i)
                tree.update(i, vec[i]);

            std::vector<int> res(tree.size() + 1);
            for(size_t i = 0; i <= tree.size(); ++i)
                res[i] = tree.query(i);
            EXPECT_EQ(res, prefix);
        }
    }

    TEST(FenwickTree, IteratorConstruction)
    {
        for(size_t sz = 1; sz <= 10; ++sz)
        {
            std::vector<int> const vec(lst1.begin(), lst1.begin() + sz);
            std::vector<int> const prefix(pre1.begin(), pre1.begin() + sz + 1);

            FenwickTree<int> tree(vec.begin(), vec.end());
            EXPECT_EQ(tree.size(), vec.size());

            std::vector<int> res(tree.size() + 1);
            for(size_t i = 0; i <= tree.size(); ++i)
                res[i] = tree.query(i);
            EXPECT_EQ(res, prefix);
        }
    }

    TEST(FenwickTree, Resize)
    {
        for(size_t sz = 1; sz <= lst1.size(); ++sz)
        {
            std::vector<int> const vec(lst1.begin(), lst1.begin() + sz);
            std::vector<int> const prefix(pre1.begin(), pre1.begin() + sz + 1);
            FenwickTree<int> tree(0);
            EXPECT_EQ(tree.size(), 0);

            for(size_t i = 0; i < vec.size(); ++i)
            {
                tree.resize(i + 1);
                tree.update(i, vec[i]);
            }

            std::vector<int> res(tree.size() + 1);
            for(size_t i = 0; i <= tree.size(); ++i)
                res[i] = tree.query(i);
            EXPECT_EQ(prefix, res);
        }
    }


#if defined (__cpp_deduction_guides)

    template<class TypeTest>
    struct FenwickTreeCTADTest
    {
        using container_type = std::vector<TypeTest>;
        using iterator_type = decltype(std::begin(std::declval<container_type>()));
        using sentinel_type = decltype(std::end(std::declval<container_type>()));
        using result_type = decltype(FenwickTree(std::declval<iterator_type>(), std::declval<sentinel_type>()));
        static constexpr bool value = std::is_same<result_type, FenwickTree<TypeTest>>::value;
    };

    static_assert(FenwickTreeCTADTest<int>::value, "FenwickTree<int> CTAD test failed");
    static_assert(FenwickTreeCTADTest<double>::value, "FenwickTree<double> CTAD test failed");
    static_assert(FenwickTreeCTADTest<float>::value, "FenwickTree<float> CTAD test failed");
    
#endif
} // namespace test

QS_NAMESPACE_END
