# 创建测试可执行文件
add_executable(mystl_test
  main.cpp
  type_traits_test.cpp
  iterator_test.cpp
  util_test.cpp
  construct_test.cpp
  allocator_test.cpp
  vector_test.cpp
  list_test.cpp
)

# 包含自己实现的 STL 头文件目录
target_include_directories(mystl_test PRIVATE ${PROJECT_SOURCE_DIR}/src)

# 链接 GoogleTest
target_link_libraries(mystl_test PRIVATE GTest::gtest_main)

# 注册测试到 CTest
include(GoogleTest)
gtest_discover_tests(mystl_test)
