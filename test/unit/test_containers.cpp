#include <gtest/gtest.h>
#include "fakengine.h"

TEST(FArrayTest, BasicOperations) {
    farray<int, 5> arr;
    EXPECT_TRUE(arr.empty());
    EXPECT_EQ(arr.size(), 0u);

    EXPECT_TRUE(arr.push_back(10));
    EXPECT_TRUE(arr.push_back(20));
    EXPECT_TRUE(arr.push_back(30));
    EXPECT_EQ(arr.size(), 3u);

    int front_val = 0;
    int back_val = 0;
    EXPECT_TRUE(arr.front(front_val));
    EXPECT_TRUE(arr.back(back_val));
    EXPECT_EQ(front_val, 10);
    EXPECT_EQ(back_val, 30);

    EXPECT_EQ(arr[0], 10);
    EXPECT_EQ(arr[1], 20);
    EXPECT_EQ(arr[2], 30);

    int popped = 0;
    EXPECT_TRUE(arr.pop_back(popped));
    EXPECT_EQ(popped, 30);
    EXPECT_EQ(arr.size(), 2u);

    EXPECT_TRUE(arr.pop_front(popped));
    EXPECT_EQ(popped, 10);
    EXPECT_EQ(arr.size(), 1u);
}

TEST(FListTest, PushAndTraverse) {
    flist<int, 5> lst;
    EXPECT_TRUE(lst.empty());

    EXPECT_TRUE(lst.push_back(2));
    EXPECT_TRUE(lst.push_front(1));
    EXPECT_TRUE(lst.push_back(3));
    EXPECT_EQ(lst.size(), 3u);

    int val = 0;
    EXPECT_TRUE(lst.front(val));
    EXPECT_EQ(val, 1);
    EXPECT_TRUE(lst.back(val));
    EXPECT_EQ(val, 3);

    int expected[] = {1, 2, 3};
    int idx = 0;
    for (auto it = lst.begin(); it != lst.end(); ++it, ++idx) {
        EXPECT_EQ(*it, expected[idx]);
    }
}

TEST(FHashMapTest, InsertFindAndErase) {
    fhashmap<int, stringc, 16> map;
    EXPECT_TRUE(map.empty());

    map.insert(1, stringc("one"));
    map.insert(2, stringc("two"));
    map.insert(3, stringc("three"));
    EXPECT_EQ(map.size(), 3u);

    auto it = map.find(2);
    ASSERT_NE(it, map.end());
    EXPECT_STREQ((const char*)it->second.c_str(), "two");

    EXPECT_EQ(map.find(999), map.end());

    map.erase(2);
    EXPECT_EQ(map.size(), 2u);
    EXPECT_EQ(map.find(2), map.end());
}

TEST(FHashSetTest, InsertAndFind) {
    fhashset<int, 16> set;
    EXPECT_TRUE(set.empty());

    set.insert(42);
    set.insert(100);
    EXPECT_EQ(set.size(), 2u);

    EXPECT_NE(set.find(42), set.end());
    EXPECT_NE(set.find(100), set.end());
    EXPECT_EQ(set.find(7), set.end());

    set.erase(42);
    EXPECT_EQ(set.size(), 1u);
    EXPECT_EQ(set.find(42), set.end());
}

TEST(FPoolTest, AllocationAndFree) {
    fpool<int, 4> pool;
    EXPECT_EQ(pool.size(), 0u);

    int32_t idx1 = pool.allocindex();
    int32_t idx2 = pool.allocindex();
    int32_t idx3 = pool.allocindex();
    int32_t idx4 = pool.allocindex();

    EXPECT_NE(idx1, -1);
    EXPECT_NE(idx2, -1);
    EXPECT_NE(idx3, -1);
    EXPECT_NE(idx4, -1);
    EXPECT_EQ(pool.size(), 4u);

    // Pool full
    int32_t idx5 = pool.allocindex();
    EXPECT_EQ(idx5, -1);

    EXPECT_TRUE(pool.deallocindex(idx2));
    EXPECT_EQ(pool.size(), 3u);

    int32_t idx_reused = pool.allocindex();
    EXPECT_NE(idx_reused, -1);
    EXPECT_EQ(pool.size(), 4u);
}
