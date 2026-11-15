#pragma once

#include <type_traits>
#include <iterator>
#include <vector>

namespace toolcpp {

// 模板函数
// template<class Container, class MapRule>
// auto mapping(Container&& c, MapRule rule) {
//     size_t size = 0;
//     for (const auto& v : c) ++size;

//     using T = int; // get type of rule return;
//     auto result = make_unique<T[]>(size);
//     int i = 0;
//     for (const auto& v : c) {
//         result[i] = rule(v);
//         ++i;
//     }
// }


// usage mapping(arr, [](const T& item) { return int(item);})
// return: unique_ptr<int>[n];
#if c_plus_plus <= 201103
template<class Container, class MapRule>
auto mapping(Container&& c, MapRule rule)
    -> std::vector<typename std::decay<decltype(rule(*std::begin(c)))>::type>
{
    using T = typename std::decay<decltype(rule(*std::begin(c)))>::type;

    const std::size_t size = std::distance(std::begin(c), std::end(c));
    std::vector<T> result;
    result.reserve(size);

    for (const auto& v : c) {
        result.emplace_back(rule(v));
    }
    return result;
}
#elif c_plus_plus < 201412
template<class Container, class MapRule>
auto mapping(Container&& c, MapRule rule) {
    using T = std::decay_t<decltype(rule(*std::begin(c)));
    // 或：using T = std::decay_t<std::invoke_result_t<MapRule, decltype(*std::begin(c))>>;

    const size_t size = std::distance(std::begin(c), std::end(c));
    std::vector<T> result;
    result.reserve(size);

    for (const auto& v : c) {
        result.emplace_back(rule(v));
    }
    return result;   // ← 别忘了返回
}
#endif

}


