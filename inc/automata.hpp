#pragma once

#include <iostream>
#include <vector>
#include <map>
#include <set>
#include <deque>
#include <utility>
#include <algorithm>

namespace automata {

using std::vector;
using std::set;
using std::map;
using std::deque;
using std::pair;

struct Transition {
    const int from;
    const char guard;
    const int to;

    bool operator==(const Transition& other) const {
        return  (from == other.from) &&
                (guard == other.guard) &&
                (to == other.to);
    }
};

class Automata {
private:
    int start = 0;
    set<char> alphabet;
    vector<Transition> transitions;
    set<int> ends;

    inline set<int> get_neighboors(const set<int>& states, const char guard) const {
        set<int> neighboors;

        for (const auto& trans: transitions) {
            if (trans.guard != guard) continue;
            if (states.count(trans.from))
                neighboors.insert(trans.to);
        }

        return neighboors;
    }

    inline set<int> gen_states() const {
        set<int> states = {start};
        for (const auto& trans: transitions) {
            states.insert(trans.from);
            states.insert(trans.to);
        }

        return states;
    }
public:
    Automata(   const set<char>& alphabet = set<char>(),
                const vector<Transition>& transitions = vector<Transition>(),
                const set<int>& ends = set<int>()) :
        alphabet(alphabet),
        transitions(transitions),
        ends(ends)
    {
    }

    void insert_transition(const Transition& trans) {
        if (!alphabet.count(trans.guard)) {
            alphabet.insert(trans.guard);
            transitions.push_back(trans);
        }
        else if (find(  transitions.begin(),
                        transitions.end(),
                        trans   ) == transitions.end())
            transitions.push_back(trans);
    }

    void insert_end(const int& end) {
        ends.insert(end);
    }

    const vector<Transition>& get_transitions() const {
        return transitions;
    }

    const set<int>& get_ends() const {
        return ends;
    }

    /* NFA -> DFA */
    Automata determinized() const {
        vector<Transition> new_transitions;
        set<int> new_ends;

        /* New states - sets of previous states */
        set<set<int>> new_states = {{start}};
        /* Map new states to their new id */
        map<set<int>, int> new_ids = {
            {{start}, 0}
        };
        /* Count new states */
        int count = 1;

        deque<set<int>> queue = {{start}};
        while (!queue.empty()) {
            const auto cur = queue.front();
            queue.pop_front();

            const int& cur_id = new_ids.at(cur);
            /* Check if new state is end */
            for (const auto& state: cur)
                if (ends.count(state)) {
                    new_ends.insert(cur_id);
                    break;
                }

            /* Generate new states and transitions */
            for (const auto& guard: alphabet) {
                const auto& neighboors = get_neighboors(cur, guard);
                /* Create new state if there isn't */
                if (!new_states.count(neighboors)) {
                    new_ids.insert({neighboors, count++});
                    new_states.insert(neighboors);
                    queue.push_back(neighboors);
                }

                const int neighboors_id = new_ids.at(neighboors);
                /* Add transition */
                new_transitions.push_back({cur_id, guard, neighboors_id});
            }
        }

        return Automata(alphabet, new_transitions, new_ends);
    }

    /* DFA -> Full DFA */
    Automata fullified() const {
        const auto& states = gen_states();

        /* Map state to nonsaturated guards for it */
        map<int, set<char>> nonsaturated;
        for (const auto& state: states)
            nonsaturated.insert({state, alphabet});

        for (const auto& trans: transitions)
            nonsaturated.at(trans.from).erase(trans.guard);

        /* Check if automata is full already */
        bool is_full = true;
        for (const auto& [_, nonsat]: nonsaturated)
            if (!nonsat.empty()) {
                is_full = false;
                break;
            }

        if (is_full)
            return *this;

        /* Add new state with all necessary transitions to it */
        const int new_state = *states.rbegin() + 1;
        vector<Transition> new_transitions = transitions;

        for (const auto& [state, nonsat]: nonsaturated)
            for (const auto& guard: nonsat)
                new_transitions.push_back({state, guard, new_state});

        for (const auto& guard: alphabet)
            new_transitions.push_back({new_state, guard, new_state});

        return Automata(alphabet, new_transitions, ends);
    }

    /* Full DFA -> Inversed Full DFA */
    Automata inversed() const {
        const auto& states = gen_states();
        set<int> new_ends;

        std::set_difference(    states.begin(), states.end(),
                                ends.begin(), ends.end(),
                                std::inserter(new_ends, new_ends.begin()));

        return Automata(alphabet, transitions, new_ends);
    }

    /* Full DFA -> Minimal Full DFA */
    Automata minimized() {
        const auto& states = gen_states();

        /* Map state to its group */
        vector<int> group(states.size(), 0);
        for (const auto& state: ends)
            group.at(state) = 1;

        /* Map state + guard to next state */
        map<pair<int, char>, int> next;
        for (const auto& trans: transitions)
            next.insert({{trans.from, trans.guard}, trans.to});

        for (int i = 0; i < states.size(); ++i) {
            /* Map signature to group */
            map<vector<int>, int> sign2group;
            /* Map state to its new group */
            vector<int> new_group(states.size());
            int new_group_count = 0;

            /* Generate new groups */
            for (const auto& state: states) {
                vector<int> sign = {group.at(state)};
                for (const auto& guard: alphabet)
                    sign.push_back(
                        group.at(next.at({state, guard}))
                    );

                if (!sign2group.count(sign))
                    sign2group.insert({sign, new_group_count++});

                const int& group_id = sign2group.at(sign);
                new_group.at(state) = group_id;
            }

            /* If groups didn't change - stop */
            if (group == new_group)
                break;

            group = new_group;
        }

        /* Generate new transitions based on previous */
        vector<Transition> new_transitions;
        for (const auto& trans: transitions) {
            const Transition new_trans = {
                group.at(trans.from), trans.guard, group.at(trans.to)
            };
            if (find(   new_transitions.begin(),
                        new_transitions.end(),
                        new_trans) == new_transitions.end())
                new_transitions.push_back(new_trans);
        }

        /* Generate new ends based on previous */
        set<int> new_ends;
        for (const auto& end: ends)
            new_ends.insert(group.at(end));

        return Automata(alphabet, new_transitions, new_ends);
    }
};

} // namespace automata

std::istream& operator>>(std::istream& in, automata::Automata& automata) {
    int transitions_count = 0;
    in >> transitions_count;

    while (transitions_count--) {
        int from = 0, to = 0;
        char guard = 0;
        in >> from >> guard >> to;
        automata.insert_transition({from, guard, to});
    }

    int ends_count = 0;
    in >> ends_count;

    while (ends_count--) {
        int end = 0;
        in >> end;
        automata.insert_end(end);
    }

    return in;
}

std::ostream& operator<<(std::ostream& out, const automata::Automata& automata) {
    const auto& transitions = automata.get_transitions();
    out << transitions.size() << "\n";
    for (const auto& trans: transitions)
        out << trans.from << " " << trans.guard << " " << trans.to << "\n";

    const auto& ends = automata.get_ends();
    out << ends.size() << "\n";
    for (const int& end: ends)
        out << end << " ";
    out << "\n";

    return out;
}
