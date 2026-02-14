// MySTL
//
// This header implements a simplified std::vector — a dynamic array with
// contiguous storage, automatic resizing, and amortized O(1) push_back.

#pragma once

#include <cstddef>
#include <stdexcept>

#include "allocator.h"
#include "construct.h"
#include "util.h"

namespace mystl {

template <class T, class Alloc = mystl::allocator<T>>
class vector {
public:
  // Type aliases following STL conventions.
  typedef T         value_type;
  typedef T*        iterator;
  typedef const T*  const_iterator;
  typedef T&        reference;
  typedef const T&  const_reference;
  typedef size_t    size_type;
  typedef ptrdiff_t difference_type;

  // -- Constructors --

  // Default constructor: empty vector.
  vector() noexcept : begin_(nullptr), end_(nullptr), cap_(nullptr) {}

  // Constructs a vector with n default-initialized elements.
  explicit vector(size_type n) : begin_(nullptr), end_(nullptr), cap_(nullptr) {
    fill_init(n, value_type());
  }

  // Constructs a vector with n copies of value.
  vector(size_type n, const value_type& value)
      : begin_(nullptr), end_(nullptr), cap_(nullptr) {
    fill_init(n, value);
  }

  // Copy constructor.
  vector(const vector& other)
      : begin_(nullptr), end_(nullptr), cap_(nullptr) {
    range_init(other.begin_, other.end_);
  }

  // Move constructor: steal resources from other.
  vector(vector&& other) noexcept
      : begin_(other.begin_), end_(other.end_), cap_(other.cap_) {
    other.begin_ = nullptr;
    other.end_ = nullptr;
    other.cap_ = nullptr;
  }

  // -- Destructor --

  ~vector() {
    destroy_and_deallocate();
  }

  // -- Assignment --

  // Copy assignment.
  vector& operator=(const vector& other) {
    if (this != &other) {
      vector tmp(other);
      swap(tmp);
    }
    return *this;
  }

  // Move assignment.
  vector& operator=(vector&& other) noexcept {
    if (this != &other) {
      destroy_and_deallocate();
      begin_ = other.begin_;
      end_ = other.end_;
      cap_ = other.cap_;
      other.begin_ = nullptr;
      other.end_ = nullptr;
      other.cap_ = nullptr;
    }
    return *this;
  }

  // -- Element access --

  reference operator[](size_type n) { return *(begin_ + n); }
  const_reference operator[](size_type n) const { return *(begin_ + n); }

  reference at(size_type n) {
    if (n >= size()) throw std::out_of_range("vector::at");
    return *(begin_ + n);
  }
  const_reference at(size_type n) const {
    if (n >= size()) throw std::out_of_range("vector::at");
    return *(begin_ + n);
  }

  reference front() { return *begin_; }
  const_reference front() const { return *begin_; }

  reference back() { return *(end_ - 1); }
  const_reference back() const { return *(end_ - 1); }

  iterator data() noexcept { return begin_; }
  const_iterator data() const noexcept { return begin_; }

  // -- Iterators --

  iterator begin() noexcept { return begin_; }
  const_iterator begin() const noexcept { return begin_; }

  iterator end() noexcept { return end_; }
  const_iterator end() const noexcept { return end_; }

  // -- Capacity --

  size_type size() const noexcept { return static_cast<size_type>(end_ - begin_); }
  size_type capacity() const noexcept { return static_cast<size_type>(cap_ - begin_); }
  bool empty() const noexcept { return begin_ == end_; }

  // reserve ensures capacity is at least n. Does nothing if n <= capacity().
  void reserve(size_type n) {
    if (n <= capacity()) return;
    reallocate(n);
  }

  // resize changes size to n. If n > size(), appends default-initialized
  // elements. If n < size(), destroys trailing elements.
  void resize(size_type n) {
    resize(n, value_type());
  }

  void resize(size_type n, const value_type& value) {
    if (n < size()) {
      erase(begin_ + n, end_);
    } else if (n > size()) {
      reserve(n);
      while (size() < n) {
        mystl::construct(end_, value);
        ++end_;
      }
    }
  }

  // shrink_to_fit releases unused capacity.
  void shrink_to_fit() {
    if (end_ != cap_) {
      reallocate(size());
    }
  }

  // -- Modifiers --

  void push_back(const value_type& value) {
    if (end_ == cap_) {
      reallocate(grow_size());
    }
    mystl::construct(end_, value);
    ++end_;
  }

  void push_back(value_type&& value) {
    if (end_ == cap_) {
      reallocate(grow_size());
    }
    mystl::construct(end_, mystl::move(value));
    ++end_;
  }

