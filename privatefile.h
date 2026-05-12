#ifndef PRIVATEFILE_H
#define PRIVATEFILE_H

#include "File.h"
#include <string>
using namespace std;

class PrivateFile : public File
{
private:
    string passkey;
    bool checkPass();

public:
    PrivateFile(const string& name, Node* parent, const string& key);
    ~PrivateFile();

    void open() override;
    void display() const override;
    bool isPrivate() const override;
    bool verifyPassword(const string& input) const;
};

#endif
