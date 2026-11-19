#pragma once

#include <vector>
#include <stdexcept>

namespace toolcpp {

// 堆通常使用数组来进行存储
// 先写一个最简单的 只用于存放int类型的堆

const int HEAP_INIT_SIZE = 1024;

class IntHeap {
private:
    size_t m_capacity;
    size_t m_size;
    std::vector<int> m_nums;

    using Elem = int;

private:
    // 当前这个下标是新插入的，需要对整个上方进行重新检查
    void update(int i) {

        while (true) {
            if (!i) break;

            int head_id = (i+1)/2 - 1;

            if (m_nums[head_id] > m_nums[i]) {
                std::swap(m_nums[head_id], m_nums[i]);
                i = head_id;
            } else {
                break;
            }
        }

    }

    // 当前头节点失效，需要进行处理
    void adjust() {
        int i = 0;

        while (true) {
            int left_id = i*2+1;
            
            if (left_id + 1 >= m_size) break;

            int right_id = left_id+1;
    
            int id;
            if (right_id + 1 >= m_size || m_nums[left_id] < m_nums[right_id]) {
                id = left_id;
            } else {
                id = right_id;
            }
    
            if (m_nums[i] > m_nums[id]) {
                std::swap(m_nums[i], m_nums[id]);
                i = id;
            } else {
                break;
            }
        }

    }

public:
    IntHeap(std::size_t capacity = HEAP_INIT_SIZE) : m_capacity(capacity), m_size(0), m_nums(capacity) {

    }

public:
    size_t capacity() const {
        return m_capacity;
    }

    size_t size() const {
        return m_size;
    }

    bool empty() const {
        return !m_size;
    }

    // 将元素放入
    void push(Elem elem) {
        if (m_size == m_capacity) {
            throw std::runtime_error("heap is full.");
        }

        m_nums[m_size] = elem;
        update(m_size);
        ++m_size;
    }

    // 将堆顶的元素弹出，并将最后一个元素放到这个位置，并通过交换来保证整体无异常
    Elem pop() {
        if (!m_size) {
            throw std::runtime_error("heap is empty.");
        }

        if (m_size == 1) {
            --m_size;
            return m_nums[0];
        }

        Elem elem = m_nums[0];

        std::swap(m_nums[0], m_nums[m_size-1]);
        --m_size;
        adjust();

        return elem;
    }
};


// 模板内模板
template<
    class Elem,
    class Container = std::vector<Elem>,
    class Compare = std::less<typename Container::value_type>
>
class priority_queue
{
private:
    Container m_nums;
    Compare m_comp;

private:
    // 当前这个下标是新插入的，需要对整个上方进行重新检查
    void update(int i) {

        while (true) {
            if (!i) break;

            int head_id = (i+1)/2 - 1;

            if (m_comp(m_nums[head_id], m_nums[i])) {
                std::swap(m_nums[head_id], m_nums[i]);
                i = head_id;
            } else {
                break;
            }
        }

    }

    // 当前头节点失效，需要进行处理
    void adjust() {
        int i = 0;
        int m_size = m_nums.size();

        while (true) {
            int left_id = i*2+1;
            if (left_id + 1 > m_size) break;

            int right_id = left_id+1;
    
            int id;
            if (right_id + 1 > m_size || m_comp(m_nums[right_id], m_nums[left_id])) {
                id = left_id;
            } else {
                id = right_id;
            }
    
            if (m_comp(m_nums[i], m_nums[id]) ) {
                std::swap(m_nums[i], m_nums[id]);
                i = id;
            } else {
                break;
            }
        }

    }

public:
    priority_queue(std::size_t capacity = HEAP_INIT_SIZE) {
        m_nums.reserve(capacity);
    }

public:
    size_t capacity() const {
        return m_nums.compacity();
    }

    size_t size() const {
        return m_nums.size();
    }

    bool empty() const {
        return m_nums.empty();
    }

    // 将元素放入
    void push(Elem elem) {
        int size = m_nums.size();
        m_nums.emplace_back(elem);
        update(size);
    }

    // 将堆顶的元素弹出，并将最后一个元素放到这个位置，并通过交换来保证整体无异常
    Elem pop() {
        int size = m_nums.size();
        if (!size) {
            throw std::runtime_error("heap is empty.");
        }

        if (size == 1) {
            m_nums.pop_back();
            return m_nums[0];
        }

        Elem elem = m_nums[0];

        std::swap(m_nums[0], m_nums[size-1]);
        m_nums.pop_back();
        adjust();

        return elem;
    }
};


}