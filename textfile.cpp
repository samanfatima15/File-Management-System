#include "Config.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <filesystem>
#include "node.h"
#include "textfile.h"

using namespace std;

namespace fs = std::filesystem;

TxtFile::TxtFile(const string& name, Node* parent)
    : File(name + ".txt", parent)
{
    string diskPath = getDiskPath();
    if (!fs::exists(diskPath)) {
        ofstream ofs(diskPath);
        ofs.close();
    }
}

TxtFile::TxtFile(const string& fullName, Node* parent, bool isUnzipped)
    : File(fullName, parent, true)
{
    string diskPath = getDiskPath();
    if (!fs::exists(diskPath)) {
        cout << "Warning: Unzipped file not found at " << diskPath << endl;
    }
}

TxtFile::~TxtFile() {}

void TxtFile::display() const {
    cout << "[TXT] " << name;
}

void TxtFile::open() {
    string diskPath = getDiskPath();

    if (!fs::exists(diskPath)) {
        cout << "Error: File not found on disk\n";
        return;
    }

    vector<string> lines;
    ifstream infile(diskPath);
    string line;
    while (getline(infile, line)) {
        lines.push_back(line);
    }
    infile.close();

    int choice;
    do {
        cout << "\n===== " << name << " =====" << endl;
        cout << "1. Add line\n2. Edit line\n3. Show content\n0. Close\nChoice: ";
        cin >> choice;
        cin.ignore();

        if (choice == 1) {
            string newLine;
            cout << "Enter line: ";
            getline(cin, newLine);
            lines.push_back(newLine);
            cout << "Line added.\n";
        }
        else if (choice == 2) {
            if (lines.empty()) {
                cout << "No lines to edit.\n";
                continue;
            }
            cout << "\nCurrent content:\n";
            for (size_t i = 0; i < lines.size(); i++)
                cout << i + 1 << ". " << lines[i] << endl;
            int idx;
            cout << "Line number to edit: ";
            cin >> idx;
            cin.ignore();
            if (idx >= 1 && idx <= (int)lines.size()) {
                cout << "New text: ";
                getline(cin, lines[idx - 1]);
                cout << "Line edited.\n";
            }
            else {
                cout << "Invalid line number.\n";
            }
        }
        else if (choice == 3) {
            cout << "\n--- " << name << " ---\n";
            if (lines.empty())
                cout << "(empty file)\n";
            else
                for (size_t i = 0; i < lines.size(); i++)
                    cout << i + 1 << ". " << lines[i] << endl;
        }
    } while (choice != 0);

    ofstream outfile(diskPath);
    for (const auto& l : lines)
        outfile << l << endl;
    outfile.close();

    cout << "Closed " << name << endl;
}
