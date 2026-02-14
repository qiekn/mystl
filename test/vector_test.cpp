#include "vector.h"

#include <gtest/gtest.h>

#include <string>

// -- Constructor tests --

TEST(VectorTest, DefaultConstruct) {
  mystl::vector<int> v;
  EXPECT_EQ(v.size(), 0);
  EXPECT_EQ(v.capacity(), 0);
  EXPECT_TRUE(v.empty());
}

TEST(VectorTest, ConstructWithSize) {
  mystl::vector<int> v(5);
  EXPECT_EQ(v.size(), 5);
  for (size_t i = 0; i < 5; ++i)
    EXPECT_EQ(v[i], 0);
}

TEST(VectorTest, ConstructWithSizeAndValue) {
  mystl::vector<int> v(3, 42);
  EXPECT_EQ(v.size(), 3);
  for (size_t i = 0; i < 3; ++i)
    EXPECT_EQ(v[i], 42);
}

TEST(VectorTest, CopyConstruct) {
  mystl::vector<int> a(3, 7);
  mystl::vector<int> b(a);
  EXPECT_EQ(b.size(), 3);
  EXPECT_EQ(b[0], 7);
  // Modifying b should not affect a.
  b[0] = 99;
  EXPECT_EQ(a[0], 7);
}

TEST(VectorTest, MoveConstruct) {
  mystl::vector<std::string> a(2, "hello");
  mystl::vector<std::string> b(mystl::move(a));
  EXPECT_EQ(b.size(), 2);
  EXPECT_EQ(b[0], "hello");
  EXPECT_EQ(a.size(), 0);
  EXPECT_TRUE(a.empty());
}

// -- Assignment tests --

TEST(VectorTest, CopyAssign) {
  mystl::vector<int> a(3, 1);
  mystl::vector<int> b;
  b = a;
  EXPECT_EQ(b.size(), 3);
  EXPECT_EQ(b[0], 1);
}

TEST(VectorTest, MoveAssign) {
  mystl::vector<int> a(3, 1);
  mystl::vector<int> b;
  b = mystl::move(a);
  EXPECT_EQ(b.size(), 3);
  EXPECT_EQ(a.size(), 0);
}

// -- Element access tests --

TEST(VectorTest, At) {
  mystl::vector<int> v(3, 0);
  v[1] = 42;
  EXPECT_EQ(v.at(1), 42);
  EXPECT_THROW(v.at(3), std::out_of_range);
}

TEST(VectorTest, FrontAndBack) {
  mystl::vector<int> v;
  v.push_back(1);
  v.push_back(2);
  v.push_back(3);
  EXPECT_EQ(v.front(), 1);
  EXPECT_EQ(v.back(), 3);
}

TEST(VectorTest, Data) {
  mystl::vector<int> v(3, 5);
  int* p = v.data();
  EXPECT_EQ(p[0], 5);
  EXPECT_EQ(p[2], 5);
}

// -- Iterator tests --

TEST(VectorTest, BeginEnd) {
  mystl::vector<int> v;
  v.push_back(10);
  v.push_back(20);
  v.push_back(30);

  int sum = 0;
  for (auto it = v.begin(); it != v.end(); ++it)
    sum += *it;
  EXPECT_EQ(sum, 60);
}

TEST(VectorTest, RangeFor) {
  mystl::vector<int> v;
  v.push_back(1);
  v.push_back(2);
  v.push_back(3);

  int sum = 0;
  for (int x : v)
    sum += x;
  EXPECT_EQ(sum, 6);
}

// -- Capacity tests --

TEST(VectorTest, Reserve) {
  mystl::vector<int> v;
  v.reserve(100);
  EXPECT_GE(v.capacity(), 100u);
  EXPECT_EQ(v.size(), 0);
}

TEST(VectorTest, Resize) {
  mystl::vector<int> v(3, 1);
  v.resize(5, 2);
  EXPECT_EQ(v.size(), 5);
  EXPECT_EQ(v[3], 2);
  EXPECT_EQ(v[4], 2);

  v.resize(2);
  EXPECT_EQ(v.size(), 2);
  EXPECT_EQ(v[0], 1);
}

