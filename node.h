#pragma once
#ifndef NODE_H
#define NODE_H

#include <string>
using namespace std;

class Node {
protected:
    string name;
    Node* parent;

public:
    Node(const string& name, Node* parent);

    virtual ~Node();
    string getName() const;
    Node* getParent() const;
    void setName(const string& newName);
    string getPath() const; 

    virtual void open() = 0;
    virtual void display() const = 0;
    virtual bool isFolder() const = 0;
};

#endif
