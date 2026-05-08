#include "TxtFile.h"
#include <iostream>
#include "Textfile.h"
using namespace std;

TxtFile::TxtFile(const string& name, Node* parent)
    : File(name + ".txt", parent) {

    capacity = 5;
    lineCount = 0;
    lines = new string[capacity];
}

TxtFile::~TxtFile() {
    delete[] lines;
}

void TxtFile::expand() {
    capacity *= 2;

    string* newLines = new string[capacity];

    for (int i = 0; i < lineCount; i++) {
        newLines[i] = lines[i];
    }

    delete[] lines;
    lines = newLines;
}

void TxtFile::addLine(const string& line) {
    if (lineCount == capacity) {
        expand();
    }

    lines[lineCount++] = line;
}

void TxtFile::editLine(int index, const string& newLine) {
    if (index >= 0 && index < lineCount) {
        lines[index] = newLine;
    }
    else {
        cout << "Invalid line number.\n";
    }
}

void TxtFile::showContent() {
    cout << "\nFile: " << name << endl;

    for (int i = 0; i < lineCount; i++) {
        cout << i + 1 << ". " << lines[i] << endl;
    }
}

void TxtFile::open() {
    int choice;

    do {
        cout << "\nEditing: " << name << endl;
        cout << "1. Add Line\n";
        cout << "2. Edit Line\n";
        cout << "3. Show Content\n";
        cout << "0. Close\n";
        cout << "Choice: ";
        cin >> choice;
        cin.ignore();

        if (choice == 1) {
            string line;
            cout << "Enter line: ";
            getline(cin, line);
            addLine(line);
        }

        else if (choice == 2) {
            int index;
            string line;

            cout << "Line number: ";
            cin >> index;
            cin.ignore();

            cout << "New text: ";
            getline(cin, line);

            editLine(index - 1, line);
        }

        else if (choice == 3) {
            showContent();
        }

    } while (choice != 0);
}