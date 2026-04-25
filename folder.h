
#ifndef FOLDER_H
#define FOLDER_H

#include "Node.h"
#include <iostream>
#include <string>

using namespace std;

class Folder : public Node
{
private:
    struct ChildNode
    {
        Node* data;
        ChildNode* next;
    };

    ChildNode* head;   // first child in linked list

public:
    // Constructor
    Folder(const string& name, Node* parent);

    // Destructor
    ~Folder();

    // Core operations
    void addNode(Node* node);
    Node* find(const string& name);
    void removeNode(const string& name);

    // Display
    void list() const;
    void open();
    void display() const;

    // Type check
    bool isFolder() const;
};

#endif