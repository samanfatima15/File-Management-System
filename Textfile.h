##pragma once
#ifndef TXTFILE_H
#define TXTFILE_H

#include "File.h"
#include <string>
using namespace std;

class TxtFile : public File {
public:
    TxtFile(const string& name, Node* parent);
    ~TxtFile();

    void open() override;
    void display() const override;
};

#endif
