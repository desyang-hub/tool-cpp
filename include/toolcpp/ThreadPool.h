#pragma once

#include <thread>
#include <functional>
#include <vector>
#include <queue>
#include <mutex>
#include <future>
#include <condition_variable>

namespace toolcpp {


class ThreadPool
{
    using Job = std::function<void()>;
private:
    std::vector<std::thread> m_workers;
    std::queue<Job> m_jobs;

    std::mutex m_mutex;
    std::condition_variable m_condition;

    bool is_running{true};

public:
    ThreadPool(size_t pool_size = 1);
    ~ThreadPool();

public:
    template<class F, class ...Args>
    auto enqueue(F&& f, Args&& ...args) 
    -> std::future<typename std::result_of<F(Args...)>::type>;
};


template<class F, class ...Args>
auto ThreadPool::enqueue(F&& f, Args&& ...args) 
-> std::future<typename std::result_of<F(Args...)>::type> {

    using return_type = typename std::result_of<F(Args...)>::type;

    auto task = std::make_shared<std::packaged_task<return_type()>>(
        std::bind(std::forward<F>(f), std::forward<Args>(args)...)
    );

    auto fut = task->get_future();

    {
        std::lock_guard<std::mutex> lock(m_mutex);

        if (!is_running) {
            throw std::runtime_error("enqueue in stoped thread pool.");
        }

        m_jobs.push([task]{
            (*task)();
        });
    }
    m_condition.notify_one();

    return fut;
}

    
}