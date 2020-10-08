#include <vector>
#include <set>

#include "automata.hpp"
#include "gtest/gtest.h"

using namespace std;
using namespace automata;

TEST(Automata, Fullifization) {
    const vector<Transition> transitions = {
        {0, 'a', 1},
        {0, 'b', 2},
        {1, 'a', 2}
    };

    const set<int> ends = {2};

    Automata testing;

    for (const auto& t: transitions)
        testing.insert_transition(t);

    for(const auto& e: ends)
        testing.insert_end(e);

    const vector<Transition> fullified_transitions = {
        {0, 'a', 1},
        {0, 'b', 2},
        {1, 'a', 2},
        {1, 'b', 3},
        {2, 'a', 3},
        {2, 'b', 3},
        {3, 'a', 3},
        {3, 'b', 3}
    };

    Automata fullified = testing.fullified();

    ASSERT_TRUE(fullified.get_transitions() == fullified_transitions);
    ASSERT_TRUE(fullified.get_ends() == ends);
}

TEST(Automata, Determinization) {
    const vector<Transition> transitions = {
        {0, 'a', 0},
        {0, 'b', 0},
        {0, 'a', 1},
        {0, 'a', 3},
        {3, 'b', 4},
        {4, 'a', 6},
        {1, 'a', 2},
        {2, 'a', 2},
        {2, 'b', 2},
        {2, 'b', 5},
        {5, 'b', 6},
        {6, 'b', 6},
        {6, 'a', 7},
        {7, 'b', 7}
    };
    const set<int> ends = {7};

    Automata testing;

    for (const auto& t: transitions)
        testing.insert_transition(t);

    for(const auto& e: ends)
        testing.insert_end(e);

    const vector<Transition> determined_transitions = {
        {0, 'a', 1},
        {0, 'b', 0},
        {1, 'a', 2},
        {1, 'b', 3},
        {2, 'a', 2},
        {2, 'b', 4},
        {3, 'a', 5},
        {3, 'b', 0},
        {4, 'a', 6},
        {4, 'b', 7},
        {5, 'a', 8},
        {5, 'b', 9},
        {6, 'a', 8},
        {6, 'b', 10},
        {7, 'a', 8},
        {7, 'b', 7},
        {8, 'a', 2},
        {8, 'b', 11},
        {9, 'a', 12},
        {9, 'b', 13},
        {10, 'a', 14},
        {10, 'b', 7},
        {11, 'a', 6},
        {11, 'b', 15},
        {12, 'a', 8},
        {12, 'b', 16},
        {13, 'a', 17},
        {13, 'b', 13},
        {14, 'a', 8},
        {14, 'b', 18},
        {15, 'a', 8},
        {15, 'b', 15},
        {16, 'a', 12},
        {16, 'b', 19},
        {17, 'a', 2},
        {17, 'b', 20},
        {18, 'a', 14},
        {18, 'b', 15},
        {19, 'a', 17},
        {19, 'b', 19},
        {20, 'a', 5},
        {20, 'b', 21},
        {21, 'a', 1},
        {21, 'b', 21}
    };

    const set<int> determined_ends = {8, 11, 12, 14, 15, 16, 17, 18, 19, 20, 21};

    Automata determined = testing.determinized();

    ASSERT_TRUE(determined.get_transitions() == determined_transitions);
    ASSERT_TRUE(determined.get_ends() == determined_ends);
}

TEST(Automata, Minimization) {
    const vector<Transition> transitions = {
        {0, 'a', 1},
        {0, 'b', 2},
        {1, 'a', 3},
        {1, 'b', 4},
        {2, 'a', 2},
        {2, 'b', 2},
        {3, 'a', 3},
        {3, 'b', 5},
        {4, 'a', 1},
        {4, 'b', 6},
        {5, 'a', 3},
        {5, 'b', 7},
        {6, 'a', 4},
        {6, 'b', 2},
        {7, 'a', 5},
        {7, 'b', 6}
    };
    const set<int> ends = {0, 1, 3, 5};

    Automata testing;

    for (const auto& t: transitions)
        testing.insert_transition(t);

    for(const auto& e: ends)
        testing.insert_end(e);

    const vector<Transition> minimized_transitions = {
        {0, 'a', 1},
        {0, 'b', 2},
        {1, 'a', 3},
        {1, 'b', 4},
        {2, 'a', 2},
        {2, 'b', 2},
        {3, 'a', 3},
        {3, 'b', 1},
        {4, 'a', 1},
        {4, 'b', 5},
        {5, 'a', 4},
        {5, 'b', 2}
    };

    const set<int> minimized_ends = {0, 1, 3};

    Automata minimized = testing.minimized();

    ASSERT_TRUE(minimized.get_transitions() == minimized_transitions);
    ASSERT_TRUE(minimized.get_ends() == minimized_ends);
}