TEST(VectorTest, ShrinkToFit) {
  mystl::vector<int> v;
  v.reserve(100);
  v.push_back(1);
  v.push_back(2);
  v.shrink_to_fit();
  EXPECT_EQ(v.capacity(), 2);
  EXPECT_EQ(v.size(), 2);
  EXPECT_EQ(v[0], 1);
}

// -- Modifier tests --

TEST(VectorTest, PushBack) {
  mystl::vector<int> v;
  for (int i = 0; i < 100; ++i)
    v.push_back(i);
  EXPECT_EQ(v.size(), 100);
  EXPECT_EQ(v[0], 0);
  EXPECT_EQ(v[99], 99);
}

TEST(VectorTest, PushBackString) {
  mystl::vector<std::string> v;
  std::string s = "hello";
  v.push_back(s);                   // copy
  v.push_back(std::string("world")); // move
  EXPECT_EQ(v.size(), 2);
  EXPECT_EQ(v[0], "hello");
  EXPECT_EQ(v[1], "world");
}

TEST(VectorTest, EmplaceBack) {
  mystl::vector<std::string> v;
  v.emplace_back("hello");
  v.emplace_back(3, 'x');  // constructs "xxx"
  EXPECT_EQ(v.size(), 2);
  EXPECT_EQ(v[0], "hello");
  EXPECT_EQ(v[1], "xxx");
}

TEST(VectorTest, PopBack) {
  mystl::vector<int> v;
  v.push_back(1);
  v.push_back(2);
  v.pop_back();
  EXPECT_EQ(v.size(), 1);
  EXPECT_EQ(v[0], 1);
}

TEST(VectorTest, Insert) {
  mystl::vector<int> v;
  v.push_back(1);
  v.push_back(3);
  v.insert(v.begin() + 1, 2);
  EXPECT_EQ(v.size(), 3);
  EXPECT_EQ(v[0], 1);
  EXPECT_EQ(v[1], 2);
  EXPECT_EQ(v[2], 3);
}

TEST(VectorTest, InsertAtBegin) {
  mystl::vector<int> v;
  v.push_back(2);
  v.push_back(3);
  v.insert(v.begin(), 1);
  EXPECT_EQ(v[0], 1);
  EXPECT_EQ(v[1], 2);
  EXPECT_EQ(v[2], 3);
}

TEST(VectorTest, Emplace) {
  mystl::vector<std::string> v;
  v.push_back("aaa");
  v.push_back("ccc");
  v.emplace(v.begin() + 1, 3, 'b');  // "bbb"
  EXPECT_EQ(v.size(), 3);
  EXPECT_EQ(v[0], "aaa");
  EXPECT_EQ(v[1], "bbb");
  EXPECT_EQ(v[2], "ccc");
}

TEST(VectorTest, EraseSingle) {
  mystl::vector<int> v;
  v.push_back(1);
  v.push_back(2);
  v.push_back(3);
  auto it = v.erase(v.begin() + 1);
  EXPECT_EQ(v.size(), 2);
  EXPECT_EQ(v[0], 1);
  EXPECT_EQ(v[1], 3);
  EXPECT_EQ(*it, 3);
}

TEST(VectorTest, EraseRange) {
  mystl::vector<int> v;
  for (int i = 0; i < 5; ++i)
    v.push_back(i);
  v.erase(v.begin() + 1, v.begin() + 4);
  EXPECT_EQ(v.size(), 2);
  EXPECT_EQ(v[0], 0);
  EXPECT_EQ(v[1], 4);
}

TEST(VectorTest, Clear) {
  mystl::vector<int> v(5, 1);
  v.clear();
  EXPECT_EQ(v.size(), 0);
  EXPECT_TRUE(v.empty());
  EXPECT_GE(v.capacity(), 5u);  // capacity unchanged
}

TEST(VectorTest, Swap) {
  mystl::vector<int> a(3, 1);
  mystl::vector<int> b(2, 2);
  a.swap(b);
  EXPECT_EQ(a.size(), 2);
  EXPECT_EQ(a[0], 2);
  EXPECT_EQ(b.size(), 3);
  EXPECT_EQ(b[0], 1);
}

// -- Growth test --

TEST(VectorTest, AutoGrow) {
  mystl::vector<int> v;
  for (int i = 0; i < 1000; ++i)
    v.push_back(i);
  EXPECT_EQ(v.size(), 1000);
  for (int i = 0; i < 1000; ++i)
    EXPECT_EQ(v[i], i);
}
