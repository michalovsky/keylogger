#include "Timer.h"

namespace keylogger
{
Timer::Timer(std::function<void(void)> f) : callback{std::move(f)} {}
Timer::Timer(std::function<void(void)> f, const unsigned long i, const long repeat)
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
        thread = std::thread(&Timer::threadFunc, this);
    }
    else
    {
        threadFunc();
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

void Timer::setTotalNumberOfCalls(const long num)
{
    if (isAlive())
    {
        return;
    }
    totalNumberOfCalls = num;
}

void Timer::setInterval(const unsigned long i)
{
    if (isAlive())
    {
        return;
    }
    interval = std::chrono::milliseconds(i);
}
}
