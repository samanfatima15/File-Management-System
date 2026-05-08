#pragma once
#ifndef PRIVATEFILE_H
#define PRIVATEFILE_H

#include "File.h"
#include <string>
using namespace std;
class PrivateFile : public File {
private:
    string passkey;
    string* lines;
    int lineCount;
    int capacity;

    void expand();
    bool verifyPasskey();

public:
    PrivateFile(const string& name, Node* parent, const string& passkey);
    ~PrivateFile();
    void open() override;
    void display() const override;
};

#endif
