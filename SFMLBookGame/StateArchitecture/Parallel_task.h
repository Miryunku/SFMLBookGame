#pragma once

class Parallel_task {
public:
    Parallel_task();

    void execute();
    bool is_finished();
    float get_completion();
private:
    void run_task();

    sf::Thread thread;
    bool finished;
    sf::Clock elapsed_time;
    sf::Mutex mutex;
};
