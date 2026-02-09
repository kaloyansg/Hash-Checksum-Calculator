#include "PauseController.h"

void PauseController::pause() {
    std::cout << BLUE << "\t[System] Pausing..." << RESET << std::endl;
    std::lock_guard<std::mutex> lock(mtx);
    isPaused = true;
}

void PauseController::resume() {
    std::cout << BLUE << "\t[System] Resuming..." << RESET << std::endl;
    {
        std::lock_guard<std::mutex> lock(mtx);
        isPaused = false;
    }
    cv.notify_all();
}

void PauseController::stop()
{
    {
        std::lock_guard<std::mutex> lock(mtx);
        stopRequested = true;
        isPaused = false;
    }
    cv.notify_all();
}

void PauseController::checkWait() {
    std::unique_lock<std::mutex> lock(mtx);

    cv.wait(lock, [this] {
        return !isPaused.load() || stopRequested.load();
        });

    if (stopRequested.load()) {
        throw std::runtime_error("Scan Terminated");
    }
}
