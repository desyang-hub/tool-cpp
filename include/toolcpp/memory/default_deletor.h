#pragma once

namespace toolcpp {

template<class T>
struct default_deletor {
    void operator()(T* t) noexcept {
        delete t;
    }
};

template<class T>
struct default_deletor<T[]>
{
    void operator()(T* t) noexcept {
        delete[] t;
    }
};

}


