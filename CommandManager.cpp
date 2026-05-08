#include "CommandManager.h"
#include "AudioFile.h"
#include "ZipFile.h"
#include "TxtFile.h"
#include "PrivateFile.h"
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
        cout << "Error: " << name << " already exists." << endl;
        return;
    }
    current->addNode(new Folder(name, current));
}

void CommandManager::cd(const string& name) {
    if (name == "..") {
        if (current->getParent() == nullptr)
            cout << "Already at root." << endl;
        else
            current = (Folder*)current->getParent();
        return;
    }
    Node* found = current->find(name);
    if (found == nullptr)
        cout << "'" << name << "' not found." << endl;
    else if (!found->isFolder())
        cout << "'" << name << "' is a file, not a folder." << endl;
    else
        current = (Folder*)found;
}

void CommandManager::rm(const string& name) {
    current->removeNode(name);
}

void CommandManager::rename(const string& oldName, const string& newName) {
    Node* found = current->find(oldName);
    if (found == nullptr) {
        cout << oldName << " not found." << endl;
        return;
    }
    // ROOT PROTECTION — ADDED
    if (found == root) {
        cout << "Cannot rename root folder." << endl;
        return;
    }
    if (oldName == newName) {
        cout << "Same name. No changes made." << endl;
        return;
    }
    if (current->find(newName) != nullptr) {
        cout << "Name '" << newName << "' already exists." << endl;
        return;
    }
    found->setName(newName);
    cout << "Renamed " << oldName << " to " << newName << endl;
}

void CommandManager::searchRecursive(Folder* folder, const string& targetName, const string& path) {
}

void CommandManager::search(const string& name) {
    cout << "Searching for '" << name << "'..." << endl;
    current->searchIn(name, current->getPath());
}

void CommandManager::touch(const string& type, const string& name) {

    if (type == "txt") {
        if (current->find(name + ".txt") != nullptr) {
            cout << "Error: " << name << ".txt already exists." << endl;
            return;
        }
        current->addNode(new TxtFile(name, current));
    }

    else if (type == "private") {
        if (current->find(name + ".priv") != nullptr) {
            cout << "Error: " << name << ".priv already exists." << endl;
            return;
        }
        string passkey;
        cout << "Set a passkey for this file: ";
        cin >> passkey;
        current->addNode(new PrivateFile(name, current, passkey));
    }

    else if (type == "audio") {
        if (current->find(name + ".mpg") != nullptr) {
            cout << "Error: " << name << ".mpg already exists." << endl;
            return;
        }
        current->addNode(new AudioFile(name, current));
    }

    else if (type == "zip") {
        Node* toZip = current->find(name);
        if (toZip == nullptr) {
            cout << "Error:  " << name << "    not found. Can only zip existing nodes." << endl;
            return;
        }
        string zipName = name + "-zip.zip";
        if (current->find(zipName) != nullptr) {
            cout << "Error: " << zipName << " already exists." << endl;
            return;
        }
        string origType = toZip->isFolder() ? "folder" : "file";
        current->addNode(new ZipFile(name, current, name, origType));
    }

    else {
        cout << "Unknown type: '" << type << "'" << endl;
        cout << "Valid types: txt, private, audio, zip" << endl;
    }
}

void CommandManager::unzip(const string& zipName) {
    Node* found = current->find(zipName);
    if (found == nullptr) {
        cout << zipName << "    not found." << endl;
        return;
    }
    if (found->isFolder()) {
        cout  << zipName << "    is a folder, not a zip file." << endl;
        return;
    }
    ZipFile* zf = dynamic_cast<ZipFile*>(found);
    if (zf == nullptr) {
        cout  << zipName << " is not a zip file." << endl;
        return;
    }
    string unzippedName = zf->getOriginalName() + "-unzipped";
    if (zf->getOriginalType() == "folder") {
        current->addNode(new Folder(unzippedName, current));
    } else {
        current->addNode(new File(unzippedName, current));
    }
    cout << "Unzipped as: " << unzippedName << endl;
}

void CommandManager::run() {
    string cmd;
    cout << "File Management System Ready." << endl;
    cout << "Commands: ls  mkdir  cd  rm  rename  search  touch  open  unzip  exit" << endl;

    while (true) {
        cout << "\n[" << current->getPath() << "] > ";
        cin >> cmd;

        if (cmd == "exit") {
            break;
        }
        else if (cmd == "ls") {
            ls();
        }
        else if (cmd == "mkdir") {
            string name;
            cin >> name;
            mkdir(name);
        }
        else if (cmd == "cd") {
            string name;
            cin >> name;
            cd(name);
        }
        else if (cmd == "rm") {
            string name;
            cin >> name;
            rm(name);
        }
        else if (cmd == "rename") {
            string oldName, newName;
            cin >> oldName >> newName;
            rename(oldName, newName);
        }
        else if (cmd == "search") {
            string name;
            cin >> name;
            search(name);
        }
        else if (cmd == "touch") {
            string type, name;
            cin >> type >> name;
            touch(type, name);
        }
        else if (cmd == "open") {
            string name;
            cin >> name;
            Node* found = current->find(name);
            if (found == nullptr)
                cout  << name << "    not found." << endl;
            else
                found->open();
        }
        else if (cmd == "unzip") {
            string name;
            cin >> name;
            unzip(name);
        }
        else {
            cout << "Unknown command." << endl;
        }
    }
}
