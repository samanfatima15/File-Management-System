#pragma once
#ifndef FILE_H
#define FILE_H

#include "Node.h"
#include <string>
using namespace std;

class File : public Node
{
public:
    File(const string& name, Node* parent);
    virtual ~File();

    bool isFolder() const override;
    void display() const override;
    virtual void open() override;

    string getDiskPath() const;
    virtual bool isPrivate() const
    {
        return false;
    }

protected:
    File(const string& name, Node* parent, bool skipDiskCreate);
};

#endif
