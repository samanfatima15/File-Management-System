#ifndef COMMANDMANAGER_H
#define COMMANDMANAGER_H

#include "Folder.h"
#include <string>

class CommandManager {
private:
    Folder* root;
    Folder* current;

public:
    CommandManager();
    ~CommandManager();

    void showHelp();
    void listContents();
    void makeFolder(const std::string& name);
    void changeFolder(const std::string& name);
    void deleteNode(const std::string& name);
    void renameNode(const std::string& oldName, const std::string& newName);
    void searchNode(const std::string& name);
    void makeFile(const std::string& type, const std::string& name);
    void unzipFile(const std::string& name);
    void openNode(const std::string& name);
    void run();
};

#endif
