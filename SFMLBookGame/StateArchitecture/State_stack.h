#pragma once

#include "State.h"

class State_stack : private sf::NonCopyable {
public:
    enum class Action {
        push, pop, clear
    };

    explicit State_stack(State::Context ccontext);
    
    template<typename T>
    void register_state(Id_states iid_state);
    template<typename T, typename Param1>
    void register_state(Id_states iid_states, Param1 aarg1);
    //----
    void handle_event(const sf::Event& eevent);
    void update(sf::Time ffixed_timestep);
    void draw();
    //----
    void push_state(Id_states iid_state);
    void pop_state();
    void clear_states();
    //----
    bool is_empty() const;
private:
    struct Pending_change {
        Pending_change(Action aaction, Id_states iid_state = Id_states::none);

        Action action;
        Id_states id_state;
    };

    std::unique_ptr<State> create_state(Id_states iid_state);
    void apply_pending_changes();

    std::vector<std::unique_ptr<State>> stack;
    std::vector<Pending_change> pending_list;
    State::Context context;
    std::map<Id_states, std::function<std::unique_ptr<State>()>> factories;
};

template<typename T>
void State_stack::register_state(Id_states iid_state)
{
    factories[iid_state] = [this]()
    {
        return std::unique_ptr<State> {std::make_unique<T>(*this, context)};
    };
}

template<typename T, typename Param1>
void State_stack::register_state(Id_states iid_state, Param1 aarg1)
{
    factories[iid_state] = [this]()
    {
        return std::unique_ptr<State>{ std::make_unique<T>(*this, context, aarg1) };
    };
}
