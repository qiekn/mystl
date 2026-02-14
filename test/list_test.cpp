#include "list.h"

#include <gtest/gtest.h>

#include <string>

// -- Constructor tests --

TEST(ListTest, DefaultConstruct) {
  mystl::list<int> l;
  EXPECT_EQ(l.size(), 0);
  EXPECT_TRUE(l.empty());
}

TEST(ListTest, ConstructWithSize) {
  mystl::list<int> l(5);
  EXPECT_EQ(l.size(), 5);
  for (auto it = l.begin(); it != l.end(); ++it)
    EXPECT_EQ(*it, 0);
}

TEST(ListTest, ConstructWithSizeAndValue) {
  mystl::list<int> l(3, 42);
  EXPECT_EQ(l.size(), 3);
  for (auto it = l.begin(); it != l.end(); ++it)
    EXPECT_EQ(*it, 42);
}

TEST(ListTest, CopyConstruct) {
  mystl::list<int> a(3, 7);
  mystl::list<int> b(a);
  EXPECT_EQ(b.size(), 3);
  EXPECT_EQ(b.front(), 7);
  // Modifying b should not affect a.
  b.front() = 99;
  EXPECT_EQ(a.front(), 7);
}

TEST(ListTest, MoveConstruct) {
  mystl::list<std::string> a(2, "hello");
  mystl::list<std::string> b(mystl::move(a));
  EXPECT_EQ(b.size(), 2);
  EXPECT_EQ(b.front(), "hello");
  EXPECT_EQ(a.size(), 0);
  EXPECT_TRUE(a.empty());
}

// -- Assignment tests --

TEST(ListTest, CopyAssign) {
  mystl::list<int> a(3, 1);
  mystl::list<int> b;
  b = a;
  EXPECT_EQ(b.size(), 3);
  EXPECT_EQ(b.front(), 1);
}

TEST(ListTest, MoveAssign) {
  mystl::list<int> a(3, 1);
  mystl::list<int> b;
  b = mystl::move(a);
  EXPECT_EQ(b.size(), 3);
  EXPECT_EQ(a.size(), 0);
}

// -- Element access tests --

TEST(ListTest, FrontAndBack) {
  mystl::list<int> l;
  l.push_back(1);
  l.push_back(2);
  l.push_back(3);
  EXPECT_EQ(l.front(), 1);
  EXPECT_EQ(l.back(), 3);
}

// -- Iterator tests --

TEST(ListTest, ForwardIteration) {
  mystl::list<int> l;
  l.push_back(10);
  l.push_back(20);
  l.push_back(30);

  int sum = 0;
  for (auto it = l.begin(); it != l.end(); ++it)
    sum += *it;
  EXPECT_EQ(sum, 60);
}

TEST(ListTest, BackwardIteration) {
  mystl::list<int> l;
  l.push_back(1);
  l.push_back(2);
  l.push_back(3);

  auto it = l.end();
  --it;
  EXPECT_EQ(*it, 3);
  --it;
  EXPECT_EQ(*it, 2);
  --it;
  EXPECT_EQ(*it, 1);
  EXPECT_TRUE(it == l.begin());
}

TEST(ListTest, RangeFor) {
  mystl::list<int> l;
  l.push_back(1);
  l.push_back(2);
  l.push_back(3);

  int sum = 0;
  for (int x : l)
    sum += x;
  EXPECT_EQ(sum, 6);
}

// -- Modifier tests --

TEST(ListTest, PushBackAndFront) {
  mystl::list<int> l;
  l.push_back(2);
  l.push_front(1);
  l.push_back(3);
  EXPECT_EQ(l.size(), 3);
  EXPECT_EQ(l.front(), 1);
  EXPECT_EQ(l.back(), 3);
}

TEST(ListTest, PushBackString) {
  mystl::list<std::string> l;
  std::string s = "hello";
  l.push_back(s);                     // copy
  l.push_back(std::string("world"));  // move
  EXPECT_EQ(l.size(), 2);
  EXPECT_EQ(l.front(), "hello");
  EXPECT_EQ(l.back(), "world");
}

