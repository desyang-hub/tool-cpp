#include "toolcpp/ThreadPool.h"

using namespace toolcpp;

ThreadPool::ThreadPool(size_t pool_size)
{
    m_workers.reserve(pool_size);

    for (int i = 0; i < pool_size; ++i) {
        m_workers.emplace_back([this] {

            Job job;

            // 一直尝试从任务队列中取出任务来执行
            while (true) {
                std::unique_lock<std::mutex> lock(m_mutex);
                m_condition.wait(lock, [this]{
                    return !m_jobs.empty() || !is_running;
                });

                // 不管有没有停止，只要有任务就继续执行任务
                if (!m_jobs.empty()) {
                    job = std::move(m_jobs.front());
                    m_jobs.pop();
                } else {
                    // 没有任务，且停止了，退出
                    break;
                }

                // 任务执行需要进行异常捕获，这里不捕获，任务内部进行捕获即可
                job();
            }

        });
    }
}

ThreadPool::~ThreadPool()
{
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        // 如果已经停止直接退出即可
        if (!is_running) return;
        is_running = false;
    }

    // 通过所有进程，当前线程即将进入关闭状态
    m_condition.notify_all();

    // 所有线程join
    for (auto& woker : m_workers) {
        if (woker.joinable()) {
            woker.join();
        }
    }
}
