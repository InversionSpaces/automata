#include <iostream>

#include "automata.hpp"

using namespace std;

int main() {
    Automata a1;
    cin >> a1;
    Automata a2 = a1.determinized().fullified();
    cout << "Full DFA: \n" << a2;
    cout << "Minimized Full DFA:\n" << a2.minimized();
}
