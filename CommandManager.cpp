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