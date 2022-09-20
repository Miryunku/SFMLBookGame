#include "../pch.h"
#include "Command_queue.h"

void Command_queue::push(const Command& ccommand)
{
    command_queue.push(ccommand);
}

Command Command_queue::pop()
{
    Command command {command_queue.front()};
    command_queue.pop();
    return command;
}

bool Command_queue::is_empty() const
{
    return command_queue.empty();
}
