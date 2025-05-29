// Timer.h
#pragma once

#include <ctime>

class Timer {
public:
    Timer();

    void start();
    void stop();
    void reset();
    void update();

    int getElapsedSeconds() const;
    bool isRunning() const;

private:
    std::clock_t m_startTime;
    std::clock_t m_currentTime;
    bool m_isRunning;
};