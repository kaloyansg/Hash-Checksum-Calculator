#pragma once
#include <mutex>
#include <condition_variable>
#include <atomic>
#include <iostream>

class PauseController {
private:
    std::mutex mtx;
    std::condition_variable cv;
    std::atomic<bool> isPaused;
    std::atomic<bool> stopRequested;

    const std::string BLUE = "\033[36m";
    const std::string RESET = "\033[0m";

public:
    PauseController() : isPaused(false), stopRequested(false) {}

    void pause();
    void resume();
    void stop();
    void checkWait(); // called by the worker thread to check if it should stop
};