#include "../pch.h"
#include "State_stack.h"

State_stack::Pending_change::Pending_change(Action aaction, Id_states iid_state)
    : action {aaction},
    id_state {iid_state}
{
    //empty by default
}

State_stack::State_stack(State::Context ccontext)
    : context {ccontext}
{
    //empty by default
}

void State_stack::handle_event(const sf::Event& eevent)
{
    //iterate from top to bottom, stop as soon as handle_event() returns false
    for(std::vector<std::unique_ptr<State>>::reverse_iterator riter {stack.rbegin()}; riter != stack.rend(); ++riter) {
        if(!(*riter)->handle_event(eevent)) {
            break;
        }
    }
    apply_pending_changes();
}

void State_stack::update(sf::Time ffixed_timestep)
{
    //iterate from top to bottom, stop as soon as update() returns false
    for (std::vector<std::unique_ptr<State>>::reverse_iterator riter{ stack.rbegin() }; riter != stack.rend(); ++riter) {
        if (!(*riter)->update(ffixed_timestep)) {
            break;
        }
    }
    apply_pending_changes();
}

void State_stack::draw()
{
    //draw all active states
    for(std::unique_ptr<State>& ss : stack) {
        ss->draw();
    }
}

void State_stack::push_state(Id_states iid_state)
{
    pending_list.push_back(Pending_change {Action::push, iid_state});
}

void State_stack::pop_state()
{
    pending_list.push_back(Pending_change {Action::pop});
}

void State_stack::clear_states()
{
    pending_list.push_back(Pending_change {Action::clear});
}

bool State_stack::is_empty() const
{
    return stack.empty();
}

std::unique_ptr<State> State_stack::create_state(Id_states iid_state)
{
    std::map<Id_states, std::function<std::unique_ptr<State>()>>::iterator iter{ factories.find(iid_state) };
    if(iter == factories.end()) {
        throw std::logic_error {"State_stack::create_state"};
    }
    return iter->second();
}

void State_stack::apply_pending_changes()
{
    for(Pending_change change : pending_list) {
        switch(change.action) {
        case Action::push:
            stack.push_back(create_state(change.id_state));
            break;
        case Action::pop:
            stack.pop_back();
            break;
        case Action::clear:
            stack.clear();
            break;
        }
    }
    pending_list.clear();
}
