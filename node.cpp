#include "Node.h"

// Constructor
Node::Node(const string& name, Node* parent)
    : name(name), parent(parent) {
}

// Destructor
Node::~Node() {}

// Get name
string Node::getName() const {
    return name;
}

// Get parent
Node* Node::getParent() const {
    return parent;
}

// Set name
void Node::setName(const string& newName) {
    name = newName;
}

// Build full path recursively
string Node::getPath() const {
    // Root case
    if (parent == nullptr)
        return name;

    // Recursive case
    return parent->getPath() + "/" + name;
}