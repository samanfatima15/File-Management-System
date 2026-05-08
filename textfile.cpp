#include "Textfile.h"
#include <iostream>
using namespace std;

TxtFile::TxtFile(const string& name, Node* parent)
    : File(name + ".txt", parent), lineCount(0), capacity(5) {
    lines = new string[capacity];
}

TxtFile::~TxtFile() {
    delete[] lines;
}

void TxtFile::expand() {
    capacity *= 2;
    string* newLines = new string[capacity];
    for (int i = 0; i < lineCount; i++)
        newLines[i] = lines[i];
    delete[] lines;
    lines = newLines;
}

void TxtFile::addLine(const string& line) {
    if (lineCount == capacity)
        expand();
    lines[lineCount++] = line;
}

void TxtFile::editLine(int index, const string& newLine) {
    if (index >= 0 && index < lineCount)
        lines[index] = newLine;
    else
        cout << "Invalid line number.\n";
}

void TxtFile::showContent() {
    cout << "\n--- " << name << " ---\n";
    for (int i = 0; i < lineCount; i++)
        cout << i + 1 << ". " << lines[i] << endl;
}

void TxtFile::display() const {
    cout << "[TXT] " << name << endl;
}

void TxtFile::open() {
    int choice;
    do {
        cout << "\nEditing: " << name << endl;
        cout << "1. Add Line\n2. Edit Line\n3. Show Content\n0. Close\nChoice: ";
        cin >> choice;
        cin.ignore();

        if (choice == 1) {
            string line;
            cout << "Enter line: ";
            getline(cin, line);
            addLine(line);
        }
        else if (choice == 2) {
            int idx;
            cout << "Line number: ";
            cin >> idx;
            cin.ignore();
            string line;
            cout << "New text: ";
            getline(cin, line);
            editLine(idx - 1, line);
        }
        else if (choice == 3) {
            showContent();
        }
    } while (choice != 0);
}
