#pragma once

#include <cstddef>

namespace mystl {

// =============================================================================
// Iterator Tags
// 用空结构体标记迭代器类型，通过继承关系表达能力的包含关系
// =============================================================================

struct input_iterator_tag {};
struct output_iterator_tag {};
struct forward_iterator_tag : public input_iterator_tag {};
struct bidirectional_iterator_tag : public forward_iterator_tag {};
struct random_access_iterator_tag : public bidirectional_iterator_tag {};

// =============================================================================
// Iterator Base
// 自定义迭代器可以继承这个基类，自动获得五个必要的类型定义
// =============================================================================

// TODO: 实现 iterator 基类模板
template <class Category, class T, class Distance = ptrdiff_t,
          class Pointer = T*, class Reference = T&>
struct iterator {
  typedef Category  iterator_category;  // 迭代器类型
  typedef T         value_type;         // 元素类型
  typedef Distance  difference_type;    // 距离类型
  typedef Pointer   pointer;            // 指针类型
  typedef Reference reference;          // 引用类型
};

// =============================================================================
// Iterator Traits
// 萃取迭代器的类型信息，统一访问接口
// =============================================================================

// TODO: 实现 iterator_traits 主模板

// TODO: 实现 iterator_traits 对原生指针的特化 (T*)

// TODO: 实现 iterator_traits 对 const 指针的特化 (const T*)

// =============================================================================
// Helper Functions
// =============================================================================

// TODO: 实现 distance - 计算两个迭代器之间的距离

// TODO: 实现 advance - 将迭代器移动 n 步

}  // namespace mystl
