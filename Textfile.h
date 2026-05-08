#pragma once
#ifndef TXTFILE_H
#define TXTFILE_H

#include "File.h"
#include <string>
using namespace std;

class TxtFile : public File {
private:
    string* lines;
    int lineCount;
    int capacity;

    void expand();

public:
    TxtFile(const string& name, Node* parent);
    ~TxtFile();

    void addLine(const string& line);
    void editLine(int index, const string& newLine);
    void showContent();

    void open() override;
};

#endif