#pragma once

#include "control_block.h"
#include "shared_ptr.h"

namespace toolcpp {

template<class T>
class weak_ptr
{
private:
    control_block<T>* m_ctl_block;
    T* m_data;

public:
    weak_ptr(shared_ptr<T>& sp) : m_data(sp.m_data) {
        // 先进行弱引用计数增加
        sp.m_ctl_block->addWeakCount();
        // 持有弱引用计数
        m_ctl_block = sp.m_ctl_block;
    }

    shared_ptr<T> lock() {
        if (!m_ctl_block) return shared_ptr<T>{};
        // 增加引用计数
        bool ok = m_ctl_block->try_add_ref();
        if (ok) {
            return shared_ptr<T>{m_data, m_ctl_block};
        }

        return shared_ptr<T>{};
    }

    ~weak_ptr() {

    }
};


}