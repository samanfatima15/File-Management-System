#pragma once
#ifndef FILE_H
#define FILE_H

#include "Node.h"
#include <iostream>
#include <string>

using namespace std;

class File : public Node
{
public:
    // Constructor
    File(const string& name, Node* parent);

    // Destructor
    ~File();

    void open() override; 

    // Display file info
    void display() const;

    // Type check
    bool isFolder() const;
};

#endif