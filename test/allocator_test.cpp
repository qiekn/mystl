#include "allocator.h"

#include <gtest/gtest.h>

#include <string>

TEST(AllocatorTest, AllocateAndDeallocateInt) {
  using Alloc = mystl::allocator<int>;
  int* p = Alloc::allocate(5);
  EXPECT_NE(p, nullptr);
  Alloc::deallocate(p, 5);
}

TEST(AllocatorTest, ConstructAndDestroyInt) {
  using Alloc = mystl::allocator<int>;
  int* p = Alloc::allocate(1);

  Alloc::construct(p, 42);
  EXPECT_EQ(*p, 42);

  Alloc::destroy(p);
  Alloc::deallocate(p, 1);
}

TEST(AllocatorTest, ConstructAndDestroyString) {
  using Alloc = mystl::allocator<std::string>;
  std::string* p = Alloc::allocate(1);

  Alloc::construct(p, "hello");
  EXPECT_EQ(*p, "hello");

  Alloc::destroy(p);
  Alloc::deallocate(p, 1);
}

TEST(AllocatorTest, MultipleObjects) {
  using Alloc = mystl::allocator<std::string>;
  const int n = 3;
  std::string* arr = Alloc::allocate(n);

  Alloc::construct(arr, "aaa");
  Alloc::construct(arr + 1, "bbb");
  Alloc::construct(arr + 2, "ccc");

  EXPECT_EQ(arr[0], "aaa");
  EXPECT_EQ(arr[1], "bbb");
  EXPECT_EQ(arr[2], "ccc");

  Alloc::destroy(arr, arr + n);
  Alloc::deallocate(arr, n);
}

TEST(AllocatorTest, TypeTraits) {
  using Alloc = mystl::allocator<int>;
  static_assert(std::is_same<Alloc::value_type, int>::value);
  static_assert(std::is_same<Alloc::pointer, int*>::value);
  static_assert(std::is_same<Alloc::const_pointer, const int*>::value);
  static_assert(std::is_same<Alloc::reference, int&>::value);
  static_assert(std::is_same<Alloc::const_reference, const int&>::value);
  static_assert(std::is_same<Alloc::size_type, size_t>::value);
}
