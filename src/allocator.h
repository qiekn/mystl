// MySTL
//
// This header provides a simple default allocator that wraps global
// operator new/delete. Containers use allocators to separate memory
// allocation from object construction, allowing custom memory strategies.

#pragma once

#include <cstddef>

#include "construct.h"

namespace mystl {

template <class T>
class allocator {
public:
  typedef T         value_type;
  typedef T*        pointer;
  typedef const T*  const_pointer;
  typedef T&        reference;
  typedef const T&  const_reference;
  typedef size_t    size_type;
  typedef ptrdiff_t difference_type;

  // allocate returns raw memory for n objects of type T.
  // No constructors are called.
  static pointer allocate(size_type n) {
    return static_cast<pointer>(::operator new(n * sizeof(T)));
  }

  // deallocate frees memory previously returned by allocate.
  // No destructors are called.
  static void deallocate(pointer ptr, size_type /*n*/) {
    ::operator delete(ptr);
  }

  // construct builds an object at ptr, forwarding args to T's constructor.
  template <class... Args>
  static void construct(pointer ptr, Args&&... args) {
    mystl::construct(ptr, mystl::forward<Args>(args)...);
  }

  // destroy calls the destructor of the object at ptr.
  static void destroy(pointer ptr) {
    mystl::destroy(ptr);
  }

  // destroy a range [first, last).
  static void destroy(pointer first, pointer last) {
    mystl::destroy(first, last);
  }
};

}  // namespace mystl
