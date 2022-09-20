#include "../pch.h"
#include "Parallel_task.h"

Parallel_task::Parallel_task()
    : thread {&Parallel_task::run_task, this},
    finished {false}
{
    //empty by default
}

void Parallel_task::execute()
{
    finished = false;
    elapsed_time.restart();
    thread.launch();
}

bool Parallel_task::is_finished()
{
    sf::Lock lock {mutex};
    return finished;
}

float Parallel_task::get_completion()
{
    sf::Lock lock {mutex};
    //100% at 10 seconds of elapsed time
    return elapsed_time.getElapsedTime().asSeconds() / 10.0f;
}

void Parallel_task::run_task()
{
    bool ended {false};
    while(!ended) {
        sf::Lock lock {mutex}; //protect the clock
        if(elapsed_time.getElapsedTime().asSeconds() >= 10.0f) { //stall after 10 seconds
            ended = true;
        }
    }
    sf::Lock lock {mutex};
    finished = true;
}
