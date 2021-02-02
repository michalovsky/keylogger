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
    Timer(std::function<void(void)> f, unsigned long i, long repeat = infinite);

    void start(bool async = true);
    void stop();
    bool isAlive() const;
    void setTimerCallback(const std::function<void(void)>& f);
    void setTotalNumberOfCalls(long num);
    void setInterval(unsigned long i);

    static const long infinite{-1};

private:
    std::thread thread;
    bool alive{false};
    long totalNumberOfCalls{-1};
    long callsCounter{-1};
    std::chrono::milliseconds interval{0};
    std::function<void(void)> callback{nullptr};
};
}
