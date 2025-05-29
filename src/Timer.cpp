// Timer.cpp
#include "Timer.h"

Timer::Timer()
    : m_startTime(0),
      m_currentTime(0),
      m_isRunning(false) {
}

void Timer::start() {
    if (!m_isRunning) {
        m_startTime = std::clock();
        m_isRunning = true;
    }
}

void Timer::stop() {
    if (m_isRunning) {
        m_currentTime = std::clock();
        m_isRunning = false;
    }
}

void Timer::reset() {
    m_startTime = m_isRunning ? std::clock() : 0;
    m_currentTime = m_startTime;
}

void Timer::update() {
    if (m_isRunning) {
        m_currentTime = std::clock();
    }
}

int Timer::getElapsedSeconds() const {
    return static_cast<int>((m_currentTime - m_startTime) / CLOCKS_PER_SEC);
}

bool Timer::isRunning() const {
    return m_isRunning;
}