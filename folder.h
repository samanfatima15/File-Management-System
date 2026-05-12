#pragma once
#ifndef FOLDER_H
#define FOLDER_H

#include "Node.h"
#include <iostream>
#include <string>
using namespace std;

class Folder : public Node
{
private:
    struct ChildNode {
        Node* data;
        ChildNode* next;
    };
    ChildNode* head;

public:
    Folder(const string& name, Node* parent);
    ~Folder();

    void addNode(Node* node);
    Node* find(const string& name);
    void removeNode(const string& name);
    void list() const;
    void open() override;
    void display() const override;
    bool isFolder() const override;
    bool searchNode(const string& target, string path);
};

#endif
