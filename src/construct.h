// MySTL
//
// This header provides construct and destroy utilities for managing object
// lifetime on raw memory. construct uses placement new to build objects
// in-place, while destroy calls destructors without freeing memory. The
// destroy functions use tag dispatch to skip trivially destructible types.

#pragma once

#include <type_traits>

#include "iterator.h"
#include "type_traits.h"
#include "util.h"

namespace mystl {

// construct builds an object of type T at the address ptr, forwarding
// args to T's constructor via placement new.
template <class T, class... Args>
void construct(T* ptr, Args&&... args) {
  ::new ((void*)ptr) T(mystl::forward<Args>(args)...);
}

// destroy calls the destructor of the object pointed to by ptr.
template <class T>
void destroy(T* ptr) {
  ptr->~T();
}

// destroy a range [first, last). Uses tag dispatch to skip destruction
// for trivially destructible types (e.g., int, double).

template <class ForwardIterator>
void destroy_dispatch(ForwardIterator first, ForwardIterator last,
                      true_type) {
  // Trivially destructible: nothing to do.
}

template <class ForwardIterator>
void destroy_dispatch(ForwardIterator first, ForwardIterator last,
                      false_type) {
  for (; first != last; ++first)
    destroy(&*first);
}

template <class ForwardIterator>
void destroy(ForwardIterator first, ForwardIterator last) {
  typedef typename iterator_traits<ForwardIterator>::value_type value_type;
  destroy_dispatch(first, last,
                   bool_constant<std::is_trivially_destructible<value_type>::value>());
}

}  // namespace mystl
