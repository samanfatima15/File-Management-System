#include "CommandManager.h"
#include <iostream>
using namespace std;

CommandManager::CommandManager() {
    root = new Folder("root", nullptr);
    current = root;
}
CommandManager::~CommandManager() {
    delete root;
}
void CommandManager::ls() {
    cout << "\n[" << current->getName() << "]\n";
    current->list();
}
void CommandManager::mkdir(const string& name) {
    if (current->find(name) != nullptr) {
        cout << "Error: " << name << " already exists."<<endl;
        return;
    }
    current->addNode(new Folder(name, current));
}

void CommandManager::cd(const string& name) {
    if (name == "..") {
        if (current->getParent() == nullptr)
            cout << "Already at root.\n";
        else
            current = (Folder*)current->getParent();
        return;
    }

    Node* found = current->find(name);

    if (found == nullptr)
        cout << "'" << name << "' not found."<<endl;
    else if (!found->isFolder())
        cout << "'" << name << "' is a file, not a folder.<<endl";
    else
        current = (Folder*)found;
}

void CommandManager::rm(const string& name) {
    current->removeNode(name);
}

void CommandManager::rename(const string& oldName, const string& newName) {
    Node* found = current->find(oldName);

    if (found == nullptr) {
        cout << "'" << oldName << "' not found.\n";
        return;
    }
    if (current->find(newName) != nullptr) {
        cout << "Name '" << newName << "' already exists.\n";
        return;
    }
}