TEST(ListTest, EmplaceBack) {
  mystl::list<std::string> l;
  l.emplace_back("hello");
  l.emplace_back(3, 'x');  // constructs "xxx"
  EXPECT_EQ(l.size(), 2);
  EXPECT_EQ(l.front(), "hello");
  EXPECT_EQ(l.back(), "xxx");
}

TEST(ListTest, EmplaceFront) {
  mystl::list<std::string> l;
  l.emplace_front("world");
  l.emplace_front("hello");
  EXPECT_EQ(l.front(), "hello");
  EXPECT_EQ(l.back(), "world");
}

TEST(ListTest, PopBack) {
  mystl::list<int> l;
  l.push_back(1);
  l.push_back(2);
  l.pop_back();
  EXPECT_EQ(l.size(), 1);
  EXPECT_EQ(l.back(), 1);
}

TEST(ListTest, PopFront) {
  mystl::list<int> l;
  l.push_back(1);
  l.push_back(2);
  l.pop_front();
  EXPECT_EQ(l.size(), 1);
  EXPECT_EQ(l.front(), 2);
}

TEST(ListTest, InsertMiddle) {
  mystl::list<int> l;
  l.push_back(1);
  l.push_back(3);
  auto it = l.begin();
  ++it;  // points to 3
  l.insert(it, 2);
  EXPECT_EQ(l.size(), 3);

  it = l.begin();
  EXPECT_EQ(*it, 1);
  ++it;
  EXPECT_EQ(*it, 2);
  ++it;
  EXPECT_EQ(*it, 3);
}

TEST(ListTest, Emplace) {
  mystl::list<std::string> l;
  l.push_back("aaa");
  l.push_back("ccc");
  auto it = l.begin();
  ++it;  // points to "ccc"
  l.emplace(it, 3, 'b');  // "bbb"
  EXPECT_EQ(l.size(), 3);

  it = l.begin();
  EXPECT_EQ(*it, "aaa");
  ++it;
  EXPECT_EQ(*it, "bbb");
  ++it;
  EXPECT_EQ(*it, "ccc");
}

TEST(ListTest, EraseSingle) {
  mystl::list<int> l;
  l.push_back(1);
  l.push_back(2);
  l.push_back(3);
  auto it = l.begin();
  ++it;  // points to 2
  it = l.erase(it);
  EXPECT_EQ(l.size(), 2);
  EXPECT_EQ(*it, 3);
  EXPECT_EQ(l.front(), 1);
  EXPECT_EQ(l.back(), 3);
}

TEST(ListTest, EraseRange) {
  mystl::list<int> l;
  for (int i = 0; i < 5; ++i)
    l.push_back(i);
  auto first = l.begin();
  ++first;  // points to 1
  auto last = first;
  ++last;
  ++last;
  ++last;  // points to 4
  l.erase(first, last);
  EXPECT_EQ(l.size(), 2);
  EXPECT_EQ(l.front(), 0);
  EXPECT_EQ(l.back(), 4);
}

TEST(ListTest, Clear) {
  mystl::list<int> l(5, 1);
  l.clear();
  EXPECT_EQ(l.size(), 0);
  EXPECT_TRUE(l.empty());
}

TEST(ListTest, Swap) {
  mystl::list<int> a(3, 1);
  mystl::list<int> b(2, 2);
  a.swap(b);
  EXPECT_EQ(a.size(), 2);
  EXPECT_EQ(a.front(), 2);
  EXPECT_EQ(b.size(), 3);
  EXPECT_EQ(b.front(), 1);
}

TEST(ListTest, SwapWithEmpty) {
  mystl::list<int> a;
  mystl::list<int> b(3, 5);
  a.swap(b);
  EXPECT_EQ(a.size(), 3);
  EXPECT_EQ(a.front(), 5);
  EXPECT_EQ(b.size(), 0);
  EXPECT_TRUE(b.empty());
}

// -- Stress test --

TEST(ListTest, ManyElements) {
  mystl::list<int> l;
  for (int i = 0; i < 1000; ++i)
    l.push_back(i);
  EXPECT_EQ(l.size(), 1000);
  EXPECT_EQ(l.front(), 0);
  EXPECT_EQ(l.back(), 999);

  int expected = 0;
  for (int x : l)
    EXPECT_EQ(x, expected++);
}
