// MySTL
//
// This header implements a simplified std::list — a doubly-linked list with
// O(1) insertion and removal at any position, bidirectional iteration, and
// a sentinel node for uniform boundary handling.

#pragma once

#include <cstddef>

#include "allocator.h"
#include "construct.h"
#include "iterator.h"
#include "util.h"

namespace mystl {

// Node base class: contains only link pointers. The sentinel node uses this
// type directly, avoiding construction of an unnecessary T object.
struct list_node_base {
  list_node_base* prev;
  list_node_base* next;
};

// Data node: inherits link pointers and adds the stored value.
template <class T>
struct list_node : list_node_base {
  T data;
};

// Bidirectional iterator for list. Wraps a pointer to list_node_base,
// downcasting to list_node<T> when dereferenced.
template <class T>
struct list_iterator {
  typedef mystl::bidirectional_iterator_tag iterator_category;
  typedef T                                 value_type;
  typedef ptrdiff_t                         difference_type;
  typedef T*                                pointer;
  typedef T&                                reference;

  list_node_base* node;

  list_iterator() : node(nullptr) {}
  explicit list_iterator(list_node_base* n) : node(n) {}

  reference operator*() const {
    return static_cast<list_node<T>*>(node)->data;
  }

  pointer operator->() const {
    return &(static_cast<list_node<T>*>(node)->data);
  }

  list_iterator& operator++() {
    node = node->next;
    return *this;
  }

  list_iterator operator++(int) {
    list_iterator tmp = *this;
    node = node->next;
    return tmp;
  }

  list_iterator& operator--() {
    node = node->prev;
    return *this;
  }

  list_iterator operator--(int) {
    list_iterator tmp = *this;
    node = node->prev;
    return tmp;
  }

  bool operator==(const list_iterator& other) const {
    return node == other.node;
  }

  bool operator!=(const list_iterator& other) const {
    return node != other.node;
  }
};

// Const iterator for list.
template <class T>
struct list_const_iterator {
  typedef mystl::bidirectional_iterator_tag iterator_category;
  typedef T                                 value_type;
  typedef ptrdiff_t                         difference_type;
  typedef const T*                          pointer;
  typedef const T&                          reference;

  const list_node_base* node;

  list_const_iterator() : node(nullptr) {}
  explicit list_const_iterator(const list_node_base* n) : node(n) {}

  // Allow implicit conversion from non-const iterator.
  list_const_iterator(const list_iterator<T>& it) : node(it.node) {}

  reference operator*() const {
    return static_cast<const list_node<T>*>(node)->data;
  }

  pointer operator->() const {
    return &(static_cast<const list_node<T>*>(node)->data);
  }

  list_const_iterator& operator++() {
    node = node->next;
    return *this;
  }

  list_const_iterator operator++(int) {
    list_const_iterator tmp = *this;
    node = node->next;
    return tmp;
  }

  list_const_iterator& operator--() {
    node = node->prev;
    return *this;
  }

  list_const_iterator operator--(int) {
    list_const_iterator tmp = *this;
    node = node->prev;
    return tmp;
  }

  bool operator==(const list_const_iterator& other) const {
    return node == other.node;
  }

  bool operator!=(const list_const_iterator& other) const {
    return node != other.node;
  }
};

template <class T, class Alloc = mystl::allocator<list_node<T>>>
class list {
public:
  typedef T                     value_type;
  typedef list_iterator<T>      iterator;
  typedef list_const_iterator<T> const_iterator;
  typedef T&                    reference;
  typedef const T&              const_reference;
  typedef size_t                size_type;
  typedef ptrdiff_t             difference_type;

  // -- Constructors --

  // Default constructor: empty list with sentinel pointing to itself.
  list() : size_(0) {
    sentinel_.prev = &sentinel_;
    sentinel_.next = &sentinel_;
  }

  // Constructs a list with n default-initialized elements.
  explicit list(size_type n) : list() {
    for (size_type i = 0; i < n; ++i)
      emplace_back();
  }

  // Constructs a list with n copies of value.
  list(size_type n, const value_type& value) : list() {
    for (size_type i = 0; i < n; ++i)
      push_back(value);
  }

  // Copy constructor.
  list(const list& other) : list() {
    for (auto it = other.begin(); it != other.end(); ++it)
      push_back(*it);
  }

  // Move constructor: steal all nodes from other.
  list(list&& other) noexcept : list() {
    if (!other.empty()) {
      // Link other's nodes into our sentinel.
      sentinel_.next = other.sentinel_.next;
      sentinel_.prev = other.sentinel_.prev;
      sentinel_.next->prev = &sentinel_;
      sentinel_.prev->next = &sentinel_;
      size_ = other.size_;
      // Reset other to empty.
      other.sentinel_.next = &other.sentinel_;
      other.sentinel_.prev = &other.sentinel_;
      other.size_ = 0;
    }
  }

  // -- Destructor --

  ~list() {
    clear();
  }

  // -- Assignment --

  // Copy assignment.
  list& operator=(const list& other) {
    if (this != &other) {
      list tmp(other);
      swap(tmp);
    }
    return *this;
  }

  // Move assignment.
  list& operator=(list&& other) noexcept {
    if (this != &other) {
      clear();
      if (!other.empty()) {
        sentinel_.next = other.sentinel_.next;
        sentinel_.prev = other.sentinel_.prev;
        sentinel_.next->prev = &sentinel_;
        sentinel_.prev->next = &sentinel_;
        size_ = other.size_;
        other.sentinel_.next = &other.sentinel_;
        other.sentinel_.prev = &other.sentinel_;
        other.size_ = 0;
      }
    }
    return *this;
  }

