#include "CommandManager.h"
#include "AudioFile.h"
#include "ZipFile.h"
#include "Textfile.h"
#include "PrivateFile.h"
#include <iostream>
using namespace std;

void CommandManager::help() {
    cout << "\nAvailable commands:\n";
    cout << "  ls                 - List contents of current folder\n";
    cout << "  mkdir <name>       - Create a new folder\n";
    cout << "  cd <name>          - Change to folder (use '..' for parent)\n";
    cout << "  rm <name>          - Remove file/folder\n";
    cout << "  rename <old> <new> - Rename a node\n";
    cout << "  search <name>      - Find node recursively\n";
    cout << "  touch <type> <name>- Create file (txt, private, audio, zip)\n";
    cout << "  open <name>        - Open file/folder\n";
    cout << "  unzip <name>       - Unzip a .zip file\n";
    cout << "  exit               - Quit the program\n";
}

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
        cout << "Error: " << name << " already exists.\n";
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
        cout << "'" << name << "' not found.\n";
    else if (!found->isFolder())
        cout << "'" << name << "' is a file, not a folder.\n";
    else
        current = (Folder*)found;
}

void CommandManager::rm(const string& name) {
    Node* found = current->find(name);
    if (found == root) {
        cout << "Cannot delete root.\n";
        return;
    }
    current->removeNode(name);
}

void CommandManager::rename(const string& oldName, const string& newName) {
    Node* found = current->find(oldName);
    if (found == nullptr) {
        cout << oldName << " not found.\n";
        return;
    }
    if (found == root) {
        cout << "Cannot rename root folder.\n";
        return;
    }
    if (oldName == newName) {
        cout << "Same name. No changes made.\n";
        return;
    }
    if (current->find(newName) != nullptr) {
        cout << "Name '" << newName << "' already exists.\n";
        return;
    }
    found->setName(newName);
    cout << "Renamed " << oldName << " to " << newName << endl;
}

void CommandManager::search(const string& name) {
    cout << "Searching for '" << name << "'...\n";
    bool found = current->searchIn(name, current->getPath());
    if (!found)
        cout << "'" << name << "' not found.\n";
}

void CommandManager::touch(const string& type, const string& name) {
    if (type == "txt") {
        if (current->find(name + ".txt") != nullptr) {
            cout << "Error: " << name << ".txt already exists.\n";
            return;
        }
        current->addNode(new TxtFile(name, current));
    }
    else if (type == "private") {
        if (current->find(name + ".priv") != nullptr) {
            cout << "Error: " << name << ".priv already exists.\n";
            return;
        }
        string passkey;
        cout << "Set a passkey for this file: ";
        cin >> passkey;
        current->addNode(new PrivateFile(name, current, passkey));
    }
    else if (type == "audio") {
        if (current->find(name + ".mpg") != nullptr) {
            cout << "Error: " << name << ".mpg already exists.\n";
            return;
        }
        current->addNode(new AudioFile(name, current));
    }
    else if (type == "zip") {
        Node* toZip = current->find(name);
        if (toZip == nullptr) {
            cout << "Error: " << name << " not found. Can only zip existing nodes.\n";
            return;
        }
        string zipName = name + "-zip.zip";
        if (current->find(zipName) != nullptr) {
            cout << "Error: " << zipName << " already exists.\n";
            return;
        }
        string origType = toZip->isFolder() ? "folder" : "file";
        current->addNode(new ZipFile(name, current, name, origType));
    }
    else {
        cout << "Unknown type: '" << type << "'\n";
        cout << "Valid types: txt, private, audio, zip\n";
    }
}

void CommandManager::unzip(const string& zipName) {
    Node* found = current->find(zipName);
    if (found == nullptr) {
        cout << zipName << " not found.\n";
        return;
    }
    if (found->isFolder()) {
        cout << zipName << " is a folder, not a zip file.\n";
        return;
    }
    ZipFile* zf = dynamic_cast<ZipFile*>(found);
    if (zf == nullptr) {
        cout << zipName << " is not a zip file.\n";
        return;
    }
    string unzippedName = zf->getOriginalName() + "-unzipped";
    if (zf->getOriginalType() == "folder")
        current->addNode(new Folder(unzippedName, current));
    else
        current->addNode(new File(unzippedName, current));
    cout << "Unzipped as: " << unzippedName << endl;
}

void CommandManager::run() {
    string cmd;
    cout << "File Management System Ready.\n";
    cout << "Commands: ls, mkdir, cd, rm, rename, search, touch, open, unzip, exit\n";
    while (true) {
        cout << "\n[" << current->getPath() << "] > ";
        cin >> cmd;
        if (cmd == "exit") break;
        else if (cmd == "help") help();
        else if (cmd == "ls") ls();
        else if (cmd == "mkdir") { string n; cin >> n; mkdir(n); }
        else if (cmd == "cd") { string n; cin >> n; cd(n); }
        else if (cmd == "rm") { string n; cin >> n; rm(n); }
        else if (cmd == "rename") { string o, n; cin >> o >> n; rename(o, n); }
        else if (cmd == "search") { string n; cin >> n; search(n); }
        else if (cmd == "touch") { string t, n; cin >> t >> n; touch(t, n); }
        else if (cmd == "open") {
            string n; cin >> n;
            Node* f = current->find(n);
            if (!f) cout << n << " not found.\n";
            else f->open();
        }
        else if (cmd == "unzip") { string n; cin >> n; unzip(n); }
        else cout << "Unknown command.\n";
    }
}
