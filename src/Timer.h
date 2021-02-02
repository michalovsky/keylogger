#pragma once

#include <chrono>
#include <functional>
#include <thread>
#include <utility>

namespace keylogger
{
class Timer
{
public:
    Timer() = default;
    Timer(std::function<void(void)> f);
    Timer(std::function<void(void)> f, const unsigned long i, const long repeat = infinite);

    void start(bool async = true);
    void stop();
    bool isAlive() const;
    void setTimerCallback(const std::function<void(void)>& f);
    void setTotalNumberOfCalls(const long num);
    void setInterval(const unsigned long i);

    static const long infinite{-1};

private:
    void sleepAndRun();
    void threadFunc();

    std::thread thread;
    bool alive{false};
    long totalNumberOfCalls{-1};
    long callsCounter{-1};
    std::chrono::milliseconds interval{0};
    std::function<void(void)> callback{nullptr};
};
}
