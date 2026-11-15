# tool-cpp 是一个基于C++的常用工具库

## 1. 介绍

## 2. 内容
- 模仿cpp标准库智能指针实现

- 实现一个类似映射操作，返回一个新容器
```cpp
// usage mapping(arr, [](const T& item) { return int(item);})
// return: unique_ptr<int>[n];
template<class Container, class MapRule>
auto mapping(Container&& c, MapRule rule)
    -> std::vector<
        typename std::decay<decltype(rule(*std::begin(c)))>::type
    >
```

待解决问题

[] 1. shared_ptr::reset 方法实现
[] 2. shared_ptr::swap 方法实现
