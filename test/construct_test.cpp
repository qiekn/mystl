#include "construct.h"

#include <gtest/gtest.h>

#include <string>

TEST(ConstructTest, ConstructInt) {
  // Allocate raw memory, no object constructed yet.
  void* mem = ::operator new(sizeof(int));
  int* p = static_cast<int*>(mem);

  mystl::construct(p, 42);
  EXPECT_EQ(*p, 42);

  mystl::destroy(p);
  ::operator delete(mem);
}

TEST(ConstructTest, ConstructString) {
  void* mem = ::operator new(sizeof(std::string));
  std::string* p = static_cast<std::string*>(mem);

  mystl::construct(p, "hello");
  EXPECT_EQ(*p, "hello");

  mystl::destroy(p);
  ::operator delete(mem);
}

TEST(ConstructTest, ConstructDefault) {
  void* mem = ::operator new(sizeof(int));
  int* p = static_cast<int*>(mem);

  mystl::construct(p);
  EXPECT_EQ(*p, 0);

  mystl::destroy(p);
  ::operator delete(mem);
}

TEST(DestroyTest, DestroyRangeOfStrings) {
  const int n = 3;
  void* mem = ::operator new(sizeof(std::string) * n);
  std::string* arr = static_cast<std::string*>(mem);

  mystl::construct(arr, "aaa");
  mystl::construct(arr + 1, "bbb");
  mystl::construct(arr + 2, "ccc");

  EXPECT_EQ(arr[0], "aaa");
  EXPECT_EQ(arr[1], "bbb");
  EXPECT_EQ(arr[2], "ccc");

  mystl::destroy(arr, arr + n);
  ::operator delete(mem);
}

TEST(DestroyTest, DestroyRangeOfTrivialType) {
  // For trivially destructible types, destroy should be a no-op.
  int arr[] = {1, 2, 3};
  mystl::destroy(arr, arr + 3);
  // No crash = success. Values may still be readable.
  EXPECT_EQ(arr[0], 1);
}
