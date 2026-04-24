#include "Node.h"

Node::Node(const string& name, Node* parent)
    : name(name), parent(parent) {
}

Node::~Node() {}


string Node::getName() const {
    return name;
}


Node* Node::getParent() const {
    return parent;
}
void Node::setName(const string& newName) {
    name = newName;
}

string Node::getPath() const {
    // Root case
    if (parent == nullptr)
        return name;

    // Recursive case
    return parent->getPath() + "/" + name;
}
