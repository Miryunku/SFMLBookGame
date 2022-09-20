#pragma once

#include "Command.h"

class Command_queue {
public:
    void push(const Command& ccommand);
    Command pop();
    bool is_empty() const;
private:
    std::queue<Command> command_queue;
};
