#include <iostream>
#include <fstream>
#include <string>

#include "automata.hpp"

using namespace std;

int main(int argc, char* argv[]) {
    if (argc != 2) {
        cout << "Usage: " << argv[0] << " AUTOMATA_FILE" << endl;
        return 0;
    }

    Automata a;
    {
        ifstream in(argv[1]);
        in >> a;
    }

    string command;
    cout << ">> ";
    while (cin >> command) {
        if (command == "print")
            cout << a;
        else if (command == "exit")
            return 0;
        else if (command == "determine")
            a = a.determinized();
        else if (command == "fullify")
            a = a.fullified();
        else if (command == "inverse")
            a = a.inversed();
        else if (command == "minimize")
            a = a.minimized();
        else
            cout << "Unknown command\n";
        cout << ">> ";
    }

    return 0;
}
