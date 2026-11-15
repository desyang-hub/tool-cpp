#pragma once

#include "control_block.h"

// 前置声明
class weak_ptr;

namespace toolcpp {

template<class T>
class shared_ptr {
    friend class weak_ptr;

private:
    T* m_data;
    control_block<T>* m_ctl_block;

    using pointer = T*;
    using const_pointer = const T*;
    using reference = T&;
    using const_reference = const T&;

    // 资源回收
    void destroy() noexcept {
        // 1. 判断数据是否有效
        if (!m_data && !m_ctl_block) return;

        // 2. 判断引用计数
        if (m_ctl_block->subStrongCount() == 0) {
            m_ctl_block->Dispose();
            if (m_ctl_block->subWeakCount() == 0) {
                m_ctl_block->Delete();
            }
        }
    }
public:

    explicit shared_ptr(pointer ptr = nullptr) : m_data(ptr), m_ctl_block(nullptr) {
        if (!ptr) return;
        try
        {
            m_ctl_block = new separate_control_block<T>(ptr);
        }
        catch(const std::exception& e)
        {
            delete ptr;
            throw;
        }
        
    }

    shared_ptr(pointer ptr, control_block<T>* ctrl_block) : m_data(ptr), m_ctl_block(ctrl_block) {

    }

    shared_ptr(const shared_ptr& other) {
        other.m_ctl_block->addStrongCount();
        m_data = other.m_data;
        m_ctl_block = other.m_ctl_block;
    }

    shared_ptr& operator=(const shared_ptr& other) {
        if (this == &other) {
            return *this;
        }

        destroy();
        other.m_ctl_block->addStrongCount();
        m_data = other.m_data;
        m_ctl_block = other.m_ctl_block;
        return *this;
    }

    shared_ptr(shared_ptr&& other) {
        m_data = other.m_data;
        m_ctl_block = other.m_ctl_block;
        other.m_data = nullptr;
        other.m_ctl_block = nullptr;
    }

    shared_ptr& operator=(shared_ptr&& other) {
        if (this == &other) {
            return *this;
        }

        destroy();
        m_data = other.m_data;
        m_ctl_block = other.m_ctl_block;
        other.m_data = nullptr;
        other.m_ctl_block = nullptr;
        return *this;
    }

    ~shared_ptr() {
        // 判断引用计数
        destroy();
    }

public:
    operator bool() const {
        return m_data != nullptr;
    }

    reference get() {
        return m_data;
    }

    const_reference get() const {
        return m_data;
    }

    reference operator->() {
        return m_data;
    }

    const_reference operator->() const {
        return m_data;
    }

    reference operator*() {
        return *m_data;
    }

    const_reference operator*() const {
        return *m_data;
    }



    size_t use_count() const {
        if (!m_data) return 0;
        return m_ctl_block->getStrongCount();
    }
};

template<class T>
class shared_ptr<T[]> {
private:
    T* m_data;
    control_block<T[]>* m_ctl_block;

    using pointer = T*;
    using const_pointer = const T*;
    using reference = T&;
    using const_reference = const T&;

    // 资源回收
    void destroy() noexcept {
        // 1. 判断数据是否有效
        if (!m_data && !m_ctl_block) return;

        // 2. 判断引用计数
        if (m_ctl_block->subStrongCount() == 0) {
            m_ctl_block->Dispose();
            if (m_ctl_block->subWeakCount() == 0) {
                m_ctl_block->Delete();
            }
        }
    }
public:

    explicit shared_ptr(pointer ptr = nullptr) : m_data(ptr), m_ctl_block(nullptr) {
        if (!ptr) return;
        try
        {
            m_ctl_block = new separate_control_block<T[]>(ptr);
        }
        catch(const std::exception& e)
        {
            delete ptr;
            throw;
        }
    }

    shared_ptr(pointer ptr, control_block<T[]>* ctrl_block) : m_data(ptr), m_ctl_block(ctrl_block) {

    }

    shared_ptr(const shared_ptr& other) {
        other.m_ctl_block->addStrongCount();
        m_data = other.m_data;
        m_ctl_block = other.m_ctl_block;
    }

    shared_ptr& operator=(const shared_ptr& other) {
        if (this == &other) {
            return *this;
        }

        destroy();
        other.m_ctl_block->addStrongCount();
        m_data = other.m_data;
        m_ctl_block = other.m_ctl_block;
        return *this;
    }

    shared_ptr(shared_ptr&& other) {
        m_data = other.m_data;
        m_ctl_block = other.m_ctl_block;
        other.m_data = nullptr;
        other.m_ctl_block = nullptr;
    }

    shared_ptr& operator=(shared_ptr&& other) {
        if (this == &other) {
            return *this;
        }

        destroy();
        m_data = other.m_data;
        m_ctl_block = other.m_ctl_block;
        other.m_data = nullptr;
        other.m_ctl_block = nullptr;
        return *this;
    }

    ~shared_ptr() {
        // 判断引用计数
        destroy();
    }

public:

    operator bool() const {
        return m_data != nullptr;
    }

    size_t use_count() const {
        if (!m_data) return 0;
        return m_ctl_block->getStrongCount();
    }
};

template<class T, class ...Args>
typename std::enable_if<!std::is_array<T>::value, shared_ptr<T>>::type
make_shared(Args&& ...args) {
    // 申请一片空间
    constexpr size_t storge = sizeof(combined_control_block<T>) + sizeof(T);
    auto addr = new char[storge];
    auto ptr = new (addr + sizeof(combined_control_block<T>)) T(std::forward<Args>(args)...);
    auto ctrl_block = new (addr) combined_control_block<T>(ptr);
    return shared_ptr<T>(ptr, ctrl_block);
}

// 用于T = Elem[] 数组类型
template<class T>
typename std::enable_if<std::is_array<T>::value, shared_ptr<T>>::type
make_shared(size_t n) {
    using Elem = typename std::remove_extent<T>::type;
    // 申请空间
    size_t storge = sizeof(combined_control_block<T>) + sizeof(Elem) * n;

    auto addr = new char[storge];
    auto ptr = new (addr + sizeof(combined_control_block<T>)) Elem[n];
    auto ctrl_block = new (addr) combined_control_block<T>(ptr, n);
    return shared_ptr<T>(ptr, ctrl_block);
};

}


