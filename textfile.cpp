#include "TextFile.h"
#include "Config.h"
#include <iostream>
#include <fstream>
#include <filesystem>
using namespace std;

namespace fs = std::filesystem;

TxtFile::TxtFile(const string& name, Node* parent)
    : File(name + ".txt", parent) {
}

TxtFile::~TxtFile() {}

void TxtFile::display() const {
    cout << "[txt] " << name;
}

void TxtFile::open() {
    string diskPath = getDiskPath();

    if (!fs::exists(diskPath)) {
        cout << "Error as we have not found the file "<<endl;
        return;
    }

    // this will read content into the array masimum of the 100 lines as we have done it by creating a largg array
    string lines[100];
    int lineCount = 0;

    ifstream infile(diskPath);
    string line;
    while (getline(infile, line) && lineCount < 100) {
        lines[lineCount] = line;
        lineCount++;
    }
    infile.close();

    int choice;
    do {
        cout << name << " : "<< endl;
        cout << "1. Add line\n2. Edit line\n3. Show content\n0. Close\nChoice: ";
        cin >> choice;
        cin.ignore();

        if (choice == 1) {
            if (lineCount < 100) {
                string newLine;
                cout << "hey user pls enter the  line ";
                getline(cin, newLine);
                lines[lineCount] = newLine;
                lineCount++;
                cout << "Line is added" << endl;;
            }
            else {
                cout << "File is full as we have only created the array of 100 .\n";
            }
        }
        else if (choice == 2) {
            if (lineCount == 0) {
                cout << "there are no line to edit as we have not added a line" << endl;;
                continue;
            }
            cout << "now current content is added which is ="<<endl;
            for (int i = 0; i < lineCount; i++)
                cout << i + 1 << ". " << lines[i] << endl;
            int idx;
            cout << "Line number to edit =";
            cin >> idx;
            cin.ignore();
            if (idx >= 1 && idx <= lineCount) {
                cout << "New text: ";
                getline(cin, lines[idx - 1]);
                cout << "Line edited."<<endl;
            }
            else {
                cout << "this is the invalid line number to edit" << endl;
            }
        }
        else if (choice == 3) {
            cout << name << " : " << endl;
            if (lineCount == 0)
                cout << "(empty file)" << endl;
            else
                for (int i = 0; i < lineCount; i++)
                    cout << i + 1 << ". " << lines[i] << endl;
        }
    } while (choice != 0);

   
    ofstream outfile(diskPath);
    for (int i = 0; i < lineCount; i++) {
        outfile << lines[i] << endl;
    }
    outfile.close();

    cout << "it is mow closed " << name << endl;
}