  template <class... Args>
  void emplace_back(Args&&... args) {
    if (end_ == cap_) {
      reallocate(grow_size());
    }
    mystl::construct(end_, mystl::forward<Args>(args)...);
    ++end_;
  }

  void pop_back() {
    --end_;
    mystl::destroy(end_);
  }

  // insert a single element before pos. Returns iterator to inserted element.
  iterator insert(const_iterator pos, const value_type& value) {
    size_type offset = pos - begin_;
    if (end_ == cap_) {
      reallocate(grow_size());
    }
    iterator p = begin_ + offset;
    // Shift elements to the right.
    if (p != end_) {
      mystl::construct(end_, mystl::move(*(end_ - 1)));
      for (iterator it = end_ - 1; it != p; --it)
        *it = mystl::move(*(it - 1));
      *p = value;
    } else {
      mystl::construct(end_, value);
    }
    ++end_;
    return p;
  }

  // emplace a single element before pos.
  template <class... Args>
  iterator emplace(const_iterator pos, Args&&... args) {
    size_type offset = pos - begin_;
    if (end_ == cap_) {
      reallocate(grow_size());
    }
    iterator p = begin_ + offset;
    if (p != end_) {
      mystl::construct(end_, mystl::move(*(end_ - 1)));
      for (iterator it = end_ - 1; it != p; --it)
        *it = mystl::move(*(it - 1));
      mystl::destroy(p);
      mystl::construct(p, mystl::forward<Args>(args)...);
    } else {
      mystl::construct(end_, mystl::forward<Args>(args)...);
    }
    ++end_;
    return p;
  }

  // erase a single element. Returns iterator to the element after erased.
  iterator erase(const_iterator pos) {
    iterator p = begin_ + (pos - begin_);
    for (iterator it = p; it + 1 != end_; ++it)
      *it = mystl::move(*(it + 1));
    --end_;
    mystl::destroy(end_);
    return p;
  }

  // erase a range [first, last). Returns iterator to element after last erased.
  iterator erase(const_iterator first, const_iterator last) {
    iterator f = begin_ + (first - begin_);
    iterator l = begin_ + (last - begin_);
    // Move trailing elements forward.
    iterator new_end = f;
    for (iterator it = l; it != end_; ++it, ++new_end)
      *new_end = mystl::move(*it);
    // Destroy leftover elements at the tail.
    mystl::destroy(new_end, end_);
    end_ = new_end;
    return f;
  }

  void clear() noexcept {
    mystl::destroy(begin_, end_);
    end_ = begin_;
  }

  void swap(vector& other) noexcept {
    mystl::swap(begin_, other.begin_);
    mystl::swap(end_, other.end_);
    mystl::swap(cap_, other.cap_);
  }

private:
  iterator begin_;
  iterator end_;
  iterator cap_;

  // Destroy all elements and free memory.
  void destroy_and_deallocate() {
    if (begin_ != nullptr) {
      mystl::destroy(begin_, end_);
      Alloc::deallocate(begin_, capacity());
    }
  }

  // Compute new capacity when growing. Doubles the current capacity,
  // or starts with 16 if currently empty.
  size_type grow_size() const {
    return capacity() == 0 ? 16 : capacity() * 2;
  }

  // Reallocate to exactly new_cap. Moves existing elements to new memory.
  void reallocate(size_type new_cap) {
    const size_type old_size = size();
    iterator new_begin = Alloc::allocate(new_cap);
    // Move old elements to new memory.
    for (size_type i = 0; i < old_size; ++i)
      mystl::construct(new_begin + i, mystl::move(*(begin_ + i)));
    // Destroy old elements and free old memory.
    destroy_and_deallocate();
    begin_ = new_begin;
    end_ = new_begin + old_size;
    cap_ = new_begin + new_cap;
  }

  // Initialize with n copies of value.
  void fill_init(size_type n, const value_type& value) {
    begin_ = Alloc::allocate(n);
    cap_ = begin_ + n;
    end_ = begin_;
    for (size_type i = 0; i < n; ++i) {
      mystl::construct(end_, value);
      ++end_;
    }
  }

  // Initialize from a range [first, last).
  void range_init(const_iterator first, const_iterator last) {
    size_type n = static_cast<size_type>(last - first);
    begin_ = Alloc::allocate(n);
    cap_ = begin_ + n;
    end_ = begin_;
    for (; first != last; ++first) {
      mystl::construct(end_, *first);
      ++end_;
    }
  }
};

}  // namespace mystl
