#include "toolcpp/Thread.h"

using namespace toolcpp;

Thread::~Thread()
{
    if (m_worker.joinable()) m_worker.join();
}