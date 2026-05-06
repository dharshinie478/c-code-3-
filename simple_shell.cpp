#include <iostream>
#include <unistd.h>
#include <sys/wait.h>
#include <sstream>
#include <vector>

using namespace std;

// Function to split command into arguments
vector<char*> parseCommand(string input) {
    vector<char*> args;
    stringstream ss(input);
    string temp;

    while (ss >> temp) {
        char* arg = new char[temp.size() + 1];
        copy(temp.begin(), temp.end(), arg);
        arg[temp.size()] = '\0';
        args.push_back(arg);
    }

    args.push_back(NULL); // execvp needs NULL at end
    return args;
}

int main() {
    string input;

    while (true) {
        cout << "myShell> ";
        getline(cin, input);

        // Exit condition
        if (input == "exit") {
            cout << "Exiting shell...\n";
            break;
        }

        if (input.empty()) {
            continue;
        }

        vector<char*> args = parseCommand(input);

        pid_t pid = fork(); // create process

        if (pid < 0) {
            cerr << "Error: Fork failed!\n";
        }
        else if (pid == 0) {
            // Child process
            if (execvp(args[0], args.data()) < 0) {
                cerr << "Error: Command not found!\n";
            }
            exit(1);
        }
        else {
            // Parent process
            wait(NULL);
        }

        // Free memory
        for (char* arg : args) {
            delete[] arg;
        }
    }

    return 0;
}
