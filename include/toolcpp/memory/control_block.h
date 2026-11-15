#pragma once

#include <atomic>
#include <cstddef>

namespace toolcpp {


// 引用计数块
template<class T>
class control_block {
protected:
    std::atomic_long m_strong{1};
    std::atomic_long m_weak{0};
    T* m_data;

public:
    virtual void Dispose() = 0; // 负责销毁对象
    virtual void Delete() = 0; // 负责回收内存

    control_block(T* ptr) : m_data(ptr) {

    }

    bool try_add_ref() noexcept {
        long count = m_strong.load(std::memory_order_acq_rel);
        
        while (count != 0) {
            if (m_strong.compare_exchange_weak(
                count, 
                count + 1,
                std::memory_order_acq_rel, // 成功
                std::memory_order_acquire // 失败继续读
            )) {
                return true;
            }
        }

        return false;
    }



    size_t addStrongCount() {
        m_strong.fetch_add(1, std::memory_order::memory_order_acquire);
        return m_strong.load();
    }

    size_t subStrongCount() {
        m_strong.fetch_sub(1, std::memory_order::memory_order_acquire);
        return m_strong.load();
    }

    size_t getStrongCount() const {
        return m_strong.load();
    }

    size_t addWeakCount() {
        m_weak.fetch_add(1, std::memory_order::memory_order_acquire);
        return m_strong.load();
    }

    size_t subWeakCount() {
        m_weak.fetch_sub(1, std::memory_order::memory_order_acquire);
        return m_strong.load();
    }

    size_t getWeakCount() const {
        return m_weak.load();
    }
};

template<class T>
class control_block<T[]> {
protected:
    std::atomic_int m_strong{1};
    std::atomic_int m_weak{0};
    T* m_data;

public:
    virtual void Dispose() = 0; // 负责销毁对象
    virtual void Delete() = 0; // 负责回收内存

    control_block(T* ptr) : m_data(ptr) {

    }

    size_t addStrongCount() {
        m_strong.fetch_add(1, std::memory_order::memory_order_acquire);
        return m_strong.load();
    }

    size_t subStrongCount() {
        m_strong.fetch_sub(1, std::memory_order::memory_order_acquire);
        return m_strong.load();
    }

    size_t getStrongCount() const {
        return m_strong.load();
    }

    size_t addWeakCount() {
        m_weak.fetch_add(1, std::memory_order::memory_order_acquire);
        return m_strong.load();
    }

    size_t subWeakCount() {
        m_weak.fetch_sub(1, std::memory_order::memory_order_acquire);
        return m_strong.load();
    }

    size_t getWeakCount() const {
        return m_weak.load();
    }
};

// 单独的计数块
template<class T>
class separate_control_block : public control_block<T> {
public:
    separate_control_block(T* ptr = nullptr) : control_block<T>(ptr) {}
    
public:
    void Dispose() override {
        delete this->m_data; // delete 
    }
    void Delete() override {
        delete this;
    }
};

template<class T>
class combined_control_block : public control_block<T> {
public:
    combined_control_block(T* ptr = nullptr) : control_block<T>(ptr) {}
public:
    void Dispose() override {
        // 只调用析构函数
        reinterpret_cast<T*>(this->m_data)->~T(); // 调用析构函数
    }
    void Delete() override {
        delete[] reinterpret_cast<char*>(this);
    }
};

template<class T>
class combined_control_block<T[]> : public control_block<T[]> {
private:
    size_t m_n;
public:
    combined_control_block(T* ptr, size_t n) : control_block<T[]>(ptr), m_n(n) {}
    void Dispose() override {
        T* p = this->m_data;
        for (size_t i = 0; i < m_n; ++i) p[i].~T();
    }
    void Delete() override {
        delete[] reinterpret_cast<char*>(this);
    }
};
    
}