#pragma once
#ifndef COMMANDMANAGER_H
#define COMMANDMANAGER_H

#include "Folder.h"
#include "AudioFile.h"
#include "ZipFile.h"
#include "TxtFile.h"       
#include "PrivateFile.h" 
#include <string>
using namespace std;

class CommandManager 
{
private:
    Folder* root;
    Folder* current;
    void searchRecursive(Folder* folder, const string& targetName, const string& path);

public:
    CommandManager();
    ~CommandManager();
void help();
    void ls();
    void mkdir(const string& name);
    void cd(const string& name);
    void rm(const string& name);
    void rename(const string& oldName, const string& newName);
    void search(const string& name);
    void touch(const string& type, const string& name);
    void unzip(const string& zipName);
    void run();
};

#endif
