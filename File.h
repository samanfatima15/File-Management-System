#pragma once
#ifndef FILE_H
#define FILE_H

#include "Node.h"
#include <iostream>
#include <string>
using namespace std;

class File : public Node {
public:
    File(const string& name, Node* parent);
    ~File();
    void open() override;
    void display() const override;    
    bool isFolder() const override;   
};

#endif
