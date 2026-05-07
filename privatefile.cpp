#include "PrivateFile.h"
#include <iostream>
#include "node.h"
#include "Textfile.h"
using namespace std;

PrivateFile::PrivateFile(const string& name, Node* parent, const string& passkey)
    : TxtFile(name + ".priv", parent) {

    this->passkey = passkey;
}

bool PrivateFile::verifyPasskey() {
    string input;

    cout << "Enter passkey: ";
    cin >> input;

    return input == passkey;
}

void PrivateFile::open() {
    if (!verifyPasskey()) {
        cout << "Wrong passkey.\n";
        return;
    }

    cout << "Access granted.\n";

    TxtFile::open();
}