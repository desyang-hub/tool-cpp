#pragma once

#include <thread>

namespace toolcpp {


class Thread
{
private:
    std::thread m_worker;
public:
    template<class Job>
    Thread(Job job);

    ~Thread();
};


template<class Job>
Thread::Thread(Job job) : m_worker(std::move(job)) {

}

    
}
