#pragma once

#include "toolcpp/memory/default_deletor.h"

namespace toolcpp {

template<class T, class deletor = default_deletor<T>>
class unique_ptr : public deletor {
private:
    T* m_data;

    using pointer = T*;
    using const_pointer = const T*;
    using reference = T&;
    using const_reference = const T&;

public:
    explicit unique_ptr(T* data_ptr = nullptr) : m_data(data_ptr) {

    }

    unique_ptr(const unique_ptr&) = delete;
    unique_ptr& operator=(const unique_ptr&) = delete;

    unique_ptr(unique_ptr&&) = default;
    unique_ptr& operator=(unique_ptr&&) = default;

    ~unique_ptr() {
        deletor::operator()(m_data);
        m_data = nullptr;
    }

public:
    operator bool() const {
        return m_data != nullptr;
    }

    reference operator*() {
        return *m_data;
    }

    const_reference operator*() const {
        return *m_data;
    }

    pointer operator->() {
        return m_data;
    }

    const_pointer operator->() const {
        return m_data;
    }

    void reset(pointer p = nullptr) {
        m_data = p;
    }

    pointer get() {
        return m_data;
    }

    const_pointer get() const {
        return m_data;
    }

    pointer release() {
        pointer tmp = m_data;
        m_data = nullptr;
        return tmp;
    }
};


template<class T, class deletor>
class unique_ptr<T[], deletor> {
private:
    T* m_data;
    size_t m_size;

    using pointer = T*;
    using const_pointer = const T*;
    using reference = T&;
    using const_reference = const T&;

    using iterator = T*;
    using const_iterator = const T*;

public:
    explicit unique_ptr(T* data_ptr = nullptr, size_t n = 0) : m_data(data_ptr), m_size(n) {

    }

    unique_ptr(const unique_ptr&) = delete;
    unique_ptr& operator=(const unique_ptr&) = delete;

    unique_ptr(unique_ptr&&) noexcept = default;
    unique_ptr& operator=(unique_ptr&&) noexcept = default;

    ~unique_ptr() {
        deletor{}(m_data);
        m_data = nullptr;
    }

public:
    iterator begin() {
        return m_data;
    }

    iterator end() {
        return m_data + m_size;
    }

    const_iterator begin() const {
        return m_data;
    }

    const_iterator end() const {
        return m_data + m_size;
    }

    size_t size() const {
        return m_size;
    }

    reference operator*() {
        return *m_data;
    }

    const_reference operator*() const {
        return *m_data;
    }

    reference operator[](size_t i) {
        return m_data[i];
    }

    const_reference operator[](size_t i) const {
        return m_data[i];
    }

    pointer operator->() {
        return m_data;
    }

    const_pointer operator->() const {
        return m_data;
    }

    void reset(pointer p = nullptr) {
        m_data = p;
    }

    pointer get() {
        return m_data;
    }

    const_pointer get() const {
        return m_data;
    }

    pointer release() {
        pointer tmp = m_data;
        m_data = nullptr;
        return tmp;
    }
};

// 1. 非数组：make_unique<test>(args...)
template<class T, class... Args>
typename std::enable_if<!std::is_array<T>::value, unique_ptr<T>>::type
make_unique(Args&&... args) {
    return unique_ptr<T>(new T(std::forward<Args>(args)...));
}

// 2. 数组 T[]：make_unique<test[]>(n)
template<class T>
typename std::enable_if<
    std::is_array<T>::value && std::extent<T>::value == 0,
    unique_ptr<T>
>::type
make_unique(std::size_t n) {
    using Elem = typename std::remove_extent<T>::type;
    return unique_ptr<T>(new Elem[n](), n);
}

    
}