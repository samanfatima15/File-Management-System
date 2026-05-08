#include "PrivateFile.h"   
#include <iostream>
using namespace std;

PrivateFile::PrivateFile(const string& name, Node* parent, const string& passkey)
    : File(name + ".priv", parent)  
    this->passkey = passkey;
    this->lineCount = 0;
    this->capacity = 5;
    this->lines = new string[capacity];
}

PrivateFile::~PrivateFile() {
    delete[] lines;
}

void PrivateFile::expand() {
    capacity *= 2;
    string* newLines = new string[capacity];
    for (int i = 0; i < lineCount; i++)
        newLines[i] = lines[i];
    delete[] lines;
    lines = newLines;
}

bool PrivateFile::verifyPasskey() {
    string input;
    cout << "Enter passkey: ";
    cin >> ws;
    getline(cin, input);
    return input == passkey;
}

void PrivateFile::open() {
    if (!verifyPasskey()) {
        cout << "Wrong passkey. Access denied." << endl;
        return;
    }
    cout << "Access granted. Editing " << getName() << endl;

    int choice;
    do {
        cout << "   Choice:   1. Add Line    2. Edit Line      3. Show Content   0. Close   ";
        cin >> choice;
        cin.ignore();

        if (choice == 1) {
            string line;
            cout << "Enter line: ";
            getline(cin, line);
            if (lineCount == capacity) expand();
            lines[lineCount++] = line;
        }
        else if (choice == 2) {
            int idx;
            cout << "Line number: ";
            cin >> idx;
            cin.ignore();
            if (idx >= 1 && idx <= lineCount) {
                cout << "New text: ";
                getline(cin, lines[idx - 1]);
            } else {
                cout << "Invalid line number." << endl;
            }
        }
        else if (choice == 3) {
            cout << getName() << endl;
            for (int i = 0; i < lineCount; i++)
                cout << i + 1 << ". " << lines[i] << endl;
        }
    } while (choice != 0);
}

void PrivateFile::display() const {
    cout << "[Private] " << name << endl;
}
