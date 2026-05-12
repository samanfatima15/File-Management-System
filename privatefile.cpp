#include "PrivateFile.h"
#include "Config.h"
#include <iostream>
#include <fstream>
#include <filesystem>
using namespace std;

namespace fs = std::filesystem;

PrivateFile::PrivateFile(const string& name, Node* parent, const string& key)
    : File(name + ".priv", parent), passkey(key) 
{

    string diskPath = getDiskPath();
    string keyPath = diskPath + ".key";

    // we are creating an empty fileon the disk and also saving the password in the other file
    ofstream contentFile(diskPath);
    contentFile.close();
    ofstream keyFile(keyPath);
    keyFile << passkey;
    keyFile.close();
}

PrivateFile::~PrivateFile()
{
}
bool PrivateFile::isPrivate() const {
    return true;
}
bool PrivateFile::verifyPassword(const string& input) const
{
    if (input == passkey)
    {
        return true;
    }
    string keyPath = getDiskPath() + ".key";
    if (fs::exists(keyPath)) {
        ifstream keyFile(keyPath);
        string storedKey;
        getline(keyFile, storedKey);
        keyFile.close();
        if (input == storedKey) {
            // Update passkey
            const_cast<PrivateFile*>(this)->passkey = storedKey;
            return true;
        }
    }

    return false;
}

bool PrivateFile::checkPass() {
    string input;
    cout << "Enter passkey for '" << name << "': ";
    getline(cin, input);

    if (verifyPassword(input)) {
        cout << "Access is grantedas you haave entered the corret password\n";
        return true;
    }
    else {
        cout << " as the password is wrong we cannot grant you the access.\n";
        return false;
    }
}

void PrivateFile::display() const {
    cout << "[private] " << name;
}

void PrivateFile::open() {

    if (!checkPass())
    {
        return;
    }
    string diskPath = getDiskPath();

    //reding the content as the same i have done in the text file like we creat e a large  file
    string lines[100];
    int lineCount = 0;

    if (fs::exists(diskPath)) {
        ifstream infile(diskPath);
        string line;
        while (getline(infile, line) && lineCount < 100) {
            lines[lineCount] = line;
            lineCount++;
        }
        infile.close();
    }

    cout << "\n[OPENING] " << name << " (Private File)\n";

    int choice;
    do {
        cout << "name: " << endl;
        cout << "1. Add line" << endl;
        cout << "2. Edit line" << endl;
        cout << "3. Show content" << endl;
        cout << "0. Close" << endl;
        cout << "Choice: ";
        cin >> choice;
        cin.ignore();

        if (choice == 1) {
            string newLine;
            cout << "hey user enter the line: ";
            getline(cin, newLine);
            if (lineCount < 100) {
                lines[lineCount] = newLine;
                lineCount++;
                cout << "Line added is added" << endl;
            }
            else {
                cout << "you cannot add next as the filesize is of 100 "<<endl;
            }
        }
        else if (choice == 2) {
            if (lineCount == 0) {
                cout << "you have not added a single line so you cannot edit first you have to enter the line then edit" << endl;
                continue;
            }
            cout << "this is the current content:" << endl;
            for (int i = 0; i < lineCount; i++)
                cout << "  " << i + 1 << ". " << lines[i] << endl;
            int idx;
            cout << "pls enter the the line number to edit: ";
            cin >> idx;
            cin.ignore();
            if (idx >= 1 && idx <= lineCount) {
                cout << "New text: ";
                getline(cin, lines[idx - 1]);
                cout << "the line is " << idx << " updated.\n";
            }
            else {
                cout << "there is an errror as this is an invalid line either this does not exixt" << endl;
            }
        }
        else if (choice == 3) {
            cout << "the content of" << name << " is" << endl;
            if (lineCount == 0)
                cout << "(empty file)\n";
            else
                for (int i = 0; i < lineCount; i++)
                    cout << "  " << i + 1 << ". " << lines[i] << endl;
            cout << endl;
        }
    } while (choice != 0);

    // Save content back
    ofstream outfile(diskPath);
    for (int i = 0; i < lineCount; i++) {
        outfile << lines[i] << endl;
    }
    outfile.close();

    cout << "closed " << name << endl;
}
