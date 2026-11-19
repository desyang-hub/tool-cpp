#pragma once

#include <type_traits>

namespace toolcpp {

template<class Iterator, class Func>
void for_each(Iterator first, Iterator last, Func func) {
    while (first != last) {
        func(*first);
        ++first;
    }
}

// 非 const 版本
template <typename R, typename C, typename... Args>
struct mem_fn_wrapper {
    R (C::*pmf)(Args...);

    explicit mem_fn_wrapper(R (C::*p)(Args...)) : pmf(p) {}

    R operator()(C& obj, Args&&... args) const {
        return (obj.*pmf)(std::forward<Args>(args)...);
    }

    R operator()(C* obj, Args&&... args) const {
        return (obj->*pmf)(std::forward<Args>(args)...);
    }
};

// const 版本
template <typename R, typename C, typename... Args>
struct const_mem_fn_wrapper {
    R (C::*pmf)(Args...) const;

    explicit const_mem_fn_wrapper(R (C::*p)(Args...) const) : pmf(p) {}

    R operator()(const C& obj, Args&&... args) const {
        return (obj.*pmf)(std::forward<Args>(args)...);
    }

    R operator()(const C* obj, Args&&... args) const {
        return (obj->*pmf)(std::forward<Args>(args)...);
    }
};

// 重载：匹配非 const 成员函数
template <typename R, typename C, typename... Args>
mem_fn_wrapper<R, C, Args...>
mem_func(R (C::*pmf)(Args...)) {
    return mem_fn_wrapper<R, C, Args...>(pmf);
}

// 重载：匹配 const 成员函数 ✅ 这就是你缺的那个
template <typename R, typename C, typename... Args>
const_mem_fn_wrapper<R, C, Args...>
mem_func(R (C::*pmf)(Args...) const) {
    return const_mem_fn_wrapper<R, C, Args...>(pmf);
}

    
}