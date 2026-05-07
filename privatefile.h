#ifndef PRIVATEFILE_H
#define PRIVATEFILE_H

#include "TxtFile.h"

class PrivateFile : public TxtFile {
private:
    string passkey;

public:
    PrivateFile(const string& name, Node* parent, const string& passkey);

    bool verifyPasskey();
    void open() override;
};

#endif