  // -- Element access --

  reference front() { return *begin(); }
  const_reference front() const { return *begin(); }

  reference back() { return *(--end()); }
  const_reference back() const { return *(--end()); }

  // -- Iterators --

  iterator begin() noexcept { return iterator(sentinel_.next); }
  const_iterator begin() const noexcept {
    return const_iterator(sentinel_.next);
  }

  iterator end() noexcept { return iterator(&sentinel_); }
  const_iterator end() const noexcept {
    return const_iterator(&sentinel_);
  }

  // -- Capacity --

  size_type size() const noexcept { return size_; }
  bool empty() const noexcept { return size_ == 0; }

  // -- Modifiers --

  void push_back(const value_type& value) { insert(end(), value); }
  void push_back(value_type&& value) { insert(end(), mystl::move(value)); }

  void push_front(const value_type& value) { insert(begin(), value); }
  void push_front(value_type&& value) { insert(begin(), mystl::move(value)); }

  template <class... Args>
  void emplace_back(Args&&... args) {
    emplace(end(), mystl::forward<Args>(args)...);
  }

  template <class... Args>
  void emplace_front(Args&&... args) {
    emplace(begin(), mystl::forward<Args>(args)...);
  }

  void pop_back() { erase(--end()); }
  void pop_front() { erase(begin()); }

  // Insert a single element before pos.
  iterator insert(const_iterator pos, const value_type& value) {
    list_node<T>* new_node = create_node(value);
    link_before(pos.node, new_node);
    ++size_;
    return iterator(new_node);
  }

  // Insert (move version).
  iterator insert(const_iterator pos, value_type&& value) {
    list_node<T>* new_node = create_node(mystl::move(value));
    link_before(pos.node, new_node);
    ++size_;
    return iterator(new_node);
  }

  // Emplace a single element before pos.
  template <class... Args>
  iterator emplace(const_iterator pos, Args&&... args) {
    list_node<T>* new_node = create_node(mystl::forward<Args>(args)...);
    link_before(pos.node, new_node);
    ++size_;
    return iterator(new_node);
  }

  // Erase a single element. Returns iterator to the next element.
  iterator erase(const_iterator pos) {
    list_node_base* p = const_cast<list_node_base*>(pos.node);
    list_node_base* next_node = p->next;
    p->prev->next = p->next;
    p->next->prev = p->prev;
    destroy_node(static_cast<list_node<T>*>(p));
    --size_;
    return iterator(next_node);
  }

  // Erase a range [first, last).
  iterator erase(const_iterator first, const_iterator last) {
    while (first != last)
      first = erase(first);
    return iterator(const_cast<list_node_base*>(last.node));
  }

  void clear() noexcept {
    list_node_base* cur = sentinel_.next;
    while (cur != &sentinel_) {
      list_node_base* next = cur->next;
      destroy_node(static_cast<list_node<T>*>(cur));
      cur = next;
    }
    sentinel_.next = &sentinel_;
    sentinel_.prev = &sentinel_;
    size_ = 0;
  }

  void swap(list& other) noexcept {
    if (empty() && other.empty()) return;

    if (empty()) {
      // this is empty, other is not.
      sentinel_.next = other.sentinel_.next;
      sentinel_.prev = other.sentinel_.prev;
      sentinel_.next->prev = &sentinel_;
      sentinel_.prev->next = &sentinel_;
      other.sentinel_.next = &other.sentinel_;
      other.sentinel_.prev = &other.sentinel_;
    } else if (other.empty()) {
      // other is empty, this is not.
      other.sentinel_.next = sentinel_.next;
      other.sentinel_.prev = sentinel_.prev;
      other.sentinel_.next->prev = &other.sentinel_;
      other.sentinel_.prev->next = &other.sentinel_;
      sentinel_.next = &sentinel_;
      sentinel_.prev = &sentinel_;
    } else {
      // Both non-empty: swap link pointers and fix back-references.
      mystl::swap(sentinel_.next, other.sentinel_.next);
      mystl::swap(sentinel_.prev, other.sentinel_.prev);
      sentinel_.next->prev = &sentinel_;
      sentinel_.prev->next = &sentinel_;
      other.sentinel_.next->prev = &other.sentinel_;
      other.sentinel_.prev->next = &other.sentinel_;
    }
    mystl::swap(size_, other.size_);
  }

private:
  list_node_base sentinel_;
  size_type size_;

  // Allocate and construct a node with given arguments.
  template <class... Args>
  list_node<T>* create_node(Args&&... args) {
    list_node<T>* node = Alloc::allocate(1);
    mystl::construct(&node->data, mystl::forward<Args>(args)...);
    return node;
  }

  // Destroy and deallocate a node.
  void destroy_node(list_node<T>* node) {
    mystl::destroy(&node->data);
    Alloc::deallocate(node, 1);
  }

  // Link new_node before pos.
  void link_before(const list_node_base* pos, list_node_base* new_node) {
    list_node_base* p = const_cast<list_node_base*>(pos);
    new_node->next = p;
    new_node->prev = p->prev;
    p->prev->next = new_node;
    p->prev = new_node;
  }
};

}  // namespace mystl